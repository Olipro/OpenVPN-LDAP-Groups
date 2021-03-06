class ClientContext {
    const PluginContext& pluginCtx;
    string pf_file;
    vector<string> routes;
    static const string CIDR[];
    static string CIDRtoMask(string);
public:
    ClientContext(const PluginContext&);
    static string GetEnv(const string&, const char* const[]);
    static long FindEnv(const string&, const char* const[]);
    int verifyUser(const char** const);
    int configUser(const char** const, openvpn_plugin_args_func_return&);
    void setPfFile(const char** const);
    bool PopulateAllowedSubnets(LDAPQuerier&, LDAPObject&, ofstream&&);
};
