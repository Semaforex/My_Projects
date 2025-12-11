# include "memory.hpp"
# include <algorithm>


Memory::Memory() :
    songs_{std::unordered_set<Song>{}},
    playlists_{std::unordered_set<Playlist>{}},
    capacity_{0}
    {}

void Memory::set_capacity(unsigned int capacity){
    capacity_ = capacity;
}

std::pair<bool, std::string> Memory::add_song(Song song){
    if(song.get_length()+get_storage_used() > capacity_)
        return {false, "Not enough storage"};
    bool sucess = songs_.insert(song).second;
    return {sucess, "Song already in memory"};
}

bool Memory::remove_song(Song song){
    bool success = songs_.erase(song);
    if(success)
        for (auto playlist : playlists_){
            if(playlist.get_song_list().find(std::hash<Song>()(song)) != playlist.get_song_list().end())
                auto _ = playlist.remove_song(song);
                playlists_.erase(playlist);
                playlists_.insert(playlist);
        }
    return success;
}

bool Memory::add_playlist(Playlist playlist){
    bool sucess = playlists_.insert(playlist).second;
    return sucess;
}

bool Memory::remove_playlist(Playlist playlist){
    return playlists_.erase(playlist);
}

std::pair<bool, std::string> Memory::add_song_to_playlist(Playlist playlist, Song song){
    try{
        Playlist playlist_ = find_playlist(playlist.get_name(), playlist.get_author());
        Song song_ = find_song(song.get_title(), song.get_artist());
        remove_playlist(playlist);
        playlist_.add_song(song);
        add_playlist(playlist_);
        return {true, ""};
    }catch(std::invalid_argument& e){
        return {false, e.what()};
    }
}

std::pair<bool, std::string> Memory::remove_song_from_playlist(Playlist playlist, Song song){
    Playlist playlist_;
    try{
        playlist_ = find_playlist(playlist.get_name(), playlist.get_author());
    }catch(std::invalid_argument& e){
        return {false, e.what()};
    }
    if(!playlist_.remove_song(song)) return {false, "Song not found"};
    remove_playlist(playlist);
    add_playlist(playlist_);
    return {true, ""};
}

unsigned int Memory::get_storage_used() const{
    unsigned int storage_used = 0;
    for(const auto& song : songs_)
        storage_used += song.get_length();
    return storage_used;
}

unsigned int Memory::get_storage_left() const{
    return capacity_ - get_storage_used();
}

unsigned int Memory::get_capacity() const{
    return capacity_;
}

std::unordered_set<Song> Memory::get_songs() const{
    return songs_;
}

std::unordered_set<Playlist> Memory::get_playlists() const{
    return playlists_;
}

Song Memory::find_song(std::string title, std::string artist) const{
    auto it = std::find_if(songs_.begin(), songs_.end(), [title, artist](Song song){
        return song.get_title() == title && song.get_artist() == artist;
    });
    if(it == songs_.end())
        throw(std::invalid_argument("Song not found"));
    return *it;
}

Song Memory::find_song(size_t hash) const{
    auto it = std::find_if(songs_.begin(), songs_.end(), [hash](Song song){
        return std::hash<Song>()(song) == hash;
    });
    if(it == songs_.end())
        throw(std::invalid_argument("Song not found"));
    return *it;
}

Playlist Memory::find_playlist(std::string name, std::string author) const{
    auto it = std::find_if(playlists_.begin(), playlists_.end(), [name, author](Playlist playlist){
        return playlist.get_name() == name && playlist.get_author() == author;
    });
    if(it == playlists_.end())
        throw(std::invalid_argument("Playlist not found"));
    return *it;
}

Playlist Memory::find_playlist(size_t hash) const{
    auto it = std::find_if(playlists_.begin(), playlists_.end(), [hash](Playlist playlist){
        return std::hash<Playlist>()(playlist) == hash;
    });
    if(it == playlists_.end())
        throw(std::invalid_argument("Playlist not found"));
    return *it;
}




