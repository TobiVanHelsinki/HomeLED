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

        private bool _IsSearching;
        public bool IsSearching
        {
            get => _IsSearching;
            set { if (_IsSearching != value) { _IsSearching = value; NotifyPropertyChanged(); } }
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
                    UpdateDeviceList(element);
                }
                else
                {
                    element.IsUpToDate = true;
                    UpdateDeviceList(element);
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

        private void RemoveFromDevices(string hostName = null, string ip = null)
        {
            if (string.IsNullOrEmpty(hostName) && string.IsNullOrEmpty(ip))
            {
                return;
            }
            if (string.IsNullOrEmpty(hostName))
            {
                hostName = "?"; // prevent it from search
            }
            if (string.IsNullOrEmpty(ip))
            {
                ip = "?"; // prevent it from search
            }
            void AlterOnMainThread()
            {
                foreach (var item in DiscoveredDevices.Where(x => x.HostName == hostName || x.IPString == ip).ToArray())
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

        /// <summary>
        /// SearchForDevices
        /// </summary>
        /// <returns></returns>
        public async Task<IEnumerable<LEDDevice>> SearchForDevicesAsync()
        {
            if (IsSearching)
            {
                return Array.Empty<LEDDevice>();
            }
            IsSearching = true;
            Log.Write("Searching for Devices Now");
            foreach (var item in DiscoveredDevices.ToArray())
            {
                item.IsUpToDate = false;
                UpdateDeviceList(item);
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
            IsSearching = false;
            return DiscoveredDevices;
        }

        /// <summary>
        /// Gets the device or null in error case.
        /// </summary>
        /// <param name="device">The device.</param>
        /// <returns></returns>
        private async Task<SsdpDevice> GetDeviceOrNull(DiscoveredSsdpDevice device)
        {
            try
            {
                return await device.GetDeviceInfo();
            }
            catch (Exception ex)
            {
                try
                {
                    if (DiscoveredDevices.FirstOrDefault(x => x.IPString == device.DescriptionLocation.Host) is LEDDevice led)
                    {
                        led.IsUpToDate = false;
                    }
                }
                catch (Exception)
                {
                }
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

        internal void UpdateDeviceList(LEDDevice currentDevice)
        {
            try
            {
                //DiscoveredDevices.Remove(currentDevice);
                //DiscoveredDevices.Add(currentDevice);
            }
            catch (Exception ex)
            {
                Log.Write("Could not Refresh Devices List", ex, logType: LogType.Error);
            }
        }
    }
}