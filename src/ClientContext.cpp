#include "stdafx.h"
#include "VPNPlugin.h"
#include "LDAPQuerier.h"
#include "PluginContext.h"
#include "ClientContext.h"

const unordered_map<string,string> ClientContext::CIDR = 
{ {"0", "0.0.0.0"}, {"1", "128.0.0.0"}, {"2", "192.0.0.0"}, {"3", "224.0.0.0"}, {"4", "240.0.0.0"},
  {"5", "248.0.0.0"}, {"6", "252.0.0.0"}, {"7", "254.0.0.0"}, {"8", "255.0.0.0"},
  {"9", "255.128.0.0"}, {"10", "255.192.0.0"}, {"11", "255.224.0.0"}, {"12", "255.240.0.0"},
  {"13", "255.248.0.0"}, {"14", "255.252.0.0"}, {"15", "255.254.0.0"}, {"16", "255.255.0.0"},
  {"17", "255.255.128.0"}, {"18", "255.255.192.0"}, {"19", "255.255.224.0"}, {"20", "255.255.240.0"},
  {"21", "255.255.248.0"}, {"22", "255.255.252.0"}, {"23", "255.255.254.0"}, {"24", "255.255.255.0"},
  {"25", "255.255.255.128"}, {"26", "255.255.255.192"}, {"27", "255.255.255.224"}, {"28", "255.255.255.240"},
  {"29", "255.255.255.248"}, {"30", "255.255.255.252"}, {"31", "255.255.255.254"}, {"32", "255.255.255.255"}
};

ClientContext::ClientContext(const PluginContext& pCtx) : pluginCtx(pCtx) { }

int ClientContext::verifyUser(const char** const args)
{
    try {
	const auto& settings = pluginCtx.getSettings();
	LDAPQuerier querier(settings.LDAPuri, settings.LDAPuserDN, settings.LDAPuserPW);
	string filter = string(settings.LDAPusrFilter);
	const string&& username = GetEnv("username", args);
	pluginCtx.openvpn_log(PLOG_NOTE, PLUGIN_NAME, "Verifying user: %s", username.c_str());
	for (auto u = filter.find("%u"); u != string::npos; u = filter.find("%u"))
	    filter.replace(u, 2, username);
	auto&& result = querier.GetObjects(settings.LDAPbaseDN, LDAP_SCOPE_SUBTREE, filter, { settings.LDAPgrpAttrib.c_str(), nullptr }, false, nullptr, LDAP_NO_LIMIT);
	if (result.entries.empty() || result.entries.front().attribs.empty())
	    return OPENVPN_PLUGIN_FUNC_ERROR;
	LDAPQuerier(settings.LDAPuri, result.entries.front().dn, GetEnv("password", args)); //verify user
	return PopulateAllowedSubnets(querier, result, ofstream(pf_file, ofstream::out)) ? OPENVPN_PLUGIN_FUNC_SUCCESS : OPENVPN_PLUGIN_FUNC_ERROR;
    } catch (...) { }
    return OPENVPN_PLUGIN_FUNC_ERROR;
}

int ClientContext::configUser (const char** const args, openvpn_plugin_args_func_return& ret)
{
    auto& ptr = ret.return_list[0];
    ostringstream routeStr;
    for (auto&& ip : routes)
	routeStr << "push \"route " << ip << "\"" << endl;
    ptr = new (malloc(sizeof(*ptr))) openvpn_plugin_string_list();
    ptr->name = strdup("config");
    ptr->value = strdup(routeStr.str().c_str());
    return OPENVPN_PLUGIN_FUNC_SUCCESS;
}

void ClientContext::setPfFile (const char** const args)
{
    pf_file = GetEnv("pf_file", args);
}

bool ClientContext::PopulateAllowedSubnets(LDAPQuerier& querier, LDAPObject& data, ofstream&& pfFile)
{
    const auto& settings = pluginCtx.getSettings();
    
    pfFile << "[CLIENTS DROP]" << endl << "[SUBNETS DROP]" << endl;
    for (auto&& group : data.entries.front().attribs[settings.LDAPgrpAttrib])
    {
	auto&& result = querier.GetObjects(group, LDAP_SCOPE_BASE, "", { settings.LDAPipAttrib.c_str(), nullptr }, false, nullptr, LDAP_NO_LIMIT);
	if (!result.entries.empty() && !result.entries.front().attribs.empty())
	    for (auto&& ip : result.entries.front().attribs[settings.LDAPipAttrib])
		pfFile << "+" << ip << endl, routes.emplace_back(CIDRtoMask(ip));
    }
    pfFile << "[END]" << endl;
    return !routes.empty();
}

const string ClientContext::CIDRtoMask(string ip)
{
    const size_t subnet = ip.find("/");
    const string& cidr = CIDR.at(ip.substr(subnet+1));
    return ip.replace(subnet, string::npos, " " + cidr);
}

const string ClientContext::GetEnv(const string& key, const char* const envp[])
{
    long pos = FindEnv(key, envp);
    auto&& elem = string(envp[pos]);
    return pos != -1 ? elem.substr(elem.find('=') + 1, string::npos) : string();
}

long ClientContext::FindEnv(const string& key, const char* const envp[])
{
    if (!envp)
	return -1;
    for (int i = 0; envp[i]; i++)
    {
	const string elem = envp[i];
	if (elem.substr(0, elem.find('=')) == key)
	    return i;
    }
    return -1;
}

