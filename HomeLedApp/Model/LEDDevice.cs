﻿//Author: Tobi van Helsinki

using System.Net;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace HomeLedApp.Model
{
    public class LEDDevice : INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion NotifyPropertyChanged

        private string _HostName;
        public string HostName
        {
            get => _HostName;
            set { if (_HostName != value) { _HostName = value; NotifyPropertyChanged(); } }
        }

        private IPAddress _IP;

        public LEDDevice(string hostName, IPAddress iP)
        {
            HostName = hostName;
            IP = iP;
        }

        public IPAddress IP
        {
            get => _IP;
            set { if (_IP != value) { _IP = value; NotifyPropertyChanged(); } }
        }

        private bool _IsUpToDate = true;
        public bool IsUpToDate
        {
            get => _IsUpToDate;
            set { if (_IsUpToDate != value) { _IsUpToDate = value; NotifyPropertyChanged(); } }
        }

        /// <summary>
        /// Urlbase
        /// </summary>
        /// <exception cref="System.Net.Sockets.SocketException">Get.</exception>
        public string Urlbase => "http://" + (IP == IPAddress.None ? HostName : IP.ToString()) + "?";
    }
}