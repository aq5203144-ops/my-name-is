#include "ProceduralSynth.h"
#include <algorithm>

void ProceduralSynth::prepare(double sr) { sampleRate = sr > 1000.0 ? sr : 44100.0; reset(); }
void ProceduralSynth::reset() { for (auto& v : voices) v = Voice{}; }

void ProceduralSynth::noteOn(int midiNote, float velocity, Tone tone)
{
    Voice* chosen = nullptr;
    for (auto& v : voices) if (!v.active) { chosen = &v; break; }
    if (chosen == nullptr) chosen = &voices[0];
    *chosen = Voice{};
    chosen->active = true;
    chosen->note = juce::jlimit(0, 127, midiNote);
    chosen->velocity = juce::jlimit(0.0f, 1.0f, velocity);
    chosen->tone = tone;
}

void ProceduralSynth::noteOff(int midiNote)
{
    for (auto& v : voices) if (v.active && v.note == midiNote) v.releasing = true;
}

void ProceduralSynth::render(juce::AudioBuffer<float>& buffer, int start, int count)
{
    auto* l = buffer.getWritePointer(0, start);
    auto* r = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1, start) : l;
    const double twoPi = juce::MathConstants<double>::twoPi;

    for (int i = 0; i < count; ++i)
    {
        float out = 0.0f;
        for (auto& v : voices)
        {
            if (!v.active) continue;
            const double freq = 440.0 * std::pow(2.0, (v.note - 69) / 12.0);
            const double inc = twoPi * freq / sampleRate;
            v.phase += inc;
            v.phase2 += inc * 2.003;
            if (v.phase >= twoPi) v.phase -= twoPi;
            if (v.phase2 >= twoPi) v.phase2 -= twoPi;

            float attack = (v.tone == Tone::WarmPad) ? 0.00008f : 0.0015f;
            v.env = juce::jmin(1.0f, v.env + attack);
            if (v.releasing) v.env *= (v.tone == Tone::WarmPad ? 0.9995f : 0.9975f);
            if (v.env < 0.0005f && v.releasing) { v.active = false; continue; }

            const float s1 = static_cast<float>(std::sin(v.phase));
            const float s2 = static_cast<float>(std::sin(v.phase2));
            float toneSample = 0.0f;
            switch (v.tone)
            {
                case Tone::PianoLike: toneSample = 0.76f*s1 + 0.24f*s2; break;
                case Tone::SoftPiano: toneSample = 0.9f*s1 + 0.1f*s2; break;
                case Tone::BrightPiano: toneSample = 0.58f*s1 + 0.42f*s2; break;
                case Tone::Bell: toneSample = 0.45f*s1 + 0.55f*s2; break;
                case Tone::WarmPad: toneSample = 0.82f*s1 + 0.18f*s2; break;
                case Tone::Pluck: toneSample = (0.7f*s1 + 0.3f*s2) * (0.5f + 0.5f*v.env); break;
                case Tone::Bass: toneSample = 0.92f*s1 + 0.08f*s2; break;
            }
            float decay = 1.0f;
            if (v.tone == Tone::PianoLike || v.tone == Tone::SoftPiano || v.tone == Tone::BrightPiano || v.tone == Tone::Bell || v.tone == Tone::Pluck)
                decay = std::exp(-0.9f * static_cast<float>(i) / static_cast<float>(juce::jmax(1, count)));
            out += toneSample * v.velocity * v.env * decay;
        }
        out = std::tanh(out * 0.22f);
        l[i] += out;
        if (r != l) r[i] += out;
    }
}
