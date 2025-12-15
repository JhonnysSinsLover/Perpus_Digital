#include "BooksCollectionPage.h"
#include "AddBookDialog.h"
#include "BookCardWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QStyle>
#include <QGraphicsDropShadowEffect>
#include <set>
#include <algorithm>

BooksCollectionPage::BooksCollectionPage(QWidget *parent)
    : QWidget(parent)
    , m_isCardView(false)
    , m_sortAscending(true)
    , m_lastSortColumn("title")
{
    setupUI();
    populateGenreComboBox();
    refreshTable();
}

BooksCollectionPage::~BooksCollectionPage()
{
}

void BooksCollectionPage::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    createHeaderSection(mainLayout);
    
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #F4F7FE; border: none; }"
        "QScrollBar:vertical { background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
    );
    
    QWidget* contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");
    contentWidget->setStyleSheet("#contentWidget { background-color: #F4F7FE; }");
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(25);
    
    createControlSection(contentLayout);
    createContentSection(contentLayout);
    createActionSection(contentLayout);
    
    contentLayout->addStretch();
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void BooksCollectionPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(80);
    headerFrame->setStyleSheet("QFrame { background-color: white; border-bottom: 1px solid #E0E5F2; }");
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(15);
    
    QLabel* iconLabel = new QLabel(headerFrame);
    iconLabel->setPixmap(QPixmap(":/svg/book-collection.svg").scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    headerLayout->addWidget(iconLabel);
    
    QLabel* titleLabel = new QLabel("Koleksi Buku", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch();
    
    QLineEdit* globalSearch = new QLineEdit(headerFrame);
    globalSearch->setPlaceholderText("Search...");
    globalSearch->setFixedSize(250, 40);
    headerLayout->addWidget(globalSearch);
    
    QPushButton* profileBtn = new QPushButton(headerFrame);
    profileBtn->setIcon(QIcon(":/svg/user.svg"));
    profileBtn->setIconSize(QSize(24, 24));
    profileBtn->setFixedSize(45, 45);
    profileBtn->setStyleSheet("QPushButton { background-color: #112D4E; border: none; border-radius: 22px; }");
    headerLayout->addWidget(profileBtn);
    
    mainLayout->addWidget(headerFrame);
}

QFrame* BooksCollectionPage::createCardFrame()
{
    QFrame* card = new QFrame(this);
    card->setObjectName("card");
    
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 20));
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);
    
    return card;
}

void BooksCollectionPage::createControlSection(QVBoxLayout* contentLayout)
{
    QFrame* controlCard = createCardFrame();
    
    QVBoxLayout* cardLayout = new QVBoxLayout(controlCard);
    cardLayout->setContentsMargins(25, 25, 25, 25);
    cardLayout->setSpacing(20);
    
    QLabel* lblHeader = new QLabel("Pencarian & Filter", controlCard);
    lblHeader->setStyleSheet("color: #2B3674; font-size: 18px; font-weight: 700;");
    cardLayout->addWidget(lblHeader);

    // Search Fields Row
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(15);

    // Search Title
    QVBoxLayout* box1 = new QVBoxLayout(); 
    box1->setSpacing(8);
    QLabel* lbl1 = new QLabel("Cari Judul:");
    lbl1->setStyleSheet("color: #64748B; font-weight: 600; font-size: 13px;");
    m_searchBox = new QLineEdit(controlCard);
    m_searchBox->setPlaceholderText("Ketik judul buku...");
    box1->addWidget(lbl1);
    box1->addWidget(m_searchBox);
    searchLayout->addLayout(box1, 2);

    // Search Author
    QVBoxLayout* box2 = new QVBoxLayout(); 
    box2->setSpacing(8);
    QLabel* lbl2 = new QLabel("Cari Penulis:");
    lbl2->setStyleSheet("color: #64748B; font-weight: 600; font-size: 13px;");
    m_authorSearch = new QLineEdit(controlCard);
    m_authorSearch->setPlaceholderText("Ketik nama penulis...");
    box2->addWidget(lbl2);
    box2->addWidget(m_authorSearch);
    searchLayout->addLayout(box2, 2);

    // Filter Genre
    QVBoxLayout* box3 = new QVBoxLayout(); 
    box3->setSpacing(8);
    QLabel* lbl3 = new QLabel("Filter Genre:");
    lbl3->setStyleSheet("color: #64748B; font-weight: 600; font-size: 13px;");
    m_genreCombo = new QComboBox(controlCard);
    box3->addWidget(lbl3);
    box3->addWidget(m_genreCombo);
    searchLayout->addLayout(box3, 1);

    // Action Buttons
    QVBoxLayout* boxBtn = new QVBoxLayout(); 
    boxBtn->setSpacing(8);
    boxBtn->addWidget(new QLabel(" "));
    
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->setSpacing(10);
    
    QPushButton* btnFilter = new QPushButton("Cari");
    btnFilter->setIcon(QIcon(":/svg/search.svg"));
    btnFilter->setIconSize(QSize(16, 16));
    btnFilter->setCursor(Qt::PointingHandCursor);
    btnFilter->setFixedSize(80, 38);
    btnFilter->setStyleSheet(
        "QPushButton { background-color: #4318FF; color: white; border-radius: 8px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #3311DB; }"
    );
    
    QPushButton* btnReset = new QPushButton("Reset");
    btnReset->setIcon(QIcon(":/svg/refresh.svg"));
    btnReset->setIconSize(QSize(16, 16));
    btnReset->setCursor(Qt::PointingHandCursor);
    btnReset->setFixedSize(80, 38);
    btnReset->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #94A3B8; color: #64748B; border-radius: 8px; font-weight: bold; }"
        "QPushButton:hover { border: 1px solid #EE5D50; color: #EE5D50; }"
    );

    btnRow->addWidget(btnFilter);
    btnRow->addWidget(btnReset);
    boxBtn->addLayout(btnRow);
    
    searchLayout->addLayout(boxBtn);
    cardLayout->addLayout(searchLayout);
    
    // Separator
    QFrame* line = new QFrame();
    line->setFixedHeight(1);
    line->setStyleSheet("background-color: #E0E5F2; border: none;");
    cardLayout->addWidget(line);
    
    // Sorting Row
    QHBoxLayout* sortLayout = new QHBoxLayout();
    sortLayout->setSpacing(10);
    
    QLabel* lblSort = new QLabel("Urutkan:");
    lblSort->setStyleSheet("color: #64748B; font-weight: 600; font-size: 13px;");
    sortLayout->addWidget(lblSort);
    
    auto createSortBtn = [](QString text, QString iconPath) -> QPushButton* {
        QPushButton* btn = new QPushButton(text);
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(16, 16));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(32);
        btn->setStyleSheet(
            "QPushButton { background-color: #F1F5F9; color: #2B3674; border: none; border-radius: 6px; padding: 0 15px; font-weight: 600; }"
            "QPushButton:hover { background-color: #EAEFFC; color: #4318FF; }"
        );
        return btn;
    };
    
    QPushButton* btnSortTitle = createSortBtn("Judul", ":/svg/sort-alpha.svg");
    QPushButton* btnSortYear = createSortBtn("Tahun", ":/svg/calendar.svg");
    QPushButton* btnSortRating = createSortBtn("Rating", ":/svg/star.svg");
    QPushButton* btnSortAuthor = createSortBtn("Penulis", ":/svg/author.svg");
    
    sortLayout->addWidget(btnSortTitle);
    sortLayout->addWidget(btnSortYear);
    sortLayout->addWidget(btnSortRating);
    sortLayout->addWidget(btnSortAuthor);
    sortLayout->addStretch();
    
    m_btnToggleView = new QPushButton("Switch View");
    m_btnToggleView->setIcon(QIcon(":/svg/view-grid.svg"));
    m_btnToggleView->setIconSize(QSize(18, 18));
    m_btnToggleView->setCursor(Qt::PointingHandCursor);
    m_btnToggleView->setFixedHeight(35);
    m_btnToggleView->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #4318FF; color: #4318FF; border-radius: 8px; padding: 0 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #4318FF; color: white; }"
    );
    sortLayout->addWidget(m_btnToggleView);
    
    cardLayout->addLayout(sortLayout);
    
    // BST Section
    QFrame* lineBST = new QFrame();
    lineBST->setFixedHeight(1);
    lineBST->setStyleSheet("background-color: #E0E5F2; border: none;");
    cardLayout->addWidget(lineBST);
    
    QHBoxLayout* bstLayout = new QHBoxLayout();
    bstLayout->setSpacing(10);
    
    QLabel* lblBST = new QLabel("Binary Search Tree:");
    lblBST->setPixmap(QPixmap(":/svg/tree.svg").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    lblBST->setStyleSheet("font-weight: bold; color: #2B3674;");
    bstLayout->addWidget(lblBST);
    
    m_btnBuildBST = new QPushButton("Build BST");
    m_btnBuildBST->setIcon(QIcon(":/svg/build.svg"));
    m_btnBuildBST->setIconSize(QSize(16, 16));
    m_btnBuildBST->setCursor(Qt::PointingHandCursor);
    m_btnBuildBST->setFixedHeight(32);
    m_btnBuildBST->setStyleSheet(
        "QPushButton { background-color: #10B981; color: white; border: none; border-radius: 6px; padding: 0 15px; font-weight: 600; }"
        "QPushButton:hover { background-color: #059669; }"
    );
    bstLayout->addWidget(m_btnBuildBST);
    
    m_bstSearchBox = new QLineEdit();
    m_bstSearchBox->setPlaceholderText("Cari judul buku dengan BST...");
    m_bstSearchBox->setFixedHeight(32);
    bstLayout->addWidget(m_bstSearchBox, 1);
    
    m_btnSearchBST = new QPushButton("Search BST");
    m_btnSearchBST->setIcon(QIcon(":/svg/search.svg"));
    m_btnSearchBST->setIconSize(QSize(16, 16));
    m_btnSearchBST->setCursor(Qt::PointingHandCursor);
    m_btnSearchBST->setFixedHeight(32);
    m_btnSearchBST->setStyleSheet(
        "QPushButton { background-color: #6366F1; color: white; border: none; border-radius: 6px; padding: 0 15px; font-weight: 600; }"
        "QPushButton:hover { background-color: #4F46E5; }"
    );
    bstLayout->addWidget(m_btnSearchBST);
    
    cardLayout->addLayout(bstLayout);
    contentLayout->addWidget(controlCard);
    
    // Connect Signals
    connect(m_searchBox, &QLineEdit::textChanged, this, &BooksCollectionPage::onSearchTextChanged);
    connect(btnReset, &QPushButton::clicked, this, &BooksCollectionPage::onClearSearch);
    connect(btnFilter, &QPushButton::clicked, this, &BooksCollectionPage::onSearchByGenre); 
    connect(m_authorSearch, &QLineEdit::returnPressed, this, &BooksCollectionPage::onSearchByAuthor);
    connect(btnSortTitle, &QPushButton::clicked, this, &BooksCollectionPage::onSortByTitle);
    connect(btnSortYear, &QPushButton::clicked, this, &BooksCollectionPage::onSortByYear);
    connect(btnSortRating, &QPushButton::clicked, this, &BooksCollectionPage::onSortByRating);
    connect(btnSortAuthor, &QPushButton::clicked, this, &BooksCollectionPage::onSortByAuthor);
    connect(m_btnToggleView, &QPushButton::clicked, this, &BooksCollectionPage::onToggleView);
    connect(m_btnBuildBST, &QPushButton::clicked, this, &BooksCollectionPage::onBuildBST);
    connect(m_btnSearchBST, &QPushButton::clicked, this, &BooksCollectionPage::onSearchBST);
    connect(m_bstSearchBox, &QLineEdit::returnPressed, this, &BooksCollectionPage::onSearchBST);
}

void BooksCollectionPage::createContentSection(QVBoxLayout* contentLayout)
{
    m_viewStack = new QStackedWidget(this);
    
    // Card View
    m_cardScrollArea = new QScrollArea();
    m_cardScrollArea->setWidgetResizable(true);
    m_cardScrollArea->setFrameShape(QFrame::NoFrame);
    m_cardScrollArea->setStyleSheet("background: transparent; border: none;");
    
    m_cardContainer = new QWidget();
    m_cardContainer->setStyleSheet("background: transparent;");
    m_cardLayout = new QGridLayout(m_cardContainer);
    m_cardLayout->setSpacing(20);
    m_cardLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    m_cardScrollArea->setWidget(m_cardContainer);
    m_viewStack->addWidget(m_cardScrollArea);
    
    // Table View
    QFrame* tableFrame = createCardFrame();
    QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);
    tableLayout->setContentsMargins(0, 0, 0, 0); 
    
    m_tableBooks = new QTableWidget();
    m_tableBooks->setColumnCount(6);
    m_tableBooks->setHorizontalHeaderLabels({"ID", "Judul Buku", "Penulis", "Genre", "Tahun", "Rating"});
    
    m_tableBooks->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: none;"
        "   gridline-color: transparent;"
        "   border-radius: 16px;"
        "   selection-background-color: #F4F7FE;"
        "   selection-color: #4318FF;"
        "}"
        "QTableWidget::item {"
        "   padding: 12px;"
        "   border-bottom: 1px solid #F1F5F9;"
        "   color: #2B3674;"
        "   font-size: 13px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #F8F9FC;"
        "   color: #64748B;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-bottom: 2px solid #E0E5F2;"
        "   padding: 12px 10px;"
        "   text-align: left;"
        "}"
    );
    
    m_tableBooks->verticalHeader()->setVisible(false);
    m_tableBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableBooks->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableBooks->setShowGrid(false);
    m_tableBooks->setFocusPolicy(Qt::NoFocus);
    m_tableBooks->setAlternatingRowColors(false);
    
    m_tableBooks->horizontalHeader()->setStretchLastSection(true);
    m_tableBooks->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_tableBooks->setColumnWidth(0, 60);
    m_tableBooks->setColumnWidth(2, 180);
    
    connect(m_tableBooks, &QTableWidget::itemSelectionChanged, this, &BooksCollectionPage::onTableSelectionChanged);
    
    tableLayout->addWidget(m_tableBooks);
    m_viewStack->addWidget(tableFrame);
    
    m_viewStack->setCurrentIndex(1);
    m_isCardView = false;
    
    contentLayout->addWidget(m_viewStack, 1);
}

void BooksCollectionPage::createActionSection(QVBoxLayout* contentLayout)
{
    QHBoxLayout* actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(15);
    
    QString btnStyle = 
        "QPushButton { border-radius: 10px; padding: 12px 25px; font-weight: bold; color: white; font-size: 13px; border: none; font-family: 'Segoe UI'; }"
        "QPushButton:disabled { background-color: #E0E5F2; color: #A3AED0; }";
    
    m_btnUndoDelete = new QPushButton("Undo Hapus");
    m_btnUndoDelete->setIcon(QIcon(":/svg/undo.svg"));
    m_btnUndoDelete->setIconSize(QSize(18, 18));
    m_btnUndoDelete->setCursor(Qt::PointingHandCursor);
    m_btnUndoDelete->setStyleSheet(btnStyle + "QPushButton { background-color: #FFB547; } QPushButton:hover { background-color: #E59A2B; }");
    
    m_btnDeleteBook = new QPushButton("Hapus Buku");
    m_btnDeleteBook->setIcon(QIcon(":/svg/delete.svg"));
    m_btnDeleteBook->setIconSize(QSize(18, 18));
    m_btnDeleteBook->setEnabled(false);
    m_btnDeleteBook->setCursor(Qt::PointingHandCursor);
    m_btnDeleteBook->setStyleSheet(btnStyle + "QPushButton { background-color: #EE5D50; } QPushButton:hover { background-color: #D43F33; }");
    
    m_btnEditBook = new QPushButton("Edit Buku");
    m_btnEditBook->setIcon(QIcon(":/svg/edit.svg"));
    m_btnEditBook->setIconSize(QSize(18, 18));
    m_btnEditBook->setEnabled(false);
    m_btnEditBook->setCursor(Qt::PointingHandCursor);
    m_btnEditBook->setStyleSheet(btnStyle + "QPushButton { background-color: #4318FF; } QPushButton:hover { background-color: #3311DB; }");
    
    actionLayout->addStretch();
    actionLayout->addWidget(m_btnUndoDelete);
    actionLayout->addWidget(m_btnDeleteBook);
    actionLayout->addWidget(m_btnEditBook);
    
    contentLayout->addLayout(actionLayout);
    
    connect(m_btnEditBook, &QPushButton::clicked, this, &BooksCollectionPage::onEditBook);
    connect(m_btnDeleteBook, &QPushButton::clicked, this, &BooksCollectionPage::onDeleteBook);
    connect(m_btnUndoDelete, &QPushButton::clicked, this, &BooksCollectionPage::onUndoDelete);
}

void BooksCollectionPage::populateGenreComboBox()
{
    m_genreCombo->clear();
    m_genreCombo->addItem("-- Semua Genre --");
    
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::set<QString> genres;
    for (const Book& book : dbManager.getAllBooks()) {
        for (const QString& genre : book.getGenre()) {
            genres.insert(genre.trimmed());
        }
    }
    for (const QString& genre : genres) {
        m_genreCombo->addItem(genre);
    }
}

void BooksCollectionPage::refreshTable()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books);
    else loadBooksToTable(books);
    populateGenreComboBox();
}

void BooksCollectionPage::loadBooksToTable(const std::vector<Book>& books)
{
    m_tableBooks->setRowCount(0);
    m_tableBooks->setSortingEnabled(false);
    
    for (const Book& book : books) {
        int row = m_tableBooks->rowCount();
        m_tableBooks->insertRow(row);
        m_tableBooks->setRowHeight(row, 60);
        
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(book.getId()));
        idItem->setTextAlignment(Qt::AlignCenter);
        m_tableBooks->setItem(row, 0, idItem);
        
        QTableWidgetItem* titleItem = new QTableWidgetItem(book.getJudul());
        titleItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        m_tableBooks->setItem(row, 1, titleItem);
        
        m_tableBooks->setItem(row, 2, new QTableWidgetItem(book.getPenulis()));
        m_tableBooks->setItem(row, 3, new QTableWidgetItem(book.getGenre().join(", ")));
        
        QTableWidgetItem* yearItem = new QTableWidgetItem(QString::number(book.getTahun()));
        yearItem->setTextAlignment(Qt::AlignCenter);
        m_tableBooks->setItem(row, 4, yearItem);
        
        QString ratingText = QString::number(book.getRating(), 'f', 1);
        QTableWidgetItem* ratingItem = new QTableWidgetItem(ratingText);
        QIcon starIcon(":/svg/star.svg");
        ratingItem->setIcon(starIcon);
        ratingItem->setTextAlignment(Qt::AlignCenter);
        ratingItem->setForeground(QBrush(QColor("#FFB547")));
        ratingItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        m_tableBooks->setItem(row, 5, ratingItem);
    }
}

void BooksCollectionPage::loadBooksToCards(const std::vector<Book>& books)
{
    QLayoutItem* item;
    while ((item = m_cardLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    
    int col = 0; 
    int row = 0; 
    int maxCols = 4;
    
    for (const Book& book : books) {
        BookCardWidget* card = new BookCardWidget(book, m_cardContainer);
        connect(card, &BookCardWidget::editRequested, this, &BooksCollectionPage::editBookRequested);
        connect(card, &BookCardWidget::deleteRequested, this, &BooksCollectionPage::deleteBookRequested);
        
        m_cardLayout->addWidget(card, row, col);
        col++;
        if (col >= maxCols) { 
            col = 0; 
            row++; 
        }
    }
    m_cardLayout->setRowStretch(row + 1, 1);
}

void BooksCollectionPage::onToggleView()
{
    m_isCardView = !m_isCardView;
    if (m_isCardView) {
        m_viewStack->setCurrentIndex(0);
        m_btnToggleView->setText("Table View");
        m_btnToggleView->setIcon(QIcon(":/svg/view-list.svg"));
    } else {
        m_viewStack->setCurrentIndex(1);
        m_btnToggleView->setText("Card View");
        m_btnToggleView->setIcon(QIcon(":/svg/view-grid.svg"));
    }
    refreshTable();
}

int BooksCollectionPage::getSelectedBookId()
{
    QList<QTableWidgetItem*> selected = m_tableBooks->selectedItems();
    if (selected.isEmpty()) return -1;
    int row = selected.first()->row();
    return m_tableBooks->item(row, 0)->text().toInt();
}

void BooksCollectionPage::onSearchTextChanged()
{
    QString searchText = m_searchBox->text().trimmed();
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> results;
    if (searchText.isEmpty()) results = dbManager.getAllBooks();
    else results = dbManager.searchByTitle(searchText);
    if (m_isCardView) loadBooksToCards(results); 
    else loadBooksToTable(results);
}

void BooksCollectionPage::onSearchByGenre()
{
    QString genre = m_genreCombo->currentText();
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> results;
    if (genre == "-- Semua Genre --") results = dbManager.getAllBooks();
    else results = dbManager.searchByGenre(genre);
    if (m_isCardView) loadBooksToCards(results); 
    else loadBooksToTable(results);
}

void BooksCollectionPage::onSearchByAuthor()
{
    QString author = m_authorSearch->text().trimmed();
    if (author.isEmpty()) { 
        QMessageBox::warning(this, "Peringatan", "Masukkan nama penulis!"); 
        return; 
    }
    std::vector<Book> results = DatabaseManager::instance().searchByAuthor(author);
    if (results.empty()) 
        QMessageBox::information(this, "Info", QString("Tidak ada buku dari '%1'").arg(author));
    if (m_isCardView) loadBooksToCards(results); 
    else loadBooksToTable(results);
}

void BooksCollectionPage::onClearSearch()
{
    m_searchBox->clear(); 
    m_authorSearch->clear(); 
    m_genreCombo->setCurrentIndex(0);
    refreshTable();
}

void BooksCollectionPage::onSortByTitle()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByTitle(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); 
    else loadBooksToTable(books);
    m_sortAscending = !m_sortAscending;
}

void BooksCollectionPage::onSortByYear()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByYear(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); 
    else loadBooksToTable(books);
    m_sortAscending = !m_sortAscending;
}

void BooksCollectionPage::onSortByRating()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByRating(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); 
    else loadBooksToTable(books);
    m_sortAscending = !m_sortAscending;
}

void BooksCollectionPage::onSortByAuthor()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByAuthor(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); 
    else loadBooksToTable(books);
    m_sortAscending = !m_sortAscending;
}

void BooksCollectionPage::onEditBook()
{
    int bookId = getSelectedBookId();
    if (bookId != -1) emit editBookRequested(bookId);
}

void BooksCollectionPage::onDeleteBook()
{
    int bookId = getSelectedBookId();
    if (bookId != -1) emit deleteBookRequested(bookId);
}

void BooksCollectionPage::onUndoDelete()
{
    DatabaseManager& db = DatabaseManager::instance();
    if (db.getBookManager().undoDelete()) {
        db.saveBookManagerToDatabase();
        refreshTable();
        QMessageBox::information(this, "Sukses", "Undo berhasil!");
    } else {
        QMessageBox::warning(this, "Gagal", "Tidak ada yang bisa di-undo.");
    }
}

void BooksCollectionPage::onTableSelectionChanged()
{
    bool hasSelection = !m_tableBooks->selectedItems().isEmpty();
    m_btnEditBook->setEnabled(hasSelection);
    m_btnDeleteBook->setEnabled(hasSelection);
    if (hasSelection) emit bookSelected(getSelectedBookId());
}

void BooksCollectionPage::onBuildBST()
{
    DatabaseManager& db = DatabaseManager::instance();
    db.getBookManager().buildBST();
    
    QMessageBox::information(this, "BST Berhasil", 
        QString("Binary Search Tree berhasil dibangun dengan %1 buku!\n\n"
                "BST diurutkan berdasarkan judul buku.\n"
                "Gunakan fitur 'Search BST' untuk mencari buku dengan cepat.")
                .arg(db.getBookManager().getBookCount()));
}

void BooksCollectionPage::onSearchBST()
{
    QString searchTitle = m_bstSearchBox->text().trimmed();
    
    if (searchTitle.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Masukkan judul buku yang ingin dicari!");
        return;
    }
    
    DatabaseManager& db = DatabaseManager::instance();
    
    if (!db.getBookManager().hasBST()) {
        QMessageBox::warning(this, "BST Belum Dibangun", 
            "Binary Search Tree belum dibangun!\n\nKlik tombol 'Build BST' terlebih dahulu.");
        return;
    }
    
    Book* found = db.getBookManager().searchBST(searchTitle);
    
    if (found) {
        std::vector<Book> result;
        result.push_back(*found);
        
        if (m_isCardView) {
            loadBooksToCards(result);
        } else {
            loadBooksToTable(result);
        }
        
        QMessageBox::information(this, "Buku Ditemukan", 
            QString("Buku ditemukan menggunakan Binary Search Tree!\n\n"
                    "Judul: %1\n"
                    "Penulis: %2\n"
                    "Tahun: %3\n"
                    "Rating: %4")
                    .arg(found->getJudul())
                    .arg(found->getPenulis())
                    .arg(found->getTahun())
                    .arg(found->getRating()));
    } else {
        QMessageBox::information(this, "Tidak Ditemukan", 
            QString("Buku dengan judul '%1' tidak ditemukan di BST.\n\n"
                    "Pastikan ejaan judul benar dan BST sudah dibangun.")
                    .arg(searchTitle));
    }
}
