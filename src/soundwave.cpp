#include <cmath>

#include "soundwave.h"
#include "ToneInfo.h"

unsigned int sampleRate = 44100;
unsigned int bufferFrames = 256;
double phase = 0.0;
const double FREQUENCY = 400.0;
double cus_data = 0.0;

int single_tone(void* o_buff, void* i_buff, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void* userData) {

    double* buffer = (double*) o_buff;
    std::vector<ToneInfo>* tones = (std::vector<ToneInfo>*) userData;

    // Buffer zunächst leeren
    for(unsigned int i = 0; i < nBufferFrames * 2; ++i) {
        buffer[i] = 0.0;
    }

    // Zählen der aktiven Töne für die Normalisierung
    int activeCount = 0;
    for(auto& tone : *tones) {
        if(tone.active) activeCount++;
    }

    // Wenn keine Töne aktiv sind, Stille zurückgeben
    //if(activeCount == 0) return 0;

    // Normalisierungsfaktor berechnen (verhindert Clipping)
    double normFactor = 0.5 / activeCount;

    // Für jeden aktiven Ton den Beitrag zum Buffer hinzufügen
    for(auto& tone : *tones) {
        if(!tone.active) continue;

        double* bufPtr = buffer;
        for(unsigned int i = 0; i < nBufferFrames; ++i) {
            // Beitrag zum linken und rechten Kanal hinzufügen
            *bufPtr++ += normFactor * sin(tone.phase);
            *bufPtr++ += normFactor * sin(tone.phase);

            // Phase für diesen Ton aktualisieren
            tone.phase += (2.0 * M_PI) * tone.frequency / sampleRate;
            if(tone.phase >= (2.0 * M_PI)) {
                tone.phase -= (2.0 * M_PI);
            }
        }
    }

    return 0;
}
