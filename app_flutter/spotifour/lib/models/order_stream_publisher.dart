import 'package:firebase_database/firebase_database.dart';

import 'orders.dart';

class OrderStreamPublisher {
  final _database = FirebaseDatabase.instance.ref();
  Stream<List<Order>> getOrderStream() {
    final orderStream = _database.child('orders').onValue;
    final streamToPublish = orderStream.map((event) {
      final orderMap = event.snapshot.value as Map<dynamic, dynamic>;
      final orderList = orderMap.entries.map((element) {
        return Order.fromRTDB(Map<dynamic, dynamic>.from(element.value));
      }).toList();
      return orderList;
    });
    return streamToPublish;
  }
}
