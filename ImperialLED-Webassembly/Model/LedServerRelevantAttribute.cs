//Author: Tobi van Helsinki

using System;

namespace HomeLedApp.Model
{
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property, Inherited = true, AllowMultiple = false)]
    internal sealed class LedServerRelevantAttribute : Attribute
    {
        public string ParamName { get; }
        public object DefaultValue { get; }

        public LedServerRelevantAttribute(string paramname, object defaultValue)
        {
            DefaultValue = defaultValue;
            ParamName = paramname;
        }
    }
}