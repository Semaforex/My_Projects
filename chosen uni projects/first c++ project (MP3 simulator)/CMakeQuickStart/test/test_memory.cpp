#include <catch2/catch_test_macros.hpp>
#include "../src/memory.hpp"

TEST_CASE("Memory_test-contructor", "[Memory]"){
    Memory memory;
    REQUIRE(memory.get_capacity() == 0);
    REQUIRE(memory.get_storage_used() == 0);
    REQUIRE(memory.get_songs().empty());
    REQUIRE(memory.get_playlists().empty());
}

TEST_CASE("Memory_test-set_capacity", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    REQUIRE(memory.get_capacity() == 10);
}

TEST_CASE("Memory_test-add_song", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    REQUIRE(memory.get_songs().size() == 1);
    REQUIRE(memory.get_storage_used() == 5);
}

TEST_CASE("Memory_test-remove_song", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    memory.remove_song(song);
    REQUIRE(memory.get_songs().empty());
    REQUIRE(memory.get_storage_used() == 0);
}

TEST_CASE("Memory_test-add_playlist", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Playlist playlist = {"Name", "Author"};
    memory.add_playlist(playlist);
    REQUIRE(memory.get_playlists().size() == 1);
}

TEST_CASE("Memory_test-remove_playlist", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Playlist playlist = {"Name", "Author"};
    memory.add_playlist(playlist);
    memory.remove_playlist(playlist);
    REQUIRE(memory.get_playlists().empty());
}

TEST_CASE("Memory_test-add_song_to_playlist", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    Playlist playlist = {"Name", "Author"};
    memory.add_song(song);
    memory.add_playlist(playlist);
    memory.add_song_to_playlist(playlist, song);
    auto playl = memory.get_playlists().begin();
    //REQUIRE(playl.get_song_list().size() == 1);
}

TEST_CASE("Memory_test-remove_song_from_playlist", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    Playlist playlist = {"Name", "Author"};
    memory.add_song(song);
    memory.add_playlist(playlist);
    memory.add_song_to_playlist(playlist, song);
    memory.remove_song_from_playlist(playlist, song);
    REQUIRE(memory.get_playlists().begin()->get_song_list().empty());
}

TEST_CASE("Memory_test-get_storage_used", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    REQUIRE(memory.get_storage_used() == 5);
}

TEST_CASE("Memory_test-get_storage_left", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 1};
    memory.add_song(song);
    REQUIRE(memory.get_storage_left() == 9);
}

TEST_CASE("Memory_test-get_capacity", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    REQUIRE(memory.get_capacity() == 10);
}

TEST_CASE("Memory_test-to_json", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    Playlist playlist = {"Name", "Author"};
    memory.add_playlist(playlist);
    memory.add_song_to_playlist(playlist, song);
    json data = memory;
    REQUIRE(data["capacity"] == 10);
    REQUIRE(data["songs"].size() == 1);
    REQUIRE(data["playlists"].size() == 1);
}

TEST_CASE("Memory_test-find_song", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    REQUIRE(memory.find_song("Title", "Artist") == song);
}

TEST_CASE("Memory_test-find_playlist", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Playlist playlist = {"Name", "Author"};
    memory.add_playlist(playlist);
    REQUIRE(memory.find_playlist("Name", "Author") == playlist);
}

TEST_CASE("Memory_test-find_song_hash", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    std::hash<Song> hasher;
    REQUIRE(memory.find_song(hasher(song)) == song);
}

TEST_CASE("Memory_test-find_playlist_hash", "[Memory]"){
    Memory memory;
    Playlist playlist = {"Name", "Author"};
    memory.add_playlist(playlist);
    std::hash<Playlist> hasher;
    REQUIRE(memory.find_playlist(hasher(playlist)) == playlist);
}

TEST_CASE("Memory_test-find_song_not_found", "[Memory]"){
    Memory memory;
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    REQUIRE_THROWS_AS(memory.find_song("Title2", "Artist"), std::invalid_argument);
}

TEST_CASE("Memory_test-find_playlist_not_found", "[Memory]"){
    Memory memory;
    Playlist playlist = {"Name", "Author"};
    memory.add_playlist(playlist);
    REQUIRE_THROWS_AS(memory.find_playlist("Name2", "Author"), std::invalid_argument);
}

TEST_CASE("Memory_test-find_song_hash_not_found", "[Memory]"){
    Memory memory;
    Song song = {"Artist", "Title", 5};
    memory.add_song(song);
    std::hash<Song> hasher;
    REQUIRE_THROWS_AS(memory.find_song(hasher(Song("Artist2", "Title", 5))), std::invalid_argument);
}

TEST_CASE("Memory_test-find_playlist_hash_not_found", "[Memory]"){
    Memory memory;
    Playlist playlist = {"Name", "Author"};
    memory.add_playlist(playlist);
    std::hash<Playlist> hasher;
    REQUIRE_THROWS_AS(memory.find_playlist(hasher(Playlist("Name2", "Author"))), std::invalid_argument);
}

TEST_CASE("Memory_test-to_json_empty", "[Memory]"){
    Memory memory;
    json data = memory;
    REQUIRE(data["capacity"] == 0);
    REQUIRE(data["songs"].empty());
    REQUIRE(data["playlists"].empty());
}

TEST_CASE("Memory_test-from_json", "[Memory]"){
    std::hash<Song> song_hasher;
    Song song = {"Artist", "Title", 5};
    auto song_hash = song_hasher(song);
    json data = {
        {"capacity", 10},
        {"storage_used", 1},
        {"songs", nlohmann::json::array({
            {
                {"artist", "Artist"},
                {"title", "Title"},
                {"length", 5}
            }
        })},
        {"playlists", nlohmann::json::array({
            {
                {"name", "Name"},
                {"author", "Author"},
                {"song_list", nlohmann::json::array({song_hash})}
            }
        })}
    };
    Memory memory;
    memory = data;
    REQUIRE(memory.get_capacity() == 10);
    REQUIRE(memory.get_storage_used() == 5);
    REQUIRE(memory.get_songs().size() == 1);
    REQUIRE(memory.get_playlists().size() == 1);
    REQUIRE(memory.find_song(song_hash).get_title() == "Title");
    REQUIRE(memory.find_song(song_hash).get_artist() == "Artist");
    REQUIRE(memory.find_song(song_hash).get_length() == 5);
}

TEST_CASE("Memory_test-from_json_empty", "[Memory]"){
    json data = {
        {"capacity", 0},
        {"storage_used", 0},
        {"songs", nlohmann::json::array()},
        {"playlists", nlohmann::json::array()}
    };
    Memory memory;
    memory = data;
    REQUIRE(memory.get_capacity() == 0);
    REQUIRE(memory.get_storage_used() == 0);
    REQUIRE(memory.get_songs().empty());
    REQUIRE(memory.get_playlists().empty());
}

TEST_CASE("Memory_test-add_song_to_playlist_error", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    Playlist playlist = {"Name", "Author"};
    memory.add_song(song);
    memory.add_playlist(playlist);
    
    // Add the same song to the playlist again
    auto result1 = memory.add_song_to_playlist(playlist, song);
    REQUIRE(result1.first == true);
    
    // Add a song that doesn't exist in the memory
    Song song2 = {"Artist2", "Title2", 3};
    auto result2 = memory.add_song_to_playlist(playlist, song2);
    REQUIRE(result2.first == false);
    REQUIRE(result2.second == "Song not found");
}

TEST_CASE("Memory_test-remove_song_from_playlist_error", "[Memory]"){
    Memory memory;
    memory.set_capacity(10);
    Song song = {"Artist", "Title", 5};
    Playlist playlist = {"Name", "Author"};
    memory.add_song(song);
    memory.add_playlist(playlist);
    memory.add_song_to_playlist(playlist, song);
    
    // Remove a song that doesn't exist in the playlist
    Song song2 = {"Artist2", "Title2", 3};
    auto result1 = memory.remove_song_from_playlist(playlist, song2);
    REQUIRE(result1.first == false);
    
    // Remove a song from a playlist that doesn't exist
    Playlist playlist2 = {"Name2", "Author2"};
    auto result2 = memory.remove_song_from_playlist(playlist2, song);
    REQUIRE(result2.first == false);
}