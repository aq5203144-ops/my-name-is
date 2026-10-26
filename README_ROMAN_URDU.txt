OFFLINE SONG STUDIO AI CORE — SOURCE PACKAGE
===========================================

IMPORTANT / SEEDHI BAAT
-----------------------
Yeh expanded native-audio engine starter hai, complete tested AI music generator nahi.
Is package mein:
- Synthesizer engine: piano-like, bell, pad, bass aur pluck timbres ko code se khud generate karta hai.
- MIDI-style note events se notes ban sakte hain; external sample packs zaroori nahi.
- Vocal file ka basic analysis: RMS/peak aur rough autocorrelation tempo estimate.
- Simple arrangement generator: chord progression, bass notes aur drum pattern events.
- CMake + JUCE VST3 project structure.

Abhi ismein trained offline AI model, reliable vocal pitch/key extraction, vocal-conditioned neural sound generation,
full vocal separation, automatic professional mixing/mastering, aur 1000s of distinct piano presets nahi hain.
Generated timbres are procedural synthesizer sounds, not recorded piano samples. "AI" ka daawa trained model ke baghair nahi kiya gaya.

BUILD
-----
Required: Windows 10/11, CMake, Visual Studio 2022 C++ Desktop workload, internet for first JUCE FetchContent download.
1. ZIP extract karo, e.g. C:\OfflineSongStudio_AI_Core
2. PowerShell/CMD mein us folder ke andar:
   cmake -S . -B build -G "Visual Studio 17 2022" -A x64
   cmake --build build --config Release
3. VST3 output path CMake build output mein check karo. Build successful hone ke baad hi install karo.
4. Agar CMake/JUCE API issue aaye, error text share karo; source ko fix karna hoga.

Offline target:
- Build dependencies pehle download honge.
- Plugin chalne ke baad is source ke procedural synth ko internet nahi chahiye.
- Neural AI generation tab tak nahi hogi jab tak actual local model aur inference runtime add, license-check aur test na ho.
- 8 GB RAM PC ke liye large generative audio models slow/unsuitable ho sakte hain.

NO FAKE CLAIM:
Yeh source project hai; compiled/tested Windows VST3 binary nahi.
