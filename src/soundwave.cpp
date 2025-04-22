#include <cmath>

#include "soundwave.h"
#include "ToneInfo.h"

unsigned int sampleRate = 44100;
unsigned int bufferFrames = 256;
double phase = 0.0;
const double FREQUENCY = 290.0;
double cus_data = 0.0;

int single_tone(void* o_buff, void* i_buff, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void* userData) {

    double* buffer = (double*) o_buff;
    std::vector<ToneInfo>* tones = (std::vector<ToneInfo>*) userData;

    for(unsigned int i = 0; i < nBufferFrames * 2; ++i) {
        buffer[i] = 0.0;
    }

    // Zählen der aktiven Töne für die Normalisierung
    int activeCount = 0;
    for(auto& tone : *tones) {
        if(tone.active) {
            activeCount++;
        }
    }

    // Normalisierungsfaktor berechnen
    double normFactor = 0.5 / activeCount;

    // Für jeden aktiven Ton den Beitrag zum Buffer hinzufügen
    for(auto& tone : *tones) {
        if(!tone.active) continue;

        double* bufPtr = buffer;
        for(unsigned int i = 0; i < nBufferFrames; ++i) {
            *bufPtr++ += normFactor * sin(tone.phase) * tone.attack;
            *bufPtr++ += normFactor * sin(tone.phase) * tone.attack;

            // Phase für diesen Ton aktualisieren
            tone.phase += (2.0 * M_PI) * tone.frequency / sampleRate;
            if(tone.phase >= (2.0 * M_PI)) {
                tone.phase -= (2.0 * M_PI);
            }
            tone.attack = std::min(tone.attack + 0.04, 1.0);
        }
    }

    return 0;
}
