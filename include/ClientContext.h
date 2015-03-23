class ClientContext {
    const PluginContext::PluginSettings& settings;
    string pf_file;
    vector<string> routes;
    static const unordered_map<string,string> CIDR;
    const static string CIDRtoMask(string ip);
public:
    ClientContext(const PluginContext&);
    static const string GetEnv(const string& key, const char* const envp[]);
    static long FindEnv(const string& key, const char* const envp[]);
    int verifyUser(const char** const args);
    int configUser(const char** const args, openvpn_plugin_args_func_return& ret);
    void setPfFile(const char** const args);
    bool PopulateAllowedSubnets(LDAPQuerier& querier, LDAPObject& data, ofstream&& pfFile);
};
