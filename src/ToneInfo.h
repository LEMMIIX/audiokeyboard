#ifndef TONEINFO_H
#define TONEINFO_H

struct ToneInfo {
    bool active;
    double frequency;
    double phase;
    // Attack-Decay-Sustain-Release (ADSR) Hülle
    double attack;	// Tonlautstärke stiegt an
    double decay;
    double sustain;
    double release;	// Tonlautstärke nimmt ab
};

#endif // TONEINFO_H
