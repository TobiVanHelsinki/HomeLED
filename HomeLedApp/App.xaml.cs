//Author: Tobi van Helsinki

using HomeLedApp.Model;
using HomeLedApp.UI;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

[assembly: XamlCompilation(XamlCompilationOptions.Compile)]

namespace HomeLedApp
{
    public partial class App : Application
    {
        /// <summary>
        /// ctor
        /// </summary>
        /// <exception cref="System.TypeLoadException"></exception>
        /// <exception cref="System.InvalidOperationException"></exception>
        public App()
        {
            SSDP.Instance.CreateInstance();
            InitializeComponent();

            MainPage = new MainPage();
        }

        protected override void OnStart()
        {
            // Handle when your app starts
        }

        protected override void OnSleep()
        {
            // Handle when your app sleeps
        }

        protected override void OnResume()
        {
            // Handle when your app resumes
        }
    }
}