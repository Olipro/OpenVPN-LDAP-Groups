class PluginContext
{
public:
      struct PluginSettings {
	vector<string> LDAPuris;
	string LDAPuserDN;
	string LDAPuserPW;
	string LDAPbaseDN;
	string LDAPusrFilter;
	string LDAPgrpAttrib;
	string LDAPipAttrib;
	int LDAPtimeout;
	vector<string> vpnSubnets;
    };
    plugin_log_t const openvpn_log;
    PluginContext(const string&, const plugin_log_t);
    const PluginSettings& getSettings() const;
private:
    PluginSettings settings;
    template <typename T>
    static T get(YAML::Node&, const string&&);
    template <typename T>
    static T get(YAML::Node&, const string&&, T);
};
