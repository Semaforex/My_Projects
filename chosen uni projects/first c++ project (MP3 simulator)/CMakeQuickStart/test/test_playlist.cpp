#include <catch2/catch_test_macros.hpp>
#include "playlist.hpp"

TEST_CASE("playlist_test-contructor and getters", "[Playlist]"){
    Playlist playlist{"Name", "Author"};
    REQUIRE(playlist.get_name() == "Name");
    REQUIRE(playlist.get_author() == "Author");
    REQUIRE(playlist.get_song_list().size() == 0);
}

TEST_CASE("playlist_test-constructor from json", "[Playlist]"){
    nlohmann::json json_data;
    json_data["name"] = "Name";
    json_data["author"] = "Author";
    json_data["song_list"] = nlohmann::json::array();
    Playlist playlist = json_data.template get<Playlist>();
    REQUIRE(playlist.get_name() == "Name");
    REQUIRE(playlist.get_author() == "Author");
    REQUIRE(playlist.get_song_list().size() == 0);
}

TEST_CASE("playlist_test-constructor invalid arguments", "[Playlist]"){
    REQUIRE_THROWS_AS(Playlist("", "Author"), std::invalid_argument);
    REQUIRE_THROWS_AS(Playlist("Name", ""), std::invalid_argument);
}

TEST_CASE("playlist_test-setters"){
    Playlist playlist{"Name", "Author"};
    playlist.change_name("Name2");
    playlist.change_author("Author2");
    REQUIRE(playlist.get_name() == "Name2");
    REQUIRE(playlist.get_author() == "Author2");
}

TEST_CASE("playlist_test-setters errors"){
    Playlist playlist{"Name", "Author"};
    REQUIRE_THROWS_AS(playlist.change_name(""), std::invalid_argument);
    REQUIRE_THROWS_AS(playlist.change_author(""), std::invalid_argument);
}

TEST_CASE("playlist_test-add_song"){
    Playlist playlist{"Name", "Author"}; 
    Song song{"Artist", "Title", 5};
    playlist.add_song(song);
    REQUIRE(playlist.get_song_list().size() == 1);
    Song song2{"Artist2", "Title2", 10};
    playlist.add_song(song2);
    REQUIRE(playlist.get_song_list().size() == 2);
}

TEST_CASE("playlist_test-add_song errors"){
    Playlist playlist{"Name", "Author"};
    REQUIRE_THROWS_AS(playlist.add_song(Song{"", "Title", 5}), std::invalid_argument);
    REQUIRE_THROWS_AS(playlist.add_song(Song{"Artist", "", 5}), std::invalid_argument);
    REQUIRE_THROWS_AS(playlist.add_song(Song{"Artist", "Title", 0}), std::invalid_argument);
}

TEST_CASE("playlist_test-add_song duplicate song"){
    Playlist playlist{"Name", "Author"};
    Song song{"Artist", "Title", 5};
    playlist.add_song(song);
    playlist.add_song(song);
    REQUIRE(playlist.get_song_list().size() == 1);
}

TEST_CASE("playlist_test-remove_song"){
    Playlist playlist{"Name", "Author"};
    Song song{"Artist", "Title", 5};
    playlist.add_song(song);
    REQUIRE(playlist.remove_song(song));
    REQUIRE(playlist.get_song_list().size() == 0);
}

TEST_CASE("playlist_test-remove_song errors"){
    Playlist playlist{"Name", "Author"};
    Song song{"Artist", "Title", 5};
    REQUIRE(!playlist.remove_song(song));
    REQUIRE(playlist.get_song_list().size() == 0);
}

TEST_CASE("playlist_test-remove_song not in playlist"){
    Playlist playlist{"Name", "Author"};
    Song song{"Artist", "Title", 5};
    playlist.add_song(song);
    Song song2{"Artist2", "Title2", 10};
    REQUIRE(!playlist.remove_song(song2));
    REQUIRE(playlist.get_song_list().size() == 1);
}

TEST_CASE("playlist_test-operator=="){
    Playlist playlist{"Name", "Author"};
    Playlist playlist2{"Name", "Author"};
    REQUIRE(playlist == playlist2);
    Playlist playlist3{"Name2", "Author"};
    REQUIRE(!(playlist == playlist3));
    Playlist playlist4{"Name", "Author2"};
    REQUIRE(!(playlist == playlist4));
}

TEST_CASE("playlist_test-to_json"){
    Playlist playlist{"Name", "Author"};
    nlohmann::json json_data = playlist;
    REQUIRE(json_data["name"] == "Name");
    REQUIRE(json_data["author"] == "Author");
    REQUIRE(json_data["song_list"].size() == 0);
}
