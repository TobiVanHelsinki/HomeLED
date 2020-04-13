//Author: Tobi van Helsinki

using HomeLedApp.Platform;
using System.Globalization;
using System.Threading;
using Xamarin.Forms;

[assembly: Dependency(typeof(HomeLedApp.iOS.Platform.Localize))]

namespace HomeLedApp.iOS.Platform
{
    public class Localize : ILocalize
    {
        public void SetLocale(CultureInfo ci)
        {
            Thread.CurrentThread.CurrentCulture = ci;
            Thread.CurrentThread.CurrentUICulture = ci;
        }

        public CultureInfo GetCurrentCultureInfo()
        {
            return CultureInfo.InvariantCulture;
        }
    }
}