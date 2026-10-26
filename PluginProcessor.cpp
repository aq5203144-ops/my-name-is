#include "PluginProcessor.h"
#include "PluginEditor.h"

OfflineSongStudioAudioProcessor::OfflineSongStudioAudioProcessor()
 : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    formats.registerBasicFormats();
}
void OfflineSongStudioAudioProcessor::prepareToPlay(double sr, int) { synth.prepare(sr); }
void OfflineSongStudioAudioProcessor::releaseResources() { synth.reset(); }

void OfflineSongStudioAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    for (int c = 0; c < buffer.getNumChannels(); ++c) buffer.clear(c, 0, buffer.getNumSamples());

    for (const auto metadata : midi) {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn()) synth.noteOn(msg.getNoteNumber(), msg.getFloatVelocity(), static_cast<ProceduralSynth::Tone>(currentTone));
        else if (msg.isNoteOff()) synth.noteOff(msg.getNoteNumber());
    }

    // Demo arrangement is rendered by beat position using current host tempo when available.
    if (demoOn) {
        double bpm = 100.0;
        if (auto* ph = getPlayHead()) {
            if (auto pos = ph->getPosition())
                if (auto hostBpm = pos->getBpm()) bpm = *hostBpm;
        }
        const double sr = getSampleRate() > 0 ? getSampleRate() : 44100.0;
        const double beatSamples = sr * 60.0 / bpm;
        const double startBeat = demoBeat;
        const double endBeat = startBeat + buffer.getNumSamples() / beatSamples;
        for (const auto& ev : demoEvents) {
            if (ev.beat >= startBeat && ev.beat < endBeat) {
                const int offset = juce::jlimit(0, buffer.getNumSamples()-1, (int)((ev.beat-startBeat)*beatSamples));
                const auto tone = ev.instrument == 6 ? ProceduralSynth::Tone::Bass :
                                  ev.instrument >= 7 ? ProceduralSynth::Tone::Pluck :
                                  static_cast<ProceduralSynth::Tone>(currentTone);
                synth.noteOn(ev.midi, ev.velocity, tone);
            }
        }
        synth.render(buffer, 0, buffer.getNumSamples());
        demoBeat = endBeat;
        if (demoBeat > 32.0) { demoOn = false; }
    } else {
        synth.render(buffer, 0, buffer.getNumSamples());
    }
}

bool OfflineSongStudioAudioProcessor::loadVocal(const juce::File& f)
{
    std::unique_ptr<juce::AudioFormatReader> reader(formats.createReaderFor(f));
    if (!reader) { vocalStatus = "File format not supported"; return false; }
    if (reader->lengthInSamples <= 0 || reader->lengthInSamples > (juce::int64)reader->sampleRate * 60 * 20) {
        vocalStatus = "File empty or longer than 20 minutes"; return false;
    }
    const int chans = (int)juce::jlimit((juce::int64)1, (juce::int64)2, reader->numChannels);
    const int samples = (int)reader->lengthInSamples;
    vocalBuffer.setSize(chans, samples);
    reader->read(&vocalBuffer, 0, samples, 0, true, chans > 1);
    vocalSampleRate = reader->sampleRate;
    stats = VocalAnalysis::analyze(vocalBuffer, vocalSampleRate);
    vocalStatus = f.getFileName() + " | " + juce::String(stats.durationSeconds, 1) + " sec | Peak " + juce::String(stats.peak, 3)
        + " | RMS " + juce::String(stats.rms, 3) + " | Rough BPM " + juce::String(stats.estimatedBpm, 1)
        + " (confidence " + juce::String(stats.bpmConfidence, 2) + ")";
    return true;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new OfflineSongStudioAudioProcessor(); }
