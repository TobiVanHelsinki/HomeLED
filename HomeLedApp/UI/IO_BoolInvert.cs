﻿//Author: Tobi van Helsinki

using System;
using System.Globalization;
using Xamarin.Forms;

namespace HomeLedApp.UI
{
    internal class IO_BoolInvert : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool b)
            {
                return !b;
            }
            return value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool b)
            {
                return !b;
            }
            return value;
        }
    }
}