//Author: Tobi van Helsinki

using HomeLedApp.Platform;
using HomeLedApp.iOS.Platform;
using Xamarin.Forms;
using Xamarin.Forms.Platform.iOS;

[assembly: Dependency(typeof(IOSFormsInteractions))]

namespace HomeLedApp.iOS.Platform
{
    public class IOSFormsInteractions : IFormsInteractions
    {
        public object GetRenderer(VisualElement source)
        {
            try
            {
                return source.GetRenderer()?.Element;
            }
            catch (System.Exception)
            {
                return null;
            }
        }
    }
}