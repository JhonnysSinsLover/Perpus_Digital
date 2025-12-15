#include "BooksCollectionPage.h"
#include "AddBookDialog.h"
#include "BookCardWidget.h" 
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QStyle>
#include <QGraphicsDropShadowEffect> // Pastikan ini ada
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
    // 1. Layout Utama
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. Header
    createHeaderSection(mainLayout);
    
    // 3. Scroll Area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    // Background Abu-abu (#F4F7FE) agar kontras dengan kartu putih
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
    
    // --- Bagian Konten ---
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
    
    QLabel* iconLabel = new QLabel("📚", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent;");
    headerLayout->addWidget(iconLabel);
    
    QLabel* titleLabel = new QLabel("Koleksi Buku", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674; margin-left: 10px; background: transparent;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch();
    
    // Global Search (Header)
    QLineEdit* globalSearch = new QLineEdit(headerFrame);
    globalSearch->setPlaceholderText("Search...");
    globalSearch->setFixedSize(250, 40);
    // Style khusus header: Border jelas, background putih
    globalSearch->setStyleSheet(
        "QLineEdit { "
        "   background-color: #FFFFFF; "
        "   border: 1px solid #94A3B8; " /* Border Slate Grey */
        "   border-radius: 20px; "
        "   padding-left: 15px; "
        "   color: #2B3674; " /* Text Navy */
        "   font-family: 'Segoe UI'; font-size: 13px;"
        "}"
        "QLineEdit:focus { border: 2px solid #4318FF; }"
    );
    headerLayout->addWidget(globalSearch);
    
    // Profile
    QPushButton* profileBtn = new QPushButton("👤", headerFrame);
    profileBtn->setFixedSize(45, 45);
    profileBtn->setStyleSheet("QPushButton { background-color: #112D4E; border: none; border-radius: 22px; color: white; }");
    headerLayout->addWidget(profileBtn);
    
    mainLayout->addWidget(headerFrame);
}

// Helper untuk membuat kartu putih dengan bayangan
QFrame* BooksCollectionPage::createCardFrame()
{
    QFrame* card = new QFrame(this);
    card->setStyleSheet("background-color: white; border-radius: 16px;");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 20)); // Shadow lebih gelap sedikit agar pop-up
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);
    return card;
}

void BooksCollectionPage::createControlSection(QVBoxLayout* contentLayout)
{
    QFrame* controlCard = createCardFrame();
    controlCard->setObjectName("controlCard"); 
    
    // --- STYLE GLOBAL UNTUK KARTU INI (PENYELAMAT TAMPILAN) ---
    controlCard->setStyleSheet(
        "#controlCard { background-color: white; border-radius: 16px; }"
        
        /* STYLE INPUT & DROPDOWN AGAR TERLIHAT */
        "QLineEdit, QComboBox {"
        "   background-color: #FFFFFF;"    /* Pastikan putih bersih */
        "   border: 1px solid #94A3B8;"    /* BORDER ABU TUA (PENTING!) */
        "   border-radius: 8px;"
        "   padding: 8px 12px;"
        "   font-size: 13px;"
        "   color: #2B3674;"               /* TEXT BIRU GELAP (PENTING!) */
        "   min-height: 25px;"
        "   font-family: 'Segoe UI';"
        "}"
        "QLineEdit:focus, QComboBox:focus {"
        "   border: 2px solid #4318FF;"    /* Fokus jadi biru */
        "   background-color: #FDFDFD;"
        "}"
        
        /* STYLE DROPDOWN LIST (POPUP) AGAR TIDAK PUTIH POLOS */
        "QComboBox::drop-down { border: none; width: 30px; }"
        "QComboBox::down-arrow { image: none; border-left: 2px solid transparent; border-top: 2px solid #64748B; width: 8px; height: 8px; margin-right: 10px; }"
        "QComboBox QAbstractItemView {"
        "   background-color: #FFFFFF;"
        "   border: 1px solid #94A3B8;"
        "   selection-background-color: #EAEFFC;"
        "   selection-color: #2B3674;"
        "   color: #2B3674;" /* Warna teks item */
        "   outline: none;"
        "}"
        
        /* Label Title */
        "QLabel { color: #64748B; font-weight: bold; font-family: 'Segoe UI'; font-size: 13px; background: transparent; }"
    );
    
    QVBoxLayout* cardLayout = new QVBoxLayout(controlCard);
    cardLayout->setContentsMargins(25, 25, 25, 25);
    cardLayout->setSpacing(20);
    
    // --- ROW 1: HEADER ---
    QLabel* lblHeader = new QLabel("Pencarian & Filter", controlCard);
    lblHeader->setStyleSheet("color: #2B3674; font-size: 18px; font-weight: 700; border: none;");
    cardLayout->addWidget(lblHeader);

    // --- ROW 2: SEARCH FIELDS ---
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(15);

    // Group Search Judul
    QVBoxLayout* box1 = new QVBoxLayout(); box1->setSpacing(5);
    QLabel* lbl1 = new QLabel("Cari Judul:");
    m_searchBox = new QLineEdit(controlCard);
    m_searchBox->setPlaceholderText("Ketik judul buku...");
    box1->addWidget(lbl1);
    box1->addWidget(m_searchBox);
    searchLayout->addLayout(box1, 2);

    // Group Search Penulis
    QVBoxLayout* box2 = new QVBoxLayout(); box2->setSpacing(5);
    QLabel* lbl2 = new QLabel("Cari Penulis:");
    m_authorSearch = new QLineEdit(controlCard);
    m_authorSearch->setPlaceholderText("Ketik nama penulis...");
    box2->addWidget(lbl2);
    box2->addWidget(m_authorSearch);
    searchLayout->addLayout(box2, 2);

    // Group Filter Genre
    QVBoxLayout* box3 = new QVBoxLayout(); box3->setSpacing(5);
    QLabel* lbl3 = new QLabel("Filter Genre:");
    m_genreCombo = new QComboBox(controlCard);
    box3->addWidget(lbl3);
    box3->addWidget(m_genreCombo);
    searchLayout->addLayout(box3, 1);

    // Tombol Actions
    QVBoxLayout* boxBtn = new QVBoxLayout(); 
    boxBtn->setSpacing(5);
    boxBtn->addWidget(new QLabel(" ")); // Spacer
    
    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* btnFilter = new QPushButton("Cari");
    btnFilter->setCursor(Qt::PointingHandCursor);
    btnFilter->setFixedSize(70, 38); // Ukuran tombol pas
    btnFilter->setStyleSheet("QPushButton { background-color: #4318FF; color: white; border-radius: 8px; font-weight: bold; border: none; } QPushButton:hover { background-color: #3311DB; }");
    
    QPushButton* btnReset = new QPushButton("Reset");
    btnReset->setCursor(Qt::PointingHandCursor);
    btnReset->setFixedSize(70, 38);
    btnReset->setStyleSheet("QPushButton { background-color: white; border: 1px solid #94A3B8; color: #64748B; border-radius: 8px; font-weight: bold; } QPushButton:hover { border: 1px solid #EE5D50; color: #EE5D50; }");

    btnRow->addWidget(btnFilter);
    btnRow->addWidget(btnReset);
    boxBtn->addLayout(btnRow);
    
    searchLayout->addLayout(boxBtn);
    cardLayout->addLayout(searchLayout);
    
    // --- ROW 3: SEPARATOR ---
    QFrame* line = new QFrame();
    line->setFixedHeight(1);
    line->setStyleSheet("background-color: #E0E5F2; border: none;");
    cardLayout->addWidget(line);
    
    // --- ROW 4: SORTING ---
    QHBoxLayout* sortLayout = new QHBoxLayout();
    sortLayout->setSpacing(10);
    
    QLabel* lblSort = new QLabel("Urutkan:");
    sortLayout->addWidget(lblSort);
    
    auto createSortBtn = [](QString text) -> QPushButton* {
        QPushButton* btn = new QPushButton(text);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(32);
        btn->setStyleSheet(
            "QPushButton { background-color: #F1F5F9; color: #2B3674; border: none; border-radius: 6px; padding: 0 15px; font-weight: 600; }"
            "QPushButton:hover { background-color: #EAEFFC; color: #4318FF; }"
        );
        return btn;
    };
    
    QPushButton* btnSortTitle = createSortBtn("Judul");
    QPushButton* btnSortYear = createSortBtn("Tahun");
    QPushButton* btnSortRating = createSortBtn("Rating");
    QPushButton* btnSortAuthor = createSortBtn("Penulis");
    
    sortLayout->addWidget(btnSortTitle);
    sortLayout->addWidget(btnSortYear);
    sortLayout->addWidget(btnSortRating);
    sortLayout->addWidget(btnSortAuthor);
    sortLayout->addStretch();
    
    m_btnToggleView = new QPushButton("🗃️ Switch View");
    m_btnToggleView->setCursor(Qt::PointingHandCursor);
    m_btnToggleView->setFixedHeight(35);
    m_btnToggleView->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #4318FF; color: #4318FF; border-radius: 8px; padding: 0 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #4318FF; color: white; }"
    );
    sortLayout->addWidget(m_btnToggleView);
    
    cardLayout->addLayout(sortLayout);
    
    // --- ROW 5: BST SEARCH FEATURE (TREE DATA STRUCTURE) ---
    QFrame* lineBST = new QFrame();
    lineBST->setFixedHeight(1);
    lineBST->setStyleSheet("background-color: #E0E5F2; border: none;");
    cardLayout->addWidget(lineBST);
    
    QHBoxLayout* bstLayout = new QHBoxLayout();
    bstLayout->setSpacing(10);
    
    QLabel* lblBST = new QLabel("🌳 Binary Search Tree:");
    lblBST->setStyleSheet("font-weight: bold; color: #2B3674;");
    bstLayout->addWidget(lblBST);
    
    m_btnBuildBST = new QPushButton("Build BST");
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
    m_bstSearchBox->setStyleSheet("QLineEdit { border: 1px solid #CBD5E1; border-radius: 6px; padding: 0 10px; }");
    bstLayout->addWidget(m_bstSearchBox, 1);
    
    m_btnSearchBST = new QPushButton("🔍 Search BST");
    m_btnSearchBST->setCursor(Qt::PointingHandCursor);
    m_btnSearchBST->setFixedHeight(32);
    m_btnSearchBST->setStyleSheet(
        "QPushButton { background-color: #6366F1; color: white; border: none; border-radius: 6px; padding: 0 15px; font-weight: 600; }"
        "QPushButton:hover { background-color: #4F46E5; }"
    );
    bstLayout->addWidget(m_btnSearchBST);
    
    cardLayout->addLayout(bstLayout);
    contentLayout->addWidget(controlCard);
    
    // --- CONNECT SIGNALS ---
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
    
    // 1. CARD VIEW (Grid)
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
    
    // 2. TABLE VIEW (List)
    QFrame* tableFrame = createCardFrame();
    QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);
    tableLayout->setContentsMargins(0, 0, 0, 0); 
    
    m_tableBooks = new QTableWidget();
    m_tableBooks->setColumnCount(6);
    m_tableBooks->setHorizontalHeaderLabels({"ID", "Judul Buku", "Penulis", "Genre", "Tahun", "Rating"});
    
    // --- STYLE TABLE (Agar text header nampak) ---
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
        "   color: #2B3674;" /* Warna Text Row */
        "   font-size: 13px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #F8F9FC;"
        "   color: #64748B;" /* Warna Text Header (Abu Gelap) */
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
    
    m_viewStack->setCurrentIndex(1); // Default Table View
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
    
    m_btnUndoDelete = new QPushButton("↩️ Undo Hapus");
    m_btnUndoDelete->setCursor(Qt::PointingHandCursor);
    m_btnUndoDelete->setStyleSheet(btnStyle + "QPushButton { background-color: #FFB547; } QPushButton:hover { background-color: #E59A2B; }");
    
    m_btnDeleteBook = new QPushButton("🗑️ Hapus Buku");
    m_btnDeleteBook->setEnabled(false);
    m_btnDeleteBook->setCursor(Qt::PointingHandCursor);
    m_btnDeleteBook->setStyleSheet(btnStyle + "QPushButton { background-color: #EE5D50; } QPushButton:hover { background-color: #D43F33; }");
    
    m_btnEditBook = new QPushButton("✏️ Edit Buku");
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

// --- LOGIC FUNCTIONS (TIDAK BERUBAH) ---

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
        
        QString ratingText = QString::number(book.getRating(), 'f', 1) + " ⭐";
        QTableWidgetItem* ratingItem = new QTableWidgetItem(ratingText);
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
    
    int col = 0; int row = 0; int maxCols = 4; // 4 Kolom agar lebih padat
    
    for (const Book& book : books) {
        BookCardWidget* card = new BookCardWidget(book, m_cardContainer);
        connect(card, &BookCardWidget::editRequested, this, &BooksCollectionPage::editBookRequested);
        connect(card, &BookCardWidget::deleteRequested, this, &BooksCollectionPage::deleteBookRequested);
        
        m_cardLayout->addWidget(card, row, col);
        col++;
        if (col >= maxCols) { col = 0; row++; }
    }
    m_cardLayout->setRowStretch(row + 1, 1);
}

void BooksCollectionPage::onToggleView()
{
    m_isCardView = !m_isCardView;
    if (m_isCardView) {
        m_viewStack->setCurrentIndex(0);
        m_btnToggleView->setText("🗃️ Table View");
    } else {
        m_viewStack->setCurrentIndex(1);
        m_btnToggleView->setText("🗃️ Card View");
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
    if (m_isCardView) loadBooksToCards(results); else loadBooksToTable(results);
}

void BooksCollectionPage::onSearchByGenre()
{
    QString genre = m_genreCombo->currentText();
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> results;
    if (genre == "-- Semua Genre --") results = dbManager.getAllBooks();
    else results = dbManager.searchByGenre(genre);
    if (m_isCardView) loadBooksToCards(results); else loadBooksToTable(results);
}

void BooksCollectionPage::onSearchByAuthor()
{
    QString author = m_authorSearch->text().trimmed();
    if (author.isEmpty()) { QMessageBox::warning(this, "Peringatan", "Masukkan nama penulis!"); return; }
    std::vector<Book> results = DatabaseManager::instance().searchByAuthor(author);
    if (results.empty()) QMessageBox::information(this, "Info", QString("Tidak ada buku dari '%1'").arg(author));
    if (m_isCardView) loadBooksToCards(results); else loadBooksToTable(results);
}

void BooksCollectionPage::onClearSearch()
{
    m_searchBox->clear(); m_authorSearch->clear(); m_genreCombo->setCurrentIndex(0);
    refreshTable();
}

void BooksCollectionPage::onSortByTitle()
{
    // Menggunakan QuickSort dari BookManager (implementasi custom)
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByTitle(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); else loadBooksToTable(books);
    m_sortAscending = !m_sortAscending;
}

void BooksCollectionPage::onSortByYear()
{
    // Menggunakan QuickSort dari BookManager (implementasi custom)
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByYear(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); else loadBooksToTable(books);
    m_sortAscending = !m_sortAscending;
}

void BooksCollectionPage::onSortByRating()
{
    // Menggunakan QuickSort dari BookManager (implementasi custom)
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByRating(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); else loadBooksToTable(books);
    m_sortAscending = !m_sortAscending;
}

void BooksCollectionPage::onSortByAuthor()
{
    // Menggunakan QuickSort dari BookManager (implementasi custom)
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.getBookManager().quickSortByAuthor(m_sortAscending);
    std::vector<Book> books = dbManager.getAllBooks();
    if (m_isCardView) loadBooksToCards(books); else loadBooksToTable(books);
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

// ============================================================================
// BINARY SEARCH TREE (BST) - TREE DATA STRUCTURE IMPLEMENTATION
// ============================================================================

void BooksCollectionPage::onBuildBST()
{
    DatabaseManager& db = DatabaseManager::instance();
    
    // Build BST dari semua buku
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
    
    // Cek apakah BST sudah dibangun
    if (!db.getBookManager().hasBST()) {
        QMessageBox::warning(this, "BST Belum Dibangun", 
            "Binary Search Tree belum dibangun!\n\n"
            "Klik tombol 'Build BST' terlebih dahulu.");
        return;
    }
    
    // Cari di BST
    Book* found = db.getBookManager().searchBST(searchTitle);
    
    if (found) {
        // Tampilkan hasil pencarian
        std::vector<Book> result;
        result.push_back(*found);
        
        if (m_isCardView) {
            loadBooksToCards(result);
        } else {
            loadBooksToTable(result);
        }
        
        QMessageBox::information(this, "Buku Ditemukan", 
            QString("✅ Buku ditemukan menggunakan Binary Search Tree!\n\n"
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
            QString("❌ Buku dengan judul '%1' tidak ditemukan di BST.\n\n"
                    "Pastikan ejaan judul benar dan BST sudah dibangun.")
                    .arg(searchTitle));
    }
}