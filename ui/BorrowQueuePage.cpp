#include "BorrowQueuePage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QScrollArea>

BorrowQueuePage::BorrowQueuePage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    refreshQueue();
}

BorrowQueuePage::~BorrowQueuePage()
{
}

void BorrowQueuePage::setupUI()
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

void BorrowQueuePage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(80);
    headerFrame->setStyleSheet(
        "QFrame { background-color: white; border-bottom: 1px solid #E0E5F2; }"
    );
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    
    QLabel* iconLabel = new QLabel("📋", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent;");
    headerLayout->addWidget(iconLabel);
    
    QLabel* titleLabel = new QLabel("Antrian Peminjaman", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674; margin-left: 10px;");
    headerLayout->addWidget(titleLabel);
    
    headerLayout->addStretch();
    
    // Badge Antrian
    QFrame* badge = new QFrame();
    badge->setStyleSheet("background-color: #EAEFFC; border-radius: 10px; padding: 5px 15px;");
    QHBoxLayout* badgeLayout = new QHBoxLayout(badge);
    badgeLayout->setContentsMargins(0,0,0,0);
    
    m_queueSizeLabel = new QLabel("Antrian: 0", badge);
    m_queueSizeLabel->setStyleSheet("color: #4318FF; font-weight: bold; font-family: 'Segoe UI'; font-size: 14px;");
    badgeLayout->addWidget(m_queueSizeLabel);
    
    headerLayout->addWidget(badge);
    
    mainLayout->addWidget(headerFrame);
}

QFrame* BorrowQueuePage::createCardFrame()
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

void BorrowQueuePage::createContentSection(QVBoxLayout* contentLayout)
{
    // Layout Horizontal: Kiri (Form Input) - Kanan (Tabel Antrian)
    QHBoxLayout* splitLayout = new QHBoxLayout();
    splitLayout->setSpacing(25);
    splitLayout->setAlignment(Qt::AlignTop);

    // --- PANEL KIRI: FORMULIR ---
    QFrame* leftPanel = createCardFrame();
    leftPanel->setFixedWidth(350); // Fixed width untuk panel kiri
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(25, 25, 25, 25);
    leftLayout->setSpacing(20);
    
    QLabel* lblFormTitle = new QLabel("Tambah Antrian", leftPanel);
    lblFormTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    leftLayout->addWidget(lblFormTitle);
    
    // Info Box (Konsep FIFO)
    QLabel* infoBox = new QLabel(
        "ℹ️ <b>Konsep FIFO (First In First Out)</b><br>"
        "Orang yang mendaftar pertama akan dilayani pertama. Antrian bersifat adil."
    );
    infoBox->setWordWrap(true);
    infoBox->setStyleSheet(
        "background-color: #E6F7FF; color: #0095FF; border: 1px solid #BAE7FF; "
        "border-radius: 8px; padding: 12px; font-size: 12px; line-height: 1.4;"
    );
    leftLayout->addWidget(infoBox);

    // Input Fields (Style konsisten dengan AddBookPage)
    // Styling Global untuk Panel Kiri
    leftPanel->setStyleSheet(leftPanel->styleSheet() + 
        "QLineEdit, QComboBox {"
        "   background-color: white;"
        "   border: 1px solid #94A3B8;" /* Border Jelas */
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "   font-family: 'Segoe UI';"
        "}"
        "QLineEdit:focus, QComboBox:focus {"
        "   border: 2px solid #4318FF;"
        "}"
        "QComboBox::drop-down { border: none; width: 30px; }"
        "QComboBox::down-arrow { image: none; border-left: 2px solid transparent; border-top: 2px solid #64748B; width: 8px; height: 8px; margin-right: 10px; }"
    );

    m_borrowerNameInput = new QLineEdit();
    m_borrowerNameInput->setPlaceholderText("Nama Peminjam...");
    leftLayout->addWidget(createInputGroup("Nama Peminjam", m_borrowerNameInput));

    m_bookCombo = new QComboBox();
    leftLayout->addWidget(createInputGroup("Pilih Buku", m_bookCombo));

    // Tombol Tambah
    m_btnAddToBorrow = new QPushButton("Masuk Antrian");
    m_btnAddToBorrow->setCursor(Qt::PointingHandCursor);
    m_btnAddToBorrow->setFixedHeight(45);
    m_btnAddToBorrow->setStyleSheet(
        "QPushButton { background-color: #4318FF; color: white; border-radius: 10px; font-weight: bold; font-size: 14px; border: none; }"
        "QPushButton:hover { background-color: #3311DB; }"
        "QPushButton:pressed { background-color: #2108B7; }"
    );
    leftLayout->addWidget(m_btnAddToBorrow);
    leftLayout->addStretch(); // Push content up

    splitLayout->addWidget(leftPanel);

    // --- PANEL KANAN: TABEL ANTRIAN ---
    QFrame* rightPanel = createCardFrame();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(25, 25, 25, 25);
    rightLayout->setSpacing(15);
    
    // Header Panel Kanan
    QHBoxLayout* rightHeader = new QHBoxLayout();
    QLabel* lblTableTitle = new QLabel("Daftar Antrian", rightPanel);
    lblTableTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    rightHeader->addWidget(lblTableTitle);
    rightHeader->addStretch();
    
    // Tombol Aksi Kanan Atas
    m_btnClearQueue = new QPushButton("Kosongkan Antrian");
    m_btnClearQueue->setCursor(Qt::PointingHandCursor);
    m_btnClearQueue->setFixedSize(140, 35);
    m_btnClearQueue->setStyleSheet(
        "QPushButton { background-color: white; color: #EE5D50; border: 1px solid #EE5D50; border-radius: 8px; font-weight: 600; font-size: 12px; }"
        "QPushButton:hover { background-color: #FFF5F5; }"
    );
    rightHeader->addWidget(m_btnClearQueue);
    rightLayout->addLayout(rightHeader);

    // Tabel Antrian
    m_tableQueue = new QTableWidget();
    m_tableQueue->setColumnCount(5);
    m_tableQueue->setHorizontalHeaderLabels({"No", "Nama Peminjam", "ID Buku", "Judul Buku", "Waktu"});
    
    // Table Styling (Modern Grid)
    m_tableQueue->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: 1px solid #E0E5F2;"
        "   border-radius: 8px;"
        "   gridline-color: transparent;"
        "   font-family: 'Segoe UI';"
        "}"
        "QTableWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #F4F7FE;"
        "   color: #2B3674;"
        "   font-size: 13px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #F4F7FE;"
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
        "   font-size: 12px;"
        "}"
    );
    
    m_tableQueue->verticalHeader()->setVisible(false);
    m_tableQueue->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableQueue->setShowGrid(false);
    m_tableQueue->setFocusPolicy(Qt::NoFocus);
    m_tableQueue->horizontalHeader()->setStretchLastSection(true);
    m_tableQueue->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Nama stretch
    m_tableQueue->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch); // Judul stretch
    
    rightLayout->addWidget(m_tableQueue);

    // Bottom Action (Process Next)
    m_btnProcessNext = new QPushButton("🚀 Proses Peminjam Berikutnya (Next)");
    m_btnProcessNext->setCursor(Qt::PointingHandCursor);
    m_btnProcessNext->setFixedHeight(50);
    m_btnProcessNext->setStyleSheet(
        "QPushButton { background-color: #05CD99; color: white; border-radius: 12px; font-weight: bold; font-size: 15px; border: none; }"
        "QPushButton:hover { background-color: #04B889; }"
        "QPushButton:pressed { background-color: #039E75; }"
    );
    rightLayout->addWidget(m_btnProcessNext);

    splitLayout->addWidget(rightPanel, 1); // Expand right panel
    contentLayout->addLayout(splitLayout);
}

QWidget* BorrowQueuePage::createInputGroup(const QString& labelText, QWidget* inputWidget)
{
    QWidget* group = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(group);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);
    
    QLabel* label = new QLabel(labelText);
    label->setStyleSheet("color: #2B3674; font-weight: 600; font-size: 13px; font-family: 'Segoe UI';");
    layout->addWidget(label);
    
    layout->addWidget(inputWidget);
    return group;
}

void BorrowQueuePage::refreshQueue()
{
    DatabaseManager& db = DatabaseManager::instance();
    BookManager& bookManager = db.getBookManager();
    
    // Update Badge
    size_t queueSize = bookManager.getBorrowQueueSize();
    m_queueSizeLabel->setText(QString("Antrian: %1").arg(queueSize));
    
    // Update Combo Box
    m_bookCombo->clear();
    std::vector<Book> books = bookManager.getAllBooks();
    for (const Book& book : books) {
        m_bookCombo->addItem(QString("%1 - %2").arg(book.getId()).arg(book.getJudul()), book.getId());
    }
    
    loadQueueToTable();
}

void BorrowQueuePage::loadQueueToTable()
{
    DatabaseManager& db = DatabaseManager::instance();
    BookManager& bookManager = db.getBookManager();
    
    std::vector<BookManager::BorrowRequest> requests = bookManager.getAllBorrowRequests();
    m_tableQueue->setRowCount(requests.size());
    
    for (size_t i = 0; i < requests.size(); i++) {
        // No
        QTableWidgetItem* noItem = new QTableWidgetItem(QString::number(i + 1));
        noItem->setTextAlignment(Qt::AlignCenter);
        m_tableQueue->setItem(i, 0, noItem);
        
        // Nama (Bold)
        QTableWidgetItem* nameItem = new QTableWidgetItem(requests[i].borrowerName);
        nameItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        m_tableQueue->setItem(i, 1, nameItem);
        
        // ID Buku
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(requests[i].bookId));
        idItem->setTextAlignment(Qt::AlignCenter);
        m_tableQueue->setItem(i, 2, idItem);
        
        // Judul
        m_tableQueue->setItem(i, 3, new QTableWidgetItem(requests[i].bookTitle));
        
        // Waktu
        m_tableQueue->setItem(i, 4, new QTableWidgetItem(requests[i].requestTime));
    }
}

// --- LOGIC HANDLERS (Tidak Berubah) ---

void BorrowQueuePage::onAddToBorrow()
{
    QString borrowerName = m_borrowerNameInput->text().trimmed();
    
    if (borrowerName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Nama peminjam tidak boleh kosong!");
        return;
    }
    
    if (m_bookCombo->count() == 0) {
        QMessageBox::warning(this, "Error", "Tidak ada buku tersedia!");
        return;
    }
    
    int bookId = m_bookCombo->currentData().toInt();
    
    DatabaseManager& db = DatabaseManager::instance();
    BookManager& bookManager = db.getBookManager();
    
    if (bookManager.addToBorrowQueue(borrowerName, bookId)) {
        QMessageBox::information(this, "Sukses", 
            QString("Peminjam '%1' berhasil masuk antrian!").arg(borrowerName));
        
        m_borrowerNameInput->clear();
        refreshQueue();
    } else {
        QMessageBox::warning(this, "Error", "Gagal menambahkan ke antrian!");
    }
}

void BorrowQueuePage::onProcessNext()
{
    DatabaseManager& db = DatabaseManager::instance();
    BookManager& bookManager = db.getBookManager();
    
    if (bookManager.isBorrowQueueEmpty()) {
        QMessageBox::information(this, "Antrian Kosong", "Tidak ada peminjam dalam antrian saat ini.");
        return;
    }
    
    BookManager::BorrowRequest request = bookManager.processNextBorrow();
    
    if (!request.borrowerName.isEmpty()) {
        QString msg = QString(
            "✅ <b>Peminjaman Diproses</b><br><br>"
            "Peminjam: <b>%1</b><br>"
            "Buku: <b>%2</b><br>"
            "<br><i>Antrian berkurang 1</i>"
        ).arg(request.borrowerName).arg(request.bookTitle);
        
        QMessageBox::information(this, "Sukses", msg);
        refreshQueue();
    }
}

void BorrowQueuePage::onClearQueue()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Konfirmasi", 
        "Yakin ingin menghapus SELURUH antrian? Tindakan ini tidak dapat dibatalkan.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        DatabaseManager::instance().getBookManager().clearBorrowQueue();
        refreshQueue();
        QMessageBox::information(this, "Info", "Antrian berhasil dikosongkan.");
    }
}