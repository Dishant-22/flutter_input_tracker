# 🖥️ flutter_input_tracker

[![Pub Version](https://img.shields.io/pub/v/flutter_input_tracker?color=blue)](https://pub.dev/packages/flutter_input_tracker)
![Platform](https://img.shields.io/badge/platform-windows-blue)
![License](https://img.shields.io/github/license/Dishant-22/flutter_input_tracker)
![Stars](https://img.shields.io/github/stars/Dishant-22/flutter_input_tracker?style=social)

A native Flutter plugin to **track global keyboard and mouse input events on Windows**.

It allows you to hook into all global key presses and mouse clicks in real-time — great for accessibility, logging, control apps, and automation tools.

---

## ✨ Features

✅ Real-time **global keyboard** tracking (key name + ASCII code)  
✅ Real-time **global mouse** click tracking (button + screen coordinates)  
✅ Native **C++ to Dart event stream**  
✅ Start/stop **hooks directly from Flutter**  
✅ Collect and visualize key events inside your app

---

## 🖥️ Supported Platforms

| Platform | Status  |
|----------|---------|
| Windows  | ✅ Stable |
| Others   | 🔜 Coming soon |

---

## 🚀 Getting Started

Add the dependency in your `pubspec.yaml`:

```yaml
dependencies:
  flutter_input_tracker: ^0.0.1
```
Import the package:

```
import 'package:flutter_input_tracker/flutter_input_tracker.dart';
```

##  🎯 API Reference

### `FlutterInputTracker` Methods

| Method           | Description                                  |
|------------------|----------------------------------------------|
| `startHooks()`   | Starts listening to global keyboard & mouse  |
| `stopHooks()`    | Stops the global hook                        |
| `getAllKeys()`   | Returns all supported key definitions        |
| `keyHitStream`   | Stream that emits key press events           |


## 📦 Usage Example
Here's how you can listen for all global key hits and render them as a grid in real-time:
```
class _MyAppState extends State<MyApp> {
  final _windowsPlugin = FlutterInputTracker();
  late StreamSubscription<Map> _keyHitSubscription;

  List<Map> keyList = <Map>[];

  @override
  void initState() {
    super.initState();
    _startListening();
  }

  Future<void> _startListening() async {
    List<Map<String, dynamic>> keyLists = await _windowsPlugin.getAllKeys();
    print("All Keys: ${jsonEncode(keyLists)}");

    _keyHitSubscription = _windowsPlugin.keyHitStream!.listen((keyHit) {
      keyList.add(keyHit);
      print("Key Pressed: $keyHit");
      setState(() {});
    });

    await _windowsPlugin.startHooks();
  }

  Future<void> _stopListening() async {
    await _windowsPlugin.stopHooks();
    await _keyHitSubscription.cancel();
  }

  @override
  void dispose() {
    _stopListening();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: const Text('Global Input Tracker')),
        body: GridView.builder(
          itemCount: keyList.length,
          gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: 25,
            crossAxisSpacing: 4,
            mainAxisSpacing: 4,
          ),
          itemBuilder: (context, index) {
            return Container(
              alignment: Alignment.center,
              decoration: BoxDecoration(
                color: Colors.grey[200],
                borderRadius: BorderRadius.circular(5),
              ),
              child: Text(
                keyList[index]['key'].toString(),
                style: const TextStyle(fontSize: 12),
                textAlign: TextAlign.center,
              ),
            );
          },
        ),
      ),
    );
  }
}

```
## 💡 Use Cases
* Global shortcut/keylogger systems

* Accessibility apps

* Custom input control/automation

* Game or e-sports event tracking

* Remote control & dashboards

---
## 📃 License
MIT License © Dishant-22