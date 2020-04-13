//Author: Tobi van Helsinki

using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace HomeLedApp.Platform
{
    public interface IFormsInteractions
    {
        object GetRenderer(VisualElement source);
    }
}