#ifndef QUIZ_H
#define QUIZ_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QTimer>
#include <QVector>
#include "questions.h"
#include <QStyle>

class Quiz : public QWidget
{
    Q_OBJECT

public:
    explicit Quiz(Questions *questions, QWidget *parent = nullptr);
    void start_quiz(int level_id);
    void reset_quiz();

private slots:
    void on_answer_selected();
    void on_next_question();
    void on_learn_more();
    void on_timer_tick();
    void on_time_up();

private:
    Questions *question_manager;
    QTimer *countdown_timer;

    QLabel *question_label;
    QVector<QRadioButton *> option_buttons;
    QButtonGroup *button_group;
    QPushButton *next_button;
    QPushButton *learn_more_button;
    QPushButton *back_button;
    QLabel *timer_label;
    QLabel *progress_label;
    QLabel *explanation_label;

    int time_remaining;
    int score;
    bool question_answered;
    bool explanation_shown;
    int selected_answer;

    void setup_ui();
    void display_current_question();
    void reset_timer();
    void update_progress();
    void show_explanation();
    void hide_explanation();
    void enable_navigation();
    void disable_navigation();

signals:
    void quiz_completed(int final_score);
    void return_to_welcome();
};

#endif