#pragma once
#include "song.hpp"
#include <unordered_set>
#include <string>
#include <vector>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Playlist{
    private:
        std::string name_;
        std::string author_;
        std::unordered_set<size_t> song_list_;
    
        [[nodiscard]] static bool valid_json(const nlohmann::json& json_data);
    public:
        Playlist() = default;
        Playlist(
            std::string Name,
            std::string Author        
        );
        [[nodiscard]] std::string get_name() const;
        [[nodiscard]] std::string get_author() const;
        [[nodiscard]] std::unordered_set<size_t> get_song_list() const;
        void change_name(std::string Name);
        void change_author(std::string Author);
        void add_song(Song song);
        void add_song(size_t song_hash);
        [[nodiscard]] bool remove_song(Song song);
        [[nodiscard]] bool operator==(const Playlist& other) const;
};

namespace std{
    template<>
    struct hash<Playlist>{
        std::size_t operator()(const Playlist& playlist) const{
            return std::hash<std::string>()(playlist.get_author()) ^ std::hash<std::string>()(playlist.get_name());
        }
    };
}

inline void from_json(const json& json_data, Playlist& playlist){
    playlist.change_name(json_data["name"]);
    playlist.change_author(json_data["author"]);
    for(const auto& song : json_data["song_list"])
        playlist.add_song(song.get<size_t>());
}

inline void to_json(json& j, const Playlist& playlist) {
    j["name"] = playlist.get_name();
    j["author"] = playlist.get_author();
    j["song_list"] = playlist.get_song_list();
}