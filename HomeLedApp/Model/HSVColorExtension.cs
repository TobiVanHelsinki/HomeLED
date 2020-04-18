//Author: Tobi van Helsinki

using System;
using Xamarin.Forms;

namespace HomeLedApp.Model
{
    public static class HSVColorExtension
    {
        public static void ToHSV(this Color color, out double hue, out double saturation, out double value)
        {
            var max = Math.Max(color.R, Math.Max(color.G, color.B));
            var min = Math.Min(color.R, Math.Min(color.G, color.B));

            hue = color.Hue;
            saturation = (max == 0) ? 0 : 1d - 1d * min / max;
            value = max / 255d;
        }

        public static double Saturation_HSV(this Color color)
        {
            var max = Math.Max(color.R, Math.Max(color.G, color.B));
            var min = Math.Min(color.R, Math.Min(color.G, color.B));
            return (max == 0) ? 0 : 1d - min / max;
        }

        public static double Value_HSV(this Color color)
        {
            var max = Math.Max(color.R, Math.Max(color.G, color.B));
            return max;
        }

        public static Color ColorFromHSV(double hue, double saturation, double value)
        {
            var hi = Convert.ToInt32(Math.Floor(hue / 60)) % 6;
            var f = hue / 60 - Math.Floor(hue / 60);

            value *= 255;
            var v = Convert.ToInt32(value);
            var p = Convert.ToInt32(value * (1 - saturation));
            var q = Convert.ToInt32(value * (1 - f * saturation));
            var t = Convert.ToInt32(value * (1 - (1 - f) * saturation));

            if (hi == 0)
            {
                return Color.FromRgb(v, t, p);
            }
            else if (hi == 1)
            {
                return Color.FromRgb(q, v, p);
            }
            else if (hi == 2)
            {
                return Color.FromRgb(p, v, t);
            }
            else if (hi == 3)
            {
                return Color.FromRgb(p, q, v);
            }
            else if (hi == 4)
            {
                return Color.FromRgb(t, p, v);
            }
            else
            {
                return Color.FromRgb(v, p, q);
            }
        }
    }
}