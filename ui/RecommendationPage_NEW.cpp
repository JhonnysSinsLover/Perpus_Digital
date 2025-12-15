#include "RecommendationPage.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QScrollBar>
#include <QDebug>

RecommendationPage::RecommendationPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

RecommendationPage::~RecommendationPage()
{
}

void RecommendationPage::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(25);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(15);
    headerLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/svg/lightbulb.svg").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setFixedSize(32, 32);
    headerLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel("Rekomendasi Buku");
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    QLabel* subtitleLabel = new QLabel("Sistem rekomendasi menggunakan Graph (Adjacency List) berdasarkan kesamaan genre");
    subtitleLabel->setStyleSheet("color: #A3AED0; font-size: 14px; margin-bottom: 5px; font-family: 'Segoe UI';");
    mainLayout->addWidget(subtitleLabel);

    QFrame* infoCard = new QFrame();
    infoCard->setStyleSheet(
        "QFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #E8F5E9, stop:1 #C8E6C9); border-left: 4px solid #43A047; border-radius: 12px; padding: 18px; }"
    );
    QHBoxLayout* infoLayout = new QHBoxLayout(infoCard);
    infoLayout->setContentsMargins(18, 15, 18, 15);
    infoLayout->setSpacing(15);

    QLabel* graphIcon = new QLabel();
    graphIcon->setPixmap(QPixmap(":/svg/network.svg").scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    graphIcon->setFixedSize(28, 28);
    infoLayout->addWidget(graphIcon);

    QVBoxLayout* infoTextLayout = new QVBoxLayout();
    infoTextLayout->setSpacing(6);

    QLabel* infoTitle = new QLabel("Algoritma Graph - Adjacency List");
    infoTitle->setStyleSheet("color: #2E7D32; font-weight: 700; font-size: 14px; background: transparent; font-family: 'Segoe UI';");
    infoTextLayout->addWidget(infoTitle);

    QLabel* infoDesc = new QLabel(
        "Algoritma ini membangun graf dimana setiap buku adalah node, dan edge menghubungkan buku-buku dengan genre yang sama. "
        "Semakin banyak genre yang sama, semakin kuat koneksinya. Sistem kemudian merekomendasikan buku yang paling terhubung dengan buku yang dipilih."
    );
    infoDesc->setWordWrap(true);
    infoDesc->setStyleSheet("color: #388E3C; font-size: 12px; background: transparent; font-family: 'Segoe UI';");
    infoTextLayout->addWidget(infoDesc);

    infoLayout->addLayout(infoTextLayout, 1);
    mainLayout->addWidget(infoCard);

    QFrame* searchCard = new QFrame();
    searchCard->setObjectName("card");

    QVBoxLayout* searchLayout = new QVBoxLayout(searchCard);
    searchLayout->setContentsMargins(30, 25, 30, 25);
    searchLayout->setSpacing(15);

    QLabel* searchTitle = new QLabel("Cari Buku untuk Rekomendasi");
    searchTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    searchLayout->addWidget(searchTitle);

    QHBoxLayout* searchInputLayout = new QHBoxLayout();
    searchInputLayout->setSpacing(12);

    QLabel* searchIcon = new QLabel();
    searchIcon->setPixmap(QPixmap(":/svg/search.svg").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    searchIcon->setFixedSize(20, 20);
    searchInputLayout->addWidget(searchIcon);

    m_lineSearchBook = new QLineEdit();
    m_lineSearchBook->setPlaceholderText("Masukkan judul buku...");
    m_lineSearchBook->setFixedHeight(42);
    searchInputLayout->addWidget(m_lineSearchBook);

    m_btnSearch = new QPushButton("Cari Rekomendasi");
    m_btnSearch->setIcon(QIcon(":/svg/target.svg"));
    m_btnSearch->setIconSize(QSize(18, 18));
    m_btnSearch->setCursor(Qt::PointingHandCursor);
    m_btnSearch->setFixedSize(180, 42);
    m_btnSearch->setStyleSheet(
        "QPushButton { background-color: #4318FF; border: none; color: white; border-radius: 12px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #3311DB; }"
        "QPushButton:pressed { background-color: #2108B7; }"
    );
    searchInputLayout->addWidget(m_btnSearch);

    searchLayout->addLayout(searchInputLayout);

    QHBoxLayout* graphButtonLayout = new QHBoxLayout();
    graphButtonLayout->setSpacing(12);
    graphButtonLayout->addStretch();

    m_btnBuildGraph = new QPushButton("Build Graph");
    m_btnBuildGraph->setIcon(QIcon(":/svg/build.svg"));
    m_btnBuildGraph->setIconSize(QSize(16, 16));
    m_btnBuildGraph->setCursor(Qt::PointingHandCursor);
    m_btnBuildGraph->setFixedSize(150, 38);
    m_btnBuildGraph->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #E0E5F2; color: #A3AED0; border-radius: 10px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { border-color: #4318FF; color: #4318FF; }"
    );
    graphButtonLayout->addWidget(m_btnBuildGraph);

    m_btnRefresh = new QPushButton("Refresh");
    m_btnRefresh->setIcon(QIcon(":/svg/refresh.svg"));
    m_btnRefresh->setIconSize(QSize(16, 16));
    m_btnRefresh->setCursor(Qt::PointingHandCursor);
    m_btnRefresh->setFixedSize(120, 38);
    m_btnRefresh->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #E0E5F2; color: #A3AED0; border-radius: 10px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { border-color: #05CD99; color: #05CD99; }"
    );
    graphButtonLayout->addWidget(m_btnRefresh);

    searchLayout->addLayout(graphButtonLayout);
    mainLayout->addWidget(searchCard);

    QFrame* resultCard = new QFrame();
    resultCard->setObjectName("card");

    QVBoxLayout* resultLayout = new QVBoxLayout(resultCard);
    resultLayout->setContentsMargins(30, 30, 30, 30);
    resultLayout->setSpacing(15);

    QHBoxLayout* resultHeaderLayout = new QHBoxLayout();
    resultHeaderLayout->setSpacing(12);

    QLabel* resultIcon = new QLabel();
    resultIcon->setPixmap(QPixmap(":/svg/list.svg").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    resultIcon->setFixedSize(24, 24);
    resultHeaderLayout->addWidget(resultIcon);

    QLabel* resultTitle = new QLabel("Hasil Rekomendasi");
    resultTitle->setStyleSheet("font-size: 18px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    resultHeaderLayout->addWidget(resultTitle);
    resultHeaderLayout->addStretch();

    resultLayout->addLayout(resultHeaderLayout);

    m_browserResults = new QTextBrowser();
    m_browserResults->setOpenExternalLinks(false);
    m_browserResults->setStyleSheet(
        "QTextBrowser { border: none; background-color: #F4F7FE; padding: 15px; color: #2B3674; font-family: 'Segoe UI'; font-size: 13px; }"
    );
    m_browserResults->setHtml(
        "<div style='text-align: center; color: #A3AED0; padding: 50px;'>"
        "<p style='font-size: 15px; font-weight: 600;'>Belum ada rekomendasi</p>"
        "<p style='font-size: 13px;'>Cari buku terlebih dahulu untuk mendapatkan rekomendasi</p>"
        "</div>"
    );

    resultLayout->addWidget(m_browserResults);
    mainLayout->addWidget(resultCard);

    connect(m_btnSearch, &QPushButton::clicked, this, &RecommendationPage::onSearchClicked);
    connect(m_btnBuildGraph, &QPushButton::clicked, this, &RecommendationPage::onBuildGraphClicked);
    connect(m_btnRefresh, &QPushButton::clicked, this, &RecommendationPage::onRefreshClicked);
    connect(m_lineSearchBook, &QLineEdit::returnPressed, this, &RecommendationPage::onSearchClicked);
}

void RecommendationPage::onSearchClicked()
{
    QString query = m_lineSearchBook->text().trimmed();
    if (query.isEmpty()) {
        QMessageBox::warning(this, "Input Kosong", "Mohon masukkan judul buku terlebih dahulu!");
        return;
    }

    BookManager* manager = DatabaseManager::getInstance()->getBookManager();
    if (!manager) {
        QMessageBox::warning(this, "Error", "BookManager tidak tersedia!");
        return;
    }

    std::vector<Book*> allBooks = manager->getAllBooks();
    Book* targetBook = nullptr;

    for (Book* book : allBooks) {
        if (book && book->getJudul().contains(query, Qt::CaseInsensitive)) {
            targetBook = book;
            break;
        }
    }

    if (!targetBook) {
        m_browserResults->setHtml(
            "<div style='text-align: center; color: #EE5D50; padding: 40px;'>"
            "<p style='font-size: 15px; font-weight: 600;'>Buku tidak ditemukan</p>"
            "<p style='font-size: 13px;'>Coba dengan judul lain atau pastikan ejaan sudah benar</p>"
            "</div>"
        );
        return;
    }

    Graph graph;
    for (Book* book : allBooks) {
        if (book) {
            graph.addBook(book);
        }
    }

    std::vector<Book*> recommendations = graph.getRecommendations(targetBook, 8);

    if (recommendations.empty()) {
        m_browserResults->setHtml(
            "<div style='text-align: center; color: #FFA000; padding: 40px;'>"
            "<p style='font-size: 15px; font-weight: 600;'>Tidak ada rekomendasi tersedia</p>"
            "<p style='font-size: 13px;'>Buku ini tidak memiliki kesamaan genre dengan buku lain</p>"
            "</div>"
        );
        return;
    }

    QString html = "<div style='font-family: Segoe UI;'>";
    html += QString(
        "<div style='background: linear-gradient(90deg, #E8F5E9, #C8E6C9); padding: 20px; border-radius: 12px; margin-bottom: 20px; border-left: 4px solid #43A047;'>"
        "<h3 style='color: #2E7D32; margin: 0 0 8px 0;'>%1</h3>"
        "<p style='color: #388E3C; margin: 0; font-size: 13px;'>Oleh: %2 | Genre: %3</p>"
        "</div>"
    ).arg(targetBook->getJudul()).arg(targetBook->getPenulis()).arg(targetBook->getGenre().join(", "));

    html += "<h4 style='color: #2B3674; margin: 20px 0 15px 0;'>Rekomendasi untuk Anda:</h4>";
    html += "<div style='display: grid; gap: 12px;'>";

    for (size_t i = 0; i < recommendations.size(); ++i) {
        Book* book = recommendations[i];
        html += QString(
            "<div style='background: white; padding: 15px; border-radius: 10px; border: 1px solid #E0E5F2;'>"
            "<div style='display: flex; align-items: start;'>"
            "<div style='background: #4318FF; color: white; width: 30px; height: 30px; border-radius: 8px; display: flex; align-items: center; justify-content: center; font-weight: bold; margin-right: 12px;'>%1</div>"
            "<div style='flex: 1;'>"
            "<h4 style='color: #2B3674; margin: 0 0 6px 0; font-size: 14px;'>%2</h4>"
            "<p style='color: #A3AED0; margin: 0; font-size: 12px;'>%3 | %4 | Rating: <span style='color: #FFA000; font-weight: bold;'>%5</span></p>"
            "<p style='color: #4318FF; margin: 6px 0 0 0; font-size: 11px;'><strong>Genre:</strong> %6</p>"
            "</div>"
            "</div>"
            "</div>"
        ).arg(i + 1)
         .arg(book->getJudul())
         .arg(book->getPenulis())
         .arg(book->getTahun())
         .arg(book->getRating(), 0, 'f', 1)
         .arg(book->getGenre().join(", "));
    }

    html += "</div></div>";
    m_browserResults->setHtml(html);
}

void RecommendationPage::onBuildGraphClicked()
{
    BookManager* manager = DatabaseManager::getInstance()->getBookManager();
    if (!manager) {
        QMessageBox::warning(this, "Error", "BookManager tidak tersedia!");
        return;
    }

    std::vector<Book*> allBooks = manager->getAllBooks();
    Graph graph;

    for (Book* book : allBooks) {
        if (book) {
            graph.addBook(book);
        }
    }

    int totalBooks = allBooks.size();
    int totalEdges = 0;

    for (Book* book : allBooks) {
        if (book) {
            totalEdges += graph.getRecommendations(book, 100).size();
        }
    }

    QMessageBox::information(this, "Graph Berhasil Dibangun",
        QString("Graph telah berhasil dibangun!\n\nTotal Nodes (Buku): %1\nTotal Edges (Koneksi): %2")
            .arg(totalBooks)
            .arg(totalEdges / 2));
}

void RecommendationPage::onRefreshClicked()
{
    m_lineSearchBook->clear();
    m_browserResults->setHtml(
        "<div style='text-align: center; color: #A3AED0; padding: 50px;'>"
        "<p style='font-size: 15px; font-weight: 600;'>Belum ada rekomendasi</p>"
        "<p style='font-size: 13px;'>Cari buku terlebih dahulu untuk mendapatkan rekomendasi</p>"
        "</div>"
    );
}
