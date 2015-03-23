#include "stdafx.h"
#include "PluginContext.h"

PluginContext::PluginContext(const string& file)
{
    YAML::Node cfg = YAML::LoadFile(file)["config"];
    settings = { get(cfg, "LDAPuri"),
		 get(cfg, "LDAPuserDN"),
		 get(cfg, "LDAPuserPW"),
		 get(cfg, "LDAPbaseDN"),
		 get(cfg, "LDAPusrFilter"),
		 get(cfg, "LDAPgrpAttrib"),
		 get(cfg, "LDAPipAttrib"),
		};
}

const PluginContext::PluginSettings& PluginContext::getSettings() const
{
    return settings;
}

inline const string PluginContext::get(YAML::Node& cfg, const string&& key)
{
    return cfg[key].as<string>();
}
