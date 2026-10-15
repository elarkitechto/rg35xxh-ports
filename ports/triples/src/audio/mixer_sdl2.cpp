#include "audio/mixer.hpp"
#include <SDL.h>
#include <algorithm>
#include <array>
#include <vector>

namespace triples::audio {
struct Mixer::Impl {
    struct Voice { const SfxBuffer* buffer; size_t position; float gain; };
    SDL_AudioDeviceID device = 0;
    float volume = 1.0f;
    std::array<SfxBuffer, static_cast<size_t>(Sfx::Count_)> buffers;
    std::vector<Voice> voices;
    static void callback(void* userdata, Uint8* stream, int length) {
        auto& self = *static_cast<Impl*>(userdata);
        auto* out = reinterpret_cast<float*>(stream);
        int count = length / sizeof(float);
        std::fill(out, out + count, 0.0f);
        for (auto& v : self.voices) {
            for (int i = 0; i < count && v.position < v.buffer->samples.size(); ++i)
                out[i] += v.buffer->samples[v.position++] * v.gain;
        }
        for (int i = 0; i < count; ++i) out[i] = std::clamp(out[i], -1.0f, 1.0f) * self.volume;
        std::erase_if(self.voices, [](const Voice& v) { return v.position >= v.buffer->samples.size(); });
    }
    bool open() {
        for (int i = 0; i < static_cast<int>(Sfx::Count_); ++i) buffers[i] = synthesize(static_cast<Sfx>(i));
        voices.reserve(32);
        SDL_AudioSpec spec{};
        spec.freq = kSampleRate; spec.format = AUDIO_F32SYS; spec.channels = 1; spec.samples = 512;
        spec.callback = callback; spec.userdata = this;
        device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
        if (!device) return false;
        SDL_PauseAudioDevice(device, 0);
        return true;
    }
    ~Impl() { if (device) SDL_CloseAudioDevice(device); }
};
Mixer::Mixer() = default;
Mixer::~Mixer() = default;
bool Mixer::initialize() {
    impl_ = std::make_unique<Impl>();
    if (impl_->open()) return true;
    impl_.reset(); return false;
}
void Mixer::play(Sfx s, float gain) {
    if (!impl_) return;
    SDL_LockAudioDevice(impl_->device);
    if (impl_->voices.size() < 32) impl_->voices.push_back({&impl_->buffers[static_cast<size_t>(s)], 0, gain});
    SDL_UnlockAudioDevice(impl_->device);
}
void Mixer::poll() {}
void Mixer::set_volume(int percent) {
    if (!impl_) return;
    SDL_LockAudioDevice(impl_->device);
    impl_->volume = std::clamp(percent, 0, 100) / 100.0f;
    SDL_UnlockAudioDevice(impl_->device);
}
}
