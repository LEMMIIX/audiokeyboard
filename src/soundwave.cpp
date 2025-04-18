#include <cmath>

#include "soundwave.h"

unsigned int sampleRate = 44100;
unsigned int bufferFrames = 256;
double phase = 0.0;
const double FREQUENCY = 400.0;
double cus_data = 0.0;

int single_tone(void* o_buff, void* i_buff, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void* userData) {

    double* buffer = (double*) o_buff;
    double plus_freq = *((double*) userData);
    if(*((double*) userData) > 0.0) {
        //std::cout << "single_tone spielt gerade einen hübschen Ton ja ja" << std::endl;

        for(unsigned int i = 0; i < nBufferFrames; ++i) {
            *buffer++ = 0.5 * sin(phase);
            *buffer++ = 0.5 * sin(phase);

            phase += (2.0 * M_PI) * (FREQUENCY + plus_freq) / sampleRate;
            if(phase >= (2.0 * M_PI)) {
                phase -= (2.0 * M_PI);
            }
        }
    } else {
        for(unsigned int i = 0; i < nBufferFrames; ++i) {
            *buffer++ = 0.0;
            *buffer++ = 0.0;
        }
        phase = 0.0;
    }

    return 0;
}
