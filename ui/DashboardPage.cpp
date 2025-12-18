#include "DashboardPage.h"
#include <QGroupBox>
#include <QPushButton>
#include <QScrollArea>
#include <QHeaderView>
#include <set>
#include <map>
#include <QGraphicsDropShadowEffect> 

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    // Update data di akhir agar UI siap dulu
    updateDashboard(); 
}

DashboardPage::~DashboardPage()
{
}

void DashboardPage::setupUI()
{
    // 1. Main Layout Setup
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. Header Section (Fixed Top)
    createHeaderSection(mainLayout);
    
    // 3. Scroll Area 
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    // Styling Scrollbar agar menyatu dengan background
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #F4F7FE; border: none; }" 
        "QScrollBar:vertical { border: none; background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );
    
    // 4. Content Widget
    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #F4F7FE;"); 
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30); // Margin lega
    contentLayout->setSpacing(30); // Jarak antar section
    
    // --- Dashboard Content Sections ---
    
    // A. Statistics Cards
    createStatCards(contentLayout);
    
    // B. Recent Books Table
    createRecentBooksTable(contentLayout);
    
    // Spacer di bawah
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void DashboardPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setStyleSheet(
        "QFrame {"
        "   background-color: white;" 
        "   border-bottom: 1px solid #E0E5F2;" 
        "}"
    );
    headerFrame->setFixedHeight(80); // Tinggi header disamakan
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(20);
    
    // Title
    QLabel* titleLabel = new QLabel("Dashboard", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 26px; font-weight: 700; color: #2B3674; border: none;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch(); 
    
    // Search Bar (Visual Only for Dashboard)
    m_searchBar = new QLineEdit(headerFrame);
    m_searchBar->setPlaceholderText("Search...");
    m_searchBar->setFixedSize(300, 45);
    m_searchBar->setStyleSheet(
        "QLineEdit {"
        "   background-color: #F4F7FE;"
        "   border: none;"
        "   border-radius: 22px;" 
        "   padding-left: 20px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "}"
        "QLineEdit:focus { background-color: #EAEFFC; border: 1px solid #4318FF; }"
    );
    headerLayout->addWidget(m_searchBar);
    
    // Notification Button
    m_notificationBtn = new QPushButton("🔔", headerFrame);
    m_notificationBtn->setFixedSize(45, 45);
    m_notificationBtn->setCursor(Qt::PointingHandCursor);
    m_notificationBtn->setStyleSheet(
        "QPushButton { border: none; background: transparent; font-size: 20px; color: #A3AED0; }"
        "QPushButton:hover { color: #4318FF; background-color: #F4F7FE; border-radius: 22px; }"
    );
    headerLayout->addWidget(m_notificationBtn);
    
    // Profile Button
    m_profileBtn = new QPushButton("Admin", headerFrame);
    m_profileBtn->setFixedSize(80, 34);
    m_profileBtn->setStyleSheet(
        "QPushButton { background-color: #1E293B; color: white; border-radius: 17px; font-weight: 600; border: none; }"
    );
    headerLayout->addWidget(m_profileBtn);
    
    mainLayout->addWidget(headerFrame);
}

void DashboardPage::createStatCards(QVBoxLayout* mainLayout)
{
    // Menggunakan Layout Horizontal dengan logika Stretch agar memenuhi lebar layar
    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(20); 

    // Helper: Title, Value, Icon, IconBgColor, showTrend
    QWidget* card1 = createCompactStatCard("Total Buku", "0", "📚", "#EAEFFC", true); // Biru muda
    QWidget* card2 = createCompactStatCard("Total Penulis", "0", "✍️", "#FFF7E7", false); // Oranye muda
    QWidget* card3 = createCompactStatCard("Total Genre", "0", "🎭", "#FEEFEF", false); // Merah muda
    QWidget* card4 = createCompactStatCard("Rata-rata Rating", "0.0", "⭐", "#F0FDF4", false); // Hijau muda
    
    // Tambahkan ke layout
    statsLayout->addWidget(card1);
    statsLayout->addWidget(card2);
    statsLayout->addWidget(card3);
    statsLayout->addWidget(card4);

    mainLayout->addLayout(statsLayout);
}

QWidget* DashboardPage::createCompactStatCard(const QString& title, const QString& value, 
                                               const QString& icon, const QString& iconColorBg, bool showTrend)
{
    QFrame* card = new QFrame(this);
    card->setFixedHeight(120);
    // STRETCH LOGIC: Agar kartu melebar mengisi ruang kosong
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    card->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border-radius: 16px;" // Radius disamakan dengan style buku
        "   border: 1px solid #E0E5F2;" // Border halus
        "}"
    );

    QHBoxLayout* layout = new QHBoxLayout(card);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // 1. Icon Container (Kiri)
    QLabel* iconLabel = new QLabel(icon);
    iconLabel->setFixedSize(56, 56);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet(QString(
        "background-color: %1; border-radius: 28px; font-size: 24px; border: none;"
    ).arg(iconColorBg));
    
    layout->addWidget(iconLabel);

    // 2. Text Content (Kanan)
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(4);
    textLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* lblTitle = new QLabel(title, card);
    lblTitle->setStyleSheet("color: #A3AED0; font-size: 13px; font-weight: 600; font-family: 'Segoe UI'; border: none;");
    
    QLabel* lblValue = new QLabel(value, card);
    lblValue->setStyleSheet("color: #2B3674; font-size: 26px; font-weight: 700; font-family: 'Segoe UI'; border: none;");
    
    textLayout->addWidget(lblTitle);
    textLayout->addWidget(lblValue);
    
    // Mapping label pointer untuk update data nanti
    if (title == "Total Buku") m_lblTotalBooks = lblValue;
    else if (title == "Total Penulis") m_lblTotalAuthors = lblValue;
    else if (title == "Total Genre") m_lblTotalGenres = lblValue;
    else if (title == "Rata-rata Rating") m_lblAvgRating = lblValue;
    
    layout->addLayout(textLayout);
    layout->addStretch(); // Push content ke kiri
    
    // 3. Trend (Optional - Visual Only)
    if (showTrend) {
        QLabel* trend = new QLabel("📈 +2", card);
        trend->setStyleSheet("color: #05CD99; font-size: 11px; font-weight: bold; background: transparent; border: none;");
        layout->addWidget(trend, 0, Qt::AlignTop | Qt::AlignRight);
    }

    return card;
}

void DashboardPage::createRecentBooksTable(QVBoxLayout* mainLayout)
{
    // Container Frame untuk Tabel
    QFrame* tableFrame = new QFrame(this);
    tableFrame->setStyleSheet(
        "QFrame { background-color: white; border-radius: 16px; border: 1px solid #E0E5F2; }"
    );
    QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);
    tableLayout->setContentsMargins(25, 25, 25, 25);
    
    // Header Text & Action
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* tableTitle = new QLabel("🏆 Top 5 Buku Rating Tertinggi (Max Heap)", tableFrame);
    tableTitle->setStyleSheet("color: #2B3674; font-size: 18px; font-weight: 700; font-family: 'Segoe UI'; border: none;");
    
    QPushButton* viewAllBtn = new QPushButton("Lihat Semua", tableFrame);
    viewAllBtn->setCursor(Qt::PointingHandCursor);
    viewAllBtn->setStyleSheet("QPushButton { color: #4318FF; font-weight: 600; border: none; background: transparent; } QPushButton:hover { text-decoration: underline; }");
    
    titleLayout->addWidget(tableTitle);
    titleLayout->addStretch();
    titleLayout->addWidget(viewAllBtn);
    
    tableLayout->addLayout(titleLayout);
    tableLayout->addSpacing(10);
    
    // Table Widget Setup
    m_recentBooksTable = new QTableWidget(this);
    m_recentBooksTable->setColumnCount(6);
    m_recentBooksTable->setHorizontalHeaderLabels({"Cover", "Judul Buku", "Penulis", "Kategori", "Tahun", "Rating"});
    
    // --- TABLE STYLESHEET ---
    m_recentBooksTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: none;"
        "   gridline-color: transparent;"
        "}"
        "QTableWidget::item {"
        "   padding-left: 10px; padding-right: 10px;"
        "   border-bottom: 1px solid #F4F7FE;"
        "   color: #2B3674;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #F4F7FE;"
        "   color: #4318FF;"
        "}"
        "QHeaderView::section {"
        "   background-color: white;"
        "   color: #A3AED0;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "   text-transform: uppercase;"
        "   border: none;"
        "   border-bottom: 1px solid #E0E5F2;"
        "   padding: 15px;"
        "   text-align: left;"
        "}"
    );
    
    // COLUMN RESIZING: Agar tabel Full Width dan Rapi
    QHeaderView* header = m_recentBooksTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch); // Default semua stretch
    header->setSectionResizeMode(0, QHeaderView::Fixed); header->resizeSection(0, 80); // Cover fixed
    header->setSectionResizeMode(4, QHeaderView::Fixed); header->resizeSection(4, 80); // Tahun fixed
    header->setSectionResizeMode(5, QHeaderView::Fixed); header->resizeSection(5, 120); // Status fixed
    // Judul, Penulis, Kategori akan berbagi sisa ruang
    
    m_recentBooksTable->verticalHeader()->setVisible(false);
    m_recentBooksTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_recentBooksTable->setFocusPolicy(Qt::NoFocus);
    m_recentBooksTable->setShowGrid(false);
    m_recentBooksTable->setMinimumHeight(450); 

    tableLayout->addWidget(m_recentBooksTable);
    mainLayout->addWidget(tableFrame);
}

void DashboardPage::updateDashboard()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> books = dbManager.getAllBooks();
    
    // Calculate Stats
    int totalBooks = books.size();
    std::set<QString> authors;
    std::set<QString> genres;
    double totalRating = 0.0;
    
    for (const Book& book : books) {
        authors.insert(book.getPenulis());
        for (const QString& genre : book.getGenre()) {
            genres.insert(genre.trimmed());
        }
        totalRating += book.getRating();
    }
    double avgRating = totalBooks > 0 ? totalRating / totalBooks : 0.0;
    
    // Update Labels
    m_lblTotalBooks->setText(QString::number(totalBooks));
    m_lblTotalAuthors->setText(QString::number(authors.size()));
    m_lblTotalGenres->setText(QString::number(genres.size()));
    m_lblAvgRating->setText(QString::number(avgRating, 'f', 1));
    
    // Update Table Data - MENGGUNAKAN PRIORITY QUEUE (MAX HEAP) UNTUK TOP 5 BOOKS!
    m_recentBooksTable->setRowCount(0);
    
    // Ambil top 5 buku rating tertinggi menggunakan Priority Queue (Max Heap)
    std::vector<Book> topBooks = dbManager.getBookManager().getTopRatedBooks(5);
    
    int count = 0;
    for (const Book& book : topBooks) {
        int row = m_recentBooksTable->rowCount();
        m_recentBooksTable->insertRow(row);
        m_recentBooksTable->setRowHeight(row, 70); // Tinggi baris nyaman
        
        // 1. Cover (Placeholder Modern dengan inisial)
        QLabel* coverLabel = new QLabel("📘");
        coverLabel->setAlignment(Qt::AlignCenter);
        coverLabel->setStyleSheet("font-size: 20px; background-color: #F4F7FE; border-radius: 8px; margin: 5px;");
        m_recentBooksTable->setCellWidget(row, 0, coverLabel);
        
        // 2. Title
        QTableWidgetItem* titleItem = new QTableWidgetItem(book.getJudul());
        titleItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        m_recentBooksTable->setItem(row, 1, titleItem);
        
        // 3. Author
        QTableWidgetItem* authorItem = new QTableWidgetItem(book.getPenulis());
        authorItem->setFont(QFont("Segoe UI", 10));
        m_recentBooksTable->setItem(row, 2, authorItem);
        
        // 4. Genre
        QTableWidgetItem* genreItem = new QTableWidgetItem(book.getGenre().join(", "));
        genreItem->setFont(QFont("Segoe UI", 10));
        m_recentBooksTable->setItem(row, 3, genreItem);
        
        // 5. Year
        QTableWidgetItem* yearItem = new QTableWidgetItem(QString::number(book.getTahun()));
        yearItem->setTextAlignment(Qt::AlignCenter);
        m_recentBooksTable->setItem(row, 4, yearItem);
        
        // 6. Status (Pill Badge Style) dengan rating
        QWidget* statusWidget = new QWidget();
        QHBoxLayout* statusLayout = new QHBoxLayout(statusWidget);
        statusLayout->setContentsMargins(0,0,0,0);
        statusLayout->setAlignment(Qt::AlignCenter);
        
        QLabel* statusLabel = new QLabel(QString("⭐ %1").arg(book.getRating()));
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setFixedSize(90, 28);
        statusLabel->setStyleSheet(
            "background-color: #FFB547; color: white; "
            "border-radius: 14px; font-weight: bold; font-size: 11px;"
        );
        statusLayout->addWidget(statusLabel);
        
        m_recentBooksTable->setCellWidget(row, 5, statusWidget);
        
        count++;
    }
}