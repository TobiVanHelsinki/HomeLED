//Author: Tobi van Helsinki

using HomeLedApp.Model;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Rg.Plugins.Popup.Pages;
using Rg.Plugins.Popup.Services;
using Xamarin.Forms.Xaml;

namespace HomeLedApp.UI
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class Rename : PopupPage, INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        public LEDController Model { get; set; }
        string _Hostname;
        public string Hostname
        {
            get { return _Hostname; }
            set { if (_Hostname != value) { _Hostname = value; NotifyPropertyChanged(); } }
        }

        public Rename(LEDController model, LEDDevice device)
        {
            Hostname = device.HostName;
            Model = model;
            InitializeComponent();
            BindingContext = this;
            editor.Focus();
        }

        private void OK(object sender, System.EventArgs e)
        {
            Model.UpdateHostname(Hostname);
            PopupNavigation.Instance.PopAsync();
        }
    }
}