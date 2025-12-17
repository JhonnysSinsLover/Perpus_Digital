#include "PopularBooksPage.h"
#include "BookCardWidget.h" 
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>
#include <QStyle>
#include <QTimer> // <-- PENTING: Tambahan include ini memperbaiki error kamu

PopularBooksPage::PopularBooksPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    
    // Gunakan timer agar layout width valid saat pertama kali load
    // Auto-load top books menggunakan Priority Queue (Max Heap)
    QTimer::singleShot(100, this, [this]() {
        refreshBooks();
        qDebug() << "[PopularBooksPage] Auto-loaded popular books using Max Heap";
    });
}

PopularBooksPage::~PopularBooksPage()
{
}

void PopularBooksPage::setupUI()
{
    // 1. Layout Utama
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. Header Section
    createHeaderSection(mainLayout);
    
    // 3. Scroll Area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    // Background Abu-abu kebiruan (#F4F7FE)
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #F4F7FE; border: none; }"
        "QScrollBar:vertical { background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
    );
    
    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #F4F7FE;");
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(25);
    
    // 4. Content Section
    createContentSection(contentLayout);
    
    contentLayout->addStretch();
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void PopularBooksPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(80);
    headerFrame->setStyleSheet(
        "QFrame { background-color: white; border-bottom: 1px solid #E0E5F2; }"
    );
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    
    QLabel* iconLabel = new QLabel("⭐", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent;");
    headerLayout->addWidget(iconLabel);
    
    QLabel* titleLabel = new QLabel("Buku Terpopuler", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674; margin-left: 10px;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch();
    mainLayout->addWidget(headerFrame);
}

QFrame* PopularBooksPage::createCardFrame()
{
    QFrame* card = new QFrame(this);
    card->setStyleSheet("background-color: white; border-radius: 16px; border: 1px solid #E0E5F2;");
    return card;
}

void PopularBooksPage::createContentSection(QVBoxLayout* contentLayout)
{
    // --- 1. INFO CARD (Algoritma) ---
    QFrame* infoCard = createCardFrame();
    infoCard->setStyleSheet("background-color: #FFF8E1; border-left: 5px solid #FFC107; border-radius: 8px;"); // Kuning Soft
    QVBoxLayout* infoLayout = new QVBoxLayout(infoCard);
    infoLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel* infoLabel = new QLabel(
        "💡 <b>Info Struktur Data: Priority Queue (Max Heap)</b><br>"
        "Sistem menggunakan Max Heap untuk memastikan buku dengan rating tertinggi selalu berada di posisi teratas (Root) dengan efisiensi O(n log n)."
    );
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #B00020; font-family: 'Segoe UI'; font-size: 13px;");
    infoLayout->addWidget(infoLabel);
    contentLayout->addWidget(infoCard);

    // --- 2. FILTER CONTROLS ---
    QFrame* filterCard = createCardFrame();
    QVBoxLayout* filterLayout = new QVBoxLayout(filterCard);
    filterLayout->setContentsMargins(25, 25, 25, 25);
    filterLayout->setSpacing(15);
    
    QLabel* lblFilterTitle = new QLabel("🏆 Filter Peringkat", filterCard);
    lblFilterTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    filterLayout->addWidget(lblFilterTitle);
    
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    controlsLayout->setSpacing(20);

    // Style umum untuk input
    QString inputStyle = 
        "QSpinBox, QDoubleSpinBox {"
        "   background-color: white; border: 1px solid #94A3B8; border-radius: 8px;"
        "   padding: 8px 12px; font-size: 14px; color: #2B3674;"
        "}"
        "QSpinBox:focus, QDoubleSpinBox:focus { border: 2px solid #4318FF; }";

    // Top N Input
    QLabel* lblTopN = new QLabel("Tampilkan Top:");
    lblTopN->setStyleSheet("font-weight: 600; color: #2B3674;");
    m_topNInput = new QSpinBox();
    m_topNInput->setRange(1, 100);
    m_topNInput->setValue(5);
    m_topNInput->setFixedWidth(80);
    m_topNInput->setStyleSheet(inputStyle);
    
    m_btnShowTopN = new QPushButton("Tampilkan");
    m_btnShowTopN->setCursor(Qt::PointingHandCursor);
    m_btnShowTopN->setFixedSize(100, 38);
    m_btnShowTopN->setStyleSheet("QPushButton { background-color: #4318FF; color: white; border-radius: 8px; font-weight: bold; border: none; } QPushButton:hover { background-color: #3311DB; }");

    // Min Rating Input
    QLabel* lblRating = new QLabel("Min Rating:");
    lblRating->setStyleSheet("font-weight: 600; color: #2B3674; margin-left: 20px;");
    m_minRatingInput = new QDoubleSpinBox();
    m_minRatingInput->setRange(0.0, 5.0);
    m_minRatingInput->setSingleStep(0.1);
    m_minRatingInput->setValue(4.5);
    m_minRatingInput->setFixedWidth(80);
    m_minRatingInput->setStyleSheet(inputStyle);
    
    m_btnFilterRating = new QPushButton("Filter");
    m_btnFilterRating->setCursor(Qt::PointingHandCursor);
    m_btnFilterRating->setFixedSize(100, 38);
    m_btnFilterRating->setStyleSheet("QPushButton { background-color: #FFB547; color: white; border-radius: 8px; font-weight: bold; border: none; } QPushButton:hover { background-color: #E59A2B; }");

    controlsLayout->addWidget(lblTopN);
    controlsLayout->addWidget(m_topNInput);
    controlsLayout->addWidget(m_btnShowTopN);
    controlsLayout->addWidget(lblRating);
    controlsLayout->addWidget(m_minRatingInput);
    controlsLayout->addWidget(m_btnFilterRating);
    controlsLayout->addStretch();
    
    filterLayout->addLayout(controlsLayout);
    contentLayout->addWidget(filterCard);

    // --- 3. RESULT STATUS & GRID ---
    m_resultLabel = new QLabel("Menampilkan 10 buku teratas", this);
    m_resultLabel->setStyleSheet("color: #2B3674; font-weight: 700; font-size: 16px; margin-top: 10px; margin-bottom: 5px;");
    contentLayout->addWidget(m_resultLabel);
    
    // Grid Container
    m_booksGrid = new QGridLayout();
    m_booksGrid->setSpacing(20);
    m_booksGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    contentLayout->addLayout(m_booksGrid);

    // Connect Signals
    connect(m_btnShowTopN, &QPushButton::clicked, this, &PopularBooksPage::onShowTopN);
    connect(m_btnFilterRating, &QPushButton::clicked, this, &PopularBooksPage::onFilterByRating);
}

void PopularBooksPage::refreshBooks()
{
    // Default: Show Top 10 using Priority Queue (Max Heap)
    DatabaseManager& db = DatabaseManager::instance();
    std::vector<Book> topBooks = db.getBookManager().getTopRatedBooks(10);
    
    qDebug() << "[PopularBooksPage] Loaded" << topBooks.size() << "books using Max Heap";
    m_resultLabel->setText(QString("🔥 Top %1 Buku dengan Rating Tertinggi (Max Heap)").arg(topBooks.size()));
    displayBooksAsCards(topBooks);
}

void PopularBooksPage::clearGrid()
{
    QLayoutItem* item;
    while ((item = m_booksGrid->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
}

void PopularBooksPage::displayBooksAsCards(const std::vector<Book>& books)
{
    clearGrid();

    // --- LOGIKA GRID RESPONSIVE ---
    int viewportWidth = this->width(); 
    if (viewportWidth <= 0) viewportWidth = 1000; 
    
    int minCardWidth = 220; 
    int spacing = 20;

    // Reset kolom grid
    for (int i = 0; i < m_booksGrid->columnCount(); ++i) {
        m_booksGrid->setColumnStretch(i, 0);
    }

    int numCols = (viewportWidth - 60) / (minCardWidth + spacing);
    if (numCols < 1) numCols = 1;

    // Paksa kolom terdistribusi rata
    for (int i = 0; i < numCols; ++i) {
        m_booksGrid->setColumnStretch(i, 1);
    }

    // Render Kartu
    for (int i = 0; i < books.size(); ++i) {
        BookCardWidget* card = new BookCardWidget(books[i], this);
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        
        int row = i / numCols;
        int col = i % numCols;
        
        m_booksGrid->addWidget(card, row, col);
    }
}

void PopularBooksPage::onShowTopN()
{
    int topN = m_topNInput->value();
    std::vector<Book> topBooks = DatabaseManager::instance().getBookManager().getTopRatedBooks(topN);
    
    qDebug() << "[PopularBooksPage] Showing top" << topN << "books - Retrieved:" << topBooks.size();
    m_resultLabel->setText(QString("🔥 Top %1 Buku Tertinggi (Max Heap - Priority Queue)").arg(topBooks.size()));
    displayBooksAsCards(topBooks);
}

void PopularBooksPage::onFilterByRating()
{
    double minRating = m_minRatingInput->value();
    std::vector<Book> popularBooks = DatabaseManager::instance().getBookManager().getPopularBooks(minRating);
    
    qDebug() << "[PopularBooksPage] Filtered by rating >=" << minRating << "- Found:" << popularBooks.size();
    m_resultLabel->setText(QString("⭐ Menampilkan %1 buku dengan rating ≥ %2").arg(popularBooks.size()).arg(minRating, 0, 'f', 1));
    displayBooksAsCards(popularBooks);
}