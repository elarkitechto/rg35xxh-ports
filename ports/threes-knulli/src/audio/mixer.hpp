#pragma once
#include <array>
#include <memory>

#include "audio/synth.hpp"

namespace triples::audio {

// Mixes synthesized effects through SDL2, with a locked master volume.
class Mixer {
public:
    Mixer();
    ~Mixer();
    Mixer(const Mixer&)            = delete;
    Mixer& operator=(const Mixer&) = delete;

    // Returns true if audio was successfully opened. If false, play() is a no-op.
    bool initialize();

    // Trigger an SFX. Cheap to call (one stream allocation, one fill).
    void play(Sfx s, float gain = 1.0f);

    // Frees any drained streams. Call once per frame from the main loop.
    void poll();
    void set_volume(int percent);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace triples::audio
