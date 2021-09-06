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
    public partial class DetailsPopUp : PopupPage, INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        public LEDController Model { get; set; }
        string _MyURLParam;
        public string MyURLParam
        {
            get { return _MyURLParam; }
            set { if (_MyURLParam != value) { _MyURLParam = value; NotifyPropertyChanged(); } }
        }

        public DetailsPopUp(LEDController Model)
        {
            this.Model = Model;
            MyURLParam = Model.URLParam;
            InitializeComponent();
            BindingContext = this;
            editor.Focus();
        }

        private async void OK(object sender, System.EventArgs e)
        {
            await Model.Send(MyURLParam);
            try
            {
                _ = PopupNavigation.Instance.PopAsync();
            }
            catch (System.Exception)
            {
            }
        }

        private void AddAmp(object sender, System.EventArgs e)
        {
            MyURLParam += "&";
        }

        private void AddEqu(object sender, System.EventArgs e)
        {
            MyURLParam += "=";
        }

        private void AddScale(object sender, System.EventArgs e)
        {
            MyURLParam += "Scale=0.";
        }

        private void AddDebug(object sender, System.EventArgs e)
        {
            MyURLParam += "Debug";
        }
    }
}