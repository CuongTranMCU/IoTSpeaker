import 'dart:async';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';

import '../../../models/daily_spcial.dart';
import '../../../models/order_stream_publisher.dart';
import '../../../models/orders.dart';

class ReadExample extends StatefulWidget {
  const ReadExample({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<ReadExample> createState() => _ReadExampleState();
}

class _ReadExampleState extends State<ReadExample> {
  final _database = FirebaseDatabase.instance.ref();
  String _displayText = 'Result go here';
  late StreamSubscription _dailySpecialStream;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    _activateListeners();
  }

  void _activateListeners() {
    _dailySpecialStream = _database.child("dailySpecial").onValue.listen((event) {
      final data = event.snapshot.value as Map<dynamic, dynamic>;
      final dailySpecial = DailySpecial.fromRTDB(data);
      setState(() {
        _displayText = dailySpecial.fancyDescription();
      });
    });
  }

  @override
  void deactivate() {
    // TODO: implement deactivate
    _dailySpecialStream.cancel();
    super.deactivate();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Read Examples"),
      ),
      body: Center(
          child: Padding(
        padding: EdgeInsets.only(top: 15),
        child: Column(
          children: [
            Text(
              _displayText,
              style: TextStyle(fontSize: 18),
              textAlign: TextAlign.center,
            ),
            const SizedBox(height: 20),
            StreamBuilder(
              stream: OrderStreamPublisher().getOrderStream(),
              builder: (context, snapshot) {
                final titleList = <ListTile>[];
                if (snapshot.hasData && snapshot.data != null) {
                  final myOrders = snapshot.data as List<Order>;
                  if (myOrders != null) {
                    titleList.addAll(
                      myOrders.map((nextOrders) {
                        return ListTile(
                          leading: Icon(Icons.local_cafe),
                          title: Text(nextOrders.description ?? ''), // Add null check for description
                          subtitle: Text(nextOrders.customerName ?? ''), // Add null check for customer
                        );
                      }),
                    );
                  }
                }
                return Expanded(
                  child: ListView(
                    children: titleList,
                  ),
                );
              },
            ),
          ],
        ),
      )),
    );
  }
}
