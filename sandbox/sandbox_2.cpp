#include <cstdint>
#include <iostream>

#include "../lib/rtaudio/RtAudio.h"


// Two-channel sawtooth wave generator.
int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData )
{
	unsigned int i, j;
	double *buffer = (double *) outputBuffer;
	double *lastValues = (double *) userData;

	if ( status )
		std::cout << "Stream underflow detected!" << std::endl;

	// Write interleaved audio data.
	for ( i=0; i<nBufferFrames; i++ ) {
		for ( j=0; j<2; j++ ) {
			*buffer++ = lastValues[j];

			lastValues[j] += 0.005 * (j+1+(j*0.1));
			if ( lastValues[j] >= 1.0 ) lastValues[j] -= 2.0;
		}
	}
 
  return 0;
}

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
	std::cin.ignore(); // Claudeai: durch das '\n' das nach dem ENTER entsteht, bleibt dies
					   // im Eingabepuffer zurück. Durch das nächste cin (um das Programm
					   // abzubrechen) wird nun dieser Zeilenumbruch aus dem Puffer gelesen
					   // und das Programm sofort beendet.

	parameters.deviceId = stoul(dev_id);
	parameters.nChannels = 2;
	parameters.firstChannel = 0;
	unsigned int sampleRate = 44100;
	unsigned int bufferFrames = 256; // 256 sample frames
	double data[2] = {0, 0};

	std::cout << "Gerät ausgewählt: " << parameters.deviceId << std::endl;

	if ( audio.openStream( &parameters, NULL, RTAUDIO_FLOAT64, sampleRate,
				&bufferFrames, &saw, (void *)&data ) ) {
		std::cout << '\n' << audio.getErrorText() << '\n' << std::endl;
		exit( 0 ); // problem with device settings
	}

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
