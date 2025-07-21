#include "quiz.h"
#include "sound_manager.h"
#include <QStyle> // Add this include for QStyle
#include <QDebug>

Quiz::Quiz(Questions *questions, QWidget *parent)
    : QWidget(parent), question_manager(questions), time_remaining(0),
      score(0), question_answered(false), explanation_shown(false), selected_answer(-1)
{
    countdown_timer = new QTimer(this);
    connect(countdown_timer, &QTimer::timeout, this, &Quiz::on_timer_tick);

    sound_manager = new SoundManager(this);

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

    mute_button = new QPushButton("🔊");
    mute_button->setObjectName("mute_button");
    connect(mute_button, &QPushButton::clicked, this, &Quiz::toggle_mute);

    timer_label = new QLabel("Time: 30s");
    timer_label->setObjectName("timer_label");

    top_layout->addWidget(mute_button);
    top_layout->addWidget(progress_label);
    top_layout->addStretch();
    top_layout->addWidget(timer_label);

    main_layout->addLayout(top_layout);
    smiley_label = new QLabel("🎉");
    smiley_label->setObjectName("smiley_label");
    smiley_label->setAlignment(Qt::AlignCenter);
    smiley_label->setVisible(false);
    main_layout->addWidget(smiley_label);

    question_label = new QLabel("Question will appear here");
    question_label->setObjectName("question_label");
    question_label->setWordWrap(true);
    main_layout->addWidget(question_label);

    // Create animation
    smiley_animation = new QPropertyAnimation(smiley_label, "geometry");
    smiley_animation->setDuration(1500);
    connect(smiley_animation, &QPropertyAnimation::finished, [this]()
            { smiley_label->setVisible(false); });

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
        sound_manager->playWrongSound();

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
        button->setProperty("answerState", "");
        button->style()->unpolish(button);
        button->style()->polish(button);
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

void Quiz::toggle_mute()
{
    bool muted = !sound_manager->isMuted();
    sound_manager->setMuted(muted);
    update_mute_button();
}

void Quiz::update_mute_button()
{
    if (sound_manager->isMuted())
    {
        mute_button->setText("🔇");
    }
    else
    {
        mute_button->setText("🔊");
    }
}

void Quiz::show_celebration(const QString &message)
{
    smiley_label->setVisible(true);
    smiley_label->setText(message);

    QRect start = smiley_label->geometry();
    QRect middle = start.adjusted(0, -15, 0, -15); // Smaller bounce (15px instead of 20px)

    smiley_animation->setKeyValueAt(0, start);
    smiley_animation->setKeyValueAt(0.5, middle);
    smiley_animation->setKeyValueAt(1, start);
    smiley_animation->start();
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
        sound_manager->playCorrectSound();

        // Add celebration for fast answers
        int time_taken = question_manager->time_per_question() - time_remaining;
        if (time_taken <= 5)
        {
            show_celebration("Excellent! ⚡");
        }
        else if (time_taken <= 10)
        {
            show_celebration("Great job! 🎉");
        }

        option_buttons[selected_answer]->setProperty("answerState", "correct");
        option_buttons[selected_answer]->style()->unpolish(option_buttons[selected_answer]);
        option_buttons[selected_answer]->style()->polish(option_buttons[selected_answer]);
    }
    else
    {
        sound_manager->playWrongSound();

        option_buttons[selected_answer]->setProperty("answerState", "wrong");
        option_buttons[selected_answer]->style()->unpolish(option_buttons[selected_answer]);
        option_buttons[selected_answer]->style()->polish(option_buttons[selected_answer]);

        option_buttons[current.correct_answer]->setProperty("answerState", "correct");
        option_buttons[current.correct_answer]->style()->unpolish(option_buttons[current.correct_answer]);
        option_buttons[current.correct_answer]->style()->polish(option_buttons[current.correct_answer]);

        show_explanation();
    }

    for (auto *button : option_buttons)
    {
        button->setEnabled(false);
    }

    learn_more_button->setEnabled(false);
    enable_navigation();
}

void Quiz::onModeChanged()
{
    countdown_timer->stop();
    score = 0;
    reset_quiz();

    question_label->setText("Select a level to start the quiz");
    progress_label->setText("Ready to start");
    timer_label->setText("🕐 --");
}