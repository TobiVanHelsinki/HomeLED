//Author: Tobi van Helsinki

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Net.Http;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using TLIB;
using Xamarin.Forms;

namespace HomeLedApp.Model
{
    public partial class LEDController : INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        public bool ControlsEnable => CurrentDevice != null && !_NetworkCommunicationInProgress;

        private bool _NetworkCommunicationInProgress;
        public bool NetworkCommunicationInProgress
        {
            get => _NetworkCommunicationInProgress;
            set { if (_NetworkCommunicationInProgress != value) { _NetworkCommunicationInProgress = value; NotifyPropertyChanged(); NotifyPropertyChanged(nameof(ControlsEnable)); } }
        }

        #region Params

        internal async void UpdateHostname(string hostname)
        {
            await Send("setHostname=" + hostname);
        }

        private int _CurrentModeIndex;
        public int CurrentModeIndex
        {
            get => _CurrentModeIndex;
            set { if (_CurrentModeIndex != value) { _CurrentModeIndex = value; NotifyPropertyChanged(); } }
        }

        private Modes _CurrentMode;
        [LedServerRelevant("m", Modes.sin)]
        public Modes CurrentMode
        {
            get => _CurrentMode;
            set { if (_CurrentMode != value) { _CurrentMode = value; NotifyPropertyChanged(); } }
        }

        public Color CurrentColorInverse
        {
            get
            {
                try
                {
                    return CurrentColor.WithSaturation((_Color.Saturation + 0.5) % 1).WithLuminosity((_Color.Luminosity + 0.5) % 1).WithHue((_Color.Hue + 0.5) % 1);
                }
                catch
                {
                    return Color.Default;
                }
            }
        }

        private Color _Color = new Color(0, 0, 0, 0);
        public Color CurrentColor
        {
            get => _Color;
            set
            {
                var a = value.ToString();
                var b = _Color.ToString();
                if (a != b)
                {
                    _Color = value;
                    NotifyPropertyChanged();
                    NotifyPropertyChanged(nameof(Hue));
                    NotifyPropertyChanged(nameof(Saturation));
                    NotifyPropertyChanged(nameof(Luminosity));
                    NotifyPropertyChanged(nameof(CurrentColorInverse));
                    NotifyPropertyChanged(nameof(CurrentColorInt));
                }
            }
        }

        [LedServerRelevant("color", 0xE58200)]
        public int CurrentColorInt
        {
            get => (int)(CurrentColor.R * byte.MaxValue) << 16 | (int)(CurrentColor.G * byte.MaxValue) << 8 | (int)(CurrentColor.B * byte.MaxValue) << 0;
            set => CurrentColor = new Color((byte)(value >> 16) / (double)byte.MaxValue, (byte)(value >> 8) / (double)byte.MaxValue, (byte)(value >> 0) / (double)byte.MaxValue);
        }

        public double Hue_Min => 0;
        public double Hue_Max => 360;
        public double Hue
        {
            get => CurrentColor.Hue * 360.0;
            set => CurrentColor = CurrentColor.WithHue(value / 360.0);
        }

        public double Saturation_Min => 0;
        public double Saturation_Max => 100;
        public double Saturation
        {
            get => CurrentColor.Saturation * 100.0;
            set => CurrentColor = CurrentColor.WithSaturation(value / 100.0);
        }

        public double Luminosity_Min => 0;
        public double Luminosity_Max => 100;
        public double Luminosity
        {
            get => CurrentColor.Luminosity * 100.0;
            set => CurrentColor = CurrentColor.WithLuminosity(value / 100.0);
        }

        //[LedServerRelevant("color_h", (ushort)39578)]
        //public ushort Controller_HSV_Hue
        //{
        //    get => (ushort)(CurrentColor.Hue * ushort.MaxValue);
        //    set => CurrentColor = CurrentColor.WithHue(value / (double)ushort.MaxValue);
        //}

        //[LedServerRelevant("color_s", (byte)255)]
        //public byte Controller_HSV_Saturation
        //{
        //    get => (byte)(CurrentColor.Saturation_HSV() * byte.MaxValue);
        //    set { if (Controller_HSV_Saturation != value) { CurrentColor = HSVColorExtension.ColorFromHSV(Controller_HSV_Hue / (double)ushort.MaxValue * 360, value / (double)byte.MaxValue, Controller_HSV_Value / (double)byte.MaxValue); } }
        //}

        //public double Value_Min => 0;
        //public double Value_Max => 100;
        //[LedServerRelevant("color_v", (byte)229)]
        //public byte Controller_HSV_Value
        //{
        //    get => (byte)(CurrentColor.Value_HSV() * byte.MaxValue);
        //    set { if (Controller_HSV_Value != value) { CurrentColor = HSVColorExtension.ColorFromHSV(Controller_HSV_Hue / (double)ushort.MaxValue * 360, Controller_HSV_Saturation / (double)byte.MaxValue, value / (double)byte.MaxValue); } }
        //}

        public double Brigthnes_Min => 0;
        public double Brigthnes_Max => 255;
        private int _Brigthnes;
        [LedServerRelevant("br", 127)]
        public int Brigthnes
        {
            get => _Brigthnes;
            set { if (_Brigthnes != value) { _Brigthnes = value; NotifyPropertyChanged(); } }
        }

        public double Speed_Min => 40; 
        public double Speed_Max => 1000;
        private int _Speed;
        [LedServerRelevant("v", 50)]
        public int Speed
        {
            get => _Speed;
            set { if (_Speed != value) { _Speed = value; NotifyPropertyChanged(); } }
        }

        //public double NumberOfLeds_Min => 0;
        //public double NumberOfLeds_Max => 1024;
        //private int _NumberOfLeds;
        //[LedServerRelevant("n", 1024)]
        //public int NumberOfLeds
        //{
        //    get => _NumberOfLeds;
        //    set { if (_NumberOfLeds != value) { _NumberOfLeds = value; NotifyPropertyChanged(); } }
        //}

        public double Sin_VerticalOffset_Min => 1;
        public double Sin_VerticalOffset_Max => 1024;
        private int _Sin_VerticalOffset;
        [LedServerRelevant("mul", 1)]
        public int Sin_VerticalOffset
        {
            get => _Sin_VerticalOffset;
            set { if (_Sin_VerticalOffset != value) { _Sin_VerticalOffset = value; NotifyPropertyChanged(); } }
        }

        public double Sin_HorizontalOffset_Min => 2;
        public double Sin_HorizontalOffset_Max => 1024;
        public int _Sin_HorizontalOffset;
        [LedServerRelevant("tblsz", 256)]
        public int Sin_HorizontalOffset
        {
            get => _Sin_HorizontalOffset;
            set { if (_Sin_HorizontalOffset != value) { _Sin_HorizontalOffset = value; NotifyPropertyChanged(); } }
        }

       

        public double Width_Min => 1;
        public double Width_Max => 1024;
        private double _Width;
        [LedServerRelevant("width", 50d)]
        public double Width
        {
            get => _Width;
            set { if (_Width != value) { _Width = value; NotifyPropertyChanged(); } }
        }

        public double Fringe_Min => 0;
        public double Fringe_Max => Width_Max;

        private double _Fringe;
        [LedServerRelevant("fringe", 10d)]
        public double Fringe
        {
            get => _Fringe;
            set { if (_Fringe != value) { _Fringe = value; NotifyPropertyChanged(); } }
        }

        private int _Rainbow_Rand;
        [LedServerRelevant("rand", false)]
        public bool Rainbow_Rand
        {
            get => _Rainbow_Rand == 1 ? true : false;
            set { if (_Rainbow_Rand != (value ? 1 : 0)) { _Rainbow_Rand = value ? 1 : 0; NotifyPropertyChanged(); } }
        }

        #endregion Params

        #region Server Communication
        private ObservableCollection<LEDDevice> _AvailibleControllers = new ObservableCollection<LEDDevice>();
        public ObservableCollection<LEDDevice> AvailibleControllers
        {
            get => _AvailibleControllers;
            private set { if (_AvailibleControllers != value) { _AvailibleControllers = value; NotifyPropertyChanged(); } }
        }

        private LEDDevice _CurrentDevice;
        public LEDDevice CurrentDevice
        {
            get => _CurrentDevice;
            set { if (_CurrentDevice != value) { _CurrentDevice = value; NotifyPropertyChanged(); NotifyPropertyChanged(nameof(ControlsEnable)); } }
        }

        private string _Status = "---";
        public string Status
        {
            get => _Status;
            set { if (_Status != value) { _Status = value; NotifyPropertyChanged(); } }
        }

        private string _StatusCode;
        public string StatusCode
        {
            get => _StatusCode;
            set { if (_StatusCode != value) { _StatusCode = value; NotifyPropertyChanged(); } }
        }

        private string _URLParam;
        public string URLParam
        {
            get => _URLParam;
            set { if (_URLParam != value) { _URLParam = value; NotifyPropertyChanged(); } }
        }

        private static readonly HttpClient client = new HttpClient();

        internal async Task<string> Send(string myurlparam = null)
        {
            if (myurlparam is null)
            {
                myurlparam = URLParam;
            }
            RefreshURL();
            if (NetworkCommunicationInProgress)
            {
                return string.Empty;
            }
            NetworkCommunicationInProgress = true;
            string resutlt;
            try
            {
                var mess = await client.GetAsync(CurrentDevice.Urlbase + myurlparam);
                var time = DateTime.Now.ToString("hh:mm:ss ");
                StatusCode = time + mess.StatusCode.ToString();
                resutlt = await mess.Content.ReadAsStringAsync();
                Status = time + resutlt;
            }
            catch (Exception ex)
            {
                var time = DateTime.Now.ToString("hh:mm:ss ");
                StatusCode = time + "XXX";
                Status = time + "Exception Communitcating with LEDs: " + ex.Message;
                CurrentDevice.IsUpToDate = false;
                SSDP.Instance.UpdateDeviceList(CurrentDevice);
                resutlt = string.Empty;
            }
            finally
            {
                RefreshURL();
            }
            if (Xamarin.Essentials.MainThread.IsMainThread)
            {
                NetworkCommunicationInProgress = false;
            }
            else
            {
                Xamarin.Essentials.MainThread.BeginInvokeOnMainThread(() => NetworkCommunicationInProgress = false);
            }
            return resutlt;
        }
        #endregion Server Communication

        public LEDController()
        {
            PropertyChanged += LEDController_PropertyChanged;
            SSDP.Instance.DiscoveredDevices.CollectionChanged += DiscoveredDevices_CollectionChanged;
            SetCurrentDeviceIfNull();
        }

        public void SetDefaultValues() => SetValues(GetParameterProperties().Select(x => (x.Attribute.ParamName, x.Attribute.DefaultValue)));

        public void SetValues(IEnumerable<(string name, object value)> parameter)
        {
            NetworkCommunicationInProgress = true;
            foreach ((var Attribute, var Property) in GetParameterProperties().Join(parameter, a => a.Attribute.ParamName, b => b.name, (a, b) => (b, a.Property)))
            {
                try
                {
                    object value = Convert.ChangeType(Attribute.value, Property.PropertyType);
                    Property.SetValue(this, value);
                }
                catch (Exception)
                {
                }
            }
            NetworkCommunicationInProgress = false;
        }

        public async void ReadParameterFromDevice()
        {
            var result = await Send("get");
            var pairs = result.Split('&')
                .Select(x => x.Split('='));
            SetValues(pairs.Select(x => x.Count() == 2 ? (x[0], x[1] as object) : default));
        }

        private void DiscoveredDevices_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            SetCurrentDeviceIfNull();
        }

        private void LEDController_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            SetCurrentDeviceIfNull();
            RefreshURL();
            if (e.PropertyName == nameof(CurrentMode))
            {
                if (CurrentMode == Modes.tyke)
                {
                    CurrentColor = Color.Red;
                    //SecondColor = red;
                }
                _ = Send();
            }
            else if (e.PropertyName == nameof(CurrentDevice))
            {
                ReadParameterFromDevice();
            }
        }

        private void SetCurrentDeviceIfNull()
        {
            if (CurrentDevice is null && SSDP.Instance.DiscoveredDevices.FirstOrDefault() is LEDDevice d)
            {
                CurrentDevice = d;
            }
        }

        private void RefreshURL()
        {
            try
            {
                URLParam = GetParameterProperties().Aggregate("", (a, c) => a += "&" + c.Attribute.ParamName + (string.IsNullOrEmpty(c.Property.GetValue(this)?.ToString()) ? "" : ("=" + c.Property.GetValue(this)?.ToString()))).Replace("?&", "?");
            }
            catch (Exception)
            {
                if (System.Diagnostics.Debugger.IsAttached)
                {
                    System.Diagnostics.Debugger.Break();
                }
            }
        }

        private IEnumerable<(LedServerRelevantAttribute Attribute, PropertyInfo Property)> GetParameterProperties()
        {
            return GetType().GetProperties()
                                .Where(x => x.CustomAttributes.Any(y => y.AttributeType == typeof(LedServerRelevantAttribute)))
                                .Select(Property => (Property.GetCustomAttribute<LedServerRelevantAttribute>(), Property));
        }
    }
}