
#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
    DJLibraryService::~DJLibraryService() {
        for (AudioTrack* track : library) {
            delete track;
            track = nullptr;
        }
        library.clear();
    }

 void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    //std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    for (const auto& track_info : library_tracks) {
        AudioTrack* track = nullptr;
        if (track_info.type == "MP3") {
            track = new MP3Track(
            track_info.title,            
            track_info.artists,       
            track_info.duration_seconds,  
            track_info.bpm,           
            track_info.extra_param1,        
            track_info.extra_param2      
    );
        } else if (track_info.type == "WAV") {
            track = new WAVTrack(
            track_info.title,         
            track_info.artists,         
            track_info.duration_seconds,  
            track_info.bpm,            
            track_info.extra_param1,     
            track_info.extra_param2        
    );
        } else {
            std::cout << "[WARNING] Unknown format for track: "<< track_info.title << std::endl;
            continue;
        }
        if (track != nullptr) {
            library.push_back(track);
        }
        
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    int track_count = 0;
    for (int index : track_indices) {
        if (index < 1 || static_cast<size_t>(index) > library.size()) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
            continue; 
        }
        AudioTrack* library_track = library[index - 1];
        PointerWrapper<AudioTrack> wrapper = library_track->clone();
        AudioTrack* clone = wrapper.release();
        if (clone == nullptr) {
            std::cout << "[ERROR] Failed to clone track at index "<< index << std::endl;
            continue;
        }
        clone->load();
        clone->analyze_beatgrid();
        playlist.add_track(clone);
        track_count++;
    }
    std::cout << "[INFO] Playlist loaded: "<< playlist_name << " ("<< track_count << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> track_titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();

    for (AudioTrack* track : tracks) {
        if (track != nullptr) {
            track_titles.push_back(track->get_title());
        }
    }

    return track_titles;
    
}


