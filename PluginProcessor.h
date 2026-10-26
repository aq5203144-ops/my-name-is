#pragma once
#include <JuceHeader.h>
#include "ProceduralSynth.h"
#include "Arrangement.h"
#include "VocalAnalysis.h"

class OfflineSongStudioAudioProcessor : public juce::AudioProcessor
{
public:
    OfflineSongStudioAudioProcessor();
    ~OfflineSongStudioAudioProcessor() override = default;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 3.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}
    void setTone(int t) { currentTone = juce::jlimit(0, 6, t); }
    void generateDemo() { demoEvents = Arrangement::makeMinorArrangement(60, 8); demoBeat = 0.0; demoOn = true; }
    void stopDemo() { demoOn = false; synth.reset(); }
    VocalStats getStats() const { return stats; }
    bool loadVocal(const juce::File& f);
    juce::String getVocalStatus() const { return vocalStatus; }
private:
    ProceduralSynth synth;
    int currentTone = 0;
    std::vector<NoteEvent> demoEvents;
    double demoBeat = 0.0;
    bool demoOn = false;
    juce::AudioFormatManager formats;
    juce::AudioBuffer<float> vocalBuffer;
    double vocalSampleRate = 0.0;
    VocalStats stats;
    juce::String vocalStatus = "Vocal not loaded";
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OfflineSongStudioAudioProcessor)
};
