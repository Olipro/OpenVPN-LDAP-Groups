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
	vector<string> vpnSubnets;
    };
    plugin_log_t const openvpn_log;
    PluginContext(const string&, const plugin_log_t);
    const PluginSettings& getSettings() const;
private:
    PluginSettings settings;
    static string get(YAML::Node&, const string&&);
    static vector<string> getVec(YAML::Node& cfg, const string&& key);
};
