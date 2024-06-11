import 'package:flutter/material.dart';

import 'package:provider/provider.dart';
import 'package:spotifour/ui/home/testRTdb/home.dart';
import 'package:spotifour/ui/home/timer/cd01.dart';
import 'package:spotifour/ui/home/timer/countdown.dart';
import 'package:spotifour/ui/home/timer/time_navigator.dart';
import 'package:spotifour/ui/home/timer/time_picker.dart';

import '../../cloud_functions/auth_service.dart';
import '../../models/user.dart';
import 'account/person.dart';
import 'menu/menu.dart';
import 'music/music.dart';

class HomeNavigator extends StatefulWidget {
  const HomeNavigator({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<HomeNavigator> createState() => _HomeNavigatorState();
}

class _HomeNavigatorState extends State<HomeNavigator> {
  final AuthService _authService = AuthService();

  int _currentPage = 0;

  final List<Widget> _page = [
    const MusicPage(title: "Music"),
    const Menu(),
    const AccountPage(title: "Account"),
    // const TimeNavigator(),
  ];

  @override
  Widget build(BuildContext context) {
    final user = Provider.of<UserModels?>(context);
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      bottomNavigationBar: BottomNavigationBar(
        currentIndex: _currentPage,
        onTap: (int index) {
          setState(() {
            _currentPage = index;
          });
        },
        items: const [
          BottomNavigationBarItem(
            label: "Home",
            icon: Icon(Icons.home),
          ),
          BottomNavigationBarItem(
            label: "Account",
            icon: Icon(Icons.person),
          ),
          BottomNavigationBarItem(
            label: "Setting",
            icon: Icon(Icons.settings),
          ),
        ],
      ),
      body: _page[_currentPage],
    );
  }
}
