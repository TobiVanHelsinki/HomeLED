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
    public partial class MainPage : ContentPage, INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        private LEDController _Model;
        public LEDController Model
        {
            get => _Model;
            set { if (_Model != value) { _Model = value; NotifyPropertyChanged(); } }
        }

        public SSDP SSDPInstance => SSDP.Instance;

        /// <summary>
        /// ctor
        /// </summary>
        /// <exception cref="TypeLoadException"></exception>
        /// <exception cref="InvalidOperationException"></exception>
        public MainPage()
        {
            Modes =
                (from name in typeof(LEDController.Modes).GetEnumNames()
                 let attributes = typeof(LEDController.Modes).
                 GetMember(name)[0].
                 GetCustomAttributes(typeof(VisibleAttribute), false)
                 where attributes.Length > 0
                 let custname = (attributes[0] as VisibleAttribute).Name
                 select custname
                ).ToList();
            InitializeComponent();
            BindingContext = this;
            Model = new LEDController();
            ColorSpectrum.Source = ImageSource.FromResource(nameof(HomeLedApp) + "." + "Assets.HueScale.png", typeof(MainPage).GetTypeInfo().Assembly);
            SinOptions.IsVisible = true;
            RainbowOptions.IsVisible = false;
        }
        public List<string> Modes { get; set; }

        private bool ModeUpdateInProgess;

        /// <summary>
        /// Picker_SelectedIndexChanged
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <exception cref="TypeLoadException"></exception>
        /// <exception cref="InvalidOperationException"></exception>
        private async void Picker_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sender is Picker p && p.SelectedItem is string newModeCustomName && !ModeUpdateInProgess)
            {
                ModeUpdateInProgess = true;
                var newModeName = (from name in typeof(LEDController.Modes).GetEnumNames()
                                   let attributes = typeof(LEDController.Modes).GetMember(name)[0].GetCustomAttributes(typeof(VisibleAttribute), false)
                                   let custname = (attributes[0] as VisibleAttribute).Name
                                   where custname == newModeCustomName
                                   select name
                ).FirstOrDefault();
                Enum.TryParse(newModeName, out LEDController.Modes newMode);
                Model.CurrentMode = newMode;
                DisAndEnableButtons(false);
                await Model.Send();
                DisAndEnableButtons(true);

                SinOptions.IsVisible = newMode == LEDController.Modes.sin;
                RainbowOptions.IsVisible = newMode == LEDController.Modes.rainbow;

                p.SelectedItem = null;
                ModeUpdateInProgess = false;
            }
        }

        private void Refresh(object sender, EventArgs e)
        {
            SSDPInstance.SearchForDevicesAsync();
        }

        private void DisAndEnableButtons(bool Enable)
        {
            foreach (var item in new[] { ModesView }.Concat(ControlPanel.Children).Concat(StorageCTRL.Children))
            {
                item.IsEnabled = Enable;
            }
        }

        private async void Clear_Click(object sender, EventArgs e)
        {
            DisAndEnableButtons(false);
            await Model.Send("config=clear");
            DisAndEnableButtons(true);
        }

        private async void Load_Click(object sender, EventArgs e)
        {
            DisAndEnableButtons(false);
            await Model.Send("config=load");
            DisAndEnableButtons(true);
        }

        private async void Save_Click(object sender, EventArgs e)
        {
            DisAndEnableButtons(false);
            await Model.Send("config=save");
            DisAndEnableButtons(true);
        }

        private async void Apply(object sender, EventArgs e)
        {
            DisAndEnableButtons(false);
            await Model.Send();
            DisAndEnableButtons(true);
        }

        private void ShowDetails(object sender, EventArgs e)
        {
            DisplayAlert("The LED Server returned:", Model.Status, "OK");
        }

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