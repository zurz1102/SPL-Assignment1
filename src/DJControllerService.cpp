#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
   std::string title = track.get_title();
   if(cache.contains(title)) {
        cache.get(title);
        #ifdef DEBUG
        std::cout << "[Cache HIT] '" << title << "' found in cache.\n";
        #endif
        displayCacheStatus(); 
        return 1;
   }
   PointerWrapper<AudioTrack> wrapper = track.clone();
   if(!wrapper.get()){
        std::cerr << "[ERROR] Track: \"" << track.get_title() 
                  << "\" failed to clone" << std::endl;
        return 0;
    }
   wrapper->load();
   wrapper->analyze_beatgrid();
   std::string wrapper_title = wrapper->get_title();
    bool is_evicted = cache.put(std::move(wrapper));
    #ifdef DEBUG
    std::cout << "[Cache INSERT] Added '" << wrapper_title << "' to cache." << std::endl;
    if(is_evicted){
        std::cout << "[Cache MISS] (with eviction)\n";
    } else {
        std::cout << "[Cache MISS]\n";
    }
    #endif
    displayCacheStatus();
    return is_evicted ? -1 : 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return cache.get(track_title);
}
