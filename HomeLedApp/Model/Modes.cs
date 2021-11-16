//Author: Tobi van Helsinki

using System.Collections.Generic;
using System.Linq;
using HomeLedApp.Strings;

namespace HomeLedApp.Model
{
    public enum Modes
    {
        on,
        off,
        sin,
        rainbow,
        c,
        c2,
        demo,
        pulse,
        tyke,
        knightrider,
    }

    public static class ModesExtension
    {
        public static IEnumerable<string> GetModesDisplayNames()
        {
            return from name in typeof(Modes).GetEnumNames()
                   let custname = AppResources.ResourceManager.GetStringSafe("Mode_" + name)
                   select custname;
        }

        public static IEnumerable<(string DispalyName, Modes Mode)> GetModesDisplayNamesAndValues()
        {
            return typeof(Modes).GetEnumNames().Zip(typeof(Modes).GetEnumValues().OfType<Modes>(), (a, b) => (AppResources.ResourceManager.GetStringSafe("Mode_" + a), b));
        }
    }
}