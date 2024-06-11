import 'package:flutter/material.dart';
import 'package:spotifour/ui/home/testRTdb/read_examples.dart';
import 'package:spotifour/ui/home/testRTdb/write_example.dart';

class HomePage extends StatefulWidget {
  const HomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.all(12.0),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.center,
        children: <Widget>[
          Text("Check out examples"),
          SizedBox(
            height: 6,
            width: MediaQuery.of(context).size.width,
          ),
          ElevatedButton(
            onPressed: () {
              Navigator.of(context).push(
                MaterialPageRoute(
                  builder: (context) => ReadExample(title: "Read Exmaples"),
                ),
              );
            },
            child: const Text("Read Examples"),
          ),
          ElevatedButton(
            onPressed: () {
              Navigator.of(context).push(
                MaterialPageRoute(
                  builder: (context) => WriteExample(title: "Write Exmaples"),
                ),
              );
            },
            child: const Text("Write Examples"),
          ),
        ],
      ),
    );
  }
}
