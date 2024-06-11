import 'dart:async';

import 'package:just_audio/just_audio.dart';
import 'package:rxdart/rxdart.dart';

class AudioPlayerManager {
  AudioPlayerManager({required this.songUrl});

  final player = AudioPlayer();
  var playIndex = 0;

  Stream<DurationState>? durationState;
  String songUrl;

  void init() {
    durationState = Rx.combineLatest2<Duration, PlaybackEvent, DurationState>(
      player.positionStream,
      player.playbackEventStream,
      (position, playbackEvent) => DurationState(
        progress: position,
        buffered: playbackEvent.bufferedPosition,
        total: playbackEvent.duration,
      ),
    );
    //player.setUrl(songUrl);
    player.play();
  }

  void dispose() {
    player.dispose();
  }

  playSong(String? uri, index) {
    playIndex = index;
    try {
      player.setAudioSource(AudioSource.uri(Uri.parse(uri!)));
      player.play();
    } on Exception catch (e) {
      print(e.toString());
    }
  }

  updateSongUrl(String url) {
    songUrl = url;
    init();
  }
}

class DurationState {
  final Duration progress;
  final Duration buffered;
  final Duration? total;

  DurationState({
    required this.progress,
    required this.buffered,
    this.total,
  });
}
