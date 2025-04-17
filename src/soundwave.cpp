#include <cmath>

#include "soundwave.h"

unsigned int sampleRate = 44100;
unsigned int bufferFrames = 256;
double phase = 0.0;
const double FREQUENCY = 440.0;

int single_tone(void* o_buff, void* i_buff, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void* userData) {

    std::cout << "single_tone spielt gerade einen hübschen Ton ja ja" << std::endl;
    double* buffer = (double*) o_buff;

    for(unsigned int i = 0; i < nBufferFrames; ++i) {
        *buffer++ = 0.5 * sin(phase);
        *buffer++ = 0.5 * sin(phase);

        phase += (2.0 * M_PI) * FREQUENCY / sampleRate;
        if(phase >= (2.0 * M_PI)) {
            phase -= (2.0 * M_PI);
        }
    }

    return 0;
}
