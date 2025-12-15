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
    updateDashboard(); 
}

DashboardPage::~DashboardPage()
{
}

void DashboardPage::setupUI()
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
        "QScrollBar:vertical { border: none; background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );
    
    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #F4F7FE;");
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(25);
    
    createStatCards(contentLayout);
    createRecentBooksTable(contentLayout);
    
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
    headerFrame->setFixedHeight(80);
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(20);
    
    QLabel* iconLabel = new QLabel(headerFrame);
    iconLabel->setPixmap(QPixmap(":/svg/dashboard.svg").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    headerLayout->addWidget(iconLabel);
    
    QLabel* titleLabel = new QLabel("Dashboard", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 26px; font-weight: 700; color: #2B3674;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch();
    
    m_searchBar = new QLineEdit(headerFrame);
    m_searchBar->setPlaceholderText("Search...");
    m_searchBar->setFixedSize(300, 45);
    headerLayout->addWidget(m_searchBar);
    
    m_notificationBtn = new QPushButton(headerFrame);
    m_notificationBtn->setIcon(QIcon(":/svg/notification.svg"));
    m_notificationBtn->setIconSize(QSize(24, 24));
    m_notificationBtn->setFixedSize(45, 45);
    m_notificationBtn->setCursor(Qt::PointingHandCursor);
    m_notificationBtn->setFlat(true);
    headerLayout->addWidget(m_notificationBtn);
    
    m_profileBtn = new QPushButton(headerFrame);
    m_profileBtn->setIcon(QIcon(":/svg/user.svg"));
    m_profileBtn->setIconSize(QSize(24, 24));
    m_profileBtn->setFixedSize(45, 45);
    m_profileBtn->setCursor(Qt::PointingHandCursor);
    m_profileBtn->setStyleSheet(
        "QPushButton { background-color: #112D4E; border: none; border-radius: 22px; }"
        "QPushButton:hover { background-color: #2B3674; }"
    );
    headerLayout->addWidget(m_profileBtn);
    
    mainLayout->addWidget(headerFrame);
}

void DashboardPage::createStatCards(QVBoxLayout* mainLayout)
{
    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(20);

    QWidget* card1 = createCompactStatCard("Total Buku", "0", ":/svg/book.svg", "#EAEFFC", true);
    QWidget* card2 = createCompactStatCard("Total Penulis", "0", ":/svg/author.svg", "#FFF7E7", false);
    QWidget* card3 = createCompactStatCard("Total Genre", "0", ":/svg/category.svg", "#FEEFEF", false);
    QWidget* card4 = createCompactStatCard("Rata-rata Rating", "0.0", ":/svg/star.svg", "#F0FDF4", false);
    
    statsLayout->addWidget(card1);
    statsLayout->addWidget(card2);
    statsLayout->addWidget(card3);
    statsLayout->addWidget(card4);

    mainLayout->addLayout(statsLayout);
}

QWidget* DashboardPage::createCompactStatCard(const QString& title, const QString& value, 
                                               const QString& iconPath, const QString& iconColorBg, bool showTrend)
{
    QFrame* card = new QFrame(this);
    card->setObjectName("card");
    card->setFixedHeight(120);

    QHBoxLayout* layout = new QHBoxLayout(card);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(iconPath).scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setFixedSize(60, 60);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet(QString(
        "background-color: %1; border-radius: 30px; padding: 10px;"
    ).arg(iconColorBg));
    
    layout->addWidget(iconLabel);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    textLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* lblTitle = new QLabel(title, card);
    lblTitle->setStyleSheet("color: #A3AED0; font-size: 14px; font-weight: 500; font-family: 'Segoe UI';");
    
    QLabel* lblValue = new QLabel(value, card);
    lblValue->setStyleSheet("color: #2B3674; font-size: 28px; font-weight: 700; font-family: 'Segoe UI';");
    
    textLayout->addWidget(lblTitle);
    textLayout->addWidget(lblValue);
    
    if (title == "Total Buku") m_lblTotalBooks = lblValue;
    else if (title == "Total Penulis") m_lblTotalAuthors = lblValue;
    else if (title == "Total Genre") m_lblTotalGenres = lblValue;
    else if (title == "Rata-rata Rating") m_lblAvgRating = lblValue;
    
    layout->addLayout(textLayout);
    layout->addStretch();
    
    if (showTrend) {
        QLabel* trend = new QLabel();
        trend->setPixmap(QPixmap(":/svg/trend-up.svg").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        QLabel* trendText = new QLabel("+12%", card);
        trendText->setStyleSheet("color: #05CD99; font-size: 12px; font-weight: bold;");
        
        QVBoxLayout* trendLayout = new QVBoxLayout();
        trendLayout->addWidget(trend);
        trendLayout->addWidget(trendText);
        layout->addLayout(trendLayout);
    }

    return card;
}







void DashboardPage::createRecentBooksTable(QVBoxLayout* mainLayout)
{
    QFrame* tableFrame = new QFrame(this);
    tableFrame->setObjectName("card");
    
    QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);
    tableLayout->setContentsMargins(25, 25, 25, 25);
    tableLayout->setSpacing(15);
    
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/svg/clock.svg").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    headerLayout->addWidget(iconLabel);
    
    QLabel* tableTitle = new QLabel("Buku Terbaru Ditambahkan", tableFrame);
    tableTitle->setStyleSheet("color: #2B3674; font-size: 20px; font-weight: 700; font-family: 'Segoe UI';");
    headerLayout->addWidget(tableTitle);
    headerLayout->addStretch();
    
    tableLayout->addLayout(headerLayout);
    
    m_recentBooksTable = new QTableWidget(this);
    m_recentBooksTable->setColumnCount(6);
    m_recentBooksTable->setHorizontalHeaderLabels({"Cover", "Judul Buku", "Penulis", "Kategori", "Tahun", "Status"});
    
    m_recentBooksTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: none;"
        "   gridline-color: transparent;"
        "}"
        "QTableWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #F4F7FE;"
        "   color: #2B3674;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #F4F7FE;"
        "   color: #2B3674;"
        "}"
        "QHeaderView::section {"
        "   background-color: white;"
        "   color: #A3AED0;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "   border: none;"
        "   border-bottom: 2px solid #E0E5F2;"
        "   padding: 10px;"
        "   text-align: left;"
        "}"
    );
    
    m_recentBooksTable->horizontalHeader()->setStretchLastSection(true);
    m_recentBooksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_recentBooksTable->setColumnWidth(0, 80);
    m_recentBooksTable->setColumnWidth(2, 180);
    m_recentBooksTable->setColumnWidth(3, 150);
    m_recentBooksTable->setColumnWidth(4, 80);
    m_recentBooksTable->setColumnWidth(5, 120);
    
    m_recentBooksTable->verticalHeader()->setVisible(false);
    m_recentBooksTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_recentBooksTable->setFocusPolicy(Qt::NoFocus);
    m_recentBooksTable->setShowGrid(false);
    m_recentBooksTable->setMinimumHeight(400);

    tableLayout->addWidget(m_recentBooksTable);
    mainLayout->addWidget(tableFrame);
}

void DashboardPage::updateDashboard()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> books = dbManager.getAllBooks();
    
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
    
    m_lblTotalBooks->setText(QString::number(totalBooks));
    m_lblTotalAuthors->setText(QString::number(authors.size()));
    m_lblTotalGenres->setText(QString::number(genres.size()));
    m_lblAvgRating->setText(QString::number(avgRating, 'f', 1));
    
    m_recentBooksTable->setRowCount(0);
    int count = 0;
    for (auto it = books.rbegin(); it != books.rend() && count < 6; ++it, ++count) {
        int row = m_recentBooksTable->rowCount();
        m_recentBooksTable->insertRow(row);
        m_recentBooksTable->setRowHeight(row, 80);
        
        QLabel* coverLabel = new QLabel();
        coverLabel->setPixmap(QPixmap(":/svg/book-open.svg").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        coverLabel->setAlignment(Qt::AlignCenter);
        coverLabel->setStyleSheet("background-color: #F4F7FE; border-radius: 8px; padding: 8px;");
        m_recentBooksTable->setCellWidget(row, 0, coverLabel);
        
        QTableWidgetItem* titleItem = new QTableWidgetItem(it->getJudul());
        titleItem->setFont(QFont("Segoe UI", 11, QFont::Bold));
        m_recentBooksTable->setItem(row, 1, titleItem);
        
        QTableWidgetItem* authorItem = new QTableWidgetItem(it->getPenulis());
        authorItem->setFont(QFont("Segoe UI", 10));
        m_recentBooksTable->setItem(row, 2, authorItem);
        
        QTableWidgetItem* genreItem = new QTableWidgetItem(it->getGenre().join(", "));
        genreItem->setFont(QFont("Segoe UI", 10));
        m_recentBooksTable->setItem(row, 3, genreItem);
        
        QTableWidgetItem* yearItem = new QTableWidgetItem(QString::number(it->getTahun()));
        yearItem->setTextAlignment(Qt::AlignCenter);
        m_recentBooksTable->setItem(row, 4, yearItem);
        
        QWidget* statusWidget = new QWidget();
        QHBoxLayout* statusLayout = new QHBoxLayout(statusWidget);
        statusLayout->setContentsMargins(0,0,0,0);
        statusLayout->setAlignment(Qt::AlignCenter);
        
        QLabel* statusLabel = new QLabel("Tersedia");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setFixedSize(90, 30);
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
}