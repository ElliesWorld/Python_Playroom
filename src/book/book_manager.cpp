#include "book_manager.h"
#include <QDebug>

BookManager::BookManager(SettingsManager *settingsManager, QObject *parent)
    : QObject(parent), settings_manager(settingsManager)
{
    connect(settings_manager, &SettingsManager::modeChanged,
            this, &BookManager::onModeChanged);

    loadBook();
}

QString BookManager::getBookPath() const
{
    QuizMode currentMode = settings_manager->currentMode();

    switch (currentMode)
    {
    case QuizMode::Python:
        return ":/learning_books/python.json";
    case QuizMode::Cpp:
        return ":/learning_books/cpp.json";
    case QuizMode::C:
        return ":/learning_books/c.json";
    default:
        return ":/learning_books/python.json"; // fallback
    }
}

bool BookManager::loadBook()
{
    QString bookPath = getBookPath();
    QFile bookFile(bookPath);

    qDebug() << "Attempting to load book from:" << bookPath;

    if (!bookFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open book file:" << bookPath;
        qDebug() << "Error:" << bookFile.errorString();
        return false;
    }

    QByteArray fileData = bookFile.readAll();
    bookFile.close();

    qDebug() << "Book file size:" << fileData.size() << "bytes";

    QJsonDocument doc = QJsonDocument::fromJson(fileData);

    if (doc.isNull())
    {
        qDebug() << "Invalid JSON in book file:" << bookPath;
        return false;
    }

    parseBookData(doc);
    qDebug() << "Successfully loaded" << chapters.size() << "chapters";
    return true;
}

void BookManager::parseBookData(const QJsonDocument &doc)
{
    chapters.clear();

    QJsonObject root = doc.object();
    QJsonArray chaptersArray = root["chapters"].toArray();

    qDebug() << "Parsing" << chaptersArray.size() << "chapters from JSON";

    for (const auto &chapterValue : chaptersArray)
    {
        QJsonObject chapterObj = chapterValue.toObject();

        BookChapter chapter;
        chapter.title = chapterObj["title"].toString();
        chapter.description = chapterObj["description"].toString();

        QJsonArray pagesArray = chapterObj["pages"].toArray();
        int pageNum = 1;

        for (const auto &pageValue : pagesArray)
        {
            QJsonObject pageObj = pageValue.toObject();

            BookPage page;
            page.title = pageObj["title"].toString();
            page.content = pageObj["content"].toString();
            page.category = pageObj["category"].toString();
            page.pageNumber = pageNum++;

            chapter.pages.append(page);
        }

        chapters.append(chapter);
        qDebug() << "Loaded chapter:" << chapter.title << "with" << chapter.pages.size() << "pages";
    }
}

QVector<BookChapter> BookManager::getChapters() const
{
    return chapters;
}

BookPage BookManager::getPage(int chapterIndex, int pageIndex) const
{
    if (chapterIndex >= 0 && chapterIndex < chapters.size())
    {
        const BookChapter &chapter = chapters[chapterIndex];
        if (pageIndex >= 0 && pageIndex < chapter.pages.size())
        {
            return chapter.pages[pageIndex];
        }
    }
    return BookPage();
}

int BookManager::getTotalPages() const
{
    int total = 0;
    for (const auto &chapter : chapters)
    {
        total += chapter.pages.size();
    }
    return total;
}

QString BookManager::getCurrentLanguageName() const
{
    return settings_manager->modeToDisplayName(settings_manager->currentMode());
}

void BookManager::onModeChanged(QuizMode newMode)
{
    qDebug() << "BookManager: Mode changed, reloading book";
    loadBook();
}