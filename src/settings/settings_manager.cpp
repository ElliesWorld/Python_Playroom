#include "settings_manager.h"
#include <QDebug>

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent), current_mode(QuizMode::Python)
{
    settings = new QSettings("LearningApps", "ProgrammingQuiz", this);
    loadSettings();
}

QuizMode SettingsManager::currentMode() const
{
    return current_mode;
}

void SettingsManager::setCurrentMode(QuizMode mode)
{
    if (current_mode != mode)
    {
        current_mode = mode;
        saveSettings();
        emit modeChanged(mode);
    }
}

QString SettingsManager::modeToString(QuizMode mode) const
{
    switch (mode)
    {
    case QuizMode::Python:
        return "python";
    case QuizMode::Cpp:
        return "cpp";
    case QuizMode::C:
        return "c";
    default:
        return "python";
    }
}

QuizMode SettingsManager::stringToMode(const QString &modeStr) const
{
    if (modeStr == "cpp")
        return QuizMode::Cpp;
    else if (modeStr == "c")
        return QuizMode::C;
    else
        return QuizMode::Python;
}

QString SettingsManager::modeToDisplayName(QuizMode mode) const
{
    switch (mode)
    {
    case QuizMode::Python:
        return "Python Learning Quiz";
    case QuizMode::Cpp:
        return "C++ Learning Quiz";
    case QuizMode::C:
        return "C Learning Quiz";
    default:
        return "Programming Quiz";
    }
}

QString SettingsManager::modeToIcon(QuizMode mode) const
{
    switch (mode)
    {
    case QuizMode::Python:
        return "🐍";
    case QuizMode::Cpp:
        return "⚡";
    case QuizMode::C:
        return "🔧";
    default:
        return "💻";
    }
}

QString SettingsManager::getConfigFileName() const
{
    if (current_mode == QuizMode::Python)
    {
        return ":/config.json"; 
    }
    return QString(":/config_%1.json").arg(modeToString(current_mode));
}

QString SettingsManager::getQuestionsFolder() const
{
    if (current_mode == QuizMode::Python)
    {
        return "questions"; 
    }
    return QString("questions_%1").arg(modeToString(current_mode));
}

void SettingsManager::loadSettings()
{
    QString modeStr = settings->value("quiz_mode", "python").toString();
    current_mode = stringToMode(modeStr);
}

void SettingsManager::saveSettings()
{
    settings->setValue("quiz_mode", modeToString(current_mode));
    settings->sync();
}