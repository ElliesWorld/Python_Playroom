#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QFile>

struct Question {
    int id;
    QString question_text;
    QStringList options;
    int correct_answer;
    QString explanation;
    QString topic;
};

struct LevelInfo {
    int id;
    QString name;
    QString description;
    QString file_name;
    bool unlocked;
    int prerequisite;
    int time_per_question;
    int total_questions;
    QString difficulty;
};

class Questions : public QObject
{
    Q_OBJECT

public:
    explicit Questions(QObject *parent = nullptr);
    
    bool load_configuration();
    QVector<LevelInfo> available_levels();
    bool load_level(int level_id);
    bool level_unlocked(int level_id);
    void unlock_level(int level_id);
    
    // Current level methods
    Question current_question();
    LevelInfo current_level_info();
    int time_per_question();
    QString level_name();
    bool move_to_next();
    int total_questions();
    int current_index();
    bool has_questions();

private:
    QJsonDocument config_doc;
    QJsonDocument current_level;
    QVector<Question> current_questions;
    LevelInfo current_level_info_data;
    QString resource_path;
    int current_index_value;
    QVector<LevelInfo> levels_cache;
    
    void parse_level_data();
    void parse_config_data();
};

#endif