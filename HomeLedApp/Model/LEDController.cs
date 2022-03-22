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
        
        private Color _Color = new(0, 0, 0, 0);
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
                    NotifyPropertyChanged(nameof(White));
                    NotifyPropertyChanged(nameof(Hue));
                    NotifyPropertyChanged(nameof(Saturation));
                    NotifyPropertyChanged(nameof(Luminosity));
                    NotifyPropertyChanged(nameof(CurrentColorInverse));
                    NotifyPropertyChanged(nameof(CurrentColorInt));
                }
            }
        }
     
        [LedServerRelevant("c", 0xE58200)]
        public uint CurrentColorInt
        {
            get => (uint)(CurrentColor.R * byte.MaxValue) << 16 | (uint)(CurrentColor.G * byte.MaxValue) << 8 | (uint)(CurrentColor.B * byte.MaxValue) << 0 | (uint)(CurrentColor.A * byte.MaxValue) << 24;
            set => CurrentColor = new Color((byte)(value >> 16) / (double)byte.MaxValue, (byte)(value >> 8) / (double)byte.MaxValue, (byte)(value >> 0) / (double)byte.MaxValue, (byte)(value >> 24) / (double)byte.MaxValue);
        }

        public double White_Min => 0;
        public double White_Max => 100;
        public double White
        {
            get => CurrentColor.A * 100.0;
            set => CurrentColor = new Color(CurrentColor.R, CurrentColor.G, CurrentColor.B, value / 100.0);
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

        private Color _Color2 = new Color(0, 0, 0, 0);
        public Color CurrentColor2
        {
            get => _Color2;
            set
            {
                var a = value.ToString();
                var b = _Color2.ToString();
                if (a != b)
                {
                    _Color2 = value;
                    NotifyPropertyChanged();
                    NotifyPropertyChanged(nameof(White2));
                    NotifyPropertyChanged(nameof(Hue2));
                    NotifyPropertyChanged(nameof(Saturation2));
                    NotifyPropertyChanged(nameof(Luminosity2));
                    NotifyPropertyChanged(nameof(CurrentColor2Inverse));
                    NotifyPropertyChanged(nameof(CurrentColor2Int));
                }
            }
        }

        public Color CurrentColor2Inverse
        {
            get
            {
                try
                {
                    return CurrentColor2.WithSaturation((_Color2.Saturation + 0.5) % 1).WithLuminosity((_Color2.Luminosity + 0.5) % 1).WithHue((_Color2.Hue + 0.5) % 1);
                }
                catch
                {
                    return Color.Default;
                }
            }
        }

        [LedServerRelevant("c2", 0xE58200)]
        public uint CurrentColor2Int
        {
            get => (uint)(CurrentColor2.R * byte.MaxValue) << 16 | (uint)(CurrentColor2.G * byte.MaxValue) << 8 | (uint)(CurrentColor2.B * byte.MaxValue) << 0 | (uint)(CurrentColor2.A * byte.MaxValue) << 24;
            set => CurrentColor2 = new Color((byte)(value >> 16) / (double)byte.MaxValue, (byte)(value >> 8) / (double)byte.MaxValue, (byte)(value >> 0) / (double)byte.MaxValue, (byte)(value >> 24) / (double)byte.MaxValue);
        }
        public double White2_Min => 0;
        public double White2_Max => 100;
        public double White2
        {
            get => CurrentColor2.A * 100.0;
            set => CurrentColor2 = new Color(CurrentColor2.R, CurrentColor2.G, CurrentColor2.B, value / 100.0);
        }
        public double Hue2_Min => 0;
        public double Hue2_Max => 360;
        public double Hue2
        {
            get => CurrentColor2.Hue * 360.0;
            set => CurrentColor2 = CurrentColor2.WithHue(value / 360.0);
        }

        public double Saturation2_Min => 0;
        public double Saturation2_Max => 100;
        public double Saturation2
        {
            get => CurrentColor2.Saturation * 100.0;
            set => CurrentColor2 = CurrentColor2.WithSaturation(value / 100.0);
        }

        public double Luminosity2_Min => 0;
        public double Luminosity2_Max => 100;
        public double Luminosity2
        {
            get => CurrentColor2.Luminosity * 100.0;
            set => CurrentColor2 = CurrentColor2.WithLuminosity(value / 100.0);
        }



        public double Brigthnes_Min => 0;
        public double Brigthnes_Max => 255;
        private int _Brigthnes;
        [LedServerRelevant("b", 256)]
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
        public double Skip_Min => 0;
        public double Skip_Max => 20;
        private int _Skip;
        [LedServerRelevant("sk", 1)]
        public int Skip
        {
            get => _Skip;
            set { if (_Skip != value) { _Skip = value; NotifyPropertyChanged(); } }
        }


        private double _Sin_VerticalOffset_Min = 1;
        public double Sin_VerticalOffset_Min
        {
            get => _Sin_VerticalOffset_Min;
            set { if (_Sin_VerticalOffset_Min != value) { _Sin_VerticalOffset_Min = value; NotifyPropertyChanged(); } }
        }

        private double _Sin_VerticalOffset_Max = 1024;
        public double Sin_VerticalOffset_Max
        {
            get => _Sin_VerticalOffset_Max;
            set { if (_Sin_VerticalOffset_Max != value) { _Sin_VerticalOffset_Max = value; NotifyPropertyChanged(); } }
        }

        private int _Sin_VerticalOffset;
        [LedServerRelevant("mu", 1)]
        public int Sin_VerticalOffset
        {
            get => _Sin_VerticalOffset;
            set { if (_Sin_VerticalOffset != value) { _Sin_VerticalOffset = value; NotifyPropertyChanged(); } }
        }

        public double Sin_HorizontalOffset_Min => 2;
        private double _Sin_HorizontalOffset_Max = 1024;
        public double Sin_HorizontalOffset_Max
        {
            get => _Sin_HorizontalOffset_Max;
            set { if (_Sin_HorizontalOffset_Max != value) { _Sin_HorizontalOffset_Max = value; NotifyPropertyChanged(); } }
        }

        public int _Sin_HorizontalOffset;

        [LedServerRelevant("t", 256)]
        public int Sin_HorizontalOffset
        {
            get => _Sin_HorizontalOffset;
            set { if (_Sin_HorizontalOffset != value) { _Sin_HorizontalOffset = value; NotifyPropertyChanged(); } }
        }

        public double Sin_Scale_Min => 1;
        public double Sin_Scale_Max => 50;
        
        public int _Sin_Scale;
        public int Sin_Scale
        {
            get => _Sin_Scale;
            set { if (_Sin_Scale != value) { _Sin_Scale = value; NotifyPropertyChanged(); } }
        }
        [LedServerRelevant("s", "0.45")]
        public string Sin_Scale_ForSend
        {
            get => (Sin_Scale / 100.0).ToString("G", System.Globalization.CultureInfo.CreateSpecificCulture("en-US"));
            set => Sin_Scale = (int)(double.Parse(value) * 1.0); // verstehe nicht, wieso parse("0.34") 34 zurück gibt.
        }


        public double Sin_StepSize_Min => 1;
        public double Sin_StepSize_Max => 20;

        public int _Sin_StepSize;
        [LedServerRelevant("st", "1")]
        public int Sin_StepSize
        {
            get => _Sin_StepSize;
            set { if (_Sin_StepSize != value) { _Sin_StepSize = value; NotifyPropertyChanged(); } }
        }


        public double Width_Min => 0;
        private double _Width_Max = 1024;
        public double Width_Max
        {
            get => _Width_Max;
            set { if (_Width_Max != value) { _Width_Max = value; NotifyPropertyChanged(); } }
        }

        private double _Width;
        [LedServerRelevant("w", 50d)]
        public double Width
        {
            get => _Width;
            set { if (_Width != value) { _Width = value; NotifyPropertyChanged(); } }
        }

        public double Fringe_Min => 0;
        private double _Fringe_Max = 1024;
        public double Fringe_Max
        {
            get => _Fringe_Max;
            set { if (_Fringe_Max != value) { _Fringe_Max = value; NotifyPropertyChanged(); } }
        }


        private double _Fringe;
        [LedServerRelevant("f", 10d)]
        public double Fringe
        {
            get => _Fringe;
            set { if (_Fringe != value) { _Fringe = value; NotifyPropertyChanged(); } }
        }

        private int _Rainbow_Rand;
        [LedServerRelevant("r", false)]
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
            var list = GetParameterProperties().ToArray();
            foreach ((var Attribute, var Property) in GetParameterProperties().Join(parameter, a => a.Attribute.ParamName, b => b.name, (a, b) => (b, a.Property)))
            {
                try
                {
                    if (Property.PropertyType.IsEnum && Attribute.value is string enumasstring)
                        //if the value is a string, comming from the device
                    {
                        bool isdefined= Property.PropertyType.IsEnumDefined(Attribute.value);
                        var enumvalue = Enum.Parse(Property.PropertyType, enumasstring);
                        Property.SetValue(this, enumvalue);
                    }
                    else
                    {
                        //for natives types
                        object value = Convert.ChangeType(Attribute.value, Property.PropertyType);
                        Property.SetValue(this, value);
                    }
                }
                catch (Exception )
                {
                }
            }
            NetworkCommunicationInProgress = false;
        }


        internal async Task<(int dataPin, int numberLED, string hostname)> GetSettings()
        {
            try
            {
                var result = await Send("hostname&datapin&n");
                var pairs = result.Split('&').Select(x => x.Split('=')).ToArray();
                int.TryParse(pairs.FirstOrDefault(x => x[0] == "datapin")?[1], out int dataPin);
                int.TryParse(pairs.FirstOrDefault(x => x[0] == "n")?[1], out int numberLED);
                Width_Max = numberLED.Max(1);
                Fringe_Max = numberLED.Max(1);
                Sin_HorizontalOffset_Max = numberLED.Max(3);
                Sin_VerticalOffset_Max = numberLED.Max(3);
                string hostname = pairs.FirstOrDefault(x => x[0] == "hostname")?[1];
                return (dataPin, numberLED, hostname);
            }
            catch (Exception ex)
            {

                throw;
            }
        }

        internal async void SetSettings(int dataPin, int numberLED, string hostname)
        {
            await Send("hostname=" + hostname + "&datapin=" + dataPin + "&n=" + numberLED);
        }
        public async void ReadParameterFromDevice()
        {
            await GetSettings();
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
                    CurrentColor2 = Color.Blue;
                }
                //_ = Send();
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