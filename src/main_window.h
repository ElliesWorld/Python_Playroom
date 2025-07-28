#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include "quiz.h"
#include "questions.h"
#include "settings_manager.h"
#include "settings_toolbar.h"
#include "book_manager.h"
#include "book_viewer.h"
#include "question_mark_button.h"

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
    void on_unlock_all_triggered();
    void on_book_button_clicked();
    void on_close_book();

private:
    void setup_ui();
    void setup_welcome_screen();
    void setup_results_screen();
    void update_level_buttons();
    void load_stylesheet();
    void update_window_title();
    void update_welcome_screen_content();

    // Core components
    SettingsManager *settings_manager;
    Questions *question_manager;
    BookManager *book_manager;

    // UI components
    QStackedWidget *central_stack;
    SettingsToolbar *settings_toolbar;

    // Welcome screen
    QWidget *welcome_screen;
    QLabel *title_label;
    QLabel *instructions_label;
    QWidget *levels_widget;
    QVBoxLayout *levels_layout;
    QuestionMarkButton *book_button;

    // Quiz screen
    Quiz *quiz_screen;

    // Results screen
    QWidget *results_screen;
    QLabel *score_label;
    QLabel *performance_label;
    QPushButton *retry_button;
    QPushButton *menu_button;

    // Book viewer
    BookViewer *book_viewer;

    // Menu actions
    QAction *unlock_all_action;

    // State
    int current_level_id;
    int current_score;
};

#endif // MAIN_WINDOW_H