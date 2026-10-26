#pragma once
#include <JuceHeader.h>
#include <vector>

struct NoteEvent { int midi; double beat; double length; float velocity; int instrument; };

class Arrangement
{
public:
    static std::vector<NoteEvent> makeMinorArrangement(int rootMidi = 60, int bars = 8);
};
