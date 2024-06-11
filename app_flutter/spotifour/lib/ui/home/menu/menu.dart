import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:spotifour/cloud_functions/realtime_db.dart';

import '../../../models/user.dart';
import '../../../widgets/loading.dart';

class Menu extends StatefulWidget {
  const Menu({super.key});

  @override
  State<Menu> createState() {
    return _MenuState();
  }
}

class _MenuState extends State<Menu> {
  @override
  Widget build(BuildContext context) {
    final user = Provider.of<UserModels>(context);
    double screenWidth = MediaQuery.of(context).size.width;
    double screenHeight = MediaQuery.of(context).size.height;

    return StreamBuilder<UserData>(
      stream: RealTimeDBService(uid: user.uid).getUserStream(),
      builder: (context, snapshot) {
        if (snapshot.hasData) {
          UserData userData = snapshot.data!;
          return Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: <Widget>[
              // User Information
              Container(
                alignment: Alignment.center,
                margin: const EdgeInsets.all(25.0),
                height: screenHeight / 3,
                child: Card(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.center,
                    mainAxisSize: MainAxisSize.min,
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      // Avatar
                      const CircleAvatar(
                        radius: 42,
                        backgroundImage: AssetImage("assets/images/avatar.jpeg"),
                      ),
                      const SizedBox(height: 8.0),
                      // Username
                      Text(
                        userData.userName,
                      ),
                      const SizedBox(height: 5.0),
                      // Email
                      Text(
                        userData.email,
                      ),
                    ],
                  ),
                ),
              ),
            ],
          );
        } else {
          return Loading();
        }
      },
    );
  }
}
