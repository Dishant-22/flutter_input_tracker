# flutter_input_tracker

A Flutter plugin for tracking global keyboard and mouse input events on Windows.  
Get real-time key presses (with names and ASCII codes) and mouse clicks (with button and coordinates) in your Flutter app.

## Features

- Listen for all global key presses with key names and ASCII codes.
- Listen for all global mouse clicks with button (left, right, middle) and X/Y screen coordinates.
- Start and stop hooks from Flutter.
- Real-time event streaming from Windows native C++ to Dart.

## Supported Platforms

- Windows (native)
- (Other platforms coming soon!)

## Getting Started

Add this plugin to your `pubspec.yaml`:

```yaml
dependencies:
  flutter_input_tracker: ^0.0.1
