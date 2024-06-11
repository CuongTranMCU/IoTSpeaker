import 'package:flutter/material.dart';

import '../../../cloud_functions/auth_service.dart';

class AccountPage extends StatefulWidget {
  const AccountPage({super.key, required this.title});

  final String title;

  @override
  State<AccountPage> createState() => _AccountPageState();
}

class _AccountPageState extends State<AccountPage> {
  final AuthService _authService = AuthService();
  @override
  Widget build(BuildContext context) {
    return Center(
      child: Container(
        height: 72,
        alignment: Alignment.centerLeft,
        child: IconButton(
          icon: Icon(Icons.logout),
          onPressed: () async {
            await _authService.signOut();
            //Navigator.pop(context);
          },
        ),
      ),
    );
  }
}
