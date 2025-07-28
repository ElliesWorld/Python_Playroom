#ifndef QUIZ_H
#define QUIZ_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTimer>
#include <QPropertyAnimation>
#include <QString>
#include "questions.h"

class SoundManager;

class Quiz : public QWidget
{
    Q_OBJECT

public:
    explicit Quiz(Questions *questions, QWidget *parent = nullptr);

    void start_quiz(int level_id);
    void onModeChanged(); // New method to handle mode changes

signals:
    void quiz_completed(int score);
    void return_to_welcome();

private slots:
    void on_timer_tick();
    void on_time_up();
    void on_learn_more();
    void on_next_question();
    void on_answer_selected();
    void toggle_mute();

private:
    void setup_ui();
    void display_current_question();
    void update_progress();
    void reset_timer();
    void reset_quiz();
    void show_explanation();
    void hide_explanation();
    void enable_navigation();
    void disable_navigation();
    void update_mute_button();
    void show_celebration(const QString &message = "🎉 Great job! 🎉");

    Questions *question_manager;
    SoundManager *sound_manager;

    QTimer *countdown_timer;
    int time_remaining;

    QLabel *progress_label;
    QLabel *timer_label;
    QLabel *question_label;
    QLabel *smiley_label;
    QLabel *explanation_label;
    QPushButton *mute_button;
    QPushButton *back_button;
    QPushButton *learn_more_button;
    QPushButton *next_button;
    QVector<QRadioButton *> option_buttons;
    QButtonGroup *button_group;
    QPropertyAnimation *smiley_animation;

    int score;
    bool question_answered;
    bool explanation_shown;
    int selected_answer;
};

#endif 