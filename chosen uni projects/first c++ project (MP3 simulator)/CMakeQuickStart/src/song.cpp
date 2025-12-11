#include "song.hpp"
#include <stdexcept>
#include <fmt/core.h>

void check_artist(std::string Artist){
    if(Artist == "") throw std::invalid_argument("Author name cannot be empty"); 
}

void check_title(std::string Title){
    if(Title == "") throw std::invalid_argument("Title cannot be empty");
}

void check_length(unsigned int Length){
    if(Length <= 0) throw std::invalid_argument("Length in seconds must be a positive integer");
}

Song::Song
(
    std::string Artist,
    std::string Title,
    unsigned int Length
) : artist_{Artist}, title_{Title}, length_{Length}{
    check_artist(Artist);
    check_title(Title);
    check_length(Length);
}
std::string Song::get_artist() const {
    return artist_;
}

std::string Song::get_title() const{
    return title_;
}

unsigned int Song::get_length() const{
    return length_;
}

void Song::set_artist(std::string Artist){
    check_artist(Artist);
    artist_ = Artist;
}

void Song::set_title(std::string Title){
    check_title(Title);
    title_ = Title;
}

void Song::set_length(unsigned int Length){
    check_length(Length);
    length_ = Length;
}

bool Song::operator==(const Song& other) const{
    return artist_ == other.get_artist() && title_ == other.get_title();
}

std::string Song::to_string() const{
    return fmt::format("{}-{}-{}-{}", artist_, title_, length_, "seconds");
}

bool Song::valid_json(const nlohmann::json& json_data){
    auto Artist = json_data["artist"];
    auto Title = json_data["title"];
    auto Length = json_data["length"];

    if(Artist == "") return false;
    if(Title == "") return false;
    if(Length <= 0) return false;

    return true;
}