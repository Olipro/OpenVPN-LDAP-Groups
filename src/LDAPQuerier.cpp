#include "stdafx.h"
#include "LDAPQuerier.h"

const int LDAPQuerier::LDAPVersion = 3;

LDAPQuerier::LDAPQuerier(const string& uri, const string& dn, const string& password, const int timeout) : timeout{timeout, 0}
{
    int result = tryBind(uri, dn, password);
    if (result != LDAP_SUCCESS)
	throw runtime_error((const char*)ldap_err2string(result));
}

LDAPQuerier::LDAPQuerier(const vector<string>& uris, const string& dn, const string& password, const int timeout) : timeout{timeout, 0}
{
    for (auto&& uri : uris)
	if (tryBind(uri, dn, password) == LDAP_SUCCESS)
	    return;
    throw runtime_error("Failed to bind to any of the provided LDAP servers on DN " + dn);
}

int LDAPQuerier::tryBind(const string& uri, const string& dn, const string& password)
{
    unique_ptr<char[]> passwd(new char[password.length()]);
    copy(password.begin(), password.end(), passwd.get());
    berval creds{password.length(), passwd.get()}, *srvCred;

    ldap_initialize(&ld, uri.c_str());
    ldMgr.reset(ld);
    ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &LDAPVersion);
    ldap_set_option(ld, LDAP_OPT_REFERRALS, LDAP_OPT_OFF); //Required for successful AD bind.
    ldap_set_option(ld, LDAP_OPT_NETWORK_TIMEOUT, &timeout);
    return ldap_sasl_bind_s(ld, dn.c_str(), nullptr, &creds, nullptr, nullptr, &srvCred);
}

LDAPObject LDAPQuerier::GetObjects(const string& base, int scope, const string& filter, vector<const char*>&& attrs, bool attrsOnly, timeval *timeout, int sizeLimit)
{
    if (ldap_search_ext_s(ld, base.c_str(), scope, filter.empty() ? nullptr : filter.c_str(), attrs.empty() ? nullptr : (char**)attrs.data(), attrsOnly, nullptr, nullptr, timeout, sizeLimit, &query) != LDAP_SUCCESS)
	return LDAPObject();
    unique_ptr<LDAPMessage, function<void(LDAPMessage*)>> queryPtr(query, [](LDAPMessage *m) { ldap_msgfree(m); });
    if (ldap_count_entries(ld, query) == 0)
	return LDAPObject();
    LDAPObject ret;
    static const auto&& memfree_clbk = [] (char *ptr) { ldap_memfree(ptr); };
    for (auto i = ldap_first_entry(ld, query); i != nullptr; i = ldap_next_entry(ld, i))
    {
	LDAPEntry entry;
	unique_ptr<char, function<void(char*)>> dn(ldap_get_dn(ld, i), memfree_clbk);
	entry.dn = dn.get();
	BerElement *ber;
	unique_ptr<char[], function<void(char*)>> j(ldap_first_attribute(ld, i, &ber), memfree_clbk);
	unique_ptr<BerElement, function<void(BerElement*)>> berPtr(ber, [](BerElement* ber) { ber_free(ber, 0); });
	for (; j; j.reset(ldap_next_attribute(ld, i, ber)))
	{
	    auto&& attrib = entry.attribs[j.get()];
	    unique_ptr<berval*[], function<void(berval**)>> values(ldap_get_values_len(ld, i, j.get()), [](berval** v) { ldap_value_free_len(v); });
	    for (size_t i = 0; values[i]; i++)
		attrib.emplace_back(values[i]->bv_val, values[i]->bv_len);
	}
	ret.entries.emplace_back(move(entry));
    }
    return ret;
}
