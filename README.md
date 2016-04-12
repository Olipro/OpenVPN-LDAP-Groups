# OpenVPN LDAP Groups Plugin

## About

This plugin is designed to connect to an LDAP backend in order to authenticate clients and route permitted subnets to them.

If you're using Active Directory, you will need to use ADSI Edit in order to define a new attribute on objects that this plugin will then read.

Typically the best way to use this plugin is to create Security Groups and then specify permitted subnet(s) on those groups.

This plugin will route all subnets of groups that a user is a member of. Thereby allowing you to very granularly control access.

## Installation
See the BUILD file for instructions. You will need Autotools and a relatively recent version of GCC that supports C++11.

Using the plugin from within OpenVPN requires specifying the plugin directive and the path to this plugin. The plugin takes a single argument which it expects to be the path to its YAML configuration file.

