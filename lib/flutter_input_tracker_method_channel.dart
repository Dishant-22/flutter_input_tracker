import 'dart:async';
import 'package:flutter/services.dart';

import 'flutter_input_tracker_platform_interface.dart';

class MethodChannelFlutterInputTracker extends FlutterInputTrackerPlatform {
  final methodChannel = const MethodChannel('flutter_input_tracker');
  StreamController<Map>? _keyHitStreamController;

  MethodChannelFlutterInputTracker() {
    methodChannel.setMethodCallHandler(_handleMethodCall);
    _keyHitStreamController = StreamController<Map>.broadcast();
  }

  @override
  Future<void> startHooks() async {
    await methodChannel.invokeMethod('startHooks');
  }

  @override
  Future<void> stopHooks() async {
    await methodChannel.invokeMethod('stopHooks');
  }

  @override
  Stream<Map>? get keyHitStream => _keyHitStreamController?.stream;

  Future<void> _handleMethodCall(MethodCall call) async {
    switch (call.method) {
      case 'onKeyDown':
        // final keyHit = call.arguments as String;
        final Map<dynamic, dynamic> keyEvent = call.arguments;
        keyEvent['action'] = "keyboard";
        _keyHitStreamController?.add(keyEvent);
        break;
      case 'onMouseClick':
        final Map<dynamic, dynamic> mouseClickInfo = call.arguments;
        mouseClickInfo['action'] = "mouse";
        Map<dynamic, dynamic> position = {};
        position['x'] = mouseClickInfo['x'];
        position['y'] = mouseClickInfo['y'];
        mouseClickInfo['position'] = position;
        mouseClickInfo.remove("x");
        mouseClickInfo.remove("y");
        _keyHitStreamController?.add(mouseClickInfo);
        break;
      default:
        throw MissingPluginException('notImplemented');
    }
  }
}
