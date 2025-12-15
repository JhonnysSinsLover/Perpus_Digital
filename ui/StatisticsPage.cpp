#include "StatisticsPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QScrollArea>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <set>
#include <map>
#include <algorithm> // Untuk std::sort

StatisticsPage::StatisticsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    // updateStatistics dipanggil di akhir konstruktor
    updateStatistics();
}

StatisticsPage::~StatisticsPage()
{
}

void StatisticsPage::setupUI()
{
    // 1. Layout Utama
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. Header Section
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(80);
    headerFrame->setStyleSheet(
        "QFrame { background-color: white; border-bottom: 1px solid #E0E5F2; }"
    );
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    
    QLabel* titleLabel = new QLabel("📊 Statistik Perpustakaan", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    mainLayout->addWidget(headerFrame);
    
    // 3. Scroll Area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    // Style Background Modern
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

    // --- A. SUMMARY CARDS (Top Row) ---
    QHBoxLayout* summaryRow = new QHBoxLayout();
    summaryRow->setSpacing(20);
    
    // Simpan pointer ke Frame agar bisa diupdate labelnya nanti (via m_lbl...)
    // Total books
    QFrame* totalCard = createStatCard("📚", "Total Buku", "0", "#4318FF"); // Blue
    summaryRow->addWidget(totalCard);
    
    // Total genres
    QFrame* genreCountCard = createStatCard("🎭", "Total Genre", "0", "#FFB547"); // Orange
    summaryRow->addWidget(genreCountCard);
    
    // Avg rating
    QFrame* avgRatingCard = createStatCard("⭐", "Rata-rata Rating", "0.0", "#05CD99"); // Green
    summaryRow->addWidget(avgRatingCard);
    
    contentLayout->addLayout(summaryRow);

    // --- B. CHARTS ROW 1 (Genre & Rating) ---
    QHBoxLayout* chartsRow1 = new QHBoxLayout();
    chartsRow1->setSpacing(20);

    // 1. Genre Distribution Card
    QFrame* genreCard = createCardFrame();
    QVBoxLayout* genreLayout = new QVBoxLayout(genreCard);
    genreLayout->setContentsMargins(25, 25, 25, 25);
    genreLayout->setSpacing(15);
    
    QLabel* genreTitle = new QLabel("Distribusi Genre Populer", genreCard);
    genreTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    genreLayout->addWidget(genreTitle);
    
    m_genreChartFrame = new QFrame(genreCard);
    // Style text html di dalam frame ini
    m_genreChartFrame->setStyleSheet("QLabel { font-family: 'Segoe UI'; color: #2B3674; }");
    genreLayout->addWidget(m_genreChartFrame);
    genreLayout->addStretch();
    
    chartsRow1->addWidget(genreCard, 1); // Stretch factor 1

    // 2. Rating Distribution Card
    QFrame* ratingCard = createCardFrame();
    QVBoxLayout* ratingLayout = new QVBoxLayout(ratingCard);
    ratingLayout->setContentsMargins(25, 25, 25, 25);
    ratingLayout->setSpacing(15);
    
    QLabel* ratingTitle = new QLabel("Distribusi Rating", ratingCard);
    ratingTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    ratingLayout->addWidget(ratingTitle);
    
    m_ratingChartFrame = new QFrame(ratingCard);
    m_ratingChartFrame->setStyleSheet("QLabel { font-family: 'Segoe UI'; color: #2B3674; }");
    ratingLayout->addWidget(m_ratingChartFrame);
    ratingLayout->addStretch();
    
    chartsRow1->addWidget(ratingCard, 1);

    contentLayout->addLayout(chartsRow1);

    // --- C. CHARTS ROW 2 (Year Distribution - Full Width) ---
    QFrame* yearCard = createCardFrame();
    QVBoxLayout* yearLayout = new QVBoxLayout(yearCard);
    yearLayout->setContentsMargins(25, 25, 25, 25);
    yearLayout->setSpacing(15);
    
    QLabel* yearTitle = new QLabel("Distribusi Tahun Terbit", yearCard);
    yearTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    yearLayout->addWidget(yearTitle);
    
    m_yearChartFrame = new QFrame(yearCard);
    m_yearChartFrame->setStyleSheet("QLabel { font-family: 'Segoe UI'; color: #2B3674; }");
    yearLayout->addWidget(m_yearChartFrame);
    
    contentLayout->addWidget(yearCard);

    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

// Helper: Membuat Container Kartu Putih dengan Shadow
QFrame* StatisticsPage::createCardFrame()
{
    QFrame* card = new QFrame(this);
    card->setStyleSheet(
        "QFrame {"
        "   background-color: white;"
        "   border-radius: 20px;"
        "}"
    );
    
    // Drop Shadow Effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 15)); // Soft shadow
    shadow->setOffset(0, 5);
    card->setGraphicsEffect(shadow);
    
    return card;
}

QFrame* StatisticsPage::createStatCard(const QString& icon, const QString& title, 
                                        const QString& value, const QString& color)
{
    // Gunakan helper frame untuk base style
    QFrame* card = createCardFrame();
    card->setFixedHeight(120);
    
    QHBoxLayout* layout = new QHBoxLayout(card);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(20);
    
    // Icon Container
    QLabel* iconLabel = new QLabel(icon, card);
    iconLabel->setFixedSize(60, 60);
    iconLabel->setAlignment(Qt::AlignCenter);
    // Background icon bulat dengan warna
    iconLabel->setStyleSheet(QString(
        "background-color: %1; border-radius: 30px; font-size: 28px; color: white;"
    ).arg(color));
    
    layout->addWidget(iconLabel);
    
    // Text Content
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 14px; color: #A3AED0; font-weight: 500;");
    textLayout->addWidget(titleLabel);
    
    QLabel* valueLabel = new QLabel(value, card);
    valueLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 28px; font-weight: 700; color: #2B3674;");
    textLayout->addWidget(valueLabel);
    
    // Simpan pointer label untuk update nanti
    if (title == "Total Buku") m_lblTotalBooks = valueLabel;
    else if (title == "Total Genre") m_lblTotalGenres = valueLabel;
    else if (title == "Rata-rata Rating") m_lblAvgRating = valueLabel;
    
    layout->addLayout(textLayout);
    layout->addStretch();
    
    return card;
}

void StatisticsPage::updateStatistics()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> books = dbManager.getAllBooks();
    
    // --- 1. Calculate Data (LOGIKA TIDAK DIUBAH) ---
    std::map<QString, int> genreCounts;
    std::map<int, int> ratingCounts; 
    std::map<int, int> yearCounts;
    
    double totalRatingSum = 0;
    
    for (const Book& book : books) {
        for (const QString& genre : book.getGenre()) {
            genreCounts[genre.trimmed()]++;
        }
        
        int ratingGroup = static_cast<int>(book.getRating());
        ratingCounts[ratingGroup]++;
        
        int decade = (book.getTahun() / 10) * 10;
        yearCounts[decade]++;
        
        totalRatingSum += book.getRating();
    }
    
    // Update Summary Cards
    if (m_lblTotalBooks) m_lblTotalBooks->setText(QString::number(books.size()));
    if (m_lblTotalGenres) m_lblTotalGenres->setText(QString::number(genreCounts.size()));
    
    double avg = books.empty() ? 0.0 : (totalRatingSum / books.size());
    if (m_lblAvgRating) m_lblAvgRating->setText(QString::number(avg, 'f', 1));
    
    // --- 2. Update Charts (VISUAL HTML DIPERBAIKI) ---
    
    // A. Genre HTML
    // Menggunakan Table HTML sederhana untuk layout yang lebih rapi
    QString genreHtml = "<html><body style='font-family: Segoe UI; color: #2B3674;'>";
    genreHtml += "<table width='100%' cellspacing='0' cellpadding='4'>";
    
    std::vector<std::pair<QString, int>> genreVec(genreCounts.begin(), genreCounts.end());
    std::sort(genreVec.begin(), genreVec.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    int count = 0;
    for (const auto& [genre, cnt] : genreVec) {
        if (count++ >= 8) break; // Limit 8 items
        int percentage = books.empty() ? 0 : (cnt * 100 / books.size());
        
        // Bar visual sederhana menggunakan karakter block █ atau warna background
        // Di sini kita gunakan teks rapi + persentase bold
        genreHtml += QString(
            "<tr>"
            "  <td width='60%' style='font-size: 14px;'>%1</td>"
            "  <td width='20%' style='font-size: 14px; font-weight: bold; color: #4318FF;'>%2%</td>"
            "  <td width='20%' style='font-size: 13px; color: #A3AED0; text-align: right;'>%3 buku</td>"
            "</tr>"
        ).arg(genre).arg(percentage).arg(cnt);
    }
    genreHtml += "</table></body></html>";
    
    // Update UI Genre
    if (m_genreChartFrame->layout()) delete m_genreChartFrame->layout();
    QVBoxLayout* genreLayout = new QVBoxLayout(m_genreChartFrame);
    genreLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* genreLabel = new QLabel(genreHtml, m_genreChartFrame);
    genreLabel->setTextFormat(Qt::RichText);
    genreLayout->addWidget(genreLabel);

    // B. Rating HTML
    QString ratingHtml = "<html><body style='font-family: Segoe UI; color: #2B3674;'>";
    ratingHtml += "<table width='100%' cellspacing='0' cellpadding='4'>";
    
    for (int i = 5; i >= 1; i--) {
        int cnt = ratingCounts[i];
        int percentage = books.empty() ? 0 : (cnt * 100 / books.size());
        QString stars = QString("⭐").repeated(i);
        
        ratingHtml += QString(
            "<tr>"
            "  <td width='40%' style='font-size: 12px;'>%1</td>"
            "  <td width='30%' style='font-size: 14px; font-weight: bold; color: #05CD99;'>%2%</td>"
            "  <td width='30%' style='font-size: 13px; color: #A3AED0; text-align: right;'>%3 buku</td>"
            "</tr>"
        ).arg(stars).arg(percentage).arg(cnt);
    }
    ratingHtml += "</table></body></html>";
    
    // Update UI Rating
    if (m_ratingChartFrame->layout()) delete m_ratingChartFrame->layout();
    QVBoxLayout* ratingFrameLayout = new QVBoxLayout(m_ratingChartFrame);
    ratingFrameLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* ratingLabel = new QLabel(ratingHtml, m_ratingChartFrame);
    ratingLabel->setTextFormat(Qt::RichText);
    ratingFrameLayout->addWidget(ratingLabel);

    // C. Year HTML
    QString yearHtml = "<html><body style='font-family: Segoe UI; color: #2B3674;'>";
    yearHtml += "<table width='100%' cellspacing='0' cellpadding='5'>";
    
    std::vector<std::pair<int, int>> yearVec(yearCounts.begin(), yearCounts.end());
    std::sort(yearVec.begin(), yearVec.end(), 
              [](const auto& a, const auto& b) { return a.first > b.first; });
    
    // Tampilkan Grid Horizontal
    yearHtml += "<tr>";
    int colCount = 0;
    for (const auto& [decade, cnt] : yearVec) {
        if (colCount >= 5) { yearHtml += "</tr><tr>"; colCount = 0; }
        
        yearHtml += QString(
            "<td style='background-color: #F4F7FE; border-radius: 8px; padding: 10px; text-align: center;'>"
            "  <div style='font-size: 16px; font-weight: bold; color: #4318FF;'>%1an</div>"
            "  <div style='font-size: 12px; color: #A3AED0; margin-top: 5px;'>%2 Buku</div>"
            "</td>"
        ).arg(decade).arg(cnt);
        colCount++;
    }
    yearHtml += "</tr></table></body></html>";
    
    // Update UI Year
    if (m_yearChartFrame->layout()) delete m_yearChartFrame->layout();
    QVBoxLayout* yearFrameLayout = new QVBoxLayout(m_yearChartFrame);
    yearFrameLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* yearLabel = new QLabel(yearHtml, m_yearChartFrame);
    yearLabel->setTextFormat(Qt::RichText);
    yearFrameLayout->addWidget(yearLabel);
}

void StatisticsPage::applyStyles()
{
    // Stylesheet global jika diperlukan
}