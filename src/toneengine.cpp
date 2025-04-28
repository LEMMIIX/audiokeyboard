#include <iostream>
#include <stdio.h>


#include "toneengine.h"

unsigned int sampleRate = 44100;
unsigned int bufferFrames = 256;
double phase = 0.0;
const double FREQUENCY = 290.0;

ToneEngine::ToneEngine() {}

void ToneEngine::initialize(size_t size) {
    //std::cout << "vec init Größe: " << size << std::endl;

    tones.clear();
    tones.resize(size);
    //printf("tones.size() = %d\n", tones.size());
    for(ToneInfo& tone : tones) {
        tone.active = false;
        tone.attack = 0.0;
        tone.decay = 0.1;
        tone.frequency = 0.0;
        tone.phase = 0.0;
        tone.release = 1.0;
        tone.sustain = 0.8;
    }
    /*
    for(int i = 0; i < tones.size(); ++i) {
        printf("tones[%p].decay = %f\n", &tones[i], tones[i].decay);
    }*/
}

void ToneEngine::set_tone_freq(int i, double frequency) {
    tones[i].frequency = frequency;
}

void ToneEngine::set_waveform(WaveformGenerator generator) {
    waveGenerator = generator;
}

double ToneEngine::calc_envelope(ToneInfo& tone, double deltaTime) {
    return 1.0;
}

void ToneEngine::proc_tones(double* o_buff, unsigned int nBufferFrames) {

    std::lock_guard<std::mutex> lock(tonesMutex);
    for(unsigned int i = 0; i < nBufferFrames * 2; ++i) {
        o_buff[i] = 0.0;
    }

    // Zählen der aktiven Töne für die Normalisierung
    int activeCount = 0;
    for(auto& tone : tones) {
        if(tone.active) {
            activeCount++;
        }
    }

    if(activeCount == 0) {
        return;
    }
    // Normalisierungsfaktor berechnen
    double normFactor = 0.5 / activeCount;


    // Für jeden aktiven Ton den Beitrag zum Buffer hinzufügen
    for(auto& tone : tones) {
        if(!tone.active) {
            tone.attack = 0.0;
            continue;
        }

        tone.attack = std::min(tone.attack + 0.04, 1.0);

        double* bufPtr = o_buff;
        for(unsigned int i = 0; i < nBufferFrames; ++i) {
            double sampleValue = waveGenerator(tone.phase);

            *bufPtr++ += normFactor * sampleValue * tone.attack;
            *bufPtr++ += normFactor * sampleValue * tone.attack;

            // Phase für diesen Ton aktualisieren
            tone.phase += (2.0 * M_PI) * tone.frequency / sampleRate;
            if(tone.phase >= (2.0 * M_PI)) {
                tone.phase -= (2.0 * M_PI);
            }
        }
    }
}

void ToneEngine::noteOn(int index) {

    if(tones.empty()) {
        std::cout << "tones LEER" << std::endl;
        return;
    } else if (index < 0 || index >= static_cast<int>(tones.size())) {
        std::cout << "INDEX AU?ERHALB BEREICH" << std::endl;
        return;
    }

    std::lock_guard<std::mutex> lock(tonesMutex);
    tones[index].active = true;
    tones[index].attack = 0.0;

    std::cout << "noteOn " << index << std::endl;
}

void ToneEngine::noteOff(int index) {
    if(index >= 0 && index < static_cast<int>(tones.size())) {
        std::lock_guard<std::mutex> lock(tonesMutex);
        tones[index].active = false;
    }
    std::cout << "\tnoteOff " << index << std::endl;
}



double sine_wave(double phase) {
    return sin(phase);
}
