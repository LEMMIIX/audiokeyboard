#ifndef SOUNDWAVE_H
#define SOUNDWAVE_H

#include "../lib/rtaudio/RtAudio.h"

static RtAudio::StreamParameters o_params;

int audiocallback(void* o_buff, void* i_buff, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void* userData);

#endif // SOUNDWAVE_H
