#include "StatisticsPage.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <algorithm>
#include <map>

StatisticsPage::StatisticsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadStatistics();
}

StatisticsPage::~StatisticsPage()
{
}

void StatisticsPage::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(25);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(15);
    headerLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/svg/chart.svg").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setFixedSize(32, 32);
    headerLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel("Statistik Perpustakaan");
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QPushButton* btnRefresh = new QPushButton("Refresh Data");
    btnRefresh->setIcon(QIcon(":/svg/refresh.svg"));
    btnRefresh->setIconSize(QSize(16, 16));
    btnRefresh->setCursor(Qt::PointingHandCursor);
    btnRefresh->setFixedSize(150, 40);
    btnRefresh->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #E0E5F2; color: #A3AED0; border-radius: 12px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { border-color: #4318FF; color: #4318FF; }"
    );
    headerLayout->addWidget(btnRefresh);

    mainLayout->addLayout(headerLayout);

    QLabel* subtitleLabel = new QLabel("Visualisasi data koleksi buku dalam bentuk statistik dan grafik");
    subtitleLabel->setStyleSheet("color: #A3AED0; font-size: 14px; margin-bottom: 5px; font-family: 'Segoe UI';");
    mainLayout->addWidget(subtitleLabel);

    QGridLayout* summaryGrid = new QGridLayout();
    summaryGrid->setSpacing(20);

    m_lblTotalBooks = createSummaryCard("Total Buku", "0", ":/svg/book.svg", "#4318FF");
    m_lblTotalGenres = createSummaryCard("Total Genre", "0", ":/svg/category.svg", "#05CD99");
    m_lblAvgRating = createSummaryCard("Rata-rata Rating", "0.0", ":/svg/star.svg", "#FFA000");
    m_lblLatestYear = createSummaryCard("Tahun Terbaru", "0", ":/svg/calendar.svg", "#EE5D50");

    summaryGrid->addWidget(m_lblTotalBooks, 0, 0);
    summaryGrid->addWidget(m_lblTotalGenres, 0, 1);
    summaryGrid->addWidget(m_lblAvgRating, 0, 2);
    summaryGrid->addWidget(m_lblLatestYear, 0, 3);

    mainLayout->addLayout(summaryGrid);

    QHBoxLayout* chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(20);

    QFrame* genreCard = new QFrame();
    genreCard->setObjectName("card");
    QVBoxLayout* genreLayout = new QVBoxLayout(genreCard);
    genreLayout->setContentsMargins(25, 25, 25, 25);
    genreLayout->setSpacing(15);

    QLabel* genreTitle = new QLabel("Distribusi Genre");
    genreTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    genreLayout->addWidget(genreTitle);

    m_browserGenreChart = new QTextBrowser();
    m_browserGenreChart->setOpenExternalLinks(false);
    m_browserGenreChart->setStyleSheet(
        "QTextBrowser { border: none; background-color: #F4F7FE; padding: 10px; }"
    );
    genreLayout->addWidget(m_browserGenreChart);

    QFrame* ratingCard = new QFrame();
    ratingCard->setObjectName("card");
    QVBoxLayout* ratingLayout = new QVBoxLayout(ratingCard);
    ratingLayout->setContentsMargins(25, 25, 25, 25);
    ratingLayout->setSpacing(15);

    QLabel* ratingTitle = new QLabel("Distribusi Rating");
    ratingTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    ratingLayout->addWidget(ratingTitle);

    m_browserRatingChart = new QTextBrowser();
    m_browserRatingChart->setOpenExternalLinks(false);
    m_browserRatingChart->setStyleSheet(
        "QTextBrowser { border: none; background-color: #F4F7FE; padding: 10px; }"
    );
    ratingLayout->addWidget(m_browserRatingChart);

    chartsLayout->addWidget(genreCard);
    chartsLayout->addWidget(ratingCard);

    mainLayout->addLayout(chartsLayout);

    QFrame* yearCard = new QFrame();
    yearCard->setObjectName("card");
    QVBoxLayout* yearLayout = new QVBoxLayout(yearCard);
    yearLayout->setContentsMargins(25, 25, 25, 25);
    yearLayout->setSpacing(15);

    QLabel* yearTitle = new QLabel("Distribusi Tahun Publikasi");
    yearTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    yearLayout->addWidget(yearTitle);

    m_browserYearChart = new QTextBrowser();
    m_browserYearChart->setOpenExternalLinks(false);
    m_browserYearChart->setStyleSheet(
        "QTextBrowser { border: none; background-color: #F4F7FE; padding: 10px; }"
    );
    yearLayout->addWidget(m_browserYearChart);

    mainLayout->addWidget(yearCard);

    connect(btnRefresh, &QPushButton::clicked, this, &StatisticsPage::loadStatistics);
}

QWidget* StatisticsPage::createSummaryCard(const QString& title, const QString& value, const QString& iconPath, const QString& color)
{
    QFrame* card = new QFrame();
    card->setObjectName("card");
    card->setMinimumHeight(120);

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(25, 20, 25, 20);
    layout->setSpacing(12);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(12);

    QLabel* icon = new QLabel();
    icon->setPixmap(QPixmap(iconPath).scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    icon->setFixedSize(28, 28);
    topLayout->addWidget(icon);

    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("color: #A3AED0; font-size: 13px; font-weight: 600; font-family: 'Segoe UI';");
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();

    layout->addLayout(topLayout);

    QLabel* valueLabel = new QLabel(value);
    valueLabel->setStyleSheet(QString("color: %1; font-size: 32px; font-weight: 700; font-family: 'Segoe UI';").arg(color));
    layout->addWidget(valueLabel);

    layout->addStretch();

    QLabel* storageLabel = new QLabel();
    storageLabel->setObjectName("summaryValue");
    card->setProperty("valueLabel", QVariant::fromValue(static_cast<void*>(valueLabel)));

    return card;
}

void StatisticsPage::loadStatistics()
{
    BookManager* manager = DatabaseManager::getInstance()->getBookManager();
    if (!manager) {
        QMessageBox::warning(this, "Error", "BookManager tidak tersedia!");
        return;
    }

    std::vector<Book*> books = manager->getAllBooks();

    updateSummary(books);
    updateGenreChart(books);
    updateRatingChart(books);
    updateYearChart(books);
}

void StatisticsPage::updateSummary(const std::vector<Book*>& books)
{
    int totalBooks = books.size();

    std::set<QString> uniqueGenres;
    double totalRating = 0.0;
    int latestYear = 0;

    for (Book* book : books) {
        if (book) {
            for (const QString& genre : book->getGenre()) {
                uniqueGenres.insert(genre);
            }
            totalRating += book->getRating();
            if (book->getTahun() > latestYear) {
                latestYear = book->getTahun();
            }
        }
    }

    double avgRating = (totalBooks > 0) ? (totalRating / totalBooks) : 0.0;

    QLabel* lblTotal = qvariant_cast<QLabel*>(m_lblTotalBooks->property("valueLabel"));
    if (lblTotal) lblTotal->setText(QString::number(totalBooks));

    QLabel* lblGenres = qvariant_cast<QLabel*>(m_lblTotalGenres->property("valueLabel"));
    if (lblGenres) lblGenres->setText(QString::number(uniqueGenres.size()));

    QLabel* lblRating = qvariant_cast<QLabel*>(m_lblAvgRating->property("valueLabel"));
    if (lblRating) lblRating->setText(QString::number(avgRating, 'f', 2));

    QLabel* lblYear = qvariant_cast<QLabel*>(m_lblLatestYear->property("valueLabel"));
    if (lblYear) lblYear->setText(QString::number(latestYear));
}

void StatisticsPage::updateGenreChart(const std::vector<Book*>& books)
{
    std::map<QString, int> genreCount;

    for (Book* book : books) {
        if (book) {
            for (const QString& genre : book->getGenre()) {
                genreCount[genre]++;
            }
        }
    }

    std::vector<std::pair<QString, int>> sortedGenres(genreCount.begin(), genreCount.end());
    std::sort(sortedGenres.begin(), sortedGenres.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    int maxCount = sortedGenres.empty() ? 1 : sortedGenres[0].second;

    QString html = "<div style='font-family: Segoe UI; padding: 10px;'>";
    
    QStringList colors = {"#4318FF", "#05CD99", "#FFA000", "#EE5D50", "#1B95E0", "#8B5CF6", "#EC4899", "#10B981"};
    int colorIndex = 0;

    for (const auto& pair : sortedGenres) {
        int percentage = (maxCount > 0) ? (pair.second * 100 / maxCount) : 0;
        QString color = colors[colorIndex % colors.size()];
        
        html += QString(
            "<div style='margin-bottom: 15px;'>"
            "<div style='display: flex; justify-content: space-between; margin-bottom: 5px;'>"
            "<span style='color: #2B3674; font-weight: 600; font-size: 13px;'>%1</span>"
            "<span style='color: %2; font-weight: 700; font-size: 13px;'>%3 buku</span>"
            "</div>"
            "<div style='background-color: #E0E5F2; border-radius: 8px; height: 12px; overflow: hidden;'>"
            "<div style='background-color: %2; width: %4%; height: 100%; border-radius: 8px; transition: width 0.3s;'></div>"
            "</div>"
            "</div>"
        ).arg(pair.first).arg(color).arg(pair.second).arg(percentage);
        
        colorIndex++;
    }

    html += "</div>";
    m_browserGenreChart->setHtml(html);
}

void StatisticsPage::updateRatingChart(const std::vector<Book*>& books)
{
    std::map<int, int> ratingCount;
    
    for (int i = 1; i <= 5; i++) {
        ratingCount[i] = 0;
    }

    for (Book* book : books) {
        if (book) {
            int rating = static_cast<int>(std::round(book->getRating()));
            if (rating >= 1 && rating <= 5) {
                ratingCount[rating]++;
            }
        }
    }

    int maxCount = 1;
    for (const auto& pair : ratingCount) {
        if (pair.second > maxCount) maxCount = pair.second;
    }

    QString html = "<div style='font-family: Segoe UI; padding: 10px;'>";
    
    QStringList colors = {"#EE5D50", "#FFA000", "#FFC107", "#05CD99", "#4318FF"};

    for (int rating = 5; rating >= 1; rating--) {
        int count = ratingCount[rating];
        int percentage = (maxCount > 0) ? (count * 100 / maxCount) : 0;
        QString color = colors[rating - 1];
        
        QString stars;
        for (int i = 0; i < rating; i++) {
            stars += "⭐";
        }
        
        html += QString(
            "<div style='margin-bottom: 15px;'>"
            "<div style='display: flex; justify-content: space-between; margin-bottom: 5px;'>"
            "<span style='color: #2B3674; font-weight: 600; font-size: 13px;'>%1 (%2)</span>"
            "<span style='color: %3; font-weight: 700; font-size: 13px;'>%4 buku</span>"
            "</div>"
            "<div style='background-color: #E0E5F2; border-radius: 8px; height: 12px; overflow: hidden;'>"
            "<div style='background-color: %3; width: %5%; height: 100%; border-radius: 8px;'></div>"
            "</div>"
            "</div>"
        ).arg(rating).arg(stars).arg(color).arg(count).arg(percentage);
    }

    html += "</div>";
    m_browserRatingChart->setHtml(html);
}

void StatisticsPage::updateYearChart(const std::vector<Book*>& books)
{
    std::map<int, int> yearCount;

    for (Book* book : books) {
        if (book) {
            int decade = (book->getTahun() / 10) * 10;
            yearCount[decade]++;
        }
    }

    std::vector<std::pair<int, int>> sortedYears(yearCount.begin(), yearCount.end());
    std::sort(sortedYears.begin(), sortedYears.end());

    int maxCount = 1;
    for (const auto& pair : sortedYears) {
        if (pair.second > maxCount) maxCount = pair.second;
    }

    QString html = "<div style='font-family: Segoe UI; padding: 10px;'>";
    
    QStringList colors = {"#8B5CF6", "#EC4899", "#10B981", "#F59E0B", "#3B82F6", "#EF4444", "#06B6D4", "#84CC16"};
    int colorIndex = 0;

    for (const auto& pair : sortedYears) {
        int percentage = (maxCount > 0) ? (pair.second * 100 / maxCount) : 0;
        QString color = colors[colorIndex % colors.size()];
        
        html += QString(
            "<div style='margin-bottom: 15px;'>"
            "<div style='display: flex; justify-content: space-between; margin-bottom: 5px;'>"
            "<span style='color: #2B3674; font-weight: 600; font-size: 13px;'>%1s</span>"
            "<span style='color: %2; font-weight: 700; font-size: 13px;'>%3 buku</span>"
            "</div>"
            "<div style='background-color: #E0E5F2; border-radius: 8px; height: 12px; overflow: hidden;'>"
            "<div style='background-color: %2; width: %4%; height: 100%; border-radius: 8px;'></div>"
            "</div>"
            "</div>"
        ).arg(pair.first).arg(color).arg(pair.second).arg(percentage);
        
        colorIndex++;
    }

    html += "</div>";
    m_browserYearChart->setHtml(html);
}
