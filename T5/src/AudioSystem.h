#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <string>
#include <windows.h>

class AudioSystem
{
  private:
    bool isPlaying;
    std::string currentFile;

  public:
    AudioSystem();
    ~AudioSystem();

    void playBackgroundMusic(const std::string &filename, bool loop = true);
    void stopBackgroundMusic();
    void toggleBackgroundMusic();
    bool isMusicPlaying() const;
};

#endif // AUDIO_SYSTEM_H
