#pragma once
#include <string>
#include "song.hpp"

class Interface {
public:
    static std::string format_seconds(unsigned int seconds);
    void display_message(const std::string& message) const;
    void display_current_song(const Song& song, unsigned int timer) const;
};