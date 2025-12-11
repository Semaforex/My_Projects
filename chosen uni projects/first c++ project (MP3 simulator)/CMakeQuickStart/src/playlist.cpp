#include "playlist.hpp"
#include <stdexcept>
#include <iomanip>
#include <fmt/core.h>
#include <fmt/chrono.h>


void check_name(std::string Name){
    if(Name == "")
        throw(std::invalid_argument("Name cannot be empty"));
}

void check_auhtor(std::string Author){
    if(Author == "")
        throw(std::invalid_argument("Author cannot be empty"));
}

Playlist::Playlist(
    std::string Name,
    std::string Author
) : name_{Name}, author_{Author}, song_list_{std::unordered_set<size_t>()}{
    check_name(Name);
    check_auhtor(Author);
}

bool Playlist::valid_json(const nlohmann::json& json_data){
    return json_data.contains("name") && json_data.contains("author") && json_data.contains("song_list");
}

std::string Playlist::get_name() const{
    return name_;
}

std::string Playlist::get_author() const{
    return author_;
}


std::unordered_set<size_t> Playlist::get_song_list() const{
    return song_list_;
}


void Playlist::change_name(std::string Name){
    check_name(Name);
    name_ = Name;
}

void Playlist::change_author(std::string Author){
    check_auhtor(Author);
    author_ = Author;
}

void Playlist::add_song(Song song){
    std::hash<Song> hasher;
    size_t songHash = hasher(song);
    song_list_.insert(songHash);
}

void Playlist::add_song(size_t song_hash){
    song_list_.insert(song_hash);
}

bool Playlist::remove_song(Song song){
    std::hash<Song> hasher;
    size_t songHash = hasher(song);
    if(song_list_.erase(songHash) == 0)
        return false;
    return true;
}

bool Playlist::operator==(const Playlist& other) const{
    return name_ == other.get_name() && author_ == other.get_author();
}

