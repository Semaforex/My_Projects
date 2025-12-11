#pragma once
#include "playlist.hpp"
#include "memory.hpp"
#include "battery.hpp"
#include "interface.hpp"
#include <queue>
#include <unordered_set>
#include <string>
#include <vector> 
#include <nlohmann/json.hpp>

class Player{
    private:
        std::string name_;
        std::queue<std::size_t> song_queue_;
        Battery battery_;
        Memory memory_;
        Interface interface_;
        std::string path_{};
    public:
        Player(std::string Path, std::string Name, unsigned int Battery_capacity = 1000, unsigned int Memory_capacity = 1000);
        Player(std::string Path);
        void add_song(Song song);
        void remove_song(Song song);
        void add_playlist(Playlist playlist);
        void remove_playlist(Playlist playlist);
        void add_song_to_playlist(Playlist playlist, Song song);
        void remove_song_from_playlist(Playlist playlist, Song song);
        void display_playlist_info(std::string name, std::string author);
        void display_all_playlists_info();

        inline auto get_all_song(){
            return memory_.get_songs();
        }

        inline auto get_all_playlists(){
            return memory_.get_playlists();
        }

        // przeciwna kolejność przekazywania argumentów niż w Song();
        // może podać długość piosenki
        void display_song_info(std::string title, std::string artist);
        void play_playlist(std::string name, std::string author);
        void play_song(std::string title, std::string artist);
        void play_all_songs();
        void charge_battery(unsigned int battery);
        void show_battery();
        void show_storage();
        bool battery_action();
        void play(Song song);
        void list_all_songs();
        ~Player();
};