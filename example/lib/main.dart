import 'dart:convert';

import 'package:flutter/material.dart';
import 'dart:async';
import 'package:flutter/services.dart';
import 'package:flutter_input_tracker/flutter_input_tracker.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _keyHit = 'No key hit';

  List<Map> keyList = <Map>[];

  final _windowsPlugin = FlutterInputTracker();
  late StreamSubscription<Map> _keyHitSubscription;

  Future<void> _startListening() async {
    List<Map<String, dynamic>> keyLists =await _windowsPlugin.getAllKeys();
    print(jsonEncode(keyLists));

    _keyHitSubscription = _windowsPlugin.keyHitStream!.listen((keyHit) {
      keyList.add(keyHit);

      print("=====> $keyHit || ${keyList.length}");
      // _keyHit = keyHit;
      setState(() {});
    });
    await _windowsPlugin.startHooks();
  }

  Future<void> _stopListening() async {
    await _windowsPlugin.stopHooks();
    await _keyHitSubscription.cancel();
  }

  @override
  void initState() {
    super.initState();
    _startListening();
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
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: GridView.builder(
          itemCount: keyList.length,
          // reverse: true,
          itemBuilder: (context, index) {
            return Container(
              decoration: BoxDecoration(
                color: Colors.grey[200],
                borderRadius: BorderRadius.circular(5),
              ),
              alignment: Alignment.center,
              child: Text(
                keyList[index]['key'].toString(),
                style: const TextStyle(fontSize: 12),
                textAlign: TextAlign.center,
              ),
            );
          },
          gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: 25,
            crossAxisSpacing: 4,
            mainAxisSpacing: 4
          ),
        ),
      ),
    );
  }
}
