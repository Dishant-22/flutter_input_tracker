import 'flutter_input_tracker_platform_interface.dart';

class FlutterInputTracker {
  FlutterInputTrackerPlatform get platform => FlutterInputTrackerPlatform.instance;

  Future<String?> getPlatformVersion() {
    return platform.getPlatformVersion();
  }

  Future<void> startHooks() {
    return platform.startHooks();
  }

  Future<void> stopHooks() {
    return platform.stopHooks();
  }

  Stream<Map>? get keyHitStream => platform.keyHitStream;
}
