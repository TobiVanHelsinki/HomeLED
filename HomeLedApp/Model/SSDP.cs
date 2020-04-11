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
            try
            {
                _DeviceLocator.DeviceAvailable += async (sender, e) => AddToDevices(await e.DiscoveredDevice.GetDeviceInfo());
                _DeviceLocator.DeviceUnavailable += async (sender, e) => RemoveFromDevices(await e.DiscoveredDevice.GetDeviceInfo());
                _DeviceLocator.StartListeningForNotifications();
            }
            catch (Exception)
            {
                //TODO Log
            }
            RefreshTimer.Interval = TimerInterval;
            RefreshTimer.Elapsed += RefreshTimer_Elapsed;
            PropertyChanged += SSDP_PropertyChanged;
            _DeviceLocator.SearchAsync().ContinueWith(x => RefreshTimer.Start());
        }

        ~SSDP()
        {
            try
            {
                _DeviceLocator.StopListeningForNotifications();
            }
            catch (Exception)
            {
                //TODO Log
            }
        }

        public void CreateInstance()
        {
            //Handled at Instance
        }

        private void RefreshTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            try
            {
                _ = SearchForDevices();
            }
            catch (Exception)
            {
                //TODO Log
            }
        }

        private void SSDP_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(TimerInterval))
            {
                RefreshTimer.Interval = TimerInterval;
            }
        }

        private void AddToDevices(SsdpDevice device)
        {
            if (device.DeviceType == DeviceTypeName)
            {
                AddToDevices(device.FriendlyName, IPAddress.None);
            }
        }

        private void AddToDevices(string hostName, IPAddress ipAddress)
        {
            var element = DiscoveredDevices.FirstOrDefault(x => x.HostName == hostName);
            void AlterOnMainThread()
            {
                if (element is null)
                {
                    DiscoveredDevices.Add(new LEDDevice(hostName, ipAddress));
                }
                else if (element.IP != ipAddress)
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
        /// <exception cref="System.IO.IOException"></exception>
        public async Task<IEnumerable<LEDDevice>> SearchForDevices()
        {
            if (_SearchinProgress)
            {
                return Array.Empty<LEDDevice>();
            }
            _SearchinProgress = true;
            foreach (var item in DiscoveredDevices)
            {
                item.IsUpToDate = false;
            }
            try
            {
                _ = _DeviceLocator.SearchAsync(); //TODO check ob das das selbe macht wie der restliche code
            }
            catch (Exception)
            {
                //TODO Log
            }
            _SearchinProgress = false;
            return DiscoveredDevices;
            using (var deviceLocator = new SsdpDeviceLocator())
            {
                var foundDevices = await (string.IsNullOrEmpty(SearchTarget) ? deviceLocator.SearchAsync() : deviceLocator.SearchAsync(SearchTarget)); // Can pass search arguments here (device type, uuid). No arguments means all devices.

                foreach (var foundDevice in foundDevices)
                {
                    try
                    {
                        var fullDevice = await foundDevice.GetDeviceInfo();
                        AddToDevices(fullDevice);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Ex: " + ex.Message);
                    }
                }
            }
        }
    }
}