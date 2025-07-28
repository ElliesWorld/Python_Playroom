#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <QObject>
#include <QSettings>
#include <QString>

enum class QuizMode
{
    Python,
    Cpp,
    C
};

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    explicit SettingsManager(QObject *parent = nullptr);

    QuizMode currentMode() const;
    void setCurrentMode(QuizMode mode);

    QString modeToString(QuizMode mode) const;
    QuizMode stringToMode(const QString &modeStr) const;
    QString modeToDisplayName(QuizMode mode) const;
    QString modeToIcon(QuizMode mode) const;

    QString getConfigFileName() const;
    QString getQuestionsFolder() const;

signals:
    void modeChanged(QuizMode newMode);

private:
    QSettings *settings;
    QuizMode current_mode;

    void loadSettings();
    void saveSettings();
};

#endif