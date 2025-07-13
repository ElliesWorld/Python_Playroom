#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QButtonGroup>
#include <QFile>
#include <QApplication>
#include <QDebug>

#include "questions.h"
#include "quiz.h"
#include "settings_manager.h"
#include "settings_toolbar.h"

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
    void on_quiz_mode_changed(QuizMode newMode);

private:
    void setup_ui();
    void setup_welcome_screen();
    void setup_results_screen();
    void update_level_buttons();
    void load_stylesheet();
    void update_window_title();
    void update_welcome_screen_content();

    SettingsManager *settings_manager;
    SettingsToolbar *settings_toolbar;
    Questions *question_manager;
    Quiz *quiz_screen;

    QStackedWidget *central_stack;

    QWidget *welcome_screen;
    QLabel *title_label;
    QLabel *instructions_label;
    QWidget *levels_widget;
    QVBoxLayout *levels_layout;

    QWidget *results_screen;
    QLabel *score_label;
    QLabel *performance_label;
    QPushButton *retry_button;
    QPushButton *menu_button;

    int current_level_id;
    int current_score;
};

#endif 