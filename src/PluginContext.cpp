#include "stdafx.h"
#include "PluginContext.h"

PluginContext::PluginContext(const string& file, plugin_log_t log) : openvpn_log(log)
{
    YAML::Node cfg = YAML::LoadFile(file)["config"];
    settings = { get<vector<string>>(cfg, "LDAPuri"),
		 get<string>(cfg, "LDAPuserDN"),
		 get<string>(cfg, "LDAPuserPW"),
		 get<string>(cfg, "LDAPbaseDN"),
		 get<string>(cfg, "LDAPusrFilter"),
		 get<string>(cfg, "LDAPgrpAttrib"),
		 get<string>(cfg, "LDAPipAttrib"),
		 get<int>(cfg, "LDAPtimeout", 10),
		 get<vector<string>>(cfg, "vpnSubnets"),
		};
}

const PluginContext::PluginSettings& PluginContext::getSettings() const
{
    return settings;
}

template <typename T>
inline T PluginContext::get(YAML::Node& cfg, const string&& key)
{
    return cfg[key].as<T>();
}

template <typename T>
inline T PluginContext::get(YAML::Node& cfg, const string&& key, T dflt)
{
    return cfg[key] ? cfg[key].as<T>() : dflt;
}
