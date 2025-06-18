#include "quiz.h"
#include <QDebug>

Quiz::Quiz(Questions *questions, QWidget *parent)
    : QWidget(parent), question_manager(questions), time_remaining(0),
      score(0), question_answered(false), explanation_shown(false), selected_answer(-1)
{
    countdown_timer = new QTimer(this);
    connect(countdown_timer, &QTimer::timeout, this, &Quiz::on_timer_tick);

    setup_ui();
}

void Quiz::setup_ui()
{
    setObjectName("quiz_screen");

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(15);
    main_layout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *top_layout = new QHBoxLayout();

    progress_label = new QLabel("Question 1 of 3");
    progress_label->setObjectName("progress_label");

    timer_label = new QLabel("Time: 30s");
    timer_label->setObjectName("timer_label");

    top_layout->addWidget(progress_label);
    top_layout->addStretch();
    top_layout->addWidget(timer_label);

    main_layout->addLayout(top_layout);

    question_label = new QLabel("Question will appear here");
    question_label->setObjectName("question_label");
    question_label->setWordWrap(true);
    main_layout->addWidget(question_label);

    button_group = new QButtonGroup(this);
    for (int i = 0; i < 4; ++i)
    {
        QRadioButton *option = new QRadioButton();
        option_buttons.append(option);
        button_group->addButton(option, i);
        main_layout->addWidget(option);

        connect(option, &QRadioButton::clicked, this, &Quiz::on_answer_selected);
    }

    explanation_label = new QLabel();
    explanation_label->setObjectName("explanation_label");
    explanation_label->setWordWrap(true);
    explanation_label->setVisible(false);
    explanation_label->setStyleSheet("background-color: #3a3a3a; padding: 15px; border-radius: 8px; color: #ffeb3b; border: 2px solid #ff9800;");
    main_layout->addWidget(explanation_label);

    QHBoxLayout *button_layout = new QHBoxLayout();

    back_button = new QPushButton("Back to Menu");
    back_button->setObjectName("back_button");
    connect(back_button, &QPushButton::clicked, this, &Quiz::return_to_welcome);

    learn_more_button = new QPushButton("Learn More");
    learn_more_button->setObjectName("learn_more_button");
    connect(learn_more_button, &QPushButton::clicked, this, &Quiz::on_learn_more);

    next_button = new QPushButton("Next Question");
    next_button->setObjectName("next_button");
    next_button->setEnabled(false);
    connect(next_button, &QPushButton::clicked, this, &Quiz::on_next_question);

    button_layout->addWidget(back_button);
    button_layout->addStretch();
    button_layout->addWidget(learn_more_button);
    button_layout->addWidget(next_button);

    main_layout->addLayout(button_layout);
    main_layout->addStretch();
}

void Quiz::start_quiz(int level_id)
{
    if (!question_manager->load_level(level_id))
    {
        qDebug() << "Failed to load level:" << level_id;
        return;
    }

    score = 0;
    reset_quiz();
    display_current_question();
}

void Quiz::reset_quiz()
{
    question_answered = false;
    explanation_shown = false;
    selected_answer = -1;

    button_group->setExclusive(false); 
    for (auto *button : option_buttons)
    {
        button->setChecked(false);
        button->setEnabled(true);
        button->setAutoExclusive(false); 
    }
    button_group->setExclusive(true); 

    hide_explanation();
    next_button->setEnabled(false);
    learn_more_button->setEnabled(true);
}

void Quiz::display_current_question()
{
    if (!question_manager->has_questions())
    {
        return;
    }

    Question current = question_manager->current_question();

    question_label->setText(current.question_text);

    for (int i = 0; i < option_buttons.size() && i < current.options.size(); ++i)
    {
        option_buttons[i]->setText(current.options[i]);
        option_buttons[i]->setVisible(true);
    }

    for (int i = current.options.size(); i < option_buttons.size(); ++i)
    {
        option_buttons[i]->setVisible(false);
    }

    update_progress();
    reset_timer();
    reset_quiz();
}

void Quiz::update_progress()
{
    int current = question_manager->current_index() + 1;
    int total = question_manager->total_questions();
    progress_label->setText(QString("Question %1 of %2").arg(current).arg(total));
}

void Quiz::reset_timer()
{
    time_remaining = question_manager->time_per_question();

    QString timerText = QString("🕐 %1s").arg(time_remaining);
    timer_label->setText(QString("🕐 %1s").arg(time_remaining));

    if (time_remaining > 10)
    {
        timer_label->setProperty("timerState", "green");
    }
    else if (time_remaining > 5)
    {
        timer_label->setProperty("timerState", "yellow");
    }
    else
    {
        timer_label->setProperty("timerState", "red");
    }

    timer_label->style()->unpolish(timer_label);
    timer_label->style()->polish(timer_label);

    countdown_timer->start(1000);
}

void Quiz::on_timer_tick()
{
    time_remaining--;

    QString timerText = QString("🕐 %1s").arg(time_remaining);
    timer_label->setText(timerText);

    if (time_remaining > 10)
    {
        timer_label->setProperty("timerState", "green");
    }
    else if (time_remaining > 5)
    {
        timer_label->setProperty("timerState", "yellow");
    }
    else
    {
        timer_label->setProperty("timerState", "red");
    }

    timer_label->style()->unpolish(timer_label);
    timer_label->style()->polish(timer_label);

    if (time_remaining <= 0)
    {
        on_time_up();
    }
}

void Quiz::on_time_up()
{
    countdown_timer->stop();

    if (!question_answered)
    {
        for (auto *button : option_buttons)
        {
            button->setEnabled(false);
        }

        question_answered = true;
        learn_more_button->setEnabled(false);
        enable_navigation();

        timer_label->setText("⏰ Time's Up!");
        timer_label->setProperty("timerState", "red");
        timer_label->style()->unpolish(timer_label);
        timer_label->style()->polish(timer_label);
    }
}

void Quiz::on_answer_selected()
{
    if (question_answered)
    {
        return;
    }

    for (int i = 0; i < option_buttons.size(); ++i)
    {
        if (option_buttons[i]->isChecked())
        {
            selected_answer = i;
            break;
        }
    }

    countdown_timer->stop();
    question_answered = true;

    Question current = question_manager->current_question();
    if (selected_answer == current.correct_answer)
    {
        score++;
        option_buttons[selected_answer]->setStyleSheet("QRadioButton { color: #4CAF50; font-weight: bold; }");
    }
    else
    {
        option_buttons[selected_answer]->setStyleSheet("QRadioButton { color: #f44336; font-weight: bold; }");
        option_buttons[current.correct_answer]->setStyleSheet("QRadioButton { color: #4CAF50; font-weight: bold; }");
    }

    for (auto *button : option_buttons)
    {
        button->setEnabled(false);
    }

    learn_more_button->setEnabled(false);
    enable_navigation();
}

void Quiz::on_learn_more()
{
    if (explanation_shown)
    {
        return;
    }

    countdown_timer->stop();
    question_answered = true;
    explanation_shown = true;

    show_explanation();

    for (auto *button : option_buttons)
    {
        button->setEnabled(false);
    }

    learn_more_button->setEnabled(false);
    enable_navigation();
}

void Quiz::show_explanation()
{
    Question current = question_manager->current_question();
    explanation_label->setText(QString("💡 %1").arg(current.explanation));
    explanation_label->setVisible(true);
}

void Quiz::hide_explanation()
{
    explanation_label->setVisible(false);
}

void Quiz::enable_navigation()
{
    next_button->setEnabled(true);

    if (question_manager->current_index() >= question_manager->total_questions() - 1)
    {
        next_button->setText("Finish Quiz");
    }
    else
    {
        next_button->setText("Next Question");
    }
}

void Quiz::disable_navigation()
{
    next_button->setEnabled(false);
}

void Quiz::on_next_question()
{
    for (auto *button : option_buttons)
    {
        button->setStyleSheet("");
    }

    if (question_manager->move_to_next())
    {
        display_current_question();
    }
    else
    {
        countdown_timer->stop();
        emit quiz_completed(score);
    }
}