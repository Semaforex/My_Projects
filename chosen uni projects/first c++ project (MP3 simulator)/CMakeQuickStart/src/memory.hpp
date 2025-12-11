#pragma once
#include "playlist.hpp"
#include "song.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_set>

using json = nlohmann::json;

class Memory {
    private:
        std::unordered_set<Song> songs_;
        std::unordered_set<Playlist> playlists_;
        unsigned int capacity_;

        [[nodiscard]] static bool valid_json(const json& json_data);
    public:
        Memory();
        void set_capacity(unsigned int capacity);
        std::pair<bool, std::string> add_song(Song song);
        bool remove_song(Song song);
        bool add_playlist(Playlist playlist);
        bool remove_playlist(Playlist playlist);
        std::pair<bool, std::string> add_song_to_playlist(Playlist playlist, Song song);
        std::pair<bool, std::string> remove_song_from_playlist(Playlist playlist, Song song);
        unsigned int get_storage_used() const;
        unsigned int get_storage_left() const;
        unsigned int get_capacity() const;
        std::unordered_set<Song> get_songs() const;
        std::unordered_set<Playlist> get_playlists() const;
        Song find_song(std::string title, std::string artist) const;
        Song find_song(size_t hash) const;
        Playlist find_playlist(std::string name, std::string author) const;
        Playlist find_playlist(size_t hash) const;
};

inline void to_json(json& json_data, const Memory& memory){
    json_data["capacity"] = memory.get_capacity();
    json_data["songs"] = nlohmann::json::array();
    for(const auto& song : memory.get_songs()){
        json song_data = song;
        json_data["songs"].push_back(song_data);
    }
    json_data["playlists"] = nlohmann::json::array();
    for(const auto& playlist : memory.get_playlists()){
        json playlist_data = playlist;
        json_data["playlists"].push_back(playlist_data);
    }
}

inline void from_json(const json& json_data, Memory& memory) {
    memory.set_capacity(json_data["capacity"]);
    for(const json& song_data : json_data["songs"]){
        Song song = song_data.template get<Song>();
        memory.add_song(song);
    }
    for(const auto& playlist_data : json_data["playlists"]){
        Playlist playlist = playlist_data.template get<Playlist>();
        memory.add_playlist(playlist);
    }
}