#include <iostream>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "interface.hpp"

void Interface::display_message(const std::string& message) const{
    std::cout << message << std::endl;
}

void Interface::display_current_song(const Song &song, unsigned int timer) const {
    using namespace ftxui;
    system("clear");

    auto time_bar = gauge((float) timer / song.get_length());
    auto title = song.get_title();
    auto artist = song.get_artist();

    auto formated_progress = format_seconds(timer);
    auto formated_full_length = format_seconds(song.get_length());

    auto document = vbox({
        text("Music Player") | bold | color(Color::Blue) | hcenter,
        separator(),
        hbox({text(title) | bold, text(" by "), text(artist)}) | hcenter,
        hbox({text(formated_progress), text("-"), text(formated_full_length)}) | hcenter,
        time_bar | borderEmpty
     }) | border;

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
}

std::string Interface::format_seconds(unsigned int seconds) {
    auto hours = seconds/3600;
    auto minutes = (seconds-hours*3600)/60;
    seconds = (seconds- hours*3600 - minutes*60);

    std::string s_hours = (hours < 10) ? "0"+std::to_string(hours) : std::to_string(hours);
    std::string s_minutes = (minutes < 10) ? "0"+std::to_string(minutes) : std::to_string(minutes);
    std::string s_seconds = (seconds < 10) ? "0"+std::to_string(seconds) : std::to_string(seconds);
    return (hours == 0) ? s_minutes+":"+s_seconds : s_hours+":"+s_minutes+":"+s_seconds;
}
