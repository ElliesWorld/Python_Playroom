#ifndef BOOK_MANAGER_H
#define BOOK_MANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QVector>
#include "settings_manager.h"

struct BookPage
{
    QString title;
    QString content;
    QString category;
    int pageNumber = 0;
};

struct BookChapter
{
    QString title;
    QString description;
    QVector<BookPage> pages;
};

class BookManager : public QObject
{
    Q_OBJECT

public:
    explicit BookManager(SettingsManager *settingsManager, QObject *parent = nullptr);

    QVector<BookChapter> getChapters() const;
    BookPage getPage(int chapterIndex, int pageIndex) const;
    int getTotalPages() const;
    QString getCurrentLanguageName() const;

private slots:
    void onModeChanged(QuizMode newMode);

private:
    SettingsManager *settings_manager;
    QVector<BookChapter> chapters;

    QString getBookPath() const;
    bool loadBook();
    void parseBookData(const QJsonDocument &doc);
};

#endif // BOOK_MANAGER_H