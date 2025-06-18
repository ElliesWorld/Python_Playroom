#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QFile>
#include "questions.h"
#include "quiz.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void show_welcome();
    void start_quiz(int level_id);
    void show_results(int score);
    void on_level_button_clicked();

private:
    QStackedWidget *central_stack;
    QWidget *welcome_screen;
    Quiz *quiz_screen;
    QWidget *results_screen;
    Questions *question_manager;

    QLabel *title_label;
    QLabel *instructions_label;
    QWidget *levels_widget;
    QVBoxLayout *levels_layout;

    QLabel *score_label;
    QLabel *performance_label;
    QPushButton *retry_button;
    QPushButton *menu_button;

    int current_level_id;
    int current_score;

    void setup_ui();
    void setup_welcome_screen();
    void setup_results_screen();
    void load_stylesheet();
    void update_level_buttons();
};

#endif