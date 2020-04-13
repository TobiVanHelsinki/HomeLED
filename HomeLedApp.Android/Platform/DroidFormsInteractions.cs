using HomeLedApp.Platform;
using HomeLedApp.Android.Platform;
using Xamarin.Forms;
using Xamarin.Forms.Platform.Android;

[assembly: Dependency(typeof(AndroidFormsInteractions))]
namespace HomeLedApp.Android.Platform
{
    public class AndroidFormsInteractions : IFormsInteractions
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
