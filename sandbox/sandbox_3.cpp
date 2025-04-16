#include <cstdint>
#include <iostream>

#include "../lib/rtaudio/RtAudio.h"


// nimmt Audio vom Mikrofon und kopiert es zu den Lautsprechern
int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *data )
{
	// Since the number of input and output channels is equal, we can do
	// a simple buffer copy operation here.
	if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

	unsigned int *bytes = (unsigned int *) data;
	memcpy( outputBuffer, inputBuffer, *bytes );
	return 0;
}

int main(int argc, char** argv) {
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
	unsigned int bufferBytes, bufferFrames = 512;
	RtAudio::StreamParameters iParams, oParams;

	std::cout << "INPUT Gerät auswählen: [ID] > ";
	std::cin >> dev_id;
	std::cin.ignore();
	iParams.deviceId = stoul(dev_id);
	iParams.nChannels = 2;
	std::cout << "INPUT Gerät ausgewählt: " << iParams.deviceId << std::endl;

	std::cout << "OUTPUT Gerät auswählen: [ID] > ";
	std::cin >> dev_id;
	std::cin.ignore();
	oParams.deviceId = stoul(dev_id);
	oParams.nChannels = 2;
	std::cout << "OUTPUT Gerät ausgewählt: " << oParams.deviceId << std::endl;

	if (audio.openStream(&oParams, &iParams, RTAUDIO_SINT32, 44100,
                        &bufferFrames, &inout, (void *)&bufferBytes)) {

		std::cout << '\n' << audio.getErrorText() << '\n' << std::endl;
		exit( 0 ); // problem with device settings
	}

	bufferBytes = bufferFrames * 2 * 4;

	// Stream is open ... now start it.
	if ( audio.startStream() ) {
		std::cout << audio.getErrorText() << std::endl;
		goto cleanup;
	}

	char input;
	std::cout << "\nPlaying ... press <enter> to quit.\n";
	std::cin.get( input );

	// Block released ... stop the stream
	if ( audio.isStreamRunning() )
		audio.stopStream();  // or could call audio.abortStream();

cleanup:
	if ( audio.isStreamOpen() ) audio.closeStream();


	return 0;
}
