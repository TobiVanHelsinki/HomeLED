﻿//Author: Tobi van Helsinki

using HomeLedApp.Model;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Rg.Plugins.Popup.Pages;
using Rg.Plugins.Popup.Services;
using Xamarin.Forms.Xaml;
using TLIB;
using Xamarin.Essentials;
using System;

namespace HomeLedApp.UI
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class LedCtrlSettings : PopupPage, INotifyPropertyChanged
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
        string _Hostname;
        public string Hostname
        {
            get { return _Hostname; }
            set { if (_Hostname != value) { _Hostname = value; NotifyPropertyChanged(); } }
        }

        private int _NumberLED = 1024;
        public int NumberLED
        {
            get => _NumberLED;
            set { if (_NumberLED != value) { _NumberLED = value; NotifyPropertyChanged(); } }
        }

        private int _DataPin;
        public int DataPin
        {
            get => _DataPin;
            set { if (_DataPin != value) { _DataPin = value; NotifyPropertyChanged(); } }
        }

        private string _LEDType;
        public string LEDType
        {
            get => _LEDType;
            set { if (_LEDType != value) { _LEDType = value; NotifyPropertyChanged(); } }
        }


        public LedCtrlSettings(LEDController model, LEDDevice device)
        {
            Model = model;
            InitializeComponent();
            BindingContext = this;
            Appearing += LedCtrlSettings_Appearing;
        }

        private async void LedCtrlSettings_Appearing(object sender, System.EventArgs e)
        {
            (DataPin, NumberLED, Hostname, LEDType) = await Model.GetSettings();
        }

        private void OK(object sender, System.EventArgs e)
        {
            Model.SetSettings(DataPin, NumberLED, Hostname, LEDType);
            PopupNavigation.Instance.PopAsync();
        }

        private void URL(object sender, System.EventArgs e)
        {
            try
            {
                Browser.OpenAsync("http://"+Model.CurrentDevice.IPString, BrowserLaunchMode.SystemPreferred);
            }
            catch (Exception ex)
            {
                TLIB.Log.Write("Open URL",ex);
                // An unexpected error occured. No browser may be installed on the device.
            }
        }
    }
}