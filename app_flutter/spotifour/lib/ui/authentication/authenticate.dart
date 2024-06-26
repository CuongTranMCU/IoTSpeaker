import 'package:flutter/material.dart';

import 'login.dart';
import 'register.dart';

class Authenticate extends StatefulWidget {
  const Authenticate({super.key});

  @override
  _AuthenticateState createState() => _AuthenticateState();
}

class _AuthenticateState extends State<Authenticate> {
  bool showSignIn = true;

  void toggleView() {
    setState(() => (showSignIn = !showSignIn));
  }

  @override
  Widget build(BuildContext context) {
    return (showSignIn) ? Login(toggleView: toggleView) : Register(toggleView: toggleView);
  }
}
