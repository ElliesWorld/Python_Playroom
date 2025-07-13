#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QFile>
#include <QDebug>
#include "settings_manager.h"

struct LevelInfo
{
    int id;
    QString name;
    QString file_name;
    bool unlocked;
    int prerequisite;
};

struct Question
{
    int id;
    QString question_text;
    QVector<QString> options;
    int correct_answer;
    QString explanation;
    QString topic;
};

struct CurrentLevelInfo
{
    int id;
    QString name;
    QString description;
    int time_per_question;
    int total_questions;
    QString difficulty;
};

class Questions : public QObject
{
    Q_OBJECT

public:
    explicit Questions(SettingsManager *settingsManager, QObject *parent = nullptr);

    bool load_configuration();
    void parse_config_data();

    QVector<LevelInfo> available_levels();
    bool level_unlocked(int level_id);
    void unlock_level(int level_id);

    bool load_level(int level_id);
    void parse_level_data();
    Question current_question();
    bool move_to_next();
    bool has_questions();

    CurrentLevelInfo current_level_info();
    int time_per_question();
    QString level_name();
    int total_questions();
    int current_index();

private slots:
    void on_mode_changed(QuizMode newMode);

private:
    SettingsManager *settings_manager;

    QJsonDocument config_doc;
    QVector<LevelInfo> levels_cache;

    QJsonDocument current_level;
    QVector<Question> current_questions;
    CurrentLevelInfo current_level_info_data;
    int current_index_value;

    QString getConfigPath() const;
    QString getQuestionPath(const QString &fileName) const;
};

#endif 