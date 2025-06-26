#include "main_window.h"
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), current_level_id(0), current_score(0)
{
    question_manager = new Questions(this);
    setup_ui();
    load_stylesheet();
    show_welcome();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setup_ui()
{
    central_stack = new QStackedWidget(this);
    setCentralWidget(central_stack);

    setup_welcome_screen();
    setup_results_screen();

    quiz_screen = new Quiz(question_manager, this);
    connect(quiz_screen, &Quiz::quiz_completed, this, &MainWindow::show_results);
    connect(quiz_screen, &Quiz::return_to_welcome, this, &MainWindow::show_welcome);

    central_stack->addWidget(welcome_screen);
    central_stack->addWidget(quiz_screen);
    central_stack->addWidget(results_screen);

    setWindowTitle("Python Learning Quiz");
    resize(800, 600);
}

void MainWindow::setup_welcome_screen()
{
    welcome_screen = new QWidget();
    welcome_screen->setObjectName("welcome_screen");

    QVBoxLayout *main_layout = new QVBoxLayout(welcome_screen);
    main_layout->setSpacing(20);
    main_layout->setContentsMargins(40, 40, 40, 40);

    title_label = new QLabel("🐍 Python Learning Quiz");
    title_label->setObjectName("title_label");
    title_label->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(title_label);

    QString instructions =
        "Hi there! Are you ready to become a software developer?\n\n"
        "Click to begin your journey! You will have 20-60 seconds to answer each question "
        "and each level contains multiple questions to test your knowledge.\n\n"
        "For each question there will be information in the 'Learn More' button, "
        "but if you click that you will lose the point for that question!\n\n"
        "Each question earns you 1 point!\n\n"
        "Select a level to start:";

    instructions_label = new QLabel(instructions);
    instructions_label->setObjectName("instructions_label");
    instructions_label->setAlignment(Qt::AlignCenter);
    instructions_label->setWordWrap(true);
    main_layout->addWidget(instructions_label);

    levels_widget = new QWidget();
    levels_layout = new QVBoxLayout(levels_widget);
    levels_layout->setSpacing(10);

    QScrollArea *scroll_area = new QScrollArea();
    scroll_area->setWidget(levels_widget);
    scroll_area->setWidgetResizable(true);
    scroll_area->setMaximumHeight(300);
    main_layout->addWidget(scroll_area);

    main_layout->addStretch();

    update_level_buttons();
}

void MainWindow::setup_results_screen()
{
    results_screen = new QWidget();
    results_screen->setObjectName("results_screen");

    QVBoxLayout *main_layout = new QVBoxLayout(results_screen);
    main_layout->setSpacing(30);
    main_layout->setContentsMargins(40, 40, 40, 40);

    QLabel *results_title = new QLabel("🎉 Quiz Complete!");
    results_title->setObjectName("title_label");
    results_title->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(results_title);

    score_label = new QLabel("Your Score: 0/3");
    score_label->setObjectName("score_label");
    score_label->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(score_label);

    performance_label = new QLabel("");
    performance_label->setObjectName("instructions_label");
    performance_label->setAlignment(Qt::AlignCenter);
    performance_label->setWordWrap(true);
    main_layout->addWidget(performance_label);

    QHBoxLayout *button_layout = new QHBoxLayout();

    retry_button = new QPushButton("Retry Level");
    retry_button->setObjectName("level_button");
    connect(retry_button, &QPushButton::clicked, [this]()
            { start_quiz(current_level_id); });

    menu_button = new QPushButton("Back to Menu");
    menu_button->setObjectName("action_button");
    connect(menu_button, &QPushButton::clicked, this, &MainWindow::show_welcome);

    button_layout->addStretch();
    button_layout->addWidget(retry_button);
    button_layout->addWidget(menu_button);
    button_layout->addStretch();

    main_layout->addLayout(button_layout);
    main_layout->addStretch();
}

void MainWindow::update_level_buttons()
{
    QLayoutItem *child;
    while ((child = levels_layout->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    QVector<LevelInfo> levels = question_manager->available_levels();

    for (const auto &level : levels)
    {
        QPushButton *level_button = new QPushButton();
        level_button->setObjectName("level_button");
        level_button->setProperty("level_id", level.id);

        QString button_text = QString("Level %1: %2").arg(level.id).arg(level.name);

        if (question_manager->level_unlocked(level.id))
        {
            level_button->setText(button_text);
            level_button->setEnabled(true);
            connect(level_button, &QPushButton::clicked, this, &MainWindow::on_level_button_clicked);
        }
        else
        {
            level_button->setText(button_text + " (Locked)");
            level_button->setEnabled(false);
        }

        levels_layout->addWidget(level_button);
    }
}

void MainWindow::load_stylesheet()
{
    QFile style_file(":/styles.qss");
    if (style_file.open(QIODevice::ReadOnly))
    {
        QString style_sheet = QString::fromUtf8(style_file.readAll());
        qApp->setStyleSheet(style_sheet);
        style_file.close();
    }
    else
    {
        qDebug() << "Could not load stylesheet";
    }
}

void MainWindow::show_welcome()
{
    update_level_buttons();
    central_stack->setCurrentWidget(welcome_screen);
}

void MainWindow::start_quiz(int level_id)
{
    current_level_id = level_id;
    quiz_screen->start_quiz(level_id);
    central_stack->setCurrentWidget(quiz_screen);
}

void MainWindow::show_results(int score)
{
    current_score = score;
    int total_questions = question_manager->total_questions();

    score_label->setText(QString("Your Score: %1/%2").arg(score).arg(total_questions));

    double percentage = (double)score / total_questions * 100;
    QString performance_text;

    if (percentage >= 100)
    {
        performance_text = "🏆 Perfect! You're a Python master!";
        question_manager->unlock_level(current_level_id + 1);
    }
    else if (percentage >= 66)
    {
        performance_text = "🎯 Great job! You're getting the hang of Python!";
        question_manager->unlock_level(current_level_id + 1);
    }
    else if (percentage >= 33)
    {
        performance_text = "📚 Good effort! Keep practicing to improve!";
    }
    else
    {
        performance_text = "💪 Keep trying! Python takes practice to master!";
    }

    performance_label->setText(performance_text);

    central_stack->setCurrentWidget(results_screen);
}

void MainWindow::on_level_button_clicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button)
    {
        int level_id = button->property("level_id").toInt();
        start_quiz(level_id);
    }
}