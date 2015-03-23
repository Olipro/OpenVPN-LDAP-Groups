extern plugin_log_t openvpn_log;

#pragma GCC visibility push(default)

extern "C" {
  int openvpn_plugin_open_v3(const int version, openvpn_plugin_args_open_in const *args, openvpn_plugin_args_open_return *ret);
  int openvpn_plugin_func_v3(const int version, openvpn_plugin_args_func_in const *args, openvpn_plugin_args_func_return *ret);
  void openvpn_plugin_close_v1(openvpn_plugin_handle_t handle);
  void *openvpn_plugin_client_constructor_v1(openvpn_plugin_handle_t handle);
  void openvpn_plugin_client_destructor_v1(openvpn_plugin_handle_t handle, void *inst);
};

#pragma GCC visibility pop
