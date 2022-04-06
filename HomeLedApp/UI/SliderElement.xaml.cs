//Author: Tobi van Helsinki

using System;
using System.ComponentModel;
using System.Linq;
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

        public string Type { get; set; }

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
                    ValueSlider.SetBinding(Slider.MaximumProperty, _Property + "_Max", BindingMode.OneWay); //Test: Max vor min.
                    ValueSlider.SetBinding(Slider.MinimumProperty, _Property + "_Min", BindingMode.OneWay);
                    ValueSpan.SetBinding(Span.TextProperty, _Property, BindingMode.OneWay, null, Type == "mul" ? "{0:0.00}": "{0:0}");
                }
            }
        }

        private string _ColorBindingPath;
        public string ColorBindingPath
        {
            get => _ColorBindingPath;
            set
            {
                if (_ColorBindingPath != value)
                {
                    _ColorBindingPath = value; NotifyPropertyChanged();
                    //ValueSlider.SetBinding(Slider.ThumbColorProperty, _ColorBindingPath, BindingMode.OneWay);
                    //ValueSlider.SetBinding(Slider.MinimumTrackColorProperty, _ColorBindingPath, BindingMode.OneWay);
                }
            }
        }

        private bool _DetailSteps = false;
        public bool DetailSteps
        {
            get => _DetailSteps;
            set
            {
                if (_DetailSteps != value)
                {
                    _DetailSteps = value; NotifyPropertyChanged();
                    PlusBtn.IsVisible = value;
                    MinusBtn.IsVisible = value;
                    PlusBtnColumn.Width = 50;
                    MinusBtnColumn.Width = 50;
                }
            }
        }

        public SliderElement() => InitializeComponent();

        private void Info_Tapped(object sender, EventArgs e)
        {
            MainPage.Instance.DisplayAlert(AppResources.Info, AppResources.ResourceManager.GetStringSafe(_TitleResource + "_Info"), "OK");
        }

        private void MinusBtn_Clicked(object sender, EventArgs e)
        {
            var ctrl = (BindingContext as MainPage)?.Model;
            var prop = ctrl.GetType().GetProperty(_Property.Split('.').LastOrDefault());
            var oldval = prop.GetValue(ctrl);
            prop.SetValue(ctrl,
                oldval is double d ? d - 1.0 :
                oldval is int i ? i - 1 :
                oldval);
        }

        private void PlusBtn_Clicked(object sender, EventArgs e)
        {
            var ctrl = (BindingContext as MainPage)?.Model;
            var prop = ctrl.GetType().GetProperty(_Property.Split('.').LastOrDefault());
            var oldval = prop.GetValue(ctrl);
            prop.SetValue(ctrl, 
                oldval is double d ? d + 1.0 :
                oldval is int i ? i + 1 :
                oldval);
        }
    }
}