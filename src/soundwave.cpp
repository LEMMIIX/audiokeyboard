#include <cmath>

#include "soundwave.h"
#include "toneengine.h"

int audiocallback(void* o_buff, void* i_buff, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void* userData) {

    ToneEngine* engine = static_cast<ToneEngine*>(userData);
    engine->proc_tones(static_cast<double*>(o_buff), nBufferFrames);
    return 0;
}
