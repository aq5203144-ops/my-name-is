#include "Arrangement.h"

std::vector<NoteEvent> Arrangement::makeMinorArrangement(int rootMidi, int bars)
{
    // A basic repeatable musical seed, not AI-composed music.
    const int chordDegrees[4][3] = {{0,3,7},{5,8,12},{3,7,10},{7,10,14}};
    const int roots[4] = {0, 5, 3, 7};
    std::vector<NoteEvent> events;
    for (int bar = 0; bar < bars; ++bar)
    {
        int chord = bar % 4;
        double baseBeat = bar * 4.0;
        for (int n = 0; n < 3; ++n)
            events.push_back({rootMidi + chordDegrees[chord][n], baseBeat, 3.8, 0.48f, 0});
        events.push_back({rootMidi + roots[chord] - 12, baseBeat, 1.6, 0.72f, 6});
        events.push_back({rootMidi + roots[chord] - 12, baseBeat + 2.0, 1.6, 0.65f, 6});
        for (int beat = 0; beat < 4; ++beat)
        {
            events.push_back({36, baseBeat + beat, 0.12, beat == 0 || beat == 2 ? 0.9f : 0.45f, 7});
            if (beat == 1 || beat == 3) events.push_back({38, baseBeat + beat, 0.12, 0.62f, 8});
        }
    }
    return events;
}
