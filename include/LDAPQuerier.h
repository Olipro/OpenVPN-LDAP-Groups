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
    LDAPMessage *query;
    static const int LDAPVersion;
public:
    LDAPQuerier(const string&, const string&, const string&);
    LDAPObject GetObjects(const string&, int, const string&, vector<const char*>&&, bool, timeval*, int);
    ~LDAPQuerier();
};