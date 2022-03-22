//Author: Tobi van Helsinki

using System;
using System.Globalization;
using Xamarin.Forms;

namespace HomeLedApp.UI
{
    internal class IO_JustWhite : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is Color c)
            {
                return new Color (c.A, c.A, c.A);
            }
            return value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return value;
        }
    }
}