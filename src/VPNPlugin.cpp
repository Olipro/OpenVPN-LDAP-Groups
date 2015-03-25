#include "stdafx.h"
#include "VPNPlugin.h"
#include "LDAPQuerier.h"
#include "PluginContext.h"
#include "ClientContext.h"

plugin_log_t openvpn_log = nullptr;

int openvpn_plugin_open_v3(const int version, openvpn_plugin_args_open_in const *args, openvpn_plugin_args_open_return *ret)
{
    openvpn_log = args->callbacks->plugin_log;
    if (version < OPENVPN_PLUGINv3_STRUCTVER)
	return openvpn_log(PLOG_ERR, PLUGIN_NAME, "Unexpected struct version"), OPENVPN_PLUGIN_FUNC_ERROR;
    ret->type_mask = OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY) | OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_ENABLE_PF) |
		     OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_CLIENT_CONNECT_V2);
    try {
	ret->handle = (void**)new PluginContext(args->argv[1]);
    } catch (...) {
	openvpn_log(PLOG_ERR, PLUGIN_NAME, "Failed initialization due to missing/invalid YAML config");
	return OPENVPN_PLUGIN_FUNC_ERROR;
    }
    return OPENVPN_PLUGIN_FUNC_SUCCESS;
}

void openvpn_plugin_close_v1(openvpn_plugin_handle_t handle)
{
    delete (PluginContext*)handle;
}

void *openvpn_plugin_client_constructor_v1(openvpn_plugin_handle_t handle)
{
    return new ClientContext(*(PluginContext*)handle);
}

void openvpn_plugin_client_destructor_v1(openvpn_plugin_handle_t handle, void *inst)
{
    delete (ClientContext*)inst;
}

int openvpn_plugin_func_v3(const int version, openvpn_plugin_args_func_in const *args, openvpn_plugin_args_func_return *ret)
{
    if (version < OPENVPN_PLUGINv3_STRUCTVER)
	return openvpn_log(PLOG_ERR, PLUGIN_NAME, "Unexpected struct version"), OPENVPN_PLUGIN_FUNC_ERROR;
    ClientContext* const client = (ClientContext*)args->per_client_context;
    switch (args->type)
    {
      case OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY:
	  return client->verifyUser(args->envp);

      case OPENVPN_PLUGIN_ENABLE_PF:
	  client->setPfFile(args->envp);
	  return OPENVPN_PLUGIN_FUNC_SUCCESS;
      
      case OPENVPN_PLUGIN_CLIENT_CONNECT_V2:
	  return client->configUser(args->envp, *ret);

      default:
	  openvpn_log(PLOG_ERR, PLUGIN_NAME, "Unknown check 0x%x", args->type);
	  break;
    }
    return OPENVPN_PLUGIN_FUNC_ERROR;
}
