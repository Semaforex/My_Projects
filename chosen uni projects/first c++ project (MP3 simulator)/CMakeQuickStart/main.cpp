#include "player.hpp"
#include <string>
#include <vector>
#include "include/CLI11.hpp"



int main(int argc, char** argv){
    CLI::App app{"Music Player"};
    app.require_subcommand(1);
    CLI::App* new_playlist = app.add_subcommand("new_playlist", "Create  a new playlist");
    CLI::App* new_song = app.add_subcommand("new_song", "Add a song");
    CLI::App* add_to_playlist = app.add_subcommand("add_to_playlist", "Add a song to a playlist");
    CLI::App* remove = app.add_subcommand("remove", "Remove a song from the player");
    CLI::App* remove_from_playlist = app.add_subcommand("remove_from_playlist", "Remove a song from the playlist");
    CLI::App* play_all = app.add_subcommand("play_all", "Play all songs from the player");
    CLI::App* play_song = app.add_subcommand("play_song", "Play a song from the playlist");
    CLI::App* play_playlist = app.add_subcommand("play_playlist", "Play a playlist");
    CLI::App* list_all = app.add_subcommand("list", "List all the playlists");
    CLI::App* charge = app.add_subcommand("charge", "Charge the battery");
    CLI::App* show_battery = app.add_subcommand("show_battery", "Show the battery");
    CLI::App* show_storage = app.add_subcommand("show_storage", "Show the storage");
    CLI::App* list_playlist = app.add_subcommand("list_playlist", "Show the playlist");
    CLI::App* show_song = app.add_subcommand("show_song", "Show the song");
    CLI::App* reset_player = app.add_subcommand("reset_player", "Reset the player");
    CLI::App* show_songs = app.add_subcommand("show_songs", "Show all songs in the memory");
    
    std::string path = "../Player/player.json";
    unsigned int length;
    unsigned int capacity = 1000;
    unsigned int battery = 1000;
    std::string playlist_name;
    std::string playlist_author;
    std::string song_artist;
    std::string song_title;
    std::string player_name = "deafult";

    new_playlist ->add_option("-c, --creator", playlist_author, "Author of the playlist")->required();
    new_playlist ->add_option("-n,--name", playlist_name, "Name of the playlist")->required();
    new_song->add_option("-a,--artist", song_artist, "Artist of the song")->required();
    new_song->add_option("-t,--title", song_title, "Title of the song")->required();
    new_song->add_option("-l,--length", length, "Length of the song")->required();
    add_to_playlist->add_option("-a,--artist", song_artist, "Artist of the song")->required();
    add_to_playlist->add_option("-t,--title", song_title, "Title of the song")->required();
    add_to_playlist->add_option("-n,--name", playlist_name, "Name of the playlist")->required();
    add_to_playlist->add_option("-c, --creator", playlist_author, "Author of the playlist")->required();
    remove->add_option("-a,--artist", song_artist, "Artist of the song")->required();
    remove->add_option("-t,--title", song_title, "Title of the song")->required();
    charge->add_option("-b,--battery", battery, "Battery to charge")->required();
    list_playlist->add_option("-n,--name", playlist_name, "Name of the playlist")->required();
    list_playlist->add_option("-c, --creator", playlist_author, "Author of the playlist")->required();
    show_song->add_option("-a,--artist", song_artist, "Artist of the song")->required();
    show_song->add_option("-t,--title", song_title, "Title of the song")->required();
    remove_from_playlist->add_option("-a,--artist", song_artist, "Artist of the song")->required();
    remove_from_playlist->add_option("-t,--title", song_title, "Title of the song")->required();
    remove_from_playlist->add_option("-n,--name", playlist_name, "Name of the playlist")->required();
    remove_from_playlist->add_option("-c, --creator", playlist_author, "Author of the playlist")->required();
    reset_player->add_option("-n,--name", player_name, "Name of the playlist");
    reset_player->add_option("-b,--battery", battery, "Battery to charge");
    reset_player->add_option("-m,--memory", capacity, "Capacity of the player's memory");
    play_playlist->add_option("-n,--name", playlist_name, "Name of the playlist")->required();
    play_playlist->add_option("-c, --creator", playlist_author, "Author of the playlist")->required();
    play_song->add_option("-a,--artist", song_artist, "Artist of the song")->required();
    play_song->add_option("-t,--title", song_title, "Title of the song")->required();


    
    new_playlist ->callback([&](){
        Player player{path};
        player.add_playlist(Playlist{playlist_name, playlist_author});
    });

    new_song->callback([&](){
        Player player{path};
        player.add_song(Song{song_artist, song_title, length});
    });

    add_to_playlist->callback([&](){
        Player player{path};
        player.add_song_to_playlist(Playlist{playlist_name, playlist_author}, Song{song_artist, song_title});
    });

    remove->callback([&](){
        Player player{path};
        player.remove_song(Song{song_artist, song_title});
    });

    remove_from_playlist->callback([&](){
        Player player{path};
        player.remove_song_from_playlist(Playlist{playlist_name,playlist_author}, Song{song_artist, song_title});
    });

    play_all->callback([&](){
        Player player{path};
        player.play_all_songs();
    });

    play_song->callback([&](){
        Player player{path};
        player.play_song(song_artist, song_title);
    });

    play_playlist->callback([&](){
        Player player{path};
        player.play_playlist(playlist_name,playlist_author);
    });

    list_all->callback([&](){
        Player player{path};
        player.display_all_playlists_info();
    });

    charge->callback([&](){
        Player player{path};
        player.charge_battery(battery);
    });

    show_battery->callback([&](){
        Player player{path};
        player.show_battery();
    });

    show_storage->callback([&](){
        Player player{path};
        player.show_storage();
    });

    list_playlist->callback([&](){
        Player player{path};
        player.display_playlist_info(playlist_name, playlist_author);
    });

    show_song->callback([&](){
        Player player{path};
        player.display_song_info(song_artist, song_title);
    });

    reset_player->callback([&](){
        Player player{path, player_name, battery, capacity};
    });

    show_songs->callback([&](){
        Player player{path};
        player.list_all_songs();
    });

    try 
    {
        app.parse(argc, argv);
    } 
    catch (const CLI::ParseError &e) 
    {
        return app.exit(e);
    }
} 