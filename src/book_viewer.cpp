#include "book_viewer.h"
#include <QDebug>

BookViewer::BookViewer(BookManager *bookManager, QWidget *parent)
    : QWidget(parent), book_manager(bookManager), current_chapter(0), current_page(0)
{
    setObjectName("book_viewer");

    if (!book_manager)
    {
        qDebug() << "Error: BookManager is null!";
        return;
    }

    qDebug() << "Setting up BookViewer UI...";
    setupUI();
    qDebug() << "BookViewer created successfully";
}

void BookViewer::setupUI()
{
    // Create main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(20, 20, 20, 20);
    main_layout->setSpacing(20);

    // Create stacked widget
    main_stack = new QStackedWidget(this);

    // Setup pages
    setupIndexPage();
    setupBookPage();

    // Add to stack
    main_stack->addWidget(index_widget);
    main_stack->addWidget(book_widget);

    // Add to main layout
    main_layout->addWidget(main_stack);

    // Show index by default
    showIndex();
}

void BookViewer::setupIndexPage()
{
    qDebug() << "Setting up index page...";

    index_widget = new QWidget();
    index_widget->setObjectName("book_index");

    QVBoxLayout *layout = new QVBoxLayout(index_widget);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);

    // Title
    book_title = new QLabel();
    book_title->setObjectName("book_title");
    book_title->setAlignment(Qt::AlignCenter);
    book_title->setText("📚 Python Programming Learning Book");
    layout->addWidget(book_title);

    // Subtitle
    QLabel *subtitle = new QLabel("Select a chapter to begin learning:");
    subtitle->setObjectName("book_subtitle");
    subtitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitle);

    // Create horizontal layout for lists
    QHBoxLayout *lists_layout = new QHBoxLayout();

    // Chapter list
    chapter_list = new QListWidget();
    chapter_list->setObjectName("chapter_list");
    chapter_list->setFixedWidth(300);
    chapter_list->setMinimumHeight(200);

    // Page list
    page_list = new QListWidget();
    page_list->setObjectName("page_list");
    page_list->setMinimumHeight(200);

    lists_layout->addWidget(chapter_list);
    lists_layout->addWidget(page_list);
    layout->addLayout(lists_layout);

    // Close button
    close_button = new QPushButton("📖 Close Book");
    close_button->setObjectName("book_close_button");
    close_button->setFixedWidth(150);

    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->addStretch();
    button_layout->addWidget(close_button);
    button_layout->addStretch();
    layout->addLayout(button_layout);

    // Connect signals
    connect(chapter_list, &QListWidget::currentRowChanged,
            this, &BookViewer::onChapterClicked);
    connect(page_list, &QListWidget::currentRowChanged,
            this, &BookViewer::onPageClicked);
    connect(close_button, &QPushButton::clicked, this, &BookViewer::onCloseBook);

    qDebug() << "Index page setup complete";
}

void BookViewer::setupBookPage()
{
    qDebug() << "Setting up book page...";

    book_widget = new QWidget();
    book_widget->setObjectName("book_page");

    QVBoxLayout *layout = new QVBoxLayout(book_widget);
    layout->setSpacing(15);
    layout->setContentsMargins(30, 30, 30, 30);

    // Page header
    QHBoxLayout *header_layout = new QHBoxLayout();

    page_title = new QLabel("Page Title");
    page_title->setObjectName("page_title");

    page_info = new QLabel("Page Info");
    page_info->setObjectName("page_info");

    header_layout->addWidget(page_title);
    header_layout->addStretch();
    header_layout->addWidget(page_info);
    layout->addLayout(header_layout);

    // Page content
    page_content = new QTextEdit();
    page_content->setObjectName("page_content");
    page_content->setReadOnly(true);
    page_content->setMinimumHeight(300);
    layout->addWidget(page_content);

    // Navigation buttons
    QHBoxLayout *nav_layout = new QHBoxLayout();

    prev_button = new QPushButton("◀ Previous");
    prev_button->setObjectName("book_nav_button");

    index_button = new QPushButton("📑 Index");
    index_button->setObjectName("book_nav_button");

    next_button = new QPushButton("Next ▶");
    next_button->setObjectName("book_nav_button");

    QPushButton *close_btn = new QPushButton("📖 Close Book");
    close_btn->setObjectName("book_close_button");

    nav_layout->addWidget(prev_button);
    nav_layout->addWidget(index_button);
    nav_layout->addStretch();
    nav_layout->addWidget(close_btn);
    nav_layout->addStretch();
    nav_layout->addWidget(next_button);

    layout->addLayout(nav_layout);

    // Connect navigation
    connect(prev_button, &QPushButton::clicked, this, &BookViewer::previousPage);
    connect(next_button, &QPushButton::clicked, this, &BookViewer::nextPage);
    connect(index_button, &QPushButton::clicked, this, &BookViewer::showIndex);
    connect(close_btn, &QPushButton::clicked, this, &BookViewer::onCloseBook);

    qDebug() << "Book page setup complete";
}

void BookViewer::showIndex()
{
    qDebug() << "Showing index...";

    if (!main_stack || !index_widget)
    {
        qDebug() << "Error: UI not properly initialized";
        return;
    }

    main_stack->setCurrentWidget(index_widget);
    updateContent();
}

void BookViewer::updateContent()
{
    qDebug() << "Updating content...";

    if (!book_manager || !chapter_list || !page_list)
    {
        qDebug() << "Error: Components not initialized";
        return;
    }

    // Clear lists
    chapter_list->clear();
    page_list->clear();

    // Get chapters
    QVector<BookChapter> chapters = book_manager->getChapters();
    qDebug() << "Found" << chapters.size() << "chapters";

    if (chapters.isEmpty())
    {
        chapter_list->addItem("⚠️ No content available");
        page_list->addItem("⚠️ No pages available");
        return;
    }

    // Add chapters to list
    for (int i = 0; i < chapters.size(); ++i)
    {
        const BookChapter &chapter = chapters[i];
        QString itemText = QString("📖 %1").arg(chapter.title);
        chapter_list->addItem(itemText);
        qDebug() << "Added chapter:" << chapter.title;
    }

    // Select first chapter
    if (chapters.size() > 0)
    {
        chapter_list->setCurrentRow(0);
        // Manually trigger the first chapter
        onChapterClicked(0);
    }
}

void BookViewer::onChapterClicked(int chapterIndex)
{
    qDebug() << "Chapter clicked:" << chapterIndex;

    if (!page_list)
    {
        qDebug() << "Error: page_list is null";
        return;
    }

    page_list->clear();

    QVector<BookChapter> chapters = book_manager->getChapters();
    if (chapterIndex < 0 || chapterIndex >= chapters.size())
    {
        qDebug() << "Invalid chapter index:" << chapterIndex;
        return;
    }

    const BookChapter &chapter = chapters[chapterIndex];
    qDebug() << "Chapter" << chapter.title << "has" << chapter.pages.size() << "pages";

    if (chapter.pages.isEmpty())
    {
        page_list->addItem("⚠️ No pages in this chapter");
        return;
    }

    // Add pages to list
    for (int i = 0; i < chapter.pages.size(); ++i)
    {
        const BookPage &page = chapter.pages[i];
        QString itemText = QString("📄 %1").arg(page.title);
        page_list->addItem(itemText);
        qDebug() << "Added page:" << page.title;
    }

    // Select first page
    if (chapter.pages.size() > 0)
    {
        page_list->setCurrentRow(0);
    }
}

void BookViewer::onPageClicked(int pageIndex)
{
    int chapterIndex = chapter_list->currentRow();
    qDebug() << "Page clicked:" << pageIndex << "in chapter" << chapterIndex;

    if (chapterIndex >= 0)
    {
        showPage(chapterIndex, pageIndex);
    }
}

void BookViewer::showPage(int chapterIndex, int pageIndex)
{
    qDebug() << "Showing page" << pageIndex << "of chapter" << chapterIndex;

    if (!main_stack || !book_widget || !page_title || !page_content || !page_info)
    {
        qDebug() << "Error: Book page components not initialized";
        return;
    }

    current_chapter = chapterIndex;
    current_page = pageIndex;

    BookPage page = book_manager->getPage(chapterIndex, pageIndex);
    if (page.title.isEmpty())
    {
        qDebug() << "Page is empty!";
        page_title->setText("⚠️ Page Not Found");
        page_content->setHtml("<h2>Content Not Available</h2><p>This page could not be loaded.</p>");
        page_info->setText("Error");
    }
    else
    {
        page_title->setText(page.title);
        page_content->setHtml(page.content);

        QVector<BookChapter> chapters = book_manager->getChapters();
        if (chapterIndex < chapters.size())
        {
            page_info->setText(QString("Chapter %1 - Page %2 of %3")
                                   .arg(chapterIndex + 1)
                                   .arg(pageIndex + 1)
                                   .arg(chapters[chapterIndex].pages.size()));
        }

        // Update navigation buttons
        if (prev_button && next_button)
        {
            prev_button->setEnabled(!(chapterIndex == 0 && pageIndex == 0));

            bool isLastPage = (chapterIndex == chapters.size() - 1 &&
                               pageIndex == chapters[chapterIndex].pages.size() - 1);
            next_button->setEnabled(!isLastPage);
        }
    }

    main_stack->setCurrentWidget(book_widget);
}

void BookViewer::nextPage()
{
    QVector<BookChapter> chapters = book_manager->getChapters();
    if (chapters.isEmpty())
        return;

    if (current_page < chapters[current_chapter].pages.size() - 1)
    {
        showPage(current_chapter, current_page + 1);
    }
    else if (current_chapter < chapters.size() - 1)
    {
        showPage(current_chapter + 1, 0);
    }
}

void BookViewer::previousPage()
{
    if (current_page > 0)
    {
        showPage(current_chapter, current_page - 1);
    }
    else if (current_chapter > 0)
    {
        QVector<BookChapter> chapters = book_manager->getChapters();
        showPage(current_chapter - 1, chapters[current_chapter - 1].pages.size() - 1);
    }
}

void BookViewer::onCloseBook()
{
    qDebug() << "Closing book";
    emit closeBook();
}