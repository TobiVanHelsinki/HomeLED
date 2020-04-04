using HomeLedApp.Model;
using Rg.Plugins.Popup.Services;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using Xamarin.Forms;

namespace HomeLedApp.UI
{
    public partial class MainPage : ContentPage, INotifyPropertyChanged
	{
        #region NotifyPropertyChanged
		public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
        LEDController _Model;
        public LEDController Model
        {
            get { return _Model; }
            set { if (_Model != value) { _Model = value; NotifyPropertyChanged(); } }
        }

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
                ;

            InitializeComponent();
            BindingContext = this;
            Model = new LEDController();
            Model.PropertyChanged += Model_PropertyChanged;
            Model.AvailibleControllers.CollectionChanged += AvailibleControllers_CollectionChanged;
            Model.SearchCOntroller();
            Model.CurrentController = Model.AvailibleControllers.LastOrDefault();
            ColorSpectrum.Source = ImageSource.FromResource(nameof(HomeLedApp) + "." + "Assets.HueScale.png", typeof(MainPage).GetTypeInfo().Assembly);
            SinOptions.IsVisible = true;
            RainbowOptions.IsVisible = false;
        }
        public List<string> Modes { get; set; }

        bool ModeUpdateInProgess;
        /// <summary>
        /// Picker_SelectedIndexChanged
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <exception cref="TypeLoadException"></exception>
        /// <exception cref="InvalidOperationException"></exception>
        async void Picker_SelectedIndexChanged(object sender, EventArgs e)
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
        private void Model_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(Model.CurrentController))
            {
                HighlightCurrentController();
            }
        }

        private void AvailibleControllers_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                foreach (Controller item in e.NewItems)
                {
                    var b = new Button { Text = item.Name, BindingContext = item, Style = Resources[nameof(CIButton)] as Style, Margin = new Thickness(5,2,5,2) };
                    b.Clicked += Button_Clicked;
                    AvailibleControllerPanel.Children.Add(b);
                }
            }
            HighlightCurrentController();
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            if (sender is Button b)
            {
                if (b.BindingContext is Controller c)
                {
                    Model.CurrentController = c;
                }
            }
            HighlightCurrentController();
        }

        private void HighlightCurrentController()
        {
            foreach (var item in AvailibleControllerPanel.Children)
            {
                item.BackgroundColor = Model.CurrentController == item.BindingContext ? Color.Accent : Color.Default;
            }
        }

        void DisAndEnableButtons(bool Enable)
        {
            foreach (var item in new[] { ModesView }.Concat(ControlPanel.Children).Concat(StorageCTRL.Children))
            {
                item.IsEnabled = Enable;
            }
        }

        async void Clear_Click(object sender, EventArgs e)
        {
            DisAndEnableButtons(false);
            await Model.Send("config=clear");
            DisAndEnableButtons(true);
        }

        async void Load_Click(object sender, EventArgs e)
        {
            DisAndEnableButtons(false);
            await Model.Send("config=load");
            DisAndEnableButtons(true);
        }

        async void Save_Click(object sender, EventArgs e)
        {
            DisAndEnableButtons(false);
            await Model.Send("config=save");
            DisAndEnableButtons(true);
        }

        async void Apply(object sender, EventArgs e)
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
                PopupNavigation.Instance.PushAsync(new DetailsPopUp(Model));
            }
            catch (Exception)
            {
                if (System.Diagnostics.Debugger.IsAttached) System.Diagnostics.Debugger.Break();
            }
        }

    
    }
}
