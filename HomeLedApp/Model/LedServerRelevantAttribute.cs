using System;

namespace HomeLedApp.Model
{
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property, Inherited = true, AllowMultiple = false)]
    sealed class LedServerRelevantAttribute : Attribute
    {
        public string ParamName { get; }
        public LedServerRelevantAttribute(string paramname)
        {
            ParamName = paramname;
        }
    }
}
