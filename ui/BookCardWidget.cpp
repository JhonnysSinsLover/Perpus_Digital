#include "BookCardWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

BookCardWidget::BookCardWidget(const Book& book, QWidget *parent)
    : QWidget(parent)
    , m_book(book)
{
    // Ukuran kartu yang proporsional
    setFixedSize(220, 340);
    setCursor(Qt::PointingHandCursor);
    
    setupUI();
    applyStyles();
    loadImage();
}

void BookCardWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // --- 1. IMAGE AREA (ATAS) ---
    m_imageContainer = new QWidget(this);
    m_imageContainer->setFixedHeight(240); // Rasio Cover
    m_imageContainer->setStyleSheet("background-color: #F4F7FE; border-top-left-radius: 15px; border-top-right-radius: 15px;");
    
    QVBoxLayout* imgLayout = new QVBoxLayout(m_imageContainer);
    imgLayout->setContentsMargins(0, 0, 0, 0);
    
    m_imageLabel = new QLabel(m_imageContainer);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    imgLayout->addWidget(m_imageLabel);
    
    // Hover Overlay (Hidden by default)
    m_hoverOverlay = new QFrame(m_imageContainer);
    m_hoverOverlay->setGeometry(0, 0, 220, 240);
    m_hoverOverlay->setStyleSheet(
        "background-color: rgba(43, 54, 116, 0.8);" // Biru Navy Transparan
        "border-top-left-radius: 15px;"
        "border-top-right-radius: 15px;"
    );
    m_hoverOverlay->hide();
    
    // Tombol di Overlay
    QHBoxLayout* overlayLayout = new QHBoxLayout(m_hoverOverlay);
    overlayLayout->setAlignment(Qt::AlignCenter);
    overlayLayout->setSpacing(15);
    
    auto createBtn = [](QString icon, QString color) -> QPushButton* {
        QPushButton* btn = new QPushButton(icon);
        btn->setFixedSize(45, 45);
        btn->setStyleSheet(QString(
            "QPushButton { background-color: white; border-radius: 22px; color: %1; font-size: 20px; border: none; }"
            "QPushButton:hover { background-color: %1; color: white; }"
        ).arg(color));
        return btn;
    };
    
    m_editBtn = createBtn("✏️", "#4318FF"); // Biru
    connect(m_editBtn, &QPushButton::clicked, [this]() { emit editRequested(m_book.getId()); });
    
    m_deleteBtn = createBtn("🗑️", "#EE5D50"); // Merah
    connect(m_deleteBtn, &QPushButton::clicked, [this]() { emit deleteRequested(m_book.getId()); });
    
    overlayLayout->addWidget(m_editBtn);
    overlayLayout->addWidget(m_deleteBtn);
    
    mainLayout->addWidget(m_imageContainer);
    
    // --- 2. INFO AREA (BAWAH) ---
    QWidget* infoContainer = new QWidget(this);
    infoContainer->setStyleSheet("background-color: white; border-bottom-left-radius: 15px; border-bottom-right-radius: 15px;");
    
    QVBoxLayout* infoLayout = new QVBoxLayout(infoContainer);
    infoLayout->setContentsMargins(15, 12, 15, 15);
    infoLayout->setSpacing(4);
    
    // Genre Pill (Kecil di atas judul)
    QString firstGenre = m_book.getGenre().value(0, "Umum");
    m_genreLabel = new QLabel(firstGenre.toUpper(), infoContainer);
    m_genreLabel->setStyleSheet(
        "color: #4318FF; font-size: 10px; font-weight: bold; font-family: 'Segoe UI';"
        "background-color: #EAEFFC; padding: 2px 6px; border-radius: 4px;"
    );
    m_genreLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    infoLayout->addWidget(m_genreLabel);

    // Title
    m_titleLabel = new QLabel(m_book.getJudul(), infoContainer);
    m_titleLabel->setStyleSheet(
        "font-family: 'Segoe UI'; font-size: 14px; font-weight: 700; color: #2B3674;"
    );
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setMaximumHeight(40); // Max 2 baris
    infoLayout->addWidget(m_titleLabel);
    
    // Author
    m_authorLabel = new QLabel(m_book.getPenulis(), infoContainer);
    m_authorLabel->setStyleSheet(
        "font-family: 'Segoe UI'; font-size: 12px; color: #A3AED0;"
    );
    infoLayout->addWidget(m_authorLabel);
    
    infoLayout->addStretch();
    
    // Footer (Rating & Tahun)
    QHBoxLayout* footerLayout = new QHBoxLayout();
    
    m_ratingLabel = new QLabel(QString("⭐ %1").arg(m_book.getRating(), 0, 'f', 1));
    m_ratingLabel->setStyleSheet("color: #FFB547; font-weight: bold; font-size: 12px;");
    
    m_yearLabel = new QLabel(QString::number(m_book.getTahun()));
    m_yearLabel->setStyleSheet("color: #A3AED0; font-size: 12px;");
    
    footerLayout->addWidget(m_ratingLabel);
    footerLayout->addStretch();
    footerLayout->addWidget(m_yearLabel);
    
    infoLayout->addLayout(footerLayout);
    
    mainLayout->addWidget(infoContainer);
}

void BookCardWidget::applyStyles()
{
    // Base Style: White Card with subtle Border
    setStyleSheet(
        "BookCardWidget {"
        "   background-color: white;"
        "   border: 1px solid #E0E5F2;"
        "   border-radius: 15px;"
        "}"
    );
}

void BookCardWidget::enterEvent(QEnterEvent* event)
{
    QWidget::enterEvent(event);
    m_hoverOverlay->show();
    // Highlight Border saat Hover
    setStyleSheet(
        "BookCardWidget {"
        "   background-color: white;"
        "   border: 2px solid #4318FF;" // Border Biru
        "   border-radius: 15px;"
        "}"
    );
}

void BookCardWidget::leaveEvent(QEvent* event)
{
    QWidget::leaveEvent(event);
    m_hoverOverlay->hide();
    // Kembali ke Normal
    setStyleSheet(
        "BookCardWidget {"
        "   background-color: white;"
        "   border: 1px solid #E0E5F2;"
        "   border-radius: 15px;"
        "}"
    );
}

void BookCardWidget::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (!m_editBtn->underMouse() && !m_deleteBtn->underMouse()) {
            emit cardClicked(m_book.getId());
        }
    }
}

QColor BookCardWidget::getPastelColor(const QString& text)
{
    // Generate warna pastel konsisten berdasarkan text (hash)
    unsigned int hash = qHash(text);
    int h = hash % 360;
    return QColor::fromHsl(h, 100, 230); // Saturation rendah, Lightness tinggi (Pastel)
}

void BookCardWidget::loadImage()
{
    QString imagePath = m_book.getImagePath();
    
    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            // Scale and keep aspect ratio, tapi isi penuh kotak
            m_imageLabel->setPixmap(pixmap.scaled(220, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            return;
        }
    }
    
    // --- MODERN PLACEHOLDER ---
    // Jika tidak ada gambar, buat placeholder pastel yang cantik
    QPixmap placeholder(220, 240);
    
    // Warna background berdasarkan Judul agar bervariasi
    placeholder.fill(getPastelColor(m_book.getJudul()));
    
    QPainter painter(&placeholder);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Gambar Ikon Buku Besar di tengah (Warna Putih Semi-Transparan)
    QFont iconFont = painter.font();
    iconFont.setPixelSize(60);
    painter.setFont(iconFont);
    painter.setPen(QColor(255, 255, 255, 180));
    painter.drawText(placeholder.rect(), Qt::AlignCenter, "📚");
    
    m_imageLabel->setPixmap(placeholder);
}