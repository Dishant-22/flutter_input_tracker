#include "include/flutter_input_tracker/flutter_input_tracker_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "flutter_input_tracker_plugin.h"

void FlutterInputTrackerPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
    flutter_input_tracker::FlutterInputTrackerPlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarManager::GetInstance()
        ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}