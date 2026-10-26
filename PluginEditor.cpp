#include "PluginEditor.h"

OfflineSongStudioAudioProcessorEditor::OfflineSongStudioAudioProcessorEditor(OfflineSongStudioAudioProcessor& p)
 : AudioProcessorEditor(&p), processor(p)
{
    setSize(560, 340);
    toneBox.addItem("Piano-like", 1);
    toneBox.addItem("Soft Piano", 2);
    toneBox.addItem("Bright Piano", 3);
    toneBox.addItem("Bell", 4);
    toneBox.addItem("Warm Pad", 5);
    toneBox.addItem("Pluck", 6);
    toneBox.addItem("Bass", 7);
    toneBox.setSelectedId(1);
    toneBox.onChange = [this] { processor.setTone(toneBox.getSelectedId()-1); };
    addAndMakeVisible(toneBox);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(demoButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(info);
    addAndMakeVisible(status);
    info.setText("Procedural sound engine | No external samples required", juce::dontSendNotification);
    info.setJustificationType(juce::Justification::centredLeft);
    status.setText("Load a vocal to view basic stats, or generate a synth demo.", juce::dontSendNotification);
    status.setJustificationType(juce::Justification::topLeft);
    status.setColour(juce::Label::textColourId, juce::Colours::white);

    loadButton.onClick = [this] {
        chooser = std::make_unique<juce::FileChooser>("Select vocal audio", juce::File{}, "*.wav;*.aiff;*.flac;*.mp3");
        chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc) {
                auto f = fc.getResult();
                if (f.existsAsFile()) {
                    processor.loadVocal(f);
                    status.setText(processor.getVocalStatus(), juce::dontSendNotification);
                }
            });
    };
    demoButton.onClick = [this] { processor.generateDemo(); status.setText("Demo started. It uses a fixed minor-key pattern, not vocal-conditioned AI.", juce::dontSendNotification); };
    stopButton.onClick = [this] { processor.stopDemo(); status.setText("Demo stopped.", juce::dontSendNotification); };
}
void OfflineSongStudioAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff151b1a));
    g.setColour(juce::Colour(0xff78c99b));
    g.setFont(juce::Font(23.0f, juce::Font::bold));
    g.drawText("OFFLINE SONG STUDIO", 20, 16, getWidth()-40, 34, juce::Justification::centredLeft);
    g.setColour(juce::Colours::white);
    g.setFont(13.0f);
    g.drawText("AI-ready native audio engine starter (not a trained AI model)", 22, 52, getWidth()-44, 22, juce::Justification::centredLeft);
}
void OfflineSongStudioAudioProcessorEditor::resized()
{
    toneBox.setBounds(22, 88, 190, 30);
    loadButton.setBounds(22, 135, 235, 36);
    demoButton.setBounds(22, 184, 235, 36);
    stopButton.setBounds(270, 184, 130, 36);
    info.setBounds(22, 235, getWidth()-44, 24);
    status.setBounds(22, 262, getWidth()-44, 65);
}
