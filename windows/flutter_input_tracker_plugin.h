#ifndef FLUTTER_PLUGIN_FLUTTER_INPUT_TRACKER_PLUGIN_H_
#define FLUTTER_PLUGIN_FLUTTER_INPUT_TRACKER_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <memory>

namespace flutter_input_tracker  {

    class FlutterInputTrackerPlugin: public flutter::Plugin {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

        FlutterInputTrackerPlugin();
        virtual ~FlutterInputTrackerPlugin();

        void StartHooks();
        void StopHooks();

    public:
        static HHOOK hKeyboardHook;
        static HHOOK hMouseHook;
        static FlutterInputTrackerPlugin* instance_;  // Static instance pointer

        std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;

        void HandleMethodCall(
            const flutter::MethodCall<flutter::EncodableValue>& method_call,
            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

        static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    };

}  // namespace windows_plugin

#endif  // WINDOWS_PLUGIN_H_

