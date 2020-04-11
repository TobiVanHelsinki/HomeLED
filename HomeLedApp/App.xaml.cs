//Author: Tobi van Helsinki

using HomeLedApp.Model;
using HomeLedApp.UI;
using TLIB;
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
            Log.IsInMemoryLogEnabled = true;
            Log.IsFileLogEnabled = false;
            Log.Mode = LogMode.Verbose;
            Log.NewLogArrived += Log_NewLogArrived;

            SSDP.Instance.CreateInstance();

            InitializeComponent();

            MainPage = new MainPage();
        }

        private void Log_NewLogArrived(LogMessage logmessage)
        {
            System.Diagnostics.Debug.WriteLine(logmessage.CombinedMessage);
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