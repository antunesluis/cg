#include "AudioSystem.h"

AudioSystem::AudioSystem() : isPlaying(false), currentFile("") {}

AudioSystem::~AudioSystem() { stopBackgroundMusic(); }

void AudioSystem::playBackgroundMusic(const std::string &filename, bool loop)
{
    stopBackgroundMusic();
    currentFile = filename;
    DWORD flags = SND_ASYNC | SND_FILENAME;
    if (loop)
        flags |= SND_LOOP;
    isPlaying = PlaySoundA(filename.c_str(), NULL, flags); // Usando PlaySoundA para ANSI
}

void AudioSystem::stopBackgroundMusic()
{
    if (isPlaying) {
        PlaySound(NULL, NULL, 0);
        isPlaying = false;
    }
}

void AudioSystem::toggleBackgroundMusic()
{
    if (isPlaying) {
        stopBackgroundMusic();
    } else if (!currentFile.empty()) {
        playBackgroundMusic(currentFile);
    }
}

bool AudioSystem::isMusicPlaying() const { return isPlaying; }
