//Author: Tobi van Helsinki

using System;
using System.Globalization;
using Xamarin.Forms;

namespace HomeLedApp.UI
{
    internal class IO_RemoveWhite : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is Color c)
            {
                return new Color(c.R, c.G, c.B);
            }
            return value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value;
        }
    }
}