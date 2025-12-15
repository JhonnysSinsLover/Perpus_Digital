#include "BorrowQueuePage.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QDateTime>
#include <QGroupBox>

BorrowQueuePage::BorrowQueuePage(QWidget *parent)
    : QWidget(parent)
    , m_nextBorrowId(1)
{
    setupUI();
    updateQueueTable();
}

BorrowQueuePage::~BorrowQueuePage()
{
}

void BorrowQueuePage::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(25);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(15);
    headerLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/svg/queue.svg").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setFixedSize(32, 32);
    headerLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel("Antrian Peminjaman Buku");
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    QLabel* subtitleLabel = new QLabel("Sistem antrian menggunakan struktur data Queue (FIFO - First In First Out)");
    subtitleLabel->setStyleSheet("color: #A3AED0; font-size: 14px; margin-bottom: 10px; font-family: 'Segoe UI';");
    mainLayout->addWidget(subtitleLabel);

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(25);

    QFrame* leftCard = new QFrame();
    leftCard->setObjectName("card");
    leftCard->setMinimumWidth(380);
    leftCard->setMaximumWidth(450);

    QVBoxLayout* leftLayout = new QVBoxLayout(leftCard);
    leftLayout->setContentsMargins(30, 30, 30, 30);
    leftLayout->setSpacing(20);

    QLabel* formTitle = new QLabel("Input Peminjaman Baru");
    formTitle->setStyleSheet("font-size: 18px; font-weight: 700; color: #2B3674; margin-bottom: 5px; font-family: 'Segoe UI';");
    leftLayout->addWidget(formTitle);

    m_lineNamaPeminjam = new QLineEdit();
    m_lineNamaPeminjam->setPlaceholderText("Masukkan nama lengkap");
    m_lineNamaPeminjam->setFixedHeight(40);
    leftLayout->addWidget(createInputGroup("Nama Peminjam", m_lineNamaPeminjam));

    m_lineBukuDipinjam = new QLineEdit();
    m_lineBukuDipinjam->setPlaceholderText("Judul buku yang dipinjam");
    m_lineBukuDipinjam->setFixedHeight(40);
    leftLayout->addWidget(createInputGroup("Buku Dipinjam", m_lineBukuDipinjam));

    m_dateEdit = new QDateEdit();
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDate(QDate::currentDate());
    m_dateEdit->setFixedHeight(40);
    m_dateEdit->setDisplayFormat("dd-MM-yyyy");
    leftLayout->addWidget(createInputGroup("Tanggal Peminjaman", m_dateEdit));

    QFrame* infoCard = new QFrame();
    infoCard->setStyleSheet(
        "QFrame { background-color: #E8F5FF; border-left: 4px solid #1B95E0; border-radius: 10px; padding: 15px; }"
    );
    QHBoxLayout* infoLayout = new QHBoxLayout(infoCard);
    infoLayout->setContentsMargins(15, 12, 15, 12);
    infoLayout->setSpacing(12);

    QLabel* infoIcon = new QLabel();
    infoIcon->setPixmap(QPixmap(":/svg/info.svg").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    infoIcon->setFixedSize(20, 20);
    infoLayout->addWidget(infoIcon);

    QLabel* infoText = new QLabel("Antrian diproses dengan urutan FIFO (First In First Out). Data yang pertama masuk akan diproses terlebih dahulu.");
    infoText->setWordWrap(true);
    infoText->setStyleSheet("color: #1B95E0; font-size: 12px; background: transparent; font-family: 'Segoe UI';");
    infoLayout->addWidget(infoText, 1);

    leftLayout->addWidget(infoCard);

    m_btnEnqueue = new QPushButton("Masuk Antrian");
    m_btnEnqueue->setIcon(QIcon(":/svg/add.svg"));
    m_btnEnqueue->setIconSize(QSize(18, 18));
    m_btnEnqueue->setCursor(Qt::PointingHandCursor);
    m_btnEnqueue->setFixedHeight(45);
    m_btnEnqueue->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4318FF, stop:1 #868CFF); border: none; color: white; border-radius: 15px; font-weight: bold; font-size: 15px; }"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3311DB, stop:1 #6C73E6); }"
        "QPushButton:pressed { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2108B7, stop:1 #4E54C2); }"
    );

    leftLayout->addWidget(m_btnEnqueue);
    leftLayout->addStretch();

    QFrame* rightCard = new QFrame();
    rightCard->setObjectName("card");

    QVBoxLayout* rightLayout = new QVBoxLayout(rightCard);
    rightLayout->setContentsMargins(30, 30, 30, 30);
    rightLayout->setSpacing(20);

    QHBoxLayout* rightHeaderLayout = new QHBoxLayout();
    rightHeaderLayout->setSpacing(12);

    QLabel* queueIcon = new QLabel();
    queueIcon->setPixmap(QPixmap(":/svg/list.svg").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    queueIcon->setFixedSize(24, 24);
    rightHeaderLayout->addWidget(queueIcon);

    QLabel* rightTitle = new QLabel("Daftar Antrian Peminjaman");
    rightTitle->setStyleSheet("font-size: 18px; font-weight: 700; color: #2B3674; font-family: 'Segoe UI';");
    rightHeaderLayout->addWidget(rightTitle);
    rightHeaderLayout->addStretch();

    m_lblQueueSize = new QLabel("Total: 0 antrian");
    m_lblQueueSize->setStyleSheet("font-size: 13px; color: #A3AED0; font-weight: 600; font-family: 'Segoe UI';");
    rightHeaderLayout->addWidget(m_lblQueueSize);

    rightLayout->addLayout(rightHeaderLayout);

    m_tableQueue = new QTableWidget();
    m_tableQueue->setColumnCount(4);
    m_tableQueue->setHorizontalHeaderLabels({"ID", "Nama Peminjam", "Buku Dipinjam", "Tanggal"});
    m_tableQueue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableQueue->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_tableQueue->setColumnWidth(0, 60);
    m_tableQueue->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableQueue->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableQueue->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableQueue->verticalHeader()->setVisible(false);
    m_tableQueue->setAlternatingRowColors(true);
    m_tableQueue->setStyleSheet(
        "QTableWidget { border: none; background-color: #F4F7FE; gridline-color: #E0E5F2; font-family: 'Segoe UI'; }"
        "QTableWidget::item { padding: 8px; color: #2B3674; }"
        "QTableWidget::item:selected { background-color: #EAE9FE; color: #4318FF; }"
        "QHeaderView::section { background-color: #F4F7FE; color: #A3AED0; font-weight: 600; padding: 10px; border: none; border-bottom: 2px solid #E0E5F2; font-size: 12px; }"
    );

    rightLayout->addWidget(m_tableQueue);

    QHBoxLayout* rightButtonLayout = new QHBoxLayout();
    rightButtonLayout->setSpacing(12);

    m_btnProcessNext = new QPushButton("Proses Peminjaman Pertama");
    m_btnProcessNext->setIcon(QIcon(":/svg/rocket.svg"));
    m_btnProcessNext->setIconSize(QSize(18, 18));
    m_btnProcessNext->setCursor(Qt::PointingHandCursor);
    m_btnProcessNext->setFixedHeight(42);
    m_btnProcessNext->setStyleSheet(
        "QPushButton { background-color: #05CD99; border: none; color: white; border-radius: 12px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #04B586; }"
        "QPushButton:pressed { background-color: #039D73; }"
        "QPushButton:disabled { background-color: #DFE6F6; color: #C4CDD5; }"
    );

    m_btnClearQueue = new QPushButton("Hapus Semua");
    m_btnClearQueue->setIcon(QIcon(":/svg/delete.svg"));
    m_btnClearQueue->setIconSize(QSize(16, 16));
    m_btnClearQueue->setCursor(Qt::PointingHandCursor);
    m_btnClearQueue->setFixedHeight(42);
    m_btnClearQueue->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #EE5D50; color: #EE5D50; border-radius: 12px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #FEF5F5; }"
        "QPushButton:pressed { background-color: #FEE8E7; }"
        "QPushButton:disabled { background-color: #F4F7FE; border-color: #DFE6F6; color: #C4CDD5; }"
    );

    rightButtonLayout->addWidget(m_btnProcessNext);
    rightButtonLayout->addWidget(m_btnClearQueue);

    rightLayout->addLayout(rightButtonLayout);

    contentLayout->addWidget(leftCard);
    contentLayout->addWidget(rightCard, 1);

    mainLayout->addLayout(contentLayout);

    connect(m_btnEnqueue, &QPushButton::clicked, this, &BorrowQueuePage::onEnqueueClicked);
    connect(m_btnProcessNext, &QPushButton::clicked, this, &BorrowQueuePage::onProcessNextClicked);
    connect(m_btnClearQueue, &QPushButton::clicked, this, &BorrowQueuePage::onClearQueueClicked);
}

QWidget* BorrowQueuePage::createInputGroup(const QString& labelText, QWidget* inputWidget)
{
    QWidget* group = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(group);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    
    QLabel* label = new QLabel(labelText);
    label->setStyleSheet("color: #2B3674; font-weight: 600; font-size: 13px; font-family: 'Segoe UI';");
    layout->addWidget(label);
    
    layout->addWidget(inputWidget);
    
    return group;
}

void BorrowQueuePage::onEnqueueClicked()
{
    QString nama = m_lineNamaPeminjam->text().trimmed();
    QString buku = m_lineBukuDipinjam->text().trimmed();
    QDate tanggal = m_dateEdit->date();

    if (nama.isEmpty() || buku.isEmpty()) {
        QMessageBox::warning(this, "Input Tidak Lengkap", "Nama peminjam dan buku harus diisi!");
        return;
    }

    BorrowRequest req;
    req.id = m_nextBorrowId++;
    req.namaPeminjam = nama;
    req.bukuDipinjam = buku;
    req.tanggalPinjam = tanggal;

    m_queue.push(req);

    m_lineNamaPeminjam->clear();
    m_lineBukuDipinjam->clear();
    m_dateEdit->setDate(QDate::currentDate());

    updateQueueTable();

    QMessageBox::information(this, "Berhasil", 
        QString("Peminjaman oleh %1 untuk buku '%2' telah masuk antrian!").arg(nama).arg(buku));
}

void BorrowQueuePage::onProcessNextClicked()
{
    if (m_queue.empty()) {
        QMessageBox::information(this, "Antrian Kosong", "Tidak ada antrian yang dapat diproses.");
        return;
    }

    BorrowRequest req = m_queue.front();
    m_queue.pop();

    updateQueueTable();

    QMessageBox::information(this, "Peminjaman Diproses", 
        QString("ID: %1\nNama: %2\nBuku: %3\nTanggal: %4\n\nTelah berhasil diproses!")
            .arg(req.id)
            .arg(req.namaPeminjam)
            .arg(req.bukuDipinjam)
            .arg(req.tanggalPinjam.toString("dd-MM-yyyy")));
}

void BorrowQueuePage::onClearQueueClicked()
{
    if (m_queue.empty()) {
        QMessageBox::information(this, "Antrian Kosong", "Tidak ada antrian untuk dihapus.");
        return;
    }

    auto reply = QMessageBox::question(this, "Konfirmasi", 
        "Apakah Anda yakin ingin menghapus semua antrian?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        while (!m_queue.empty()) {
            m_queue.pop();
        }
        updateQueueTable();
        QMessageBox::information(this, "Berhasil", "Semua antrian telah dihapus.");
    }
}

void BorrowQueuePage::updateQueueTable()
{
    m_tableQueue->setRowCount(0);

    std::queue<BorrowRequest> tempQueue = m_queue;
    int row = 0;

    while (!tempQueue.empty()) {
        BorrowRequest req = tempQueue.front();
        tempQueue.pop();

        m_tableQueue->insertRow(row);
        m_tableQueue->setItem(row, 0, new QTableWidgetItem(QString::number(req.id)));
        m_tableQueue->setItem(row, 1, new QTableWidgetItem(req.namaPeminjam));
        m_tableQueue->setItem(row, 2, new QTableWidgetItem(req.bukuDipinjam));
        m_tableQueue->setItem(row, 3, new QTableWidgetItem(req.tanggalPinjam.toString("dd-MM-yyyy")));

        row++;
    }

    m_lblQueueSize->setText(QString("Total: %1 antrian").arg(m_queue.size()));

    m_btnProcessNext->setEnabled(!m_queue.empty());
    m_btnClearQueue->setEnabled(!m_queue.empty());
}
