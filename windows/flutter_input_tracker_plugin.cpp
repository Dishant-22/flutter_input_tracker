#include "flutter_input_tracker_plugin.h"

#include <windows.h>

#include <flutter/standard_method_codec.h>

#include <sstream>

namespace flutter_input_tracker {

    HHOOK FlutterInputTrackerPlugin::hKeyboardHook = NULL;
    HHOOK FlutterInputTrackerPlugin::hMouseHook = NULL;
    FlutterInputTrackerPlugin* FlutterInputTrackerPlugin::instance_ = nullptr;  // Initialize static member

    void FlutterInputTrackerPlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarWindows* registrar) {
        auto channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "flutter_input_tracker", &flutter::StandardMethodCodec::GetInstance());

        auto plugin = std::make_unique<FlutterInputTrackerPlugin>();
        plugin->channel_ = std::move(channel);  // Store the channel in the plugin instance

        instance_ = plugin.get();  // Set the static instance pointer

        auto* plugin_ptr = plugin.get();
        registrar->AddPlugin(std::move(plugin));

        plugin_ptr->channel_->SetMethodCallHandler([plugin_ptr](const auto& call, auto result) {
            plugin_ptr->HandleMethodCall(call, std::move(result));
            });
    }

    FlutterInputTrackerPlugin::FlutterInputTrackerPlugin() {}

    FlutterInputTrackerPlugin::~FlutterInputTrackerPlugin() {
        StopHooks();
    }

    void FlutterInputTrackerPlugin::StartHooks() {
        if (hKeyboardHook == NULL) {
            hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
        }
        if (hMouseHook == NULL) {
            hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
        }
    }

    void FlutterInputTrackerPlugin::StopHooks() {
        if (hKeyboardHook) {
            UnhookWindowsHookEx(hKeyboardHook);
            hKeyboardHook = NULL;
        }
        if (hMouseHook) {
            UnhookWindowsHookEx(hMouseHook);
            hMouseHook = NULL;
        }
    }

    std::string KeyCodeToName(int keyCode) {
        char name[256];
        // Get the scan code for the virtual key code
        UINT scanCode = MapVirtualKeyA(keyCode, MAPVK_VK_TO_VSC);

        if (keyCode == 38) {
            return "Arrow Up";
        }
        if (keyCode == 40) {
            return "Arrow Down";
        }
        if (keyCode == 37) {
            return "Arrow Left";
        }
        if (keyCode == 39) {
            return "Arrow Right";
        }
        if (keyCode == 45) {
            return "Insert";
        }
        if (keyCode == 46) {
            return "Delete";
        }
        if (keyCode == 33) {
            return "Page Up";
        }
        if (keyCode == 34) {
            return "Page Down";
        }
        if (keyCode == 35) {
            return "End";
        }
        if (keyCode == 36) {
            return "Home";
        }
        if (keyCode == 144) {
            return "Num Lock";
        }
        if (keyCode == 145) {
            return "Scroll Lock";
        }
        if (keyCode == 110) {
            return "Num .";
        }

        // Use GetKeyNameTextA to get the name of the key
        int result = GetKeyNameTextA(scanCode << 16, name, sizeof(name));

        // Return the key name if successful, otherwise "Unknown"
        return result > 0 ? std::string(name) : "Unknown";
    }

    LRESULT CALLBACK FlutterInputTrackerPlugin::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
            KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;

            int keyCode = pKey->vkCode;

            // Convert key code to key name
            std::string keyName = KeyCodeToName(keyCode);

            // Prepare the map to send to Flutter
            flutter::EncodableMap eventMap;
            eventMap[flutter::EncodableValue("key")] = flutter::EncodableValue(keyName);
            eventMap[flutter::EncodableValue("keycode")] = flutter::EncodableValue(keyCode);

            // Send the map to Flutter
            if (instance_ && instance_->channel_) {
                instance_->channel_->InvokeMethod("onKeyDown", std::make_unique<flutter::EncodableValue>(eventMap));
            }
        }
        return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
    }

    LRESULT CALLBACK FlutterInputTrackerPlugin::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode >= 0) {
            MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

            if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_MBUTTONDOWN) {
                // Capture mouse click events
                int x = pMouse->pt.x;
                int y = pMouse->pt.y;

                // Prepare the map to send to Flutter
                flutter::EncodableMap eventMap;
                //eventMap[flutter::EncodableValue("keyName")] = flutter::EncodableValue(wParam == WM_LBUTTONDOWN ? "leftClick" : "rightClick");
                if (wParam == WM_LBUTTONDOWN) {
                    eventMap[flutter::EncodableValue("key")] = flutter::EncodableValue("Left");
                }
                else if (wParam == WM_RBUTTONDOWN) {
                    eventMap[flutter::EncodableValue("key")] = flutter::EncodableValue("Right");
                }
                else if (wParam == WM_MBUTTONDOWN) {
                    eventMap[flutter::EncodableValue("key")] = flutter::EncodableValue("Middle");
                }

                eventMap[flutter::EncodableValue("x")] = flutter::EncodableValue(x);
                eventMap[flutter::EncodableValue("y")] = flutter::EncodableValue(y);

                // Send the map to Flutter
                if (instance_ && instance_->channel_) {
                    instance_->channel_->InvokeMethod("onMouseClick", std::make_unique<flutter::EncodableValue>(eventMap));
                }
            }
        }
        return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
    }

    void FlutterInputTrackerPlugin::HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue>& method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        if (method_call.method_name().compare("startHooks") == 0) {
            StartHooks();
            result->Success();
        }
        else if (method_call.method_name().compare("stopHooks") == 0) {
            StopHooks();
            result->Success();
        }else if (method_call.method_name() == "getAllKeys") {
  std::vector<flutter::EncodableValue> all_keys;

  // Letters A-Z
  for (int code = 'A'; code <= 'Z'; ++code) {
    flutter::EncodableMap keyMap;
    keyMap[flutter::EncodableValue("key")] = flutter::EncodableValue(std::string(1, static_cast<char>(code)));
    keyMap[flutter::EncodableValue("keycode")] = flutter::EncodableValue(code);
    all_keys.push_back(flutter::EncodableValue(keyMap));
  }

  // Numbers 0-9
  for (int code = '0'; code <= '9'; ++code) {
    flutter::EncodableMap keyMap;
    keyMap[flutter::EncodableValue("key")] = flutter::EncodableValue(std::string(1, static_cast<char>(code)));
    keyMap[flutter::EncodableValue("keycode")] = flutter::EncodableValue(code);
    all_keys.push_back(flutter::EncodableValue(keyMap));
  }

  // Function keys F1-F12
  for (int i = 1; i <= 12; ++i) {
    int vk = VK_F1 + i - 1;
    std::string key = "F" + std::to_string(i);
    flutter::EncodableMap keyMap;
    keyMap[flutter::EncodableValue("key")] = flutter::EncodableValue(key);
    keyMap[flutter::EncodableValue("keycode")] = flutter::EncodableValue(vk);
    all_keys.push_back(flutter::EncodableValue(keyMap));
  }

  // Modifier and navigation keys (add as needed)
  struct KeyDef { const char* label; int vk; };
  const KeyDef specialKeys[] = {
    {"ESC", VK_ESCAPE},
    {"TAB", VK_TAB},
    {"CAPSLOCK", VK_CAPITAL},
    {"SHIFT", VK_SHIFT},
    {"CTRL", VK_CONTROL},
    {"ALT", VK_MENU},
    {"SPACE", VK_SPACE},
    {"ENTER", VK_RETURN},
    {"BACKSPACE", VK_BACK},
    {"LEFT", VK_LEFT},
    {"RIGHT", VK_RIGHT},
    {"UP", VK_UP},
    {"DOWN", VK_DOWN},
    {"HOME", VK_HOME},
    {"END", VK_END},
    {"INSERT", VK_INSERT},
    {"DELETE", VK_DELETE},
    {"PAGEUP", VK_PRIOR},
    {"PAGEDOWN", VK_NEXT},
    {"WIN", VK_LWIN},
    {"APPS", VK_APPS},
    {"PRINTSCREEN", VK_SNAPSHOT},
    {"SCROLLLOCK", VK_SCROLL},
    {"PAUSE", VK_PAUSE},
    // Add more as needed
  };

  for (const auto& k : specialKeys) {
    flutter::EncodableMap keyMap;
    keyMap[flutter::EncodableValue("key")] = flutter::EncodableValue(std::string(k.label));
    keyMap[flutter::EncodableValue("keycode")] = flutter::EncodableValue(k.vk);
    all_keys.push_back(flutter::EncodableValue(keyMap));
  }

  result->Success(flutter::EncodableValue(all_keys));
}

        else {
            result->NotImplemented();
        }
    }

}  // namespace windows_plugin

