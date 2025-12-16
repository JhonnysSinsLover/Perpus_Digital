#include "BookCardWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

BookCardWidget::BookCardWidget(const Book& book, QWidget *parent, bool showEditDelete)
    : QWidget(parent)
    , m_book(book)
{
    // Ukuran dasar (bisa melar karena SizePolicy di parent)
    setFixedSize(220, 340);
    setCursor(Qt::PointingHandCursor);
    
    setupUI();
    applyStyles();
    
    refreshContent();
    
    // Set visibility tombol edit/delete
    setEditDeleteVisible(showEditDelete);
}

void BookCardWidget::updateData(const Book& book)
{
    m_book = book;
    refreshContent(); 
}

void BookCardWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // --- 1. IMAGE AREA (ATAS) ---
    m_imageContainer = new QWidget(this);
    m_imageContainer->setFixedHeight(240); 
    m_imageContainer->setStyleSheet("background-color: #F4F7FE; border-top-left-radius: 15px; border-top-right-radius: 15px;");
    
    QVBoxLayout* imgLayout = new QVBoxLayout(m_imageContainer);
    imgLayout->setContentsMargins(0, 0, 0, 0);
    
    m_imageLabel = new QLabel(m_imageContainer);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    imgLayout->addWidget(m_imageLabel);
    
    mainLayout->addWidget(m_imageContainer);
    
    // --- 2. INFO AREA (BAWAH) ---
    QWidget* infoContainer = new QWidget(this);
    infoContainer->setStyleSheet("background-color: white; border-bottom-left-radius: 15px; border-bottom-right-radius: 15px;");
    
    QVBoxLayout* infoLayout = new QVBoxLayout(infoContainer);
    infoLayout->setContentsMargins(15, 12, 15, 15);
    infoLayout->setSpacing(4);
    
    // Genre Pill
    m_genreLabel = new QLabel(infoContainer);
    m_genreLabel->setStyleSheet(
        "color: #4318FF; font-size: 10px; font-weight: bold; font-family: 'Segoe UI';"
        "background-color: #EAEFFC; padding: 2px 6px; border-radius: 4px;"
    );
    m_genreLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    infoLayout->addWidget(m_genreLabel);

    // Title
    m_titleLabel = new QLabel(infoContainer);
    m_titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 14px; font-weight: 700; color: #2B3674;");
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setMaximumHeight(40); 
    infoLayout->addWidget(m_titleLabel);
    
    // Author
    m_authorLabel = new QLabel(infoContainer);
    m_authorLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 12px; color: #A3AED0;");
    infoLayout->addWidget(m_authorLabel);
    
    infoLayout->addStretch();
    
    // Footer (Rating & Tahun)
    QHBoxLayout* footerLayout = new QHBoxLayout();
    m_ratingLabel = new QLabel();
    m_ratingLabel->setStyleSheet("color: #FFB547; font-weight: bold; font-size: 12px;");
    
    m_yearLabel = new QLabel();
    m_yearLabel->setStyleSheet("color: #A3AED0; font-size: 12px;");
    
    footerLayout->addWidget(m_ratingLabel);
    footerLayout->addStretch();
    footerLayout->addWidget(m_yearLabel);
    
    infoLayout->addLayout(footerLayout);
    mainLayout->addWidget(infoContainer);
    
    // --- 3. HOVER OVERLAY (FULL CARD) ---
    // Parentnya 'this' agar menutupi seluruh widget, bukan cuma gambar
    m_hoverOverlay = new QFrame(this);
    // Style: Gelap transparan dengan radius sama dengan kartu utama
    m_hoverOverlay->setStyleSheet(
        "background-color: rgba(43, 54, 116, 0.85);" // Biru Navy Transparan
        "border-radius: 15px;" // Match border radius kartu
    );
    m_hoverOverlay->hide();
    
    QHBoxLayout* overlayLayout = new QHBoxLayout(m_hoverOverlay);
    overlayLayout->setAlignment(Qt::AlignCenter);
    overlayLayout->setSpacing(20);
    
    auto createBtn = [](QString icon, QString color) -> QPushButton* {
        QPushButton* btn = new QPushButton(icon);
        btn->setFixedSize(50, 50); // Tombol sedikit lebih besar agar enak diklik
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(
            "QPushButton { background-color: white; border-radius: 25px; color: %1; font-size: 22px; border: none; }"
            "QPushButton:hover { background-color: %1; color: white; transform: scale(1.1); }"
        ).arg(color));
        return btn;
    };
    
    m_editBtn = createBtn("✏️", "#4318FF"); // Biru
    connect(m_editBtn, &QPushButton::clicked, [this]() { emit editRequested(m_book.getId()); });
    
    m_deleteBtn = createBtn("🗑️", "#EE5D50"); // Merah
    connect(m_deleteBtn, &QPushButton::clicked, [this]() { emit deleteRequested(m_book.getId()); });
    
    overlayLayout->addWidget(m_editBtn);
    overlayLayout->addWidget(m_deleteBtn);
}

// [PENTING] Menjamin overlay selalu mengikuti ukuran kartu saat di-resize (responsive)
void BookCardWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (m_hoverOverlay) {
        m_hoverOverlay->resize(this->size());
        m_hoverOverlay->raise(); // Pastikan selalu di paling atas
    }
}

void BookCardWidget::refreshContent()
{
    QString firstGenre = m_book.getGenre().value(0, "UMUM");
    if(firstGenre.isEmpty()) firstGenre = "UMUM";
    m_genreLabel->setText(firstGenre.toUpper());
    
    m_titleLabel->setText(m_book.getJudul());
    m_authorLabel->setText(m_book.getPenulis());
    m_ratingLabel->setText(QString("⭐ %1").arg(m_book.getRating(), 0, 'f', 1));
    m_yearLabel->setText(QString::number(m_book.getTahun()));
    
    loadImage();
}

void BookCardWidget::applyStyles()
{
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
    m_hoverOverlay->resize(this->size()); // Pastikan ukuran pas sebelum tampil
    m_hoverOverlay->raise();
    m_hoverOverlay->show();
    
    // Border biru saat hover (di balik overlay)
    setStyleSheet(
        "BookCardWidget {"
        "   background-color: white;"
        "   border: 2px solid #4318FF;"
        "   border-radius: 15px;"
        "}"
    );
}

void BookCardWidget::leaveEvent(QEvent* event)
{
    QWidget::leaveEvent(event);
    m_hoverOverlay->hide();
    
    // Reset border
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

void BookCardWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
         if (!m_editBtn->underMouse() && !m_deleteBtn->underMouse()) {
            emit previewRequested(m_book.getId());
         }
    }
}

QColor BookCardWidget::getPastelColor(const QString& text)
{
    unsigned int hash = qHash(text);
    int h = hash % 360;
    return QColor::fromHsl(h, 100, 230);
}

void BookCardWidget::loadImage()
{
    QString imagePath = m_book.getImagePath();
    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            // Menggunakan KeepAspectRatioByExpanding agar gambar memenuhi area tanpa gepeng
            m_imageLabel->setPixmap(pixmap.scaled(220, 240, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            return;
        }
    }
    
    QPixmap placeholder(220, 240);
    placeholder.fill(getPastelColor(m_book.getJudul()));
    QPainter painter(&placeholder);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont iconFont = painter.font();
    iconFont.setPixelSize(60);
    painter.setFont(iconFont);
    painter.setPen(QColor(255, 255, 255, 180));
    painter.drawText(placeholder.rect(), Qt::AlignCenter, "📚");
    m_imageLabel->setPixmap(placeholder);
}

void BookCardWidget::setEditDeleteVisible(bool visible)
{
    if (m_editBtn) {
        m_editBtn->setVisible(visible);
    }
    if (m_deleteBtn) {
        m_deleteBtn->setVisible(visible);
    }
}