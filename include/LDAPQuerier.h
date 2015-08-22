struct LDAPEntry {
    string dn;
    unordered_map<string,list<string>> attribs;
};

struct LDAPObject
{
    list<LDAPEntry> entries;
};

class LDAPQuerier
{
    LDAP *ld;
    unique_ptr<LDAP,function<void(LDAP*)>> ldMgr{nullptr, [](LDAP* ld) { ldap_unbind_ext_s(ld, nullptr, nullptr); }};
    LDAPMessage *query;
    static const int LDAPVersion;
    static const timeval timeout;
    int tryBind(const string&, const string&, const string&);
public:
    LDAPQuerier(const string&, const string&, const string&);
    LDAPQuerier(const vector<string>&, const string&, const string&);
    LDAPObject GetObjects(const string&, int, const string&, vector<const char*>&&, bool, timeval*, int);
};