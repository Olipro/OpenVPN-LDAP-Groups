# OpenVPN LDAP Groups Plugin

## About

This plugin is designed to connect to an LDAP backend in order to authenticate clients and route permitted subnets to them.

If you're using Active Directory, you will need to use ADSI Edit in order to define a new attribute on objects that this plugin will then read.

Typically the best way to use this plugin is to create Security Groups and then specify permitted subnet(s) on those groups.

This plugin will route all subnets of groups that a user is a member of. Thereby allowing you to very granularly control access.

## Simplified Explanation

You're probably wondering what this does and don't really want to look through my code, so here we go. I'll be assuming you're using Active Directory

1. Edit your AD Schema and create a new multi-value string LDAP attribute that should exist on Security Groups. a name like "vpnSubnets" is probably good

2. Create Security Groups as desired for however you want to segregate VPN access.

3. Use ADSI Edit to modify the vpnSubnets attribute you created earlier on each Security Group and define some IPs/subnets. You MUST specify the CIDR even if it's a single IP - e.g. 192.0.2.1/32

4. Assign people to Security Groups in order to grant them VPN access. A User must be a member of at least ONE Security Group that defines a subnet in order to be permitted.

## Installation
See the BUILD file for instructions. You will need Autotools and a relatively recent version of GCC that supports C++11.

Using the plugin from within OpenVPN requires specifying the plugin directive and the path to this plugin. The plugin takes a single argument which it expects to be the path to its YAML configuration file.

