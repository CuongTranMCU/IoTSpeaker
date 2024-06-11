class DailySpecial {
  String description;
  double price;

  DailySpecial({required this.description, required this.price});

  factory DailySpecial.fromRTDB(Map<dynamic, dynamic> data) {
    return DailySpecial(
      description: data['description'] ?? "Drink",
      price: data['price'].toDouble() ?? 0.0,
    );
  }

  String fancyDescription() {
    return "Today's special: A delicious $description for the low low price of ${price.toStringAsFixed(2)}\$";
  }
}
