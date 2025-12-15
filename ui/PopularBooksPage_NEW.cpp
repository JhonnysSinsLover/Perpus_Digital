#include "PopularBooksPage.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>

PopularBooksPage::PopularBooksPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadPopularBooks();
}

PopularBooksPage::~PopularBooksPage()
{
}

void PopularBooksPage::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(25);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(15);
    headerLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/svg/star.svg").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setFixedSize(32, 32);
    headerLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel("Buku Populer (Rating Tertinggi)");
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    QLabel* subtitleLabel = new QLabel("Menggunakan Priority Queue (Max Heap) untuk menampilkan buku berdasarkan rating tertinggi");
    subtitleLabel->setStyleSheet("color: #A3AED0; font-size: 14px; margin-bottom: 5px; font-family: 'Segoe UI';");
    mainLayout->addWidget(subtitleLabel);

    QFrame* infoCard = new QFrame();
    infoCard->setStyleSheet(
        "QFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FFF8E1, stop:1 #FFECB3); border-left: 4px solid #FFA000; border-radius: 12px; padding: 18px; }"
    );
    QHBoxLayout* infoLayout = new QHBoxLayout(infoCard);
    infoLayout->setContentsMargins(18, 15, 18, 15);
    infoLayout->setSpacing(15);

    QLabel* bulbIcon = new QLabel();
    bulbIcon->setPixmap(QPixmap(":/svg/lightbulb.svg").scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    bulbIcon->setFixedSize(28, 28);
    infoLayout->addWidget(bulbIcon);

    QVBoxLayout* infoTextLayout = new QVBoxLayout();
    infoTextLayout->setSpacing(6);

    QLabel* infoTitle = new QLabel("Algoritma Max Heap (Priority Queue)");
    infoTitle->setStyleSheet("color: #E65100; font-weight: 700; font-size: 14px; background: transparent; font-family: 'Segoe UI';");
    infoTextLayout->addWidget(infoTitle);

    QLabel* infoDesc = new QLabel(
        "Algoritma ini mengatur buku dalam struktur heap dimana parent node selalu memiliki rating lebih tinggi atau sama dengan child node-nya. "
        "Ini memungkinkan kita untuk selalu mengakses buku dengan rating tertinggi dalam waktu O(1) dan menambah/menghapus dalam O(log n)."
    );
    infoDesc->setWordWrap(true);
    infoDesc->setStyleSheet("color: #F57C00; font-size: 12px; background: transparent; font-family: 'Segoe UI';");
    infoTextLayout->addWidget(infoDesc);

    infoLayout->addLayout(infoTextLayout, 1);
    mainLayout->addWidget(infoCard);

    QFrame* filterCard = new QFrame();
    filterCard->setObjectName("card");

    QHBoxLayout* filterLayout = new QHBoxLayout(filterCard);
    filterLayout->setContentsMargins(25, 20, 25, 20);
    filterLayout->setSpacing(20);

    QLabel* filterIcon = new QLabel();
    filterIcon->setPixmap(QPixmap(":/svg/filter.svg").scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    filterIcon->setFixedSize(22, 22);
    filterLayout->addWidget(filterIcon);

    QLabel* filterLabel = new QLabel("Tampilkan Top:");
    filterLabel->setStyleSheet("color: #2B3674; font-weight: 600; font-size: 14px; font-family: 'Segoe UI';");
    filterLayout->addWidget(filterLabel);

    m_spinTopN = new QSpinBox();
    m_spinTopN->setRange(1, 100);
    m_spinTopN->setValue(10);
    m_spinTopN->setSuffix(" buku");
    m_spinTopN->setFixedHeight(40);
    m_spinTopN->setFixedWidth(150);
    filterLayout->addWidget(m_spinTopN);

    m_btnApplyFilter = new QPushButton("Terapkan");
    m_btnApplyFilter->setIcon(QIcon(":/svg/check.svg"));
    m_btnApplyFilter->setIconSize(QSize(16, 16));
    m_btnApplyFilter->setCursor(Qt::PointingHandCursor);
    m_btnApplyFilter->setFixedSize(140, 40);
    m_btnApplyFilter->setStyleSheet(
        "QPushButton { background-color: #4318FF; border: none; color: white; border-radius: 12px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #3311DB; }"
        "QPushButton:pressed { background-color: #2108B7; }"
    );
    filterLayout->addWidget(m_btnApplyFilter);

    m_btnRefresh = new QPushButton("Refresh Data");
    m_btnRefresh->setIcon(QIcon(":/svg/refresh.svg"));
    m_btnRefresh->setIconSize(QSize(16, 16));
    m_btnRefresh->setCursor(Qt::PointingHandCursor);
    m_btnRefresh->setFixedSize(150, 40);
    m_btnRefresh->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #E0E5F2; color: #A3AED0; border-radius: 12px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { border-color: #4318FF; color: #4318FF; }"
    );
    filterLayout->addWidget(m_btnRefresh);

    filterLayout->addStretch();

    mainLayout->addWidget(filterCard);

    QFrame* tableCard = new QFrame();
    tableCard->setObjectName("card");

    QVBoxLayout* tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(30, 30, 30, 30);
    tableLayout->setSpacing(15);

    QHBoxLayout* tableHeaderLayout = new QHBoxLayout();
    tableHeaderLayout->setSpacing(12);

    QLabel* rankIcon = new QLabel();
    rankIcon->setPixmap(QPixmap(":/svg/trophy.svg").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    rankIcon->setFixedSize(24, 24);
    tableHeaderLayout->addWidget(rankIcon);

    QLabel* tableTitle = new QLabel("Peringkat Buku");
    tableTitle->setStyleSheet("font-size: 18px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    tableHeaderLayout->addWidget(tableTitle);
    tableHeaderLayout->addStretch();

    m_lblTotal = new QLabel("Total: 0 buku");
    m_lblTotal->setStyleSheet("font-size: 13px; color: #A3AED0; font-weight: 600; font-family: 'Segoe UI';");
    tableHeaderLayout->addWidget(m_lblTotal);

    tableLayout->addLayout(tableHeaderLayout);

    m_tableBooks = new QTableWidget();
    m_tableBooks->setColumnCount(6);
    m_tableBooks->setHorizontalHeaderLabels({"Rank", "Judul", "Penulis", "Genre", "Tahun", "Rating"});
    m_tableBooks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableBooks->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_tableBooks->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    m_tableBooks->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    m_tableBooks->setColumnWidth(0, 70);
    m_tableBooks->setColumnWidth(4, 80);
    m_tableBooks->setColumnWidth(5, 90);
    m_tableBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableBooks->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableBooks->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableBooks->verticalHeader()->setVisible(false);
    m_tableBooks->setAlternatingRowColors(true);
    m_tableBooks->setStyleSheet(
        "QTableWidget { border: none; background-color: #F4F7FE; gridline-color: #E0E5F2; font-family: 'Segoe UI'; }"
        "QTableWidget::item { padding: 10px; color: #2B3674; }"
        "QTableWidget::item:selected { background-color: #EAE9FE; color: #4318FF; }"
        "QHeaderView::section { background-color: #F4F7FE; color: #A3AED0; font-weight: 600; padding: 12px; border: none; border-bottom: 2px solid #E0E5F2; font-size: 13px; }"
    );

    tableLayout->addWidget(m_tableBooks);

    mainLayout->addWidget(tableCard);

    connect(m_btnApplyFilter, &QPushButton::clicked, this, &PopularBooksPage::loadPopularBooks);
    connect(m_btnRefresh, &QPushButton::clicked, this, &PopularBooksPage::loadPopularBooks);
}

void PopularBooksPage::loadPopularBooks()
{
    m_tableBooks->setRowCount(0);

    BookManager* manager = DatabaseManager::getInstance()->getBookManager();
    if (!manager) {
        QMessageBox::warning(this, "Error", "BookManager tidak tersedia!");
        return;
    }

    int topN = m_spinTopN->value();
    std::vector<Book*> books = manager->getAllBooks();

    std::priority_queue<std::pair<double, Book*>> maxHeap;

    for (Book* book : books) {
        if (book) {
            maxHeap.push({book->getRating(), book});
        }
    }

    int count = 0;
    int rank = 1;

    while (!maxHeap.empty() && count < topN) {
        auto pair = maxHeap.top();
        maxHeap.pop();

        Book* book = pair.second;

        m_tableBooks->insertRow(count);

        QTableWidgetItem* rankItem = new QTableWidgetItem(QString::number(rank));
        rankItem->setTextAlignment(Qt::AlignCenter);
        rankItem->setFont(QFont("Segoe UI", 13, QFont::Bold));
        
        if (rank == 1) {
            rankItem->setForeground(QBrush(QColor("#FFD700")));
        } else if (rank == 2) {
            rankItem->setForeground(QBrush(QColor("#C0C0C0")));
        } else if (rank == 3) {
            rankItem->setForeground(QBrush(QColor("#CD7F32")));
        } else {
            rankItem->setForeground(QBrush(QColor("#A3AED0")));
        }

        m_tableBooks->setItem(count, 0, rankItem);
        m_tableBooks->setItem(count, 1, new QTableWidgetItem(book->getJudul()));
        m_tableBooks->setItem(count, 2, new QTableWidgetItem(book->getPenulis()));
        m_tableBooks->setItem(count, 3, new QTableWidgetItem(book->getGenre().join(", ")));

        QTableWidgetItem* yearItem = new QTableWidgetItem(QString::number(book->getTahun()));
        yearItem->setTextAlignment(Qt::AlignCenter);
        m_tableBooks->setItem(count, 4, yearItem);

        QTableWidgetItem* ratingItem = new QTableWidgetItem(QString::number(book->getRating(), 'f', 1));
        ratingItem->setTextAlignment(Qt::AlignCenter);
        ratingItem->setFont(QFont("Segoe UI", 12, QFont::Bold));
        ratingItem->setForeground(QBrush(QColor("#FFA000")));
        m_tableBooks->setItem(count, 5, ratingItem);

        count++;
        rank++;
    }

    m_lblTotal->setText(QString("Total: %1 buku").arg(count));
}
