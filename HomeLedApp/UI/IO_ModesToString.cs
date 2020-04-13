//Author: Tobi van Helsinki

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using HomeLedApp.Model;
using HomeLedApp.Strings;
using Xamarin.Forms;

namespace HomeLedApp.UI
{
    internal class IO_ModesToString : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is Modes m)
            {
                return ModesExtension.GetModesDisplayNamesAndValues().FirstOrDefault(x => x.Mode == m).DispalyName;
            }
            return value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is string s)
            {
                return ModesExtension.GetModesDisplayNamesAndValues().FirstOrDefault(x => x.DispalyName == s).Mode;
            }
            return value;
        }
    }
}