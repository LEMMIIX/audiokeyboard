#ifndef SOUNDWAVE_H
#define SOUNDWAVE_H

#include "../lib/rtaudio/RtAudio.h"

extern unsigned int sampleRate;
extern unsigned int bufferFrames;
extern double phase;
extern const double FREQUENCY;

static RtAudio::StreamParameters o_params;

int single_tone(void* o_buff, void* i_buff, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void* userData);

#endif // SOUNDWAVE_H
