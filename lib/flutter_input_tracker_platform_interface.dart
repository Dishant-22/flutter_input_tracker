import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'flutter_input_tracker_method_channel.dart';

abstract class FlutterInputTrackerPlatform extends PlatformInterface {
  FlutterInputTrackerPlatform() : super(token: _token);

  static final Object _token = Object();

  static FlutterInputTrackerPlatform _instance = MethodChannelFlutterInputTracker();

  static FlutterInputTrackerPlatform get instance => _instance;

  static set instance(FlutterInputTrackerPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }
  
  Stream<Map>? get keyHitStream;

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<void> startHooks() {
    throw UnimplementedError('startHooks() has not been implemented.');
  }

  Future<void> stopHooks() {
    throw UnimplementedError('stopHooks() has not been implemented.');
  }
}
