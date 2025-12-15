#include "PopularBooksPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>
#include <QStyle>

PopularBooksPage::PopularBooksPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    refreshBooks();
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
    card->setStyleSheet("background-color: white; border-radius: 16px;");
    
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 15));
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);
    
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

    // --- 2. FILTER PANEL ---
    QFrame* filterCard = createCardFrame();
    QVBoxLayout* filterLayout = new QVBoxLayout(filterCard);
    filterLayout->setContentsMargins(25, 25, 25, 25);
    filterLayout->setSpacing(15);
    
    QLabel* lblFilterTitle = new QLabel("Filter & Pengaturan", filterCard);
    lblFilterTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    filterLayout->addWidget(lblFilterTitle);
    
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    controlsLayout->setSpacing(30);

    // --- STYLING SPINBOX (FIX PANAH) ---
    QString spinBoxStyle = 
        "QSpinBox, QDoubleSpinBox {"
        "   background-color: white;"
        "   border: 1px solid #94A3B8;"
        "   border-radius: 8px;"
        "   padding: 8px 12px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "   font-family: 'Segoe UI';"
        "}"
        "QSpinBox:focus, QDoubleSpinBox:focus { border: 2px solid #4318FF; }"
        // Style Tombol Up/Down
        "QSpinBox::up-button, QDoubleSpinBox::up-button {"
        "   subcontrol-origin: border; subcontrol-position: top right; width: 24px;"
        "   border-left: 1px solid #94A3B8; border-bottom: 1px solid #94A3B8;"
        "   background-color: #F1F5F9; border-top-right-radius: 8px;"
        "}"
        "QSpinBox::down-button, QDoubleSpinBox::down-button {"
        "   subcontrol-origin: border; subcontrol-position: bottom right; width: 24px;"
        "   border-left: 1px solid #94A3B8; background-color: #F1F5F9; border-bottom-right-radius: 8px;"
        "}"
        "QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover, QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {"
        "   background-color: #E2E8F0;"
        "}"
        // Gambar Panah Manual (CSS Border Trick)
        "QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {"
        "   width: 0px; height: 0px; border-left: 4px solid transparent; border-right: 4px solid transparent; border-bottom: 5px solid #475569;"
        "}"
        "QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {"
        "   width: 0px; height: 0px; border-left: 4px solid transparent; border-right: 4px solid transparent; border-top: 5px solid #475569;"
        "}";

    // Grup 1: Top N
    QWidget* groupTopN = new QWidget();
    QVBoxLayout* layTopN = new QVBoxLayout(groupTopN);
    layTopN->setContentsMargins(0,0,0,0); layTopN->setSpacing(5);
    QLabel* lblTopN = new QLabel("Tampilkan Top N:");
    lblTopN->setStyleSheet("font-weight: bold; color: #2B3674;");
    
    m_topNInput = new QSpinBox();
    m_topNInput->setRange(1, 100);
    m_topNInput->setValue(5);
    m_topNInput->setStyleSheet(spinBoxStyle);
    
    m_btnShowTopN = new QPushButton("Tampilkan");
    m_btnShowTopN->setCursor(Qt::PointingHandCursor);
    m_btnShowTopN->setStyleSheet("QPushButton { background-color: #4318FF; color: white; border-radius: 8px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #3311DB; }");
    
    layTopN->addWidget(lblTopN);
    layTopN->addWidget(m_topNInput);
    layTopN->addWidget(m_btnShowTopN);
    
    // Grup 2: Min Rating
    QWidget* groupRating = new QWidget();
    QVBoxLayout* layRating = new QVBoxLayout(groupRating);
    layRating->setContentsMargins(0,0,0,0); layRating->setSpacing(5);
    QLabel* lblRating = new QLabel("Filter Rating Minimal:");
    lblRating->setStyleSheet("font-weight: bold; color: #2B3674;");
    
    m_minRatingInput = new QDoubleSpinBox();
    m_minRatingInput->setRange(0.0, 5.0);
    m_minRatingInput->setSingleStep(0.1);
    m_minRatingInput->setValue(4.5);
    m_minRatingInput->setStyleSheet(spinBoxStyle);
    
    m_btnFilterRating = new QPushButton("Filter Rating");
    m_btnFilterRating->setCursor(Qt::PointingHandCursor);
    m_btnFilterRating->setStyleSheet("QPushButton { background-color: #FFB547; color: white; border-radius: 8px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #E59A2B; }");
    
    layRating->addWidget(lblRating);
    layRating->addWidget(m_minRatingInput);
    layRating->addWidget(m_btnFilterRating);
    
    controlsLayout->addWidget(groupTopN);
    controlsLayout->addWidget(groupRating);
    controlsLayout->addStretch();
    
    filterLayout->addLayout(controlsLayout);
    
    // Result Status Label
    m_resultLabel = new QLabel("Menampilkan 10 buku teratas", filterCard);
    m_resultLabel->setStyleSheet("color: #A3AED0; font-style: italic; font-size: 13px; margin-top: 10px;");
    filterLayout->addWidget(m_resultLabel);
    
    contentLayout->addWidget(filterCard);

    // --- 3. TABLE CARD ---
    QFrame* tableCard = createCardFrame();
    QVBoxLayout* tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(25, 25, 25, 25);
    
    QLabel* lblTableTitle = new QLabel("Daftar Peringkat Buku", tableCard);
    lblTableTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674; margin-bottom: 10px;");
    tableLayout->addWidget(lblTableTitle);
    
    m_tableBooks = new QTableWidget();
    m_tableBooks->setColumnCount(6);
    m_tableBooks->setHorizontalHeaderLabels({"Rank", "Judul Buku", "Penulis", "Genre", "Tahun", "Rating"});
    
    // Modern Table Style
    m_tableBooks->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: 1px solid #E0E5F2;"
        "   border-radius: 8px;"
        "   gridline-color: transparent;"
        "   font-family: 'Segoe UI';"
        "}"
        "QTableWidget::item {"
        "   padding: 12px;"
        "   border-bottom: 1px solid #F4F7FE;"
        "   color: #2B3674;"
        "   font-size: 13px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #EAEFFC;"
        "   color: #4318FF;"
        "}"
        "QHeaderView::section {"
        "   background-color: #F8F9FC;"
        "   color: #A3AED0;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-bottom: 2px solid #E0E5F2;"
        "   padding: 12px 10px;"
        "   text-align: left;"
        "}"
    );
    
    m_tableBooks->verticalHeader()->setVisible(false);
    m_tableBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableBooks->setFocusPolicy(Qt::NoFocus);
    m_tableBooks->setShowGrid(false);
    m_tableBooks->horizontalHeader()->setStretchLastSection(true);
    m_tableBooks->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Judul stretch
    
    tableLayout->addWidget(m_tableBooks);
    contentLayout->addWidget(tableCard, 1); // Expand table

    // Connect Signals
    connect(m_btnShowTopN, &QPushButton::clicked, this, &PopularBooksPage::onShowTopN);
    connect(m_btnFilterRating, &QPushButton::clicked, this, &PopularBooksPage::onFilterByRating);
}

void PopularBooksPage::refreshBooks()
{
    // Default show top 10
    DatabaseManager& db = DatabaseManager::instance();
    std::vector<Book> topBooks = db.getBookManager().getTopRatedBooks(10);
    loadBooksToTable(topBooks);
    m_resultLabel->setText("Menampilkan Top 10 buku dengan rating tertinggi");
}

void PopularBooksPage::loadBooksToTable(const std::vector<Book>& books)
{
    m_tableBooks->setRowCount(books.size());
    
    for (size_t i = 0; i < books.size(); i++) {
        const Book& book = books[i];
        
        // 1. Rank (dengan pewarnaan Juara)
        QTableWidgetItem* rankItem = new QTableWidgetItem(QString("#%1").arg(i + 1));
        rankItem->setTextAlignment(Qt::AlignCenter);
        rankItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        
        if (i == 0) { // Gold
            rankItem->setBackground(QColor("#FFF9C4")); 
            rankItem->setForeground(QBrush(QColor("#FBC02D")));
        } else if (i == 1) { // Silver
            rankItem->setBackground(QColor("#F5F5F5"));
            rankItem->setForeground(QBrush(QColor("#757575")));
        } else if (i == 2) { // Bronze
            rankItem->setBackground(QColor("#FBE9E7"));
            rankItem->setForeground(QBrush(QColor("#D84315")));
        }
        
        m_tableBooks->setItem(i, 0, rankItem);
        
        // 2. Judul
        QTableWidgetItem* titleItem = new QTableWidgetItem(book.getJudul());
        titleItem->setFont(QFont("Segoe UI", 10, QFont::DemiBold));
        m_tableBooks->setItem(i, 1, titleItem);
        
        // 3. Penulis
        m_tableBooks->setItem(i, 2, new QTableWidgetItem(book.getPenulis()));
        
        // 4. Genre
        m_tableBooks->setItem(i, 3, new QTableWidgetItem(book.getGenre().join(", ")));
        
        // 5. Tahun
        QTableWidgetItem* yearItem = new QTableWidgetItem(QString::number(book.getTahun()));
        yearItem->setTextAlignment(Qt::AlignCenter);
        m_tableBooks->setItem(i, 4, yearItem);
        
        // 6. Rating (Bold Orange)
        QTableWidgetItem* ratingItem = new QTableWidgetItem(QString("⭐ %1").arg(book.getRating(), 0, 'f', 1));
        ratingItem->setTextAlignment(Qt::AlignCenter);
        ratingItem->setForeground(QBrush(QColor("#FFB547")));
        ratingItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        m_tableBooks->setItem(i, 5, ratingItem);
    }
}

void PopularBooksPage::onShowTopN()
{
    int topN = m_topNInput->value();
    std::vector<Book> topBooks = DatabaseManager::instance().getBookManager().getTopRatedBooks(topN);
    loadBooksToTable(topBooks);
    m_resultLabel->setText(QString("Menampilkan Top %1 buku tertinggi (Max Heap)").arg(topN));
}

void PopularBooksPage::onFilterByRating()
{
    double minRating = m_minRatingInput->value();
    std::vector<Book> popularBooks = DatabaseManager::instance().getBookManager().getPopularBooks(minRating);
    loadBooksToTable(popularBooks);
    m_resultLabel->setText(QString("Menampilkan %1 buku dengan rating ≥ %2").arg(popularBooks.size()).arg(minRating, 0, 'f', 1));
}

// Helpers
QWidget* PopularBooksPage::createInputGroup(const QString& labelText, QWidget* inputWidget)
{
    QWidget* group = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(group);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(5);
    QLabel* lbl = new QLabel(labelText);
    lbl->setStyleSheet("font-weight: bold; color: #2B3674; font-size: 13px;");
    layout->addWidget(lbl);
    layout->addWidget(inputWidget);
    return group;
}