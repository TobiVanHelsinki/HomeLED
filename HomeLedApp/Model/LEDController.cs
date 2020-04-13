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
    public class VisibleAttribute : Attribute
    {
        public VisibleAttribute(string name) => Name = name;
        public string Name { get; internal set; }
    }

    public class LEDController : INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        #region Params

        public enum Modes
        {
            [VisibleAttribute("On")]
            on,

            [VisibleAttribute("Off")]
            off,

            [VisibleAttribute("Sin")]
            sin,

            [VisibleAttribute("Rainbow")]
            rainbow,

            [VisibleAttribute("1 Color")]
            color,

            [VisibleAttribute("1 Pixel")]
            pixel,

            [VisibleAttribute("Pulsar")]
            pulse,
            save,
            load,
            clear
        }

        internal async void UpdateHostname(string hostname)
        {
            await Send("setHostname=" + hostname);
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

        private Color _Color = new Color(0.9, 0.5, 0);
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
                NotifyPropertyChanged(nameof(R));
                NotifyPropertyChanged(nameof(G));
                NotifyPropertyChanged(nameof(B));
            }
        }

        public double Hue
        {
            get => CurrentColor.Hue * 360.0;
            set => CurrentColor = CurrentColor.WithHue(value / 360.0);
        }

        public double Hue_Max => 360;
        public double Hue_Min => 0;

        public double Saturation
        {
            get => CurrentColor.Saturation * 100.0;
            set => CurrentColor = CurrentColor.WithSaturation(value / 100.0);
        }

        public double Saturation_Max => 100;
        public double Saturation_Min => 0;
        public double Luminosity
        {
            get => CurrentColor.Luminosity * 100.0;
            set => CurrentColor = CurrentColor.WithLuminosity(value / 100.0);
        }

        public double Luminosity_Max => 100;
        public double Luminosity_Min => 0;

        [LedServerRelevant("r")]
        public int R
        {
            get => (int)(CurrentColor.R * 255);
            set => CurrentColor = new Color(value / 255.0, CurrentColor.G, CurrentColor.B);
        }

        [LedServerRelevant("g")]
        public int G
        {
            get => (int)(CurrentColor.G * 255);
            set => CurrentColor = new Color(CurrentColor.R, value / 255.0, CurrentColor.B);
        }

        [LedServerRelevant("b")]
        public int B
        {
            get => (int)(CurrentColor.B * 255);
            set => CurrentColor = new Color(CurrentColor.R, CurrentColor.G, value / 255.0);
        }

        private int _Brigthnes = 50;
        [LedServerRelevant("br")]
        public int Brigthnes
        {
            get => _Brigthnes;
            set { if (_Brigthnes != value) { _Brigthnes = value; NotifyPropertyChanged(); } }
        }

        public double Brigthnes_Max => 255;
        public double Brigthnes_Min => 0;

        [LedServerRelevant("v")]
        public int _Speed = 62;
        public int Speed
        {
            get => (int)(1000.0 / _Speed);
            set { if (_Speed != value) { _Speed = value < 1 ? 1000 : (int)(1000.0 / value); NotifyPropertyChanged(); } }
        }

        public double Speed_Max => 50;
        public double Speed_Min => 0;

        private int _NumberOfLeds = 150;
        [LedServerRelevant("n")]
        public int NumberOfLeds
        {
            get => _NumberOfLeds;
            set { if (_NumberOfLeds != value) { _NumberOfLeds = value; NotifyPropertyChanged(); } }
        }

        public double NumberOfLeds_Max => 150;
        public double NumberOfLeds_Min => 0;

        [LedServerRelevant("vo")]
        public int _Sin_VerticalOffset = 5;
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

        [LedServerRelevant("ho")]
        public int _Sin_HorizontalOffset = 2;
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

        [LedServerRelevant("rand")]
        public int _Rainbow_Rand;
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
            set { if (_CurrentDevice != value) { _CurrentDevice = value; NotifyPropertyChanged(); } }
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
        private bool SendInProgress;

        internal async Task Send(string myurlparam = null)
        {
            if (myurlparam is null)
            {
                myurlparam = URLParam;
            }
            RefreshURL();
            if (SendInProgress)
            {
                return;
            }
            SendInProgress = true;
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
            }
            finally
            {
                RefreshURL();
            }
            SendInProgress = false;
        }
        #endregion Server Communication

        public LEDController()
        {
            PropertyChanged += LEDController_PropertyChanged;
            SSDP.Instance.DiscoveredDevices.CollectionChanged += DiscoveredDevices_CollectionChanged;
            SetCurrentDeviceIfNull();
        }

        private void DiscoveredDevices_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            SetCurrentDeviceIfNull();
        }

        private void LEDController_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            RefreshURL();
            SetCurrentDeviceIfNull();
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
                var fields = GetType().GetFields().Where(x => x.CustomAttributes.FirstOrDefault(y => y.AttributeType == typeof(LedServerRelevantAttribute)) != null);
                var both = props.Select(x => (x.GetCustomAttribute<LedServerRelevantAttribute>().ParamName, x.GetValue(this)?.ToString())).Concat(fields.Select(x => (x.GetCustomAttribute<LedServerRelevantAttribute>().ParamName, x.GetValue(this)?.ToString())));
                URLParam = both.Aggregate("", (a, c) => a += "&" + c.Item1 + (string.IsNullOrEmpty(c.Item2) ? "" : ("=" + c.Item2))).Replace("?&", "?");
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