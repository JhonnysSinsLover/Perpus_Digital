#include "BooksCollectionPage.h"
#include "AddBookDialog.h"
#include "BookCardWidget.h" 
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QTimer>
#include <set>
#include <algorithm>

BooksCollectionPage::BooksCollectionPage(QWidget *parent)
    : QWidget(parent)
    , m_isCardView(true) 
{
    setupUI();
    // Panggil refreshTable di awal untuk load data & populate genre
    refreshTable();
}

BooksCollectionPage::~BooksCollectionPage()
{
}

void BooksCollectionPage::setupUI()
{
    // Layout Utama: Vertikal penuh
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header Section (Fixed Top)
    createHeaderSection(mainLayout);
    
    // 2. Control/Toolbar Section (Fixed Top, under Header)
    QWidget* toolbarWidget = new QWidget();
    toolbarWidget->setStyleSheet("background-color: #F4F7FE; border-bottom: 1px solid #E0E5F2;");
    QVBoxLayout* toolbarLayout = new QVBoxLayout(toolbarWidget);
    toolbarLayout->setContentsMargins(30, 20, 30, 10); 
    createControlSection(toolbarLayout);
    mainLayout->addWidget(toolbarWidget);

    // 3. Scroll Area (Middle - Expanded)
    // Area ini HANYA berisi daftar buku (Card/Table).
    m_contentScrollArea = new QScrollArea(this);
    m_contentScrollArea->setWidgetResizable(true);
    m_contentScrollArea->setFrameShape(QFrame::NoFrame);
    m_contentScrollArea->setStyleSheet(
        "QScrollArea { background-color: #F4F7FE; border: none; }"
        "QScrollBar:vertical { background: #F4F7FE; width: 12px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 6px; }"
    );
    
    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #F4F7FE;");
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 10, 30, 30);
    
    createContentSection(contentLayout); // Masukkan Card/Table ke sini
    
    contentLayout->addStretch(); // Push content ke atas
    m_contentScrollArea->setWidget(contentWidget);
    
    // Tambahkan scroll area ke layout utama dengan stretch factor 1 (agar memenuhi ruang kosong)
    mainLayout->addWidget(m_contentScrollArea, 1);

    // 4. Action Section (Sticky Bottom)
    // Ini ditaruh DI LUAR scroll area, langsung ke mainLayout agar menempel di bawah
    createActionSection(mainLayout);
}

void BooksCollectionPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(70);
    headerFrame->setStyleSheet("background-color: white; border-bottom: 1px solid #E0E5F2;");
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(15);
    
    QLabel* iconLabel = new QLabel("📚", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent; border: none;");
    headerLayout->addWidget(iconLabel);
    
    QLabel* titleLabel = new QLabel("Koleksi Perpustakaan", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 22px; font-weight: 700; color: #2B3674; border: none;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch();
    
    QPushButton* profileBtn = new QPushButton("Admin", headerFrame);
    profileBtn->setFixedSize(80, 34);
    profileBtn->setStyleSheet(
        "QPushButton { background-color: #1E293B; color: white; border-radius: 17px; font-weight: 600; border: none; }"
    );
    headerLayout->addWidget(profileBtn);
    
    mainLayout->addWidget(headerFrame);
}

void BooksCollectionPage::createControlSection(QVBoxLayout* containerLayout)
{
    // --- Baris 1: Filter Utama ---
    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(15);

    QString inputStyle = 
        "QLineEdit, QComboBox {"
        "   background-color: white;"
        "   border: 1px solid #94A3B8;"
        "   border-radius: 8px;"
        "   padding: 0 12px;"
        "   color: #2B3674;"
        "   font-size: 13px;"
        "   min-height: 40px;"
        "}"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #4318FF; }";

    m_searchBox = new QLineEdit();
    m_searchBox->setPlaceholderText("🔍 Cari judul, penulis...");
    m_searchBox->setMinimumWidth(250);
    m_searchBox->setStyleSheet(inputStyle);
    toolbarLayout->addWidget(m_searchBox, 1);

    m_genreCombo = new QComboBox();
    m_genreCombo->setFixedWidth(160);
    m_genreCombo->setStyleSheet(inputStyle);
    m_genreCombo->addItem("Semua Genre"); 
    toolbarLayout->addWidget(m_genreCombo);

    m_sortCombo = new QComboBox();
    m_sortCombo->addItem("Judul (A-Z)");           // Index 0
    m_sortCombo->addItem("Judul (Z-A)");           // Index 1
    m_sortCombo->addItem("Tahun (Terbaru)");       // Index 2
    m_sortCombo->addItem("Tahun (Terlama)");       // Index 3 (BARU)
    m_sortCombo->addItem("Rating (Tertinggi)");    // Index 4
    m_sortCombo->addItem("Rating (Terendah)");     // Index 5 (BARU)
    
    m_sortCombo->setFixedWidth(170);
    m_sortCombo->setStyleSheet(inputStyle);
    toolbarLayout->addWidget(m_sortCombo);

    m_btnToggleView = new QPushButton("Tampilan Tabel");
    m_btnToggleView->setCheckable(true);
    m_btnToggleView->setFixedSize(140, 40);
    m_btnToggleView->setCursor(Qt::PointingHandCursor);
    m_btnToggleView->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #4318FF; color: #4318FF; border-radius: 8px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #F4F7FE; }"
        "QPushButton:checked { background-color: #4318FF; color: white; }"
    );
    toolbarLayout->addWidget(m_btnToggleView);

    containerLayout->addLayout(toolbarLayout);

    // --- Baris 2: BST / Advanced Features ---
    QHBoxLayout* bstLayout = new QHBoxLayout();
    bstLayout->setContentsMargins(0, 5, 0, 0);

    m_btnBuildBST = new QPushButton("⚡ Re-Build Index Tree (BST)");
    m_btnBuildBST->setCursor(Qt::PointingHandCursor);
    m_btnBuildBST->setStyleSheet("QPushButton { border: none; color: #64748B; font-weight: 600; text-align: left; } QPushButton:hover { color: #4318FF; }");
    
    QLabel* bstLabel = new QLabel("Cari via ID (BST):");
    bstLabel->setStyleSheet("color: #64748B; margin-left: 10px;");

    m_bstSearchBox = new QLineEdit();
    m_bstSearchBox->setPlaceholderText("Masukkan ID Buku...");
    m_bstSearchBox->setFixedWidth(150);
    m_bstSearchBox->setStyleSheet("QLineEdit { background: transparent; border: none; border-bottom: 1px solid #CBD5E1; color: #2B3674; }");

    bstLayout->addWidget(m_btnBuildBST);
    bstLayout->addWidget(bstLabel);
    bstLayout->addWidget(m_bstSearchBox);
    bstLayout->addStretch();

    containerLayout->addLayout(bstLayout);

    // Connects
    connect(m_searchBox, &QLineEdit::textChanged, this, &BooksCollectionPage::onSearchTextChanged);
    connect(m_genreCombo, &QComboBox::currentTextChanged, this, &BooksCollectionPage::onFilterChanged);
    connect(m_sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BooksCollectionPage::onFilterChanged);
    connect(m_btnToggleView, &QPushButton::clicked, this, &BooksCollectionPage::onToggleView);
    connect(m_btnBuildBST, &QPushButton::clicked, this, &BooksCollectionPage::onBuildBST);
    connect(m_bstSearchBox, &QLineEdit::returnPressed, this, &BooksCollectionPage::onSearchBST);
}

void BooksCollectionPage::createContentSection(QVBoxLayout* contentLayout)
{
    m_viewStack = new QStackedWidget(this);
    
    // VIEW 1: Cards
    m_cardContainer = new QWidget();
    m_cardContainer->setStyleSheet("background: transparent;");
    m_cardLayout = new QGridLayout(m_cardContainer);
    m_cardLayout->setSpacing(20);
    m_cardLayout->setContentsMargins(0, 0, 0, 0);
    m_cardLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_viewStack->addWidget(m_cardContainer);
    
    // VIEW 2: Table
    QFrame* tableFrame = createCardFrame();
    QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);
    tableLayout->setContentsMargins(0, 0, 0, 0); 
    
    m_tableBooks = new QTableWidget();
    m_tableBooks->setColumnCount(6);
    m_tableBooks->setHorizontalHeaderLabels({"ID", "Judul Buku", "Penulis", "Genre", "Tahun", "Rating"});
    m_tableBooks->verticalHeader()->setVisible(false);
    m_tableBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableBooks->setShowGrid(false);
    m_tableBooks->setAlternatingRowColors(true);
    m_tableBooks->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    m_tableBooks->setStyleSheet(
        "QTableWidget { background-color: white; border: none; border-radius: 12px; }"
        "QTableWidget::item { padding: 12px; border-bottom: 1px solid #F1F5F9; color: #2B3674; }"
        "QTableWidget::item:selected { background-color: #EAEFFC; color: #4318FF; }"
        "QHeaderView::section { background-color: #F8F9FC; color: #64748B; font-weight: bold; border: none; padding: 12px; border-bottom: 2px solid #E0E5F2; }"
    );
    
    m_tableBooks->horizontalHeader()->setStretchLastSection(true);
    m_tableBooks->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    
    connect(m_tableBooks, &QTableWidget::itemSelectionChanged, this, &BooksCollectionPage::onTableSelectionChanged);
    
    tableLayout->addWidget(m_tableBooks);
    m_viewStack->addWidget(tableFrame);
    
    contentLayout->addWidget(m_viewStack);
}

void BooksCollectionPage::createActionSection(QVBoxLayout* mainLayout)
{
    // Container untuk Footer (Sticky Bottom)
    QWidget* actionWidget = new QWidget();
    actionWidget->setStyleSheet("background-color: white; border-top: 1px solid #E0E5F2;");
    actionWidget->setFixedHeight(80); // Fixed height

    QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(30, 15, 30, 15);
    actionLayout->setSpacing(15);
    
    QString btnStyle = "QPushButton { border-radius: 8px; padding: 0 20px; height: 45px; font-weight: bold; color: white; border: none; font-size: 14px; }";
    
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
    
    // Tambahkan widget ini ke layout UTAMA (bukan ke scroll area)
    mainLayout->addWidget(actionWidget);
    
    connect(m_btnEditBook, &QPushButton::clicked, this, &BooksCollectionPage::onEditBook);
    connect(m_btnDeleteBook, &QPushButton::clicked, this, &BooksCollectionPage::onDeleteBook);
    connect(m_btnUndoDelete, &QPushButton::clicked, this, &BooksCollectionPage::onUndoDelete);
}

QFrame* BooksCollectionPage::createCardFrame()
{
    QFrame* card = new QFrame(this);
    card->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E2E8F0;");
    return card;
}

// --- LOGIC ---

void BooksCollectionPage::refreshTable()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    m_currentBooks = dbManager.getAllBooks(); // Load data
    
    populateGenreComboBox(); // Populate genre
    onFilterChanged();       // Apply filter
}

void BooksCollectionPage::populateGenreComboBox()
{
    QString currentSelection = m_genreCombo->currentText();
    
    m_genreCombo->blockSignals(true); 
    m_genreCombo->clear();
    m_genreCombo->addItem("Semua Genre");
    
    std::set<QString> genres;
    // Ambil genre dari data yang sudah di-load
    for (const Book& book : m_currentBooks) {
        for (const QString& g : book.getGenre()) {
            if(!g.trimmed().isEmpty()) genres.insert(g.trimmed());
        }
    }
    
    for (const QString& genre : genres) {
        m_genreCombo->addItem(genre);
    }
    
    int index = m_genreCombo->findText(currentSelection);
    if (index != -1) m_genreCombo->setCurrentIndex(index);
    
    m_genreCombo->blockSignals(false);
}

void BooksCollectionPage::onFilterChanged()
{
    std::vector<Book> filtered = m_currentBooks;
    
    // 1. Search Logic
    QString search = m_searchBox->text().toLower().trimmed();
    if (!search.isEmpty()) {
        std::vector<Book> temp;
        for (const auto& b : filtered) {
            if (b.getJudul().toLower().contains(search) || 
                b.getPenulis().toLower().contains(search)) {
                temp.push_back(b);
            }
        }
        filtered = temp;
    }
    
    // 2. Genre Filter
    QString genre = m_genreCombo->currentText();
    if (genre != "Semua Genre" && !genre.isEmpty()) {
        std::vector<Book> temp;
        for (const auto& b : filtered) {
            if (b.hasGenre(genre)) temp.push_back(b);
        }
        filtered = temp;
    }
    
    // 3. Sorting
    int sortIdx = m_sortCombo->currentIndex();
    std::sort(filtered.begin(), filtered.end(), [sortIdx](const Book& a, const Book& b) {
        switch(sortIdx) {
            case 0: return a.getJudul() < b.getJudul(); // A-Z
            case 1: return a.getJudul() > b.getJudul(); // Z-A
            case 2: return a.getTahun() > b.getTahun(); // Tahun Terbaru
            case 3: return a.getTahun() < b.getTahun(); // Tahun Terlama (FIX)
            case 4: return a.getRating() > b.getRating(); // Rating Tertinggi
            case 5: return a.getRating() < b.getRating(); // Rating Terendah (FIX)
            default: return a.getJudul() < b.getJudul();
        }
    });
    
    // 4. Render
    if (m_isCardView) {
        loadBooksToCards(filtered);
    } else {
        loadBooksToTable(filtered);
    }
}

void BooksCollectionPage::loadBooksToTable(const std::vector<Book>& books)
{
    m_tableBooks->setRowCount(0);
    m_tableBooks->setSortingEnabled(false); 
    
    for (const Book& book : books) {
        int row = m_tableBooks->rowCount();
        m_tableBooks->insertRow(row);
        m_tableBooks->setRowHeight(row, 60);
        
        m_tableBooks->setItem(row, 0, new QTableWidgetItem(QString::number(book.getId())));
        m_tableBooks->setItem(row, 1, new QTableWidgetItem(book.getJudul()));
        m_tableBooks->setItem(row, 2, new QTableWidgetItem(book.getPenulis()));
        m_tableBooks->setItem(row, 3, new QTableWidgetItem(book.getGenre().join(", ")));
        m_tableBooks->setItem(row, 4, new QTableWidgetItem(QString::number(book.getTahun())));
        
        QTableWidgetItem* ratingItem = new QTableWidgetItem(QString("%1 ⭐").arg(book.getRating()));
        ratingItem->setTextAlignment(Qt::AlignCenter);
        m_tableBooks->setItem(row, 5, ratingItem);
    }
}

void BooksCollectionPage::loadBooksToCards(const std::vector<Book>& books)
{
    // 1. Bersihkan kartu lama & Reset Kolom
    QLayoutItem* item;
    while ((item = m_cardLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    
    // PENTING: Reset settingan kolom sebelumnya agar tidak berantakan saat resize
    for (int i = 0; i < m_cardLayout->columnCount(); ++i) {
        m_cardLayout->setColumnStretch(i, 0);
    }

    // 2. Hitung Grid
    int viewportWidth = m_contentScrollArea->viewport()->width();
    if (viewportWidth <= 0) viewportWidth = 1000; 
    
    // Kurangi sedikit padding agar tidak memicu horizontal scrollbar
    int effectiveWidth = viewportWidth - 40; 

    int minCardWidth = 220; 
    int spacing = 20;

    // Hitung berapa kolom yang muat
    int numCols = (effectiveWidth + spacing) / (minCardWidth + spacing);
    if (numCols < 1) numCols = 1;

    // 3. KUNCI PERBAIKAN: Paksa Grid membuat slot untuk SEMUA kolom
    // Walaupun bukunya cuma 1, kita paksa Grid menganggap ada 'numCols' kolom.
    // Dengan memberi stretch factor 1 ke semua kolom, pembagian ruang jadi rata.
    for (int i = 0; i < numCols; ++i) {
        m_cardLayout->setColumnStretch(i, 1);
    }

    m_cardLayout->setVerticalSpacing(spacing);
    m_cardLayout->setHorizontalSpacing(spacing);
    m_cardLayout->setAlignment(Qt::AlignTop); // Rata atas

    // 4. Masukkan Buku
    for (int i = 0; i < books.size(); ++i) {
        BookCardWidget* card = new BookCardWidget(books[i], m_cardContainer);
        
        // Tetap gunakan Expanding agar mengisi celah kecil,
        // tapi karena sudah ada 'setColumnStretch', dia tidak akan menggila.
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        
        // Opsional: Batasi lebar maksimal agar tidak terlalu gepeng jika layar ultrawide
        card->setMaximumWidth(400); 
        
        int row = i / numCols;
        int col = i % numCols;
        
        m_cardLayout->addWidget(card, row, col);
        
        connect(card, &BookCardWidget::editRequested, this, &BooksCollectionPage::editBookRequested);
        connect(card, &BookCardWidget::deleteRequested, this, &BooksCollectionPage::deleteBookRequested);
    }
}

void BooksCollectionPage::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (m_isCardView) {
        QTimer::singleShot(0, this, [this](){
            onFilterChanged();
        });
    }
}

// --- BST IMPLEMENTATION ---

void BooksCollectionPage::onBuildBST() {
    DatabaseManager::instance().getBookManager().buildBST();
    QMessageBox::information(this, "Success", "Binary Search Tree Index berhasil dibangun ulang!\nPencarian ID sekarang O(log n).");
}

void BooksCollectionPage::onSearchBST() {
    QString searchTitle = m_bstSearchBox->text().trimmed();
    
    if (searchTitle.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Masukkan judul buku yang ingin dicari!");
        return;
    }

    DatabaseManager& db = DatabaseManager::instance();
    
    // Cek apakah BST sudah dibangun
    if (!db.getBookManager().hasBST()) {
        auto reply = QMessageBox::question(this, "BST Not Built", 
            "Binary Search Tree belum dibangun!\n"
            "Apakah Anda ingin membangun BST sekarang?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            onBuildBST();
        }
        return;
    }

    // PERBAIKAN: Menggunakan searchBST dengan QString (judul), bukan int
    Book* result = db.getBookManager().searchBST(searchTitle);

    if (result != nullptr) {
        std::vector<Book> foundBooks;
        foundBooks.push_back(*result);
        
        if (m_isCardView) loadBooksToCards(foundBooks);
        else loadBooksToTable(foundBooks);
        
        QMessageBox::information(this, "Found", 
            QString("Buku ditemukan!\n\nJudul: %1\nPenulis: %2\nRating: %3")
                .arg(result->getJudul())
                .arg(result->getPenulis())
                .arg(result->getRating()));
    } else {
        QMessageBox::information(this, "Not Found", 
            QString("Buku dengan judul '%1' tidak ditemukan dalam Tree.").arg(searchTitle));
    }
}

void BooksCollectionPage::onToggleView()
{
    m_isCardView = !m_isCardView;
    if (m_isCardView) {
        m_viewStack->setCurrentIndex(0);
        m_btnToggleView->setText("Tampilan Tabel");
        m_btnToggleView->setChecked(false);
    } else {
        m_viewStack->setCurrentIndex(1);
        m_btnToggleView->setText("Tampilan Kartu");
        m_btnToggleView->setChecked(true);
    }
    onFilterChanged();
}

void BooksCollectionPage::onClearSearch() { 
    m_searchBox->clear(); 
    m_genreCombo->setCurrentIndex(0); 
}

void BooksCollectionPage::onEditBook() { 
    int id = getSelectedBookId(); 
    if(id != -1) emit editBookRequested(id); 
}

void BooksCollectionPage::onDeleteBook() { 
    int id = getSelectedBookId(); 
    if(id != -1) emit deleteBookRequested(id); 
}

void BooksCollectionPage::onUndoDelete() { 
    if(DatabaseManager::instance().getBookManager().undoDelete()) {
        DatabaseManager::instance().saveBookManagerToDatabase();
        refreshTable();
        QMessageBox::information(this, "Info", "Buku berhasil dikembalikan.");
    } else {
        QMessageBox::information(this, "Info", "Tidak ada data untuk dikembalikan.");
    }
}

void BooksCollectionPage::onTableSelectionChanged() {
    bool hasSelection = !m_tableBooks->selectedItems().isEmpty();
    m_btnEditBook->setEnabled(hasSelection); 
    m_btnDeleteBook->setEnabled(hasSelection);
}

int BooksCollectionPage::getSelectedBookId() {
    auto items = m_tableBooks->selectedItems();
    if (items.isEmpty()) return -1;
    return m_tableBooks->item(items[0]->row(), 0)->text().toInt();
}

void BooksCollectionPage::onSearchTextChanged() {
    onFilterChanged();
}