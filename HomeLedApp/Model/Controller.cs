using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace HomeLedApp.Model
{
    public class Controller : INotifyPropertyChanged
    {
        #region NotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
        string _Name;

        public string Name
        {
            get { return _Name; }
            set { if (_Name != value) { _Name = value; NotifyPropertyChanged(); } }
        }
        public string Urlbase { get; set; }

        public Controller(string urlbase, string name)
        {
            Name = name;
            Urlbase = urlbase;
        }

        public override string ToString()
        {
            return Urlbase + "  " + Name;
        }
    }
}
