//Author: Tobi van Helsinki

using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using HomeLedApp.Strings;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace HomeLedApp.UI
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SliderElement : ContentView, INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        private string _TitleResource;
        public string TitleResource
        {
            get => _TitleResource;
            set
            {
                if (_TitleResource != value)
                {
                    _TitleResource = value; NotifyPropertyChanged();
                    TitleLabel.Text = AppResources.ResourceManager.GetStringSafe(_TitleResource);
                }
            }
        }

        private string _Property;
        public string Property
        {
            get => _Property;
            set
            {
                if (_Property != value)
                {
                    _Property = value; NotifyPropertyChanged();
                    ValueSlider.SetBinding(Slider.ValueProperty, _Property, BindingMode.TwoWay); //TODO Verursacht löschen der std werte
                    ValueSlider.SetBinding(Slider.MaximumProperty, _Property + "_Max"); //Test: Max vor min.
                    ValueSlider.SetBinding(Slider.MinimumProperty, _Property + "_Min");
                    ValueSpan.SetBinding(Span.TextProperty, _Property, BindingMode.OneWay, null, "{0:0}");
                }
            }
        }

        public SliderElement() => InitializeComponent();

        private void Info_Tapped(object sender, EventArgs e)
        {
            MainPage.Instance.DisplayAlert(AppResources.Info, AppResources.ResourceManager.GetStringSafe(_TitleResource + "_Info"), "OK");
        }
    }
}