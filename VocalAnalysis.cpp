#include "VocalAnalysis.h"
#include <cmath>
#include <vector>
#include <algorithm>

VocalStats VocalAnalysis::analyze(const juce::AudioBuffer<float>& audio, double sr)
{
    VocalStats s;
    s.sampleRate = sr;
    s.channels = audio.getNumChannels();
    if (sr <= 0 || audio.getNumSamples() <= 0) return s;
    s.durationSeconds = audio.getNumSamples() / sr;

    double sumSq = 0.0;
    for (int c = 0; c < audio.getNumChannels(); ++c)
        for (int i = 0; i < audio.getNumSamples(); ++i) {
            const double x = audio.getSample(c, i);
            s.peak = std::max(s.peak, std::abs(x));
            sumSq += x*x;
        }
    const double total = (double) audio.getNumChannels() * audio.getNumSamples();
    s.rms = std::sqrt(sumSq / juce::jmax(1.0, total));
    s.clipping = s.peak >= 0.999;

    // Coarse energy-onset autocorrelation. Vocal-only input often gives unreliable tempo.
    const int hop = juce::jmax(1, (int)(sr * 0.01));
    std::vector<double> env;
    env.reserve((size_t)(audio.getNumSamples() / hop));
    for (int pos = 0; pos < audio.getNumSamples(); pos += hop) {
        double e = 0.0;
        int n = 0;
        for (int i = pos; i < juce::jmin(pos + hop, audio.getNumSamples()); ++i) {
            double x = 0.0;
            for (int c = 0; c < audio.getNumChannels(); ++c) x += audio.getSample(c, i);
            x /= audio.getNumChannels();
            e += x*x; ++n;
        }
        env.push_back(n ? std::sqrt(e/n) : 0.0);
    }
    if (env.size() > 100) {
        double best = 0.0; int bestLag = 0;
        const int minLag = 25; // 40 BPM at 100 Hz
        const int maxLag = 150; // 160 BPM at 100 Hz
        for (int lag = minLag; lag <= maxLag && lag < (int)env.size()/2; ++lag) {
            double corr = 0.0, a = 0.0, b = 0.0;
            for (int i = lag; i < (int)env.size(); ++i) {
                corr += env[i]*env[i-lag]; a += env[i]*env[i]; b += env[i-lag]*env[i-lag];
            }
            corr /= std::sqrt(std::max(1.0e-12, a*b));
            if (corr > best) { best = corr; bestLag = lag; }
        }
        if (bestLag > 0) { s.estimatedBpm = 6000.0 / bestLag; s.bpmConfidence = best; }
    }
    return s;
}
