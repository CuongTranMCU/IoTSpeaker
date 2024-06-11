import '../models/song.dart';
import 'source.dart';

abstract interface class Repository {
  Future<List<Song>?> loadData();
}

class DefaultRepository implements Repository {
  final _localDataSource = LocalDataSource();
  final _remoteDataSource = RemoteDataSource();

  @override
  Future<List<Song>?> loadData() async {
    List<Song> songList = [];
    await _remoteDataSource.loadData().then((remoteSongs) {
      if (remoteSongs == null) {
        _localDataSource.loadData().then((localSongs) {
          if (localSongs != null) {
            songList.addAll(localSongs);
          }
        });
      } else {
        songList.addAll(remoteSongs);
      }
    });
    return songList;
  }
}
