#include "audio.h"

#include <string>

#include "logger.h"

Audio::Audio(const char *audioDirectory, const int &spectrumSize) {

	FMOD_RESULT result;

	unsigned int version;

	result = FMOD::System_Create(&fmodSystem);
	errorCheck(result);

	result = fmodSystem->getVersion(&version);
	errorCheck(result);

	if (version < FMOD_VERSION) {
		Logger::getInstance()->customLog("FMOD ERROR", "You are using an older version of FMOD. ", version, " ", FMOD_VERSION);
	}

	//init fmod system
	result = fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	errorCheck(result);

	this->spectrumSize = spectrumSize;

	spectrumLeft = new float[spectrumSize];
	spectrumRight = new float[spectrumSize];

	switchAudio(audioDirectory);

}

Audio::~Audio() {

	if (errorCheck(musicChannel->stop())) { musicChannel = nullptr; }
	if (errorCheck(music->release())) { music = nullptr; }
	if (errorCheck(fmodSystem->release())) { fmodSystem = nullptr; }

	delete[] spectrumLeft;
	delete[] spectrumRight;

}

bool Audio::errorCheck(FMOD_RESULT result) {

	if (result != FMOD_OK) {
		Logger::getInstance()->customLog("FMOD ERROR", "(%d) %s", result, FMOD_ErrorString(result));
		return false;
	}

	return true;

}

void Audio::switchAudio(const char *audioDirectory) {
	errorCheck(fmodSystem->createStream(audioDirectory, FMOD_SOFTWARE, 0, &music));
}

void Audio::changeSpectrumSize(const int &spectrumSize) {

	this->spectrumSize = spectrumSize;

	delete[] spectrumLeft;
	delete[] spectrumRight;

	spectrumLeft = new float[spectrumSize];
	spectrumRight = new float[spectrumSize];

}

void Audio::playAudio(const AudioLoop &loopType) {

	if (errorCheck(fmodSystem->playSound(FMOD_CHANNEL_FREE, music, false, &musicChannel))) {
		musicChannel->setLoopCount((int)loopType);
	}

}

void Audio::update() {

	fmodSystem->update();

	//Get spectrum for left and right stereo channes
	musicChannel->getSpectrum(spectrumLeft, spectrumSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
	musicChannel->getSpectrum(spectrumRight, spectrumSize, 1, FMOD_DSP_FFT_WINDOW_RECT);

}

float& const Audio::getSpectrumRight(const int &index) {
	return spectrumRight[index];
}

float& const Audio::getSpectrumLeft(const int &index) {
	return spectrumLeft[index];
}