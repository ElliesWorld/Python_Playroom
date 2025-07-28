#ifndef BOOK_VIEWER_H
#define BOOK_VIEWER_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QScrollArea>
#include <QSplitter>
#include <QPropertyAnimation>
#include "book_manager.h"

class BookViewer : public QWidget
{
    Q_OBJECT

public:
    explicit BookViewer(BookManager *bookManager, QWidget *parent = nullptr);

    // PUBLIC METHODS - These need to be accessible from MainWindow
    void showIndex();
    void showPage(int chapterIndex, int pageIndex);

signals:
    void closeBook();

private slots:
    void onChapterClicked(int chapterIndex);
    void onPageClicked(int pageIndex);
    void nextPage();
    void previousPage();
    void onCloseBook();

private:
    void setupUI();
    void setupIndexPage();
    void setupBookPage();
    void updateContent();

    // Core components
    BookManager *book_manager;
    QStackedWidget *main_stack;

    // Index page components
    QWidget *index_widget;
    QLabel *book_title;
    QListWidget *chapter_list;
    QListWidget *page_list;
    QPushButton *close_button;

    // Book page components
    QWidget *book_widget;
    QLabel *page_title;
    QLabel *page_info;
    QTextEdit *page_content;
    QPushButton *prev_button;
    QPushButton *next_button;
    QPushButton *index_button;

    // Animation
    QPropertyAnimation *page_animation;

    // State
    int current_chapter;
    int current_page;
};

#endif // BOOK_VIEWER_H