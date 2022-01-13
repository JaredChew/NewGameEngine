#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>

class Logger;

class Audio {

public:

	enum class AudioLoop { FOREVER = -1, ONCE = 0 };

private:

	FMOD::System *fmodSystem;
	FMOD::Sound *music;
	FMOD::Channel *musicChannel;

	int spectrumSize; //should be value in power of 2

	float *spectrumLeft;
	float *spectrumRight;

private:

	bool errorCheck(FMOD_RESULT result);

public:

	Audio(const char *audioDirectory, const int &spectrumSize);
	~Audio();

	void switchAudio(const char *audioDirectory);
	void changeSpectrumSize(const int &spectrumSize);

	void playAudio(const AudioLoop &loopType);
	//void pauseAudio();
	//void stopAudio();
	//void restartAudio();

	void update();

	float& const getSpectrumRight(const int &index);
	float& const getSpectrumLeft(const int &index);

};