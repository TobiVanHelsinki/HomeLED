//Author: Tobi van Helsinki

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Timers;
using Rssdp;
using TLIB;
using Xamarin.Essentials;

namespace HomeLedApp.Model
{
    public class SSDP : INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        //https://github.com/Yortw/RSSDP
        private const string SearchTarget = null;
        private const string DeviceTypeName = "ImperialHomeLED";
        //private const string SearchTarget = "urn:schemas-upnp-org:device:Basic:1";
        //private const string SearchTarget = "urn:schemas-upnp-org:device:ImperialHomeLED:1";

        protected static SSDP _Instance;
        public static SSDP Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = new SSDP();
                }
                return _Instance;
            }
        }

        private ObservableCollection<LEDDevice> _DiscoveredDevices = new ObservableCollection<LEDDevice>();
        public ObservableCollection<LEDDevice> DiscoveredDevices
        {
            get => _DiscoveredDevices;
            set { if (_DiscoveredDevices != value) { _DiscoveredDevices = value; NotifyPropertyChanged(); } }
        }

        // Define _DeviceLocator as a field so it doesn't get GCed after the method ends, and it can
        // continue to listen for notifications until it is explicitly stopped (with a call to _DeviceLocator.StopListeningForNotifications();)
        private readonly SsdpDeviceLocator _DeviceLocator;

        private double _TimerInterval = 30_000;
        public double TimerInterval
        {
            get => _TimerInterval;
            set { if (_TimerInterval != value) { _TimerInterval = value; NotifyPropertyChanged(); } }
        }

        private readonly Timer RefreshTimer = new Timer();

        public SSDP()
        {
            _DeviceLocator = new SsdpDeviceLocator
            {
                NotificationFilter = SearchTarget
            };
            _DeviceLocator.DeviceAvailable += async (sender, e) => AddToDevices(await GetDeviceOrNull(e.DiscoveredDevice));
            _DeviceLocator.DeviceUnavailable += async (sender, e) => RemoveFromDevices(await GetDeviceOrNull(e.DiscoveredDevice));
            try
            {
                _DeviceLocator.StartListeningForNotifications();
            }
            catch (Exception ex)
            {
                Log.Write("Could not start listening", ex, logType: LogType.Error);
            }
            RefreshTimer.Interval = TimerInterval;
            RefreshTimer.Elapsed += RefreshTimer_Elapsed;
            PropertyChanged += SSDP_PropertyChanged;
            try
            {
                SearchForDevicesAsync().ContinueWith(x => RefreshTimer.Start());
            }
            catch (Exception ex)
            {
                Log.Write("Could not search async and start timer", ex, logType: LogType.Error);
            }
        }

        ~SSDP()
        {
            try
            {
                _DeviceLocator.StopListeningForNotifications();
            }
            catch (Exception ex)
            {
                Log.Write("Could not stop listening", ex, logType: LogType.Error);
            }
        }

        public void CreateInstance()
        {
            //Handled at Instance
        }

        private void RefreshTimer_Elapsed(object sender, ElapsedEventArgs e) => _ = SearchForDevicesAsync();

        private void SSDP_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(TimerInterval))
            {
                RefreshTimer.Interval = TimerInterval;
            }
        }

        private void AddToDevices(SsdpDevice device)
        {
            if (device is null)
            {
                return;
            }
            if (device.DeviceType == DeviceTypeName)
            {
                if (device is SsdpRootDevice rootDevice && IPAddress.TryParse(rootDevice.UrlBase.Host, out var ip))
                {
                    AddToDevices(device.FriendlyName, ip);
                }
                else
                {
                    AddToDevices(device.FriendlyName, IPAddress.None);
                }
            }
        }

        private void AddToDevices(string hostName, IPAddress ipAddress)
        {
            void AlterOnMainThread()
            {
                var element = DiscoveredDevices.FirstOrDefault(x => x.HostName == hostName);
                if (element is null)
                {
                    DiscoveredDevices.Add(new LEDDevice(hostName, ipAddress));
                }
                else if (element.IP.GetHashCode() != ipAddress.GetHashCode())
                {
                    element.IP = ipAddress;
                    element.IsUpToDate = true;
                }
            }
            if (MainThread.IsMainThread)
            {
                AlterOnMainThread();
            }
            else
            {
                MainThread.BeginInvokeOnMainThread(AlterOnMainThread);
            }
        }

        private void RemoveFromDevices(SsdpDevice device) => RemoveFromDevices(device.FriendlyName);

        private void RemoveFromDevices(string hostName)
        {
            void AlterOnMainThread()
            {
                foreach (var item in DiscoveredDevices.Where(x => x.HostName == hostName).ToArray())
                {
                    DiscoveredDevices.Remove(item);
                }
            }
            if (MainThread.IsMainThread)
            {
                AlterOnMainThread();
            }
            else
            {
                MainThread.BeginInvokeOnMainThread(AlterOnMainThread);
            }
        }

        private bool _SearchinProgress;

        /// <summary>
        /// SearchForDevices
        /// </summary>
        /// <returns></returns>
        public async Task<IEnumerable<LEDDevice>> SearchForDevicesAsync()
        {
            if (_SearchinProgress)
            {
                return Array.Empty<LEDDevice>();
            }
            _SearchinProgress = true;
            Log.Write("Searching for Devices Now");
            foreach (var item in DiscoveredDevices)
            {
                item.IsUpToDate = false;
            }
            try
            {
                foreach (var foundDevice in await _DeviceLocator.SearchAsync())
                {
                    AddToDevices(await GetDeviceOrNull(foundDevice));
                }
            }
            catch (Exception ex)
            {
                Log.Write("Could not search devices", ex, logType: LogType.Error);
            }
            _SearchinProgress = false;
            return DiscoveredDevices;
        }

        /// <summary>
        /// Gets the device or null in error case.
        /// </summary>
        /// <param name="device">The device.</param>
        /// <returns></returns>
        private static async Task<SsdpDevice> GetDeviceOrNull(DiscoveredSsdpDevice device)
        {
            try
            {
                return await device.GetDeviceInfo();
            }
            catch (Exception ex)
            {
                Log.Write("Could not get Device Info", ex, logType: LogType.Error);
                return null;
            }
        }

        /// <summary>
        /// PrintDevice
        /// </summary>
        /// <param name="fullDevice"></param>
        /// <exception cref="System.IO.IOException"></exception>
        private static SsdpDevice PrintFullDevice(SsdpDevice fullDevice)
        {
            foreach (var property in fullDevice.GetType().GetProperties())
            {
                var value = property.GetValue(fullDevice);
                if (value is string s)
                {
                    Log.Write(property.Name + ":--" + s + "--");
                }
                else if (value is System.Collections.IEnumerable set)
                {
                    foreach (var item in set)
                    {
                        Log.Write(property.Name + ":--" + item + "--");
                    }
                }
                else
                {
                    Log.Write(property.Name + ":--" + value + "--");
                }
            }
            Log.Write("--");
            return fullDevice;
        }
    }
}