#include "questions.h"
#include <QDebug>

Questions::Questions(SettingsManager *settingsManager, QObject *parent)
    : QObject(parent), settings_manager(settingsManager), current_index_value(0)
{
    connect(settings_manager, &SettingsManager::modeChanged,
            this, &Questions::on_mode_changed);

    load_configuration();
}

QString Questions::getConfigPath() const
{
    return settings_manager->getConfigFileName();
}

QString Questions::getQuestionPath(const QString &fileName) const
{
    QString folder = settings_manager->getQuestionsFolder();
    return QString(":/%1/%2").arg(folder).arg(fileName);
}

bool Questions::load_configuration()
{
    QString configPath = getConfigPath();
    QFile config_file(configPath);

    if (!config_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open config file:" << configPath;
        return false;
    }

    config_doc = QJsonDocument::fromJson(config_file.readAll());
    config_file.close();

    if (config_doc.isNull())
    {
        qDebug() << "Invalid JSON in config file:" << configPath;
        return false;
    }

    parse_config_data();
    return true;
}

void Questions::parse_config_data()
{
    QJsonObject root = config_doc.object();
    QJsonArray levels_array = root["levels"].toArray();

    levels_cache.clear();
    for (const auto &level_value : levels_array)
    {
        QJsonObject level_obj = level_value.toObject();

        LevelInfo level;
        level.id = level_obj["id"].toInt();
        level.name = level_obj["name"].toString();
        level.file_name = level_obj["file"].toString();
        level.unlocked = level_obj["unlocked"].toBool();
        level.prerequisite = level_obj["prerequisite"].toInt();

        levels_cache.append(level);
    }
}

QVector<LevelInfo> Questions::available_levels()
{
    return levels_cache;
}

bool Questions::load_level(int level_id)
{
    QString file_name;
    for (const auto &level : levels_cache)
    {
        if (level.id == level_id)
        {
            file_name = level.file_name;
            break;
        }
    }

    if (file_name.isEmpty())
    {
        qDebug() << "Level not found:" << level_id;
        return false;
    }

    QString file_path = getQuestionPath(file_name);
    QFile level_file(file_path);

    if (!level_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open level file:" << file_path;
        return false;
    }

    current_level = QJsonDocument::fromJson(level_file.readAll());
    level_file.close();

    if (current_level.isNull())
    {
        qDebug() << "Invalid JSON in level file:" << file_path;
        return false;
    }

    parse_level_data();
    current_index_value = 0;
    return true;
}

void Questions::parse_level_data()
{
    QJsonObject root = current_level.object();

    QJsonObject level_info_obj = root["level_info"].toObject();
    current_level_info_data.id = level_info_obj["id"].toInt();
    current_level_info_data.name = level_info_obj["name"].toString();
    current_level_info_data.description = level_info_obj["description"].toString();
    current_level_info_data.time_per_question = level_info_obj["time_per_question"].toInt();
    current_level_info_data.total_questions = level_info_obj["total_questions"].toInt();
    current_level_info_data.difficulty = level_info_obj["difficulty"].toString();

    current_questions.clear();
    QJsonArray questions_array = root["questions"].toArray();

    for (const auto &question_value : questions_array)
    {
        QJsonObject question_obj = question_value.toObject();

        Question question;
        question.id = question_obj["id"].toInt();
        question.question_text = question_obj["question_text"].toString();
        question.correct_answer = question_obj["correct_answer"].toInt();
        question.explanation = question_obj["explanation"].toString();
        question.topic = question_obj["topic"].toString();

        QJsonArray options_array = question_obj["options"].toArray();
        for (const auto &option_value : options_array)
        {
            question.options.append(option_value.toString());
        }

        current_questions.append(question);
    }
}

bool Questions::level_unlocked(int level_id)
{
    for (const auto &level : levels_cache)
    {
        if (level.id == level_id)
        {
            return level.unlocked;
        }
    }
    return false;
}

void Questions::unlock_level(int level_id)
{
    for (auto &level : levels_cache)
    {
        if (level.id == level_id)
        {
            level.unlocked = true;
            break;
        }
    }
}

Question Questions::current_question()
{
    if (current_index_value >= 0 && current_index_value < current_questions.size())
    {
        return current_questions[current_index_value];
    }
    return Question();
}

CurrentLevelInfo Questions::current_level_info()
{
    return current_level_info_data;
}

int Questions::time_per_question()
{
    return current_level_info_data.time_per_question;
}

QString Questions::level_name()
{
    return current_level_info_data.name;
}

bool Questions::move_to_next()
{
    if (current_index_value < current_questions.size() - 1)
    {
        current_index_value++;
        return true;
    }
    return false;
}

int Questions::total_questions()
{
    return current_questions.size();
}

int Questions::current_index()
{
    return current_index_value;
}

bool Questions::has_questions()
{
    return !current_questions.isEmpty();
}

void Questions::on_mode_changed(QuizMode newMode)
{
    qDebug() << "Mode changed, reloading configuration for mode:" << static_cast<int>(newMode);
    load_configuration();
}