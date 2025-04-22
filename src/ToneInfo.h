#ifndef TONEINFO_H
#define TONEINFO_H

struct ToneInfo {
    bool active;
    double frequency;
    double phase;
    double attack;	// Tonlautstärke stiegt an
    double release;	// Tonlautstärke nimmt ab
};

#endif // TONEINFO_H
