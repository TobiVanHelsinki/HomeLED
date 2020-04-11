//Author: Tobi van Helsinki

using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using Rssdp;

namespace HomeLedApp.Model
{
    public class SSDP
    {
        //https://github.com/Yortw/RSSDP
        private const string SearchTarget = "";
        private const string DeviceTypeName = "ImperialHomeLED";

        //private const string SearchTarget = "urn:schemas-upnp-org:device:Basic:1";
        //private const string SearchTarget = "urn:schemas-upnp-org:device:ImperialHomeLED:1";

        /// <summary>
        /// SearchForDevices
        /// </summary>
        /// <returns></returns>
        /// <exception cref="System.IO.IOException"></exception>
        public static async Task<IEnumerable<string>> SearchForDevices()
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("Searching For Devices now!");
            Console.ForegroundColor = ConsoleColor.White;
            var devices = new List<string>();
            using (var deviceLocator = new SsdpDeviceLocator())
            {
                var foundDevices = await (string.IsNullOrEmpty(SearchTarget) ? deviceLocator.SearchAsync() : deviceLocator.SearchAsync(SearchTarget)); // Can pass search arguments here (device type, uuid). No arguments means all devices.

                foreach (var foundDevice in foundDevices)
                {
                    try
                    {
                        var fullDevice = await GetAndPrintFoundDevice(foundDevice);
                        if (fullDevice.DeviceType == DeviceTypeName)
                        {
                            devices.Add(fullDevice.FriendlyName);
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Ex: " + ex.Message);
                    }
                }
            }
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("Finito. Type enter to repeat");
            Console.ForegroundColor = ConsoleColor.White;
            return devices;
        }

        // Define _DeviceLocator as a field so it doesn't get GCed after the method ends, and it can
        // continue to listen for notifications until it is explicitly stopped (with a call to _DeviceLocator.StopListeningForNotifications();)
        private static SsdpDeviceLocator _DeviceLocator;

        /// <summary>
        /// BeginSearch
        /// </summary>
        /// <exception cref="System.IO.IOException"></exception>
        /// <exception cref="Exception"></exception>
        public static void BeginSearch()
        {
            _DeviceLocator = new SsdpDeviceLocator
            {
                NotificationFilter = SearchTarget
            };
            _DeviceLocator.DeviceAvailable += DeviceLocator_DeviceAvailable;
            _DeviceLocator.DeviceUnavailable += DeviceLocator_DeviceUnavailable;
            _DeviceLocator.StartListeningForNotifications();
            _DeviceLocator.SearchAsync();
        }

        /// <summary>
        /// DeviceLocator_DeviceUnavailable
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <exception cref="System.IO.IOException"></exception>
        /// <exception cref="Exception"></exception>
        private static async void DeviceLocator_DeviceUnavailable(object sender, DeviceUnavailableEventArgs e)
        {
            Console.WriteLine("Discovered Device LogOff");
            await GetAndPrintFoundDevice(e.DiscoveredDevice);
        }

        /// <summary>
        /// DeviceLocator_DeviceAvailable
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <exception cref="System.IO.IOException"></exception>
        /// <exception cref="Exception"></exception>
        private static async void DeviceLocator_DeviceAvailable(object sender, DeviceAvailableEventArgs e)
        {
            Console.WriteLine("Discovered Device LogOn");
            await GetAndPrintFoundDevice(e.DiscoveredDevice);
        }

        /// <summary>
        /// PrintFoundDevice
        /// </summary>
        /// <param name="foundDevice"></param>
        /// <returns></returns>
        /// <exception cref="System.IO.IOException"></exception>
        /// <exception cref="Exception"></exception>
        private static async Task<SsdpDevice> GetAndPrintFoundDevice(DiscoveredSsdpDevice foundDevice)
        {
            var fullDevice = await foundDevice.GetDeviceInfo();
            PrintFullDevice(fullDevice);
            return fullDevice;
        }

        /// <summary>
        /// PrintDevice
        /// </summary>
        /// <param name="fullDevice"></param>
        /// <exception cref="System.IO.IOException"></exception>
        private static void PrintFullDevice(SsdpDevice fullDevice)
        {
            if (fullDevice.DeviceType == DeviceTypeName)
            {
                Console.ForegroundColor = ConsoleColor.Green;
            }
            foreach (var property in fullDevice.GetType().GetProperties())
            {
                var value = property.GetValue(fullDevice);
                if (value is string s)
                {
                    Console.WriteLine(property.Name + ":--" + s + "--");
                }
                else if (value is IEnumerable set)
                {
                    foreach (var item in set)
                    {
                        Console.WriteLine(property.Name + ":--" + item + "--");
                    }
                }
                else
                {
                    Console.WriteLine(property.Name + ":--" + value + "--");
                }
            }
            Console.WriteLine("--");
            Console.ForegroundColor = ConsoleColor.White;
        }
    }
}