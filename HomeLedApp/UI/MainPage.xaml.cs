//Author: Tobi van Helsinki

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using HomeLedApp.Model;
using HomeLedApp.Strings;
using Rg.Plugins.Popup.Services;
using TLIB;
using Xamarin.Forms;

namespace HomeLedApp.UI
{
    //TODO +- buttons for sliders and input field for number of leds and the data pin in the settings
    public partial class MainPage : ContentPage, INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        public static MainPage Instance { get; private set; }
        public SSDP SSDPInstance => SSDP.Instance;

        private LEDController _Model;
        public LEDController Model
        {
            get => _Model;
            set { if (_Model != value) { _Model = value; NotifyPropertyChanged(); } }
        }

        public List<string> ModesList { get; set; }

        /// <summary>
        /// ctor
        /// </summary>
        /// <exception cref="TypeLoadException"></exception>
        /// <exception cref="InvalidOperationException"></exception>
        public MainPage()
        {
            Instance = this;
            Log.DisplayMessageRequested += Log_DisplayMessageRequested;
            ModesList = ModesExtension.GetModesDisplayNames().ToList();
            InitializeComponent();
            BindingContext = this;
            Model = new LEDController();
            Model.PropertyChanged += Model_PropertyChanged;
            Model.SetDefaultValues();
            ColorSpectrum.Source = ImageSource.FromResource(nameof(HomeLedApp) + "." + "Assets.HueScale.png", typeof(MainPage).GetTypeInfo().Assembly);
            ColorSpectrum2.Source = ImageSource.FromResource(nameof(HomeLedApp) + "." + "Assets.HueScale.png", typeof(MainPage).GetTypeInfo().Assembly);
        }

        private void Log_DisplayMessageRequested(LogMessage logmessage) => DisplayAlert(logmessage.LogType.ToString(), logmessage.CombinedMessage, "OK");

        private void Model_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(Model.CurrentMode))
            {
                Panel_Color1.IsVisible = true;
                Panel_Color2.IsVisible = false;
                SpeedSlider.IsVisible = true;
                WidthSlider.IsVisible = false;
                FringeSlider.IsVisible = false;
                Panel_SinParameter.IsVisible = false;

                switch (Model.CurrentMode)
                {
                    case Modes.on:
                        break;
                    case Modes.off:
                        Panel_Color1.IsVisible = false;
                        Panel_Color2.IsVisible = false;
                        SpeedSlider.IsVisible = false;
                        break;
                    case Modes.sin:
                        Panel_Color1.IsVisible = true;
                        Panel_Color2.IsVisible = false;
                        Panel_SinParameter.IsVisible = true;
                        break;
                    case Modes.rainbow:
                        Panel_Color1.IsVisible = false;
                        Panel_Color2.IsVisible = false;
                        Panel_RainbowParameter.IsVisible = true;
                        break;
                    case Modes.color:
                        Panel_Color1.IsVisible = true;
                        Panel_Color2.IsVisible = false;
                        SpeedSlider.IsVisible = false;
                        break;
                    case Modes.demo:
                        break;
                    case Modes.pulse:
                        Panel_Color1.IsVisible = true;
                        Panel_Color2.IsVisible = false;
                        Panel_SinParameter.IsVisible = true;
                        break;
                    case Modes.tyke:
                        Panel_Color1.IsVisible = true;
                        Panel_Color2.IsVisible = true;
                        break;
                    case Modes.knightrider:
                        Panel_Color1.IsVisible = true;
                        Panel_Color2.IsVisible = false;
                        WidthSlider.IsVisible = true;
                        FringeSlider.IsVisible = true;
                        break;
                    default:
                        break;
                }
            }
        }

        private void Refresh(object sender, EventArgs e) => _ = SSDPInstance.SearchForDevicesAsync();

        private void Clear_Click(object sender, EventArgs e) => _ = Model.Send("config=clear");

        private void Load_Click(object sender, EventArgs e) => _ = Model.Send("config=load");

        private void Save_Click(object sender, EventArgs e) => _ = Model.Send("config=save");

        private void Apply(object sender, EventArgs e) => _ = Model.Send();

        private void ShowDetails(object sender, EventArgs e) => DisplayAlert("The LED Server returned:", Model.Status, "OK");

        private void EditURL(object sender, EventArgs e)
        {
            try
            {
                _ = PopupNavigation.Instance.PushAsync(new DetailsPopUp(Model));
            }
            catch (Exception ex)
            {
                Log.Write("Could not display popup", ex, logType: LogType.Error);
            }
        }

        private void RenameHost(object sender, EventArgs e)
        {
            if (sender is BindableObject b)
            {
                try
                {
                    _ = PopupNavigation.Instance.PushAsync(
                        new Rename(Model, b.BindingContext switch
                        {
                            LEDDevice led => led,
                            MainPage currentPage => currentPage.Model.CurrentDevice,
                            _ => null
                        }));
                }
                catch (Exception ex)
                {
                    Log.Write("Could not PopUp", ex, logType: LogType.Error);
                }
            }
        }

        private void Info_Tapped(object sender, EventArgs e)
        {
            if (e is TappedEventArgs te && te.Parameter is string s)
            {
                DisplayAlert(AppResources.Info, s, "OK");
            }
        }
    }
}