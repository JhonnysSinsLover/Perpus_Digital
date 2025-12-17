#include "BookPreviewDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>
#include <QPainter>
#include <QFile>

BookPreviewDialog::BookPreviewDialog(const Book& book, QWidget *parent)
    : QDialog(parent)
    , m_book(book)
{
    setModal(true);
    setWindowTitle("Detail Buku");
    
    // Ukuran dialog yang pas
    resize(700, 550);
    setMinimumSize(650, 500);
    
    setupUI();
    loadImage(); // Load gambar saat inisialisasi
}

BookPreviewDialog::~BookPreviewDialog()
{
}

void BookPreviewDialog::setupUI()
{
    // 1. Main Layout (Full Screen Vertical)
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Background Dialog
    setStyleSheet("QDialog { background-color: #F4F7FE; }");

    // --- HEADER SECTION ---
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(70);
    headerFrame->setStyleSheet("background-color: white; border-bottom: 1px solid #E0E5F2;");
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(15);
    
    QLabel* iconLabel = new QLabel("📖", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent;");
    headerLayout->addWidget(iconLabel);
    
    QLabel* titleLabel = new QLabel("Detail Informasi Buku", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 20px; font-weight: 700; color: #2B3674;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    mainLayout->addWidget(headerFrame);

    // --- CONTENT SECTION (SCROLLABLE) ---
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("background: transparent; border: none;");

    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #F4F7FE;");
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    
    // CARD CONTAINER (Putih di tengah)
    QFrame* card = new QFrame();
    card->setStyleSheet("background-color: white; border-radius: 16px; border: 1px solid #E0E5F2;");
    // Shadow Effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0,0,0, 15));
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);

    QHBoxLayout* cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(30, 30, 30, 30);
    cardLayout->setSpacing(30);

    // [KOLOM KIRI] COVER IMAGE
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignTop);
    
    m_lblCoverImage = new QLabel();
    m_lblCoverImage->setFixedSize(220, 320); // Ukuran cover standar
    m_lblCoverImage->setStyleSheet("border-radius: 12px; border: 1px solid #E0E5F2;");
    m_lblCoverImage->setAlignment(Qt::AlignCenter);
    
    leftLayout->addWidget(m_lblCoverImage);
    cardLayout->addLayout(leftLayout);

    // [KOLOM KANAN] DETAIL INFO
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);
    rightLayout->setAlignment(Qt::AlignTop);

    // 1. Judul Besar
    m_lblTitle = new QLabel(m_book.getJudul());
    m_lblTitle->setWordWrap(true);
    m_lblTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 800; color: #2B3674; margin-bottom: 5px;");
    rightLayout->addWidget(m_lblTitle);

    // 2. Info Bar (Genre & Tahun)
    QHBoxLayout* metaLayout = new QHBoxLayout();
    metaLayout->setSpacing(10);
    
    // Genre Badges
    QStringList genres = m_book.getGenre();
    for(const QString& g : genres) {
        if(g.trimmed().isEmpty()) continue;
        QLabel* badge = new QLabel(g.trimmed().toUpper());
        badge->setStyleSheet("background-color: #EAEFFC; color: #4318FF; font-weight: bold; font-size: 11px; padding: 4px 8px; border-radius: 6px;");
        metaLayout->addWidget(badge);
    }
    
    // Tahun
    QLabel* yearLabel = new QLabel(QString("📅 %1").arg(m_book.getTahun()));
    yearLabel->setStyleSheet("color: #A3AED0; font-weight: 600; font-size: 13px; margin-left: 10px;");
    metaLayout->addWidget(yearLabel);
    
    metaLayout->addStretch();
    rightLayout->addLayout(metaLayout);

    // Separator Line
    QFrame* line = new QFrame();
    line->setFixedHeight(1);
    line->setStyleSheet("background-color: #E0E5F2; margin: 10px 0;");
    rightLayout->addWidget(line);

    // 3. Detail Fields
    rightLayout->addWidget(createInfoRow("✍️", "Penulis", m_book.getPenulis()));
    rightLayout->addWidget(createInfoRow("⭐", "Rating", QString("%1 / 5.0").arg(m_book.getRating())));
    rightLayout->addWidget(createInfoRow("🆔", "ID Buku", QString::number(m_book.getId())));
    
    // 4. Status Ketersediaan (Dummy Logic for UI)
    // Di sini bisa ditambahkan logika cek stok real jika ada
    rightLayout->addWidget(createInfoRow("✅", "Status", "Tersedia untuk dipinjam", true));

    rightLayout->addStretch();
    cardLayout->addLayout(rightLayout);

    contentLayout->addWidget(card);
    contentLayout->addStretch();
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    // --- FOOTER SECTION (Sticky Bottom) ---
    QFrame* footerFrame = new QFrame(this);
    footerFrame->setFixedHeight(80);
    footerFrame->setStyleSheet("background-color: white; border-top: 1px solid #E0E5F2;");
    
    QHBoxLayout* footerLayout = new QHBoxLayout(footerFrame);
    footerLayout->setContentsMargins(30, 0, 30, 0);
    footerLayout->setSpacing(15);
    
    m_btnClose = new QPushButton("Tutup");
    m_btnClose->setCursor(Qt::PointingHandCursor);
    m_btnClose->setFixedSize(120, 45);
    m_btnClose->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #E0E5F2; color: #64748B; border-radius: 10px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { border: 1px solid #EE5D50; color: #EE5D50; }"
    );
    connect(m_btnClose, &QPushButton::clicked, this, &QDialog::accept);

    m_btnBorrow = new QPushButton("Pinjam Buku Ini");
    m_btnBorrow->setCursor(Qt::PointingHandCursor);
    m_btnBorrow->setFixedSize(180, 45);
    m_btnBorrow->setStyleSheet(
        "QPushButton { background-color: #4318FF; border: none; color: white; border-radius: 10px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #3311DB; }"
        "QPushButton:pressed { background-color: #2108B7; }"
    );
    connect(m_btnBorrow, &QPushButton::clicked, this, &BookPreviewDialog::onBorrowBook);

    footerLayout->addStretch();
    footerLayout->addWidget(m_btnClose);
    footerLayout->addWidget(m_btnBorrow);
    
    mainLayout->addWidget(footerFrame);
}

// Helper untuk membuat baris info rapi
QWidget* BookPreviewDialog::createInfoRow(const QString& icon, const QString& label, const QString& value, bool isBadge)
{
    QWidget* row = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    
    QLabel* lblIcon = new QLabel(icon);
    lblIcon->setFixedSize(24, 24);
    lblIcon->setAlignment(Qt::AlignCenter);
    lblIcon->setStyleSheet("font-size: 16px;");
    
    QLabel* lblLabel = new QLabel(label + ":");
    lblLabel->setFixedWidth(80);
    lblLabel->setStyleSheet("color: #A3AED0; font-weight: 600; font-size: 13px;");
    
    QLabel* lblValue = new QLabel(value);
    if (isBadge) {
        lblValue->setStyleSheet("color: #05CD99; font-weight: bold; font-size: 13px;");
    } else {
        lblValue->setStyleSheet("color: #2B3674; font-weight: 600; font-size: 14px;");
    }
    
    layout->addWidget(lblIcon);
    layout->addWidget(lblLabel);
    layout->addWidget(lblValue);
    layout->addStretch();
    
    return row;
}

QColor BookPreviewDialog::getPastelColor(const QString& text)
{
    unsigned int hash = qHash(text);
    int h = hash % 360;
    return QColor::fromHsl(h, 100, 230);
}

void BookPreviewDialog::loadImage()
{
    QString imagePath = m_book.getImagePath();
    
    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            m_lblCoverImage->setPixmap(pixmap.scaled(220, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            return;
        }
    }
    
    // --- PLACEHOLDER GENERATOR (Sama seperti BookCardWidget) ---
    QPixmap placeholder(220, 320);
    placeholder.fill(getPastelColor(m_book.getJudul()));
    
    QPainter painter(&placeholder);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QFont iconFont = painter.font();
    iconFont.setPixelSize(80); // Ikon lebih besar dari kartu
    painter.setFont(iconFont);
    painter.setPen(QColor(255, 255, 255, 180));
    painter.drawText(placeholder.rect(), Qt::AlignCenter, "📚");
    
    m_lblCoverImage->setPixmap(placeholder);
}

void BookPreviewDialog::onBorrowBook()
{
    bool ok;
    QString borrowerName = QInputDialog::getText(this, "Peminjaman Buku",
        QString("Masukkan nama peminjam untuk '%1':").arg(m_book.getJudul()),
        QLineEdit::Normal, "", &ok);
    
    if (ok && !borrowerName.trimmed().isEmpty()) {
        DatabaseManager& dbManager = DatabaseManager::instance();
        
        if (dbManager.getBookManager().addToBorrowQueue(borrowerName.trimmed(), m_book.getId())) {
            
            // Emit signal dulu sebelum tutup
            emit bookBorrowed(m_book.getId(), borrowerName.trimmed());
            
            // Pesan sukses akan ditangani oleh parent (BooksCollectionPage) atau di sini
            // Tapi karena parent sudah punya logic connect, kita cukup emit & accept.
            // Biar aman, kita close dialognya.
            accept(); 
            
        } else {
            QMessageBox::warning(this, "Gagal", "Gagal menambahkan ke antrian! (Antrian mungkin penuh)");
        }
    } else if (ok) {
        QMessageBox::warning(this, "Error", "Nama peminjam tidak boleh kosong!");
    }
}