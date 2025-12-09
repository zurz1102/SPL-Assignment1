#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
    decks[0] = nullptr;
    decks[1] = nullptr;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning decks.\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i] != nullptr) {   
            delete decks[i];         
            decks[i] = nullptr;     
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
   std::cout << "\n=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> cloned = track.clone();
    if (!cloned) {
        std::cout << "[ERROR] Track: \"" << track.get_title()
                  << "\" failed to clone\n";
        return -1;
    }
    size_t target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck << "\n";
    if (decks[target_deck] != nullptr) {
        //std::cout << "[Unload] Unloading deck " << target_deck<< " (\"" << decks[target_deck]->get_title() << "\")\n";
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    cloned->load();
    cloned->analyze_beatgrid();
    if (auto_sync && decks[active_deck] != nullptr &&
        !can_mix_tracks(cloned)) {
        sync_bpm(cloned);
    }
    decks[target_deck] = cloned.release();
    std::cout << "[Load Complete] '" << decks[target_deck]->get_title()
              << "' is now loaded on deck " << target_deck << "\n";
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << active_deck << "\n";
    displayDeckStatus();
    return static_cast<int>(target_deck);
}


/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(!decks[active_deck]){
        return false;
    }
    if(!track){
        return false;
    }
    int active_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int diffBPM = std::abs(active_bpm - new_bpm);
    return diffBPM <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(!decks[active_deck]){
        return;
    }
    if(!track){
        return;
    }
    int active_bpm = decks[active_deck]->get_bpm();
    int new_bmp = track->get_bpm();
    int avg_bpm = (active_bpm + new_bmp) / 2;
    track->set_bpm(avg_bpm);
    std::cout << "[Sync BPM] Syncing BPM from "<< new_bmp << " to " << avg_bpm << "\n";
}
