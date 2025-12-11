#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Song{
    private:
        std::string artist_;
        std::string title_;
        unsigned int length_; // in seconds

        [[nodiscard]] static bool valid_json(const nlohmann::json& json_data);
    public:
        Song() = default;
        Song
        (
            std::string Artist,
            std::string Title,
            unsigned int Length = 1   
        );
        [[nodiscard]] std::string get_artist() const;
        [[nodiscard]] std::string get_title() const;
        [[nodiscard]] unsigned int get_length() const;
        void set_artist(std::string Artist);
        void set_title(std::string Title);
        void set_length(unsigned int Length);
        [[nodiscard]] bool operator==(const Song& other) const;
        [[nodiscard]] std::string to_string() const;
};

namespace std{
    template<>
    struct hash<Song>{
        std::size_t operator()(const Song& song) const{
            return std::hash<std::string>()(song.get_artist()) ^ std::hash<std::string>()(song.get_title());
        }
    };
}

inline void to_json(nlohmann::json& json_data, const Song& song){
    json_data = json{
        {"artist", song.get_artist()},
        {"title", song.get_title()},
        {"length", song.get_length()}
    };
}

inline void from_json(const nlohmann::json& json_data, Song& song){
    song.set_artist(json_data.at("artist"));
    song.set_title(json_data.at("title"));
    song.set_length(json_data.at("length"));
}