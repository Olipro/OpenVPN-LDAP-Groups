#include "stdafx.h"
#include "PluginContext.h"

PluginContext::PluginContext(const string& file, plugin_log_t log) : openvpn_log(log)
{
    YAML::Node cfg = YAML::LoadFile(file)["config"];
    settings = { getVec(cfg, "LDAPuri"),
		 get(cfg, "LDAPuserDN"),
		 get(cfg, "LDAPuserPW"),
		 get(cfg, "LDAPbaseDN"),
		 get(cfg, "LDAPusrFilter"),
		 get(cfg, "LDAPgrpAttrib"),
		 get(cfg, "LDAPipAttrib"),
		 getVec(cfg, "vpnSubnets"),
		};
}

const PluginContext::PluginSettings& PluginContext::getSettings() const
{
    return settings;
}

inline string PluginContext::get(YAML::Node& cfg, const string&& key)
{
    return cfg[key].as<string>();
}

inline vector<string> PluginContext::getVec(YAML::Node& cfg, const string&& key)
{
    return cfg[key].as<vector<string>>();
}
