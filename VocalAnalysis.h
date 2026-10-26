#pragma once
#include <JuceHeader.h>

struct VocalStats {
    double durationSeconds = 0.0;
    double peak = 0.0;
    double rms = 0.0;
    double estimatedBpm = 0.0;
    double bpmConfidence = 0.0;
    int channels = 0;
    double sampleRate = 0.0;
    bool clipping = false;
};

class VocalAnalysis
{
public:
    static VocalStats analyze(const juce::AudioBuffer<float>& audio, double sampleRate);
};
