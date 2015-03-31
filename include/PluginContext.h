class PluginContext
{
public:
      struct PluginSettings {
	string LDAPuri;
	string LDAPuserDN;
	string LDAPuserPW;
	string LDAPbaseDN;
	string LDAPusrFilter;
	string LDAPgrpAttrib;
	string LDAPipAttrib;
	vector<string> vpnSubnets;
    };
    plugin_log_t const openvpn_log;
    PluginContext(const string&, const plugin_log_t);
    const PluginSettings& getSettings() const;
private:
    PluginSettings settings;
    static const string get(YAML::Node&, const string&&);
};
