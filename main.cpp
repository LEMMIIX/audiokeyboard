#include <iostream>

#include "lib/rtaudio/RtAudio.h"

int main(void) {
	RtAudio audio;

	std::vector<unsigned int> ids = audio.getDeviceIds();
	if(ids.size() == 0) {
		std::cout << "Keine audiogeräte gefunden!" << std::endl;
	}

	RtAudio::DeviceInfo info;

	for(int n = 0; n < ids.size();  ++n) {
		info = audio.getDeviceInfo(ids[n]);

		std::cout << "Id:\t" << info.ID << "\tGerätename:\t" << info.name << "\tChannels:\t" << info.outputChannels << std::endl;
	}

	return 0;
}
