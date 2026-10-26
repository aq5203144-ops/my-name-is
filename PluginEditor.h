#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class OfflineSongStudioAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit OfflineSongStudioAudioProcessorEditor(OfflineSongStudioAudioProcessor&);
    ~OfflineSongStudioAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    OfflineSongStudioAudioProcessor& processor;
    juce::ComboBox toneBox;
    juce::TextButton loadButton {"Load Vocal for Analysis"};
    juce::TextButton demoButton {"Generate 8-Bar Synth Demo"};
    juce::TextButton stopButton {"Stop Demo"};
    juce::Label info;
    juce::Label status;
    std::unique_ptr<juce::FileChooser> chooser;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OfflineSongStudioAudioProcessorEditor)
};
