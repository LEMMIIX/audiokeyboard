#include <cstdint>
#include <iostream>

#include "../lib/rtaudio/RtAudio.h"

int main(void) {
	RtAudio audio;

	std::vector<unsigned int> ids = audio.getDeviceIds();
	if(ids.size() == 0) {
		std::cout << "Keine audiogeräte gefunden!" << std::endl;
	}

	RtAudio::DeviceInfo info;

	std::cout << "ID\tGerätename" << std::endl;
	for(int n = 0; n < ids.size();  ++n) {
		info = audio.getDeviceInfo(ids[n]);

		std::cout << info.ID << "\t" << info.name << std::endl;
	}

	std::string dev_id;
	RtAudio::StreamParameters parameters;
	
	std::cout << "Gerät auswählen: [ID] > ";
	std::cin >> dev_id;
	
	parameters.deviceId = stoul(dev_id);
	parameters.nChannels = 2;
	parameters.firstChannel = 0;
	unsigned int sampleRate = 44100;
	unsigned int bufferFrames = 256; // 256 sample frames
	double data[2] = {0, 0};

	std::cout << "Gerät ausgewählt: " << parameters.deviceId << std::endl;

	return 0;
}
