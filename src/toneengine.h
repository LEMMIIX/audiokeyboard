#ifndef TONEENGINE_H
#define TONEENGINE_H

#include <vector>
#include <cmath>
#include <mutex>

#include "ToneInfo.h"

extern unsigned int sampleRate;
extern unsigned int bufferFrames;
extern double phase;
extern const double FREQUENCY;

typedef double (*WaveformGenerator)(double phase);

/**
 * @brief class ToneEngine managed die Berechnungen für einzelne Töne
 */
class ToneEngine
{
public:
    ToneEngine();

    /**
     * @brief setWaveform setzt ein beliebiges "Instrument"
     * @param generator
     */
    void set_waveform(WaveformGenerator generator);

    double calc_envelope(ToneInfo& tone, double deltaTime);
    void proc_tones(double* o_buff, unsigned int nFrames);

    void noteOn(int noteIndex);
    void noteOff(int noteIndex);

    void initialize(size_t size);

    void set_tone_freq(int i, double frequency);

private:
    std::vector<ToneInfo> tones;
    WaveformGenerator waveGenerator;
    std::mutex tonesMutex;
};

/*
 * Generatoren - verschiedene Tonarten/"Instrumente"
 */
double sine_wave(double phase);

#endif // TONEENGINE_H
