// File generated by FlutterFire CLI.
// ignore_for_file: type=lint
import 'package:firebase_core/firebase_core.dart' show FirebaseOptions;
import 'package:flutter/foundation.dart'
    show defaultTargetPlatform, kIsWeb, TargetPlatform;

/// Default [FirebaseOptions] for use with your Firebase apps.
///
/// Example:
/// ```dart
/// import 'firebase_options.dart';
/// // ...
/// await Firebase.initializeApp(
///   options: DefaultFirebaseOptions.currentPlatform,
/// );
/// ```
class DefaultFirebaseOptions {
  static FirebaseOptions get currentPlatform {
    if (kIsWeb) {
      return web;
    }
    switch (defaultTargetPlatform) {
      case TargetPlatform.android:
        return android;
      case TargetPlatform.iOS:
        return ios;
      case TargetPlatform.macOS:
        return macos;
      case TargetPlatform.windows:
        return windows;
      case TargetPlatform.linux:
        throw UnsupportedError(
          'DefaultFirebaseOptions have not been configured for linux - '
          'you can reconfigure this by running the FlutterFire CLI again.',
        );
      default:
        throw UnsupportedError(
          'DefaultFirebaseOptions are not supported for this platform.',
        );
    }
  }

  static const FirebaseOptions web = FirebaseOptions(
    apiKey: 'AIzaSyC4xC_dey5t1XVyW25Clbk2IaFZksFiPnM',
    appId: '1:100168795401:web:b779c76b5c8a580d583fd8',
    messagingSenderId: '100168795401',
    projectId: 'iot-speaker-e27ae',
    authDomain: 'iot-speaker-e27ae.firebaseapp.com',
    databaseURL: 'https://iot-speaker-e27ae-default-rtdb.firebaseio.com',
    storageBucket: 'iot-speaker-e27ae.appspot.com',
  );

  static const FirebaseOptions android = FirebaseOptions(
    apiKey: 'AIzaSyAlNuhvAFVVmum7AW5AgdYkOprZezeSUGc',
    appId: '1:100168795401:android:7ef3eedf2494bc72583fd8',
    messagingSenderId: '100168795401',
    projectId: 'iot-speaker-e27ae',
    databaseURL: 'https://iot-speaker-e27ae-default-rtdb.firebaseio.com',
    storageBucket: 'iot-speaker-e27ae.appspot.com',
  );

  static const FirebaseOptions ios = FirebaseOptions(
    apiKey: 'AIzaSyAloIbaUM1qPQiGDiojnjnKynY4WHdT7wc',
    appId: '1:100168795401:ios:c9dc2b0671d794b3583fd8',
    messagingSenderId: '100168795401',
    projectId: 'iot-speaker-e27ae',
    databaseURL: 'https://iot-speaker-e27ae-default-rtdb.firebaseio.com',
    storageBucket: 'iot-speaker-e27ae.appspot.com',
    iosBundleId: 'com.example.spotifour',
  );

  static const FirebaseOptions macos = FirebaseOptions(
    apiKey: 'AIzaSyAloIbaUM1qPQiGDiojnjnKynY4WHdT7wc',
    appId: '1:100168795401:ios:c9dc2b0671d794b3583fd8',
    messagingSenderId: '100168795401',
    projectId: 'iot-speaker-e27ae',
    databaseURL: 'https://iot-speaker-e27ae-default-rtdb.firebaseio.com',
    storageBucket: 'iot-speaker-e27ae.appspot.com',
    iosBundleId: 'com.example.spotifour',
  );

  static const FirebaseOptions windows = FirebaseOptions(
    apiKey: 'AIzaSyC4xC_dey5t1XVyW25Clbk2IaFZksFiPnM',
    appId: '1:100168795401:web:808e90f6aa5df100583fd8',
    messagingSenderId: '100168795401',
    projectId: 'iot-speaker-e27ae',
    authDomain: 'iot-speaker-e27ae.firebaseapp.com',
    databaseURL: 'https://iot-speaker-e27ae-default-rtdb.firebaseio.com',
    storageBucket: 'iot-speaker-e27ae.appspot.com',
  );
}