//Author: Tobi van Helsinki

using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Net.Http;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
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
        [LedServerRelevant("m")]
        public Modes CurrentMode
        {
            get => _CurrentMode;
            set { if (_CurrentMode != value) { _CurrentMode = value; NotifyPropertyChanged(); } }
        }

        private string _Config;
        [LedServerRelevant("config")]
        public string Config
        {
            get => _Config;
            set { if (_Config != value) { _Config = value; NotifyPropertyChanged(); } }
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

        private Color _Color;
        public Color CurrentColor
        {
            get => _Color;
            set
            {
                _Color = value;
                NotifyPropertyChanged();
                NotifyPropertyChanged(nameof(Hue));
                NotifyPropertyChanged(nameof(Saturation));
                NotifyPropertyChanged(nameof(Luminosity));
                NotifyPropertyChanged(nameof(CurrentColorInverse));
            }
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

        [LedServerRelevant("color_h")]
        public ushort Controller_HSV_Hue
        {
            get => (ushort)(CurrentColor.Hue * ushort.MaxValue);
            set => CurrentColor = CurrentColor.WithHue(value / ushort.MaxValue);
        }

        [LedServerRelevant("color_s")]
        public byte Controller_HSV_Saturation
        {
            get => (byte)(CurrentColor.Saturation_HSV() * byte.MaxValue);
            set { if (Controller_HSV_Saturation != value) { CurrentColor = HSVColorExtension.ColorFromHSV(Controller_HSV_Hue / ushort.MaxValue, value / byte.MaxValue, Controller_HSV_Value / byte.MaxValue); } }
        }

        public double Value_Min => 0;
        public double Value_Max => 100;
        [LedServerRelevant("color_v")]
        public byte Controller_HSV_Value
        {
            get => (byte)(CurrentColor.Value_HSV() * byte.MaxValue);
            set { if (Controller_HSV_Value != value) { CurrentColor = HSVColorExtension.ColorFromHSV(Controller_HSV_Hue / ushort.MaxValue, Controller_HSV_Saturation / byte.MaxValue, value / byte.MaxValue); } }
        }

        public double Brigthnes_Min => 0;
        public double Brigthnes_Max => 255;
        private int _Brigthnes;
        [LedServerRelevant("br")]
        public int Brigthnes
        {
            get => _Brigthnes;
            set { if (_Brigthnes != value) { _Brigthnes = value; NotifyPropertyChanged(); } }
        }

        public double Speed_Min => 0;
        public double Speed_Max => 50;
        private int _Speed;
        [LedServerRelevant("v")]
        public int Speed
        {
            get => _Speed;
            set { if (_Speed != value) { _Speed = value; NotifyPropertyChanged(); } }
        }

        public double NumberOfLeds_Min => 0;
        public double NumberOfLeds_Max => 300;
        private int _NumberOfLeds;
        [LedServerRelevant("n")]
        public int NumberOfLeds
        {
            get => _NumberOfLeds;
            set { if (_NumberOfLeds != value) { _NumberOfLeds = value; NotifyPropertyChanged(); } }
        }

        public double Sin_VerticalOffset_Min => 0;
        public double Sin_VerticalOffset_Max => 30;
        private int _Sin_VerticalOffset;
        [LedServerRelevant("vo")]
        public int Sin_VerticalOffset
        {
            get => (_Sin_VerticalOffset < 0 ? -1 : 1) * _Sin_VerticalOffset;
            set { if (_Sin_VerticalOffset != value) { _Sin_VerticalOffset = (Sin_VerticalOffset_Neg ? -1 : 1) * value; NotifyPropertyChanged(); } }
        }

        private bool _Sin_VerticalOffset_Neg;
        public bool Sin_VerticalOffset_Neg
        {
            get => _Sin_VerticalOffset_Neg;
            set { if (_Sin_VerticalOffset_Neg != value) { _Sin_VerticalOffset_Neg = value; NotifyPropertyChanged(); _Sin_VerticalOffset *= -1; NotifyPropertyChanged(nameof(Sin_VerticalOffset)); } }
        }

        public double Sin_HorizontalOffset_Min => 0;
        public double Sin_HorizontalOffset_Max => 10;
        public int _Sin_HorizontalOffset;
        [LedServerRelevant("ho")]
        public int Sin_HorizontalOffset
        {
            get => (_Sin_HorizontalOffset < 0 ? -1 : 1) * _Sin_HorizontalOffset;
            set { if (_Sin_HorizontalOffset != value) { _Sin_HorizontalOffset = (Sin_HorizontalOffset_Neg ? -1 : 1) * value; NotifyPropertyChanged(); } }
        }

        private bool _Sin_HorizontalOffset_Neg;
        public bool Sin_HorizontalOffset_Neg
        {
            get => _Sin_HorizontalOffset_Neg;
            set { if (_Sin_HorizontalOffset_Neg != value) { _Sin_HorizontalOffset_Neg = value; NotifyPropertyChanged(); _Sin_HorizontalOffset *= -1; NotifyPropertyChanged(nameof(Sin_HorizontalOffset)); } }
        }

        public double Width_Min => 0;
        public double Width_Max => NumberOfLeds_Max;
        private double _Width;
        [LedServerRelevant("width")]
        public double Width
        {
            get => _Width;
            set { if (_Width != value) { _Width = value; NotifyPropertyChanged(); } }
        }

        public double Fringe_Min => 0;
        public double Fringe_Max => Width_Max;

        private double _Fringe;
        [LedServerRelevant("fringe")]
        public double Fringe
        {
            get => _Fringe;
            set { if (_Fringe != value) { _Fringe = value; NotifyPropertyChanged(); } }
        }

        private int _Rainbow_Rand;
        [LedServerRelevant("rand")]
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

        internal async Task Send(string myurlparam = null)
        {
            if (myurlparam is null)
            {
                myurlparam = URLParam;
            }
            RefreshURL();
            if (NetworkCommunicationInProgress)
            {
                return;
            }
            NetworkCommunicationInProgress = true;
            try
            {
                var mess = await client.GetAsync(CurrentDevice.Urlbase + myurlparam);
                var time = DateTime.Now.ToString("hh:mm:ss ");
                StatusCode = time + mess.StatusCode.ToString();
                Status = time + await mess.Content.ReadAsStringAsync();
            }
            catch (Exception ex)
            {
                var time = DateTime.Now.ToString("hh:mm:ss ");
                StatusCode = time + "XXX";
                Status = time + "Exception Communitcating with LEDs: " + ex.Message;
                CurrentDevice.IsUpToDate = false;
                SSDP.Instance.UpdateDeviceList(CurrentDevice);
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
        }
        #endregion Server Communication

        public LEDController()
        {
            PropertyChanged += LEDController_PropertyChanged;
            SSDP.Instance.DiscoveredDevices.CollectionChanged += DiscoveredDevices_CollectionChanged;
            SetCurrentDeviceIfNull();
        }

        public void SetDefaultValues()
        {
            NetworkCommunicationInProgress = true;
            CurrentMode = Modes.sin;
            CurrentColor = new Color(0.9, 0.5, 0);
            Brigthnes = 50;
            Speed = 16;
            NumberOfLeds = 150;
            Sin_VerticalOffset = 5;
            Sin_HorizontalOffset = 2;
            Width = 50;
            Fringe = 10;
            NetworkCommunicationInProgress = false;
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
                _ = Send();
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
            if (CurrentDevice is null)
            {
                return;
            }
            try
            {
                var props = GetType().GetProperties().Where(x => x.CustomAttributes.FirstOrDefault(y => y.AttributeType == typeof(LedServerRelevantAttribute)) != null);
                var attributes = props.Select(x => (x.GetCustomAttribute<LedServerRelevantAttribute>().ParamName, x.GetValue(this)?.ToString()));
                URLParam = attributes.Aggregate("", (a, c) => a += "&" + c.Item1 + (string.IsNullOrEmpty(c.Item2) ? "" : ("=" + c.Item2))).Replace("?&", "?");
            }
            catch (Exception)
            {
                if (System.Diagnostics.Debugger.IsAttached)
                {
                    System.Diagnostics.Debugger.Break();
                }
            }
        }
    }
}