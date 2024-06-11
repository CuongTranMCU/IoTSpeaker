import 'dart:async';

import 'package:flutter/material.dart';

class CD01 extends StatefulWidget {
  const CD01({super.key, required this.title});

  final String title;

  @override
  State<CD01> createState() => _CD01State();
}

class _CD01State extends State<CD01> {
  static const maxSeconds = 60;
  int second = maxSeconds;
  Timer? timer;

  void startTimer({bool reset = true}) {
    if (reset) {
      resetTimer();
    }
    timer = Timer.periodic(const Duration(seconds: 1), (_) {
      if (second > 0) {
        setState(() {
          second--;
        });
      } else {
        stopTimer(reset: false);
      }
    });
  }

  void resetTimer() {
    setState(() {
      second = maxSeconds;
    });
  }

  void stopTimer({bool reset = true}) {
    if (reset) {
      resetTimer();
    }

    setState(() {
      timer?.cancel();
    });
  }

  Widget buildTimer() {
    return SizedBox(
      width: 200,
      height: 200,
      child: Stack(
        fit: StackFit.expand,
        children: [
          CircularProgressIndicator(
            value: 1 - (second / maxSeconds),
            valueColor: AlwaysStoppedAnimation(Colors.white),
            strokeWidth: 12,
            backgroundColor: Colors.greenAccent,
          ),
          Center(
            child: Text(
              "$second",
              style: const TextStyle(fontSize: 20, color: Colors.black),
            ),
          )
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final isRunning = (timer == null) ? false : timer!.isActive;
    final isCompleted = (second == maxSeconds) || (second == 0);
    return (isRunning || !isCompleted)
        ? Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                buildTimer(),
                const SizedBox(height: 15),
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    ElevatedButton(
                      style: ElevatedButton.styleFrom(backgroundColor: Colors.white),
                      child: Text(
                        (isRunning) ? "Pause" : "Resume",
                        style: TextStyle(fontSize: 20, color: Colors.black),
                      ),
                      onPressed: () {
                        if (isRunning) {
                          stopTimer(reset: false);
                        } else {
                          startTimer(reset: false);
                        }
                      },
                    ),
                    const SizedBox(width: 15),
                    ElevatedButton(
                      style: ElevatedButton.styleFrom(backgroundColor: Colors.white),
                      child: const Text(
                        "Cancel",
                        style: TextStyle(fontSize: 20, color: Colors.black),
                      ),
                      onPressed: () {
                        stopTimer();
                      },
                    ),
                  ],
                ),
              ],
            ),
          )
        : Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                buildTimer(),
                const SizedBox(height: 15),
                ElevatedButton(
                  style: ElevatedButton.styleFrom(backgroundColor: Colors.white),
                  child: const Text(
                    "Start Timer",
                    style: TextStyle(fontSize: 20, color: Colors.black),
                  ),
                  onPressed: () {
                    startTimer();
                  },
                ),
              ],
            ),
          );
  }
}
