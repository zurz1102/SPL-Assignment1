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
   if(cache.contains(track.get_title())) {
        cache.get(title); 
        return 1;
   }
   PointerWrapper<AudioTrack> wrapper = track.clone();
   AudioTrack* rawPtr = wrapper.release();
   if(rawPtr == nullptr) {
        std::cerr << "[ERROR] Failed to clone track \"" << title << "\"\n";
        return 0;
   }
   rawPtr->load();
   rawPtr->analyze_beatgrid();
   PointerWrapper<AudioTrack> preparedClone(rawPtr);
   bool eviction = cache.put(std::move(preparedClone));
   if(eviction) {
    return -1;
   }
   return 0;
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
