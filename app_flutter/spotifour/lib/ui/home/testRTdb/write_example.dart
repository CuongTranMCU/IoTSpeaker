import 'dart:math';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';

class WriteExample extends StatefulWidget {
  const WriteExample({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<WriteExample> createState() => _WriteExampleState();
}

class _WriteExampleState extends State<WriteExample> {
  final database = FirebaseDatabase.instance.ref();

  @override
  Widget build(BuildContext context) {
    final dailySpecialRef = database.child("dailySpecial");

    return Scaffold(
      appBar: AppBar(
        title: Text("Write Examples"),
      ),
      body: Center(
          child: Padding(
        padding: EdgeInsets.only(top: 15),
        child: Column(
          children: [
            // ElevatedButton(
            //   onPressed: () async {
            //     try {
            //       await dailySpecialRef.set({
            //         "description": "Vanilla",
            //         "price": 5000,
            //       });
            //     } catch (e) {
            //       print("Error : $e");
            //     }
            //   },
            //   child: Text("Simple set"),
            // ),
            ElevatedButton(
              onPressed: () async {
                try {
                  await dailySpecialRef.update({
                    "status": 5000,
                  });
                } catch (e) {
                  print("Error : $e");
                }
              },
              child: Text("Simple update"),
            ),
            ElevatedButton(
              onPressed: () async {
                final nextOrder = <String, dynamic>{
                  'description': getRandomDrink(),
                  'price': Random().nextInt(800) / 100.0,
                  'customer': getRandomName(),
                  'time': DateTime.now().millisecondsSinceEpoch,
                };

                try {
                  await database.child('orders').push().set(nextOrder);
                } catch (e) {
                  print("Error : $e");
                }
              },
              child: const Text("Append a drink order"),
            )
          ],
        ),
      )),
    );
  }
}

String getRandomDrink() {
  final drinkList = [
    'Latte',
    'Cappuccino',
    'Macchiato',
    'Cortado',
    'Mocha',
    'Drip coffee',
    'Cold brew',
    'Espresso',
    'Vanilla latte',
    'Unicorn frappe',
  ];
  return drinkList[Random().nextInt(drinkList.length)];
}

String getRandomName() {
  final drinkList = [
    'Sam',
    'Arthur',
    'Jessica',
    'Rachel',
    'Vivian',
    'Todd',
    'Morgan',
    'Peter',
    'David',
    'Sumit',
  ];
  return drinkList[Random().nextInt(drinkList.length)];
}
