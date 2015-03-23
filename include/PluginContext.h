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
    };
    PluginContext(const string&);
    const PluginSettings& getSettings() const;
private:
    PluginSettings settings;
    static const string get(YAML::Node&, const string&&);
};
