#include "sound_manager.h"
#include <QUrl>

SoundManager::SoundManager(QObject *parent)
    : QObject(parent), muted(false)
{
    correct_sound = new QSoundEffect(this);
    correct_sound->setSource(QUrl("qrc:/sounds/correct.wav"));
    correct_sound->setVolume(0.7);

    wrong_sound = new QSoundEffect(this);
    wrong_sound->setSource(QUrl("qrc:/sounds/wrong.wav"));
    wrong_sound->setVolume(0.7);
}

void SoundManager::playCorrectSound()
{
    if (!muted && correct_sound->status() == QSoundEffect::Ready)
    {
        correct_sound->play();
    }
}

void SoundManager::playWrongSound()
{
    if (!muted && wrong_sound->status() == QSoundEffect::Ready)
    {
        wrong_sound->play();
    }
}

void SoundManager::setMuted(bool muted)
{
    this->muted = muted;
}

bool SoundManager::isMuted() const
{
    return muted;
}