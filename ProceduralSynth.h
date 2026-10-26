#pragma once
#include <JuceHeader.h>
#include <array>
#include <cmath>

class ProceduralSynth
{
public:
    enum class Tone { PianoLike = 0, SoftPiano, BrightPiano, Bell, WarmPad, Pluck, Bass };
    void prepare(double sr);
    void reset();
    void noteOn(int midiNote, float velocity, Tone tone);
    void noteOff(int midiNote);
    void render(juce::AudioBuffer<float>& buffer, int start, int count);

private:
    struct Voice {
        bool active = false;
        bool releasing = false;
        int note = 60;
        double phase = 0.0;
        double phase2 = 0.0;
        float velocity = 0.0f;
        float env = 0.0f;
        Tone tone = Tone::PianoLike;
    };
    static constexpr int maxVoices = 32;
    std::array<Voice, maxVoices> voices {};
    double sampleRate = 44100.0;
};
