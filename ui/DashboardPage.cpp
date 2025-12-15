#include "DashboardPage.h"
#include <QGroupBox>
#include <QPushButton>
#include <QScrollArea>
#include <QHeaderView>
#include <set>
#include <map>
#include <QGraphicsDropShadowEffect> // Opsional jika ingin bayangan

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    // updateDashboard dipanggil di akhir agar UI siap dulu
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
    
    // 3. Scroll Area (Untuk konten dashboard yang panjang)
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    // Styling Scrollbar agar tidak terlihat 'kuno'
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #F4F7FE; border: none; }" // Background utama Light Blue-Grey
        "QScrollBar:vertical { border: none; background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );
    
    // 4. Content Widget (Inside Scroll Area)
    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #F4F7FE;"); // Samakan dengan scroll area
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30); // Margin lebih lega
    contentLayout->setSpacing(25); // Jarak antar section lebih lebar
    
    // --- Dashboard Content Sections ---
    
    // A. Statistics Cards
    createStatCards(contentLayout);
    
    // B. Recent Books Table
    createRecentBooksTable(contentLayout);
    
    // Spacer di bawah agar layout tidak 'stretching' paksa
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void DashboardPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setStyleSheet(
        "QFrame {"
        "   background-color: white;" // Header putih bersih
        "   border-bottom: 1px solid #E0E5F2;" // Garis pemisah tipis
        "}"
    );
    headerFrame->setFixedHeight(80);
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(20);
    
    // Title "Dashboard"
    QLabel* titleLabel = new QLabel("Dashboard", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 26px; font-weight: 700; color: #2B3674;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch(); // Spacer tengah
    
    // Search Bar
    m_searchBar = new QLineEdit(headerFrame);
    m_searchBar->setPlaceholderText("Search...");
    m_searchBar->setFixedSize(300, 45);
    m_searchBar->setStyleSheet(
        "QLineEdit {"
        "   background-color: #F4F7FE;" // Abu sangat muda
        "   border: none;"
        "   border-radius: 22px;" // Rounded pill shape
        "   padding-left: 20px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "}"
        "QLineEdit:focus { background-color: #EAEFFC; }"
    );
    headerLayout->addWidget(m_searchBar);
    
    // Notification Button
    m_notificationBtn = new QPushButton("🔔", headerFrame);
    m_notificationBtn->setFixedSize(45, 45);
    m_notificationBtn->setCursor(Qt::PointingHandCursor);
    m_notificationBtn->setStyleSheet(
        "QPushButton { border: none; background: transparent; font-size: 20px; color: #A3AED0; }"
        "QPushButton:hover { color: #2B3674; }"
    );
    headerLayout->addWidget(m_notificationBtn);
    
    // Profile Button
    m_profileBtn = new QPushButton("👤", headerFrame);
    m_profileBtn->setFixedSize(45, 45);
    m_profileBtn->setCursor(Qt::PointingHandCursor);
    m_profileBtn->setStyleSheet(
        "QPushButton { background-color: #112D4E; border: none; border-radius: 22px; font-size: 20px; color: white; }"
        "QPushButton:hover { background-color: #2B3674; }"
    );
    headerLayout->addWidget(m_profileBtn);
    
    mainLayout->addWidget(headerFrame);
}

void DashboardPage::createStatCards(QVBoxLayout* mainLayout)
{
    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(20); // Jarak antar kartu

    // Helper: Title, Value, Icon, IconBgColor, showTrend
    // Menggunakan warna background icon yang berbeda untuk visual yang menarik
    QWidget* card1 = createCompactStatCard("Total Buku", "0", "📚", "#EAEFFC", true); // Biru muda
    QWidget* card2 = createCompactStatCard("Total Penulis", "0", "✍️", "#FFF7E7", false); // Oranye muda
    QWidget* card3 = createCompactStatCard("Total Genre", "0", "🎭", "#FEEFEF", false); // Merah muda
    QWidget* card4 = createCompactStatCard("Rata-rata Rating", "0.0", "⭐", "#F0FDF4", false); // Hijau muda
    
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
    card->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border-radius: 20px;" // Sudut sangat membulat (Modern style)
        "   border: 1px solid white;" // Trick untuk antialiasing border
        "}"
    );

    QHBoxLayout* layout = new QHBoxLayout(card);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // 1. Icon Container (Kiri)
    QLabel* iconLabel = new QLabel(icon);
    iconLabel->setFixedSize(60, 60);
    iconLabel->setAlignment(Qt::AlignCenter);
    // Dynamic stylesheet untuk background icon
    iconLabel->setStyleSheet(QString(
        "background-color: %1; border-radius: 30px; font-size: 28px;"
    ).arg(iconColorBg));
    
    layout->addWidget(iconLabel);

    // 2. Text Content (Kanan)
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* lblTitle = new QLabel(title, card);
    lblTitle->setStyleSheet("color: #A3AED0; font-size: 14px; font-weight: 500; font-family: 'Segoe UI';");
    
    QLabel* lblValue = new QLabel(value, card);
    lblValue->setStyleSheet("color: #2B3674; font-size: 28px; font-weight: 700; font-family: 'Segoe UI';");
    
    textLayout->addWidget(lblTitle);
    textLayout->addWidget(lblValue);
    
    // Mapping label pointer untuk update data nanti
    if (title == "Total Buku") m_lblTotalBooks = lblValue;
    else if (title == "Total Penulis") m_lblTotalAuthors = lblValue;
    else if (title == "Total Genre") m_lblTotalGenres = lblValue;
    else if (title == "Rata-rata Rating") m_lblAvgRating = lblValue;
    
    layout->addLayout(textLayout);
    layout->addStretch();
    
    // 3. Trend (Optional)
    if (showTrend) {
        QLabel* trend = new QLabel("📈 +12%", card);
        trend->setStyleSheet("color: #05CD99; font-size: 12px; font-weight: bold; background: transparent;");
        layout->addWidget(trend, 0, Qt::AlignTop);
    }

    return card;
}







void DashboardPage::createRecentBooksTable(QVBoxLayout* mainLayout)
{
    // Container Frame untuk Tabel
    QFrame* tableFrame = new QFrame(this);
    tableFrame->setStyleSheet(
        "QFrame { background-color: white; border-radius: 20px; }"
    );
    QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);
    tableLayout->setContentsMargins(25, 25, 25, 25);
    
    // Header Text
    QLabel* tableTitle = new QLabel("Buku Terbaru Ditambahkan", tableFrame);
    tableTitle->setStyleSheet("color: #2B3674; font-size: 20px; font-weight: 700; font-family: 'Segoe UI'; margin-bottom: 10px;");
    tableLayout->addWidget(tableTitle);
    
    // Table Widget Setup
    m_recentBooksTable = new QTableWidget(this);
    m_recentBooksTable->setColumnCount(6);
    m_recentBooksTable->setHorizontalHeaderLabels({"Cover", "Judul Buku", "Penulis", "Kategori", "Tahun", "Status"});
    
    // --- TABLE STYLESHEET (BAGIAN PALING PENTING) ---
    m_recentBooksTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: none;"
        "   gridline-color: transparent;" // Hilangkan garis grid
        "}"
        "QTableWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #F4F7FE;" // Garis pemisah antar baris yang halus
        "   color: #2B3674;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #F4F7FE;" // Warna saat dipilih soft
        "   color: #2B3674;"
        "}"
        "QHeaderView::section {"
        "   background-color: white;"
        "   color: #A3AED0;" // Warna header abu-abu
        "   font-weight: bold;"
        "   font-size: 13px;"
        "   border: none;"
        "   border-bottom: 2px solid #E0E5F2;"
        "   padding: 10px;"
        "   text-align: left;"
        "}"
    );
    
    // Table Properties
    m_recentBooksTable->horizontalHeader()->setStretchLastSection(true);
    m_recentBooksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Judul stretch
    m_recentBooksTable->setColumnWidth(0, 80);  // Cover
    m_recentBooksTable->setColumnWidth(2, 180); // Penulis
    m_recentBooksTable->setColumnWidth(3, 150); // Kategori
    m_recentBooksTable->setColumnWidth(4, 80);  // Tahun
    m_recentBooksTable->setColumnWidth(5, 120); // Status
    
    m_recentBooksTable->verticalHeader()->setVisible(false);
    m_recentBooksTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_recentBooksTable->setFocusPolicy(Qt::NoFocus); // Hilangkan garis putus2 saat klik
    m_recentBooksTable->setShowGrid(false);
    m_recentBooksTable->setMinimumHeight(400); // Tinggi minimal agar tidak gepeng

    tableLayout->addWidget(m_recentBooksTable);
    mainLayout->addWidget(tableFrame);
}

void DashboardPage::updateDashboard()
{
    // Logika ini TIDAK DIUBAH, hanya visualisasi item tabelnya
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
    
    // Update Table Data
    m_recentBooksTable->setRowCount(0);
    int count = 0;
    for (auto it = books.rbegin(); it != books.rend() && count < 6; ++it, ++count) {
        int row = m_recentBooksTable->rowCount();
        m_recentBooksTable->insertRow(row);
        m_recentBooksTable->setRowHeight(row, 80); // Row lebih tinggi agar lega
        
        // 1. Cover (Placeholder Modern)
        QLabel* coverLabel = new QLabel("📘");
        coverLabel->setAlignment(Qt::AlignCenter);
        coverLabel->setStyleSheet("font-size: 24px; background-color: #F4F7FE; border-radius: 8px;");
        m_recentBooksTable->setCellWidget(row, 0, coverLabel);
        
        // 2. Title
        QTableWidgetItem* titleItem = new QTableWidgetItem(it->getJudul());
        titleItem->setFont(QFont("Segoe UI", 11, QFont::Bold));
        m_recentBooksTable->setItem(row, 1, titleItem);
        
        // 3. Author
        QTableWidgetItem* authorItem = new QTableWidgetItem(it->getPenulis());
        authorItem->setFont(QFont("Segoe UI", 10));
        m_recentBooksTable->setItem(row, 2, authorItem);
        
        // 4. Genre
        QTableWidgetItem* genreItem = new QTableWidgetItem(it->getGenre().join(", "));
        genreItem->setFont(QFont("Segoe UI", 10));
        m_recentBooksTable->setItem(row, 3, genreItem);
        
        // 5. Year
        QTableWidgetItem* yearItem = new QTableWidgetItem(QString::number(it->getTahun()));
        yearItem->setTextAlignment(Qt::AlignCenter);
        m_recentBooksTable->setItem(row, 4, yearItem);
        
        // 6. Status (Pill Badge Style)
        QWidget* statusWidget = new QWidget();
        QHBoxLayout* statusLayout = new QHBoxLayout(statusWidget);
        statusLayout->setContentsMargins(0,0,0,0);
        statusLayout->setAlignment(Qt::AlignCenter);
        
        QLabel* statusLabel = new QLabel("Tersedia");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setFixedSize(90, 30);
        // Style badge hijau soft
        statusLabel->setStyleSheet(
            "background-color: #05CD99; color: white; "
            "border-radius: 15px; font-weight: bold; font-size: 11px;"
        );
        statusLayout->addWidget(statusLabel);
        
        m_recentBooksTable->setCellWidget(row, 5, statusWidget);
    }
}

void DashboardPage::applyStyles()
{
    // Fungsi ini dikosongkan karena styles sudah diterapkan inline per komponen
    // untuk kontrol yang lebih granular.
}