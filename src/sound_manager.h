#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <QObject>
#include <QSoundEffect>

class SoundManager : public QObject
{
    Q_OBJECT

public:
    explicit SoundManager(QObject *parent = nullptr);

    void playCorrectSound();
    void playWrongSound();
    void setMuted(bool muted);
    bool isMuted() const;

private:
    QSoundEffect *correct_sound;
    QSoundEffect *wrong_sound;
    bool muted;
};

#endif // SOUNDMANAGER_H