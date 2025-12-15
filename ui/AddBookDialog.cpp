#include "AddBookDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QDateTime>
#include <QCoreApplication>
#include <QScrollArea>
#include <QFrame>

AddBookDialog::AddBookDialog(QWidget *parent, Book* editBook)
    : QDialog(parent)
    , m_bookId(0)
    , m_editMode(editBook != nullptr)
{
    // Setup Window Properties
    if (m_editMode) {
        setWindowTitle("Edit Informasi Buku");
    } else {
        setWindowTitle("Tambah Buku Baru");
    }
    
    // Ukuran Dialog yang nyaman
    resize(750, 600); 
    setMinimumSize(700, 550);
    
    setupUI(); // Bangun UI
    
    // Load data jika mode edit
    if (m_editMode) {
        loadBookData(editBook);
    } else {
        m_bookId = 0; // ID baru akan di-handle manager
    }
}

AddBookDialog::~AddBookDialog()
{
}

void AddBookDialog::setupUI()
{
    // 1. Main Layout Dialog
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Set Background Dialog
    setStyleSheet("QDialog { background-color: #F4F7FE; }"); // Light Grey Background

    // 2. Scroll Area (Agar aman di layar kecil)
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; border: none; }"
        "QScrollBar:vertical { background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
    );
    
    QWidget* scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: transparent;");
    QVBoxLayout* contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    // 3. CARD CONTAINER (Kartu Putih di Tengah)
    QFrame* card = new QFrame();
    card->setStyleSheet("background-color: white; border-radius: 20px;");
    
    // Layout di dalam Kartu (2 Kolom: Kiri Form, Kanan Gambar)
    QHBoxLayout* cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(30, 30, 30, 30);
    cardLayout->setSpacing(40);

    // --- KOLOM KIRI (INPUT FIELDS) ---
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);
    leftLayout->setAlignment(Qt::AlignTop);

    // Header Title di dalam card
    QLabel* titleLabel = new QLabel(m_editMode ? "Edit Buku" : "Buku Baru");
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674; margin-bottom: 10px;");
    leftLayout->addWidget(titleLabel);

    // Style Global Input
    QString inputStyle = 
        "QLineEdit, QSpinBox, QDoubleSpinBox, QTextEdit {"
        "   background-color: #F4F7FE; border: none; border-radius: 10px;"
        "   padding: 10px 15px; font-size: 13px; color: #2B3674; font-family: 'Segoe UI';"
        "}"
        "QLineEdit:focus, QTextEdit:focus { background-color: white; border: 2px solid #4318FF; }";
    card->setStyleSheet(card->styleSheet() + inputStyle);

    // Judul
    m_lineJudul = new QLineEdit();
    m_lineJudul->setPlaceholderText("Judul Buku Lengkap");
    m_labelJudulError = new QLabel("Judul wajib diisi");
    m_labelJudulError->setStyleSheet("color: #EE5D50; font-size: 11px;");
    m_labelJudulError->hide();
    leftLayout->addWidget(createInputGroup("Judul Buku *", m_lineJudul, m_labelJudulError));

    // Penulis
    m_linePenulis = new QLineEdit();
    m_linePenulis->setPlaceholderText("Nama Penulis");
    m_labelPenulisError = new QLabel("Penulis wajib diisi");
    m_labelPenulisError->setStyleSheet("color: #EE5D50; font-size: 11px;");
    m_labelPenulisError->hide();
    leftLayout->addWidget(createInputGroup("Penulis *", m_linePenulis, m_labelPenulisError));

    // Row: Tahun & Rating
    QHBoxLayout* rowMeta = new QHBoxLayout();
    rowMeta->setSpacing(15);
    
    m_spinTahun = new QSpinBox();
    m_spinTahun->setRange(1800, 2100);
    m_spinTahun->setValue(QDate::currentDate().year());
    m_spinTahun->setFixedHeight(40);
    
    m_spinRating = new QDoubleSpinBox();
    m_spinRating->setRange(0.0, 5.0);
    m_spinRating->setSingleStep(0.1);
    m_spinRating->setValue(4.5);
    m_spinRating->setFixedHeight(40);
    
    rowMeta->addWidget(createInputGroup("Tahun", m_spinTahun));
    rowMeta->addWidget(createInputGroup("Rating (0-5)", m_spinRating));
    leftLayout->addLayout(rowMeta);

    // Genre
    m_textGenre = new QTextEdit();
    m_textGenre->setPlaceholderText("Fiksi, Sains, Sejarah...");
    m_textGenre->setFixedHeight(70);
    m_labelGenreError = new QLabel("Genre wajib diisi");
    m_labelGenreError->setStyleSheet("color: #EE5D50; font-size: 11px;");
    m_labelGenreError->hide();
    leftLayout->addWidget(createInputGroup("Genre *", m_textGenre, m_labelGenreError));

    // --- KOLOM KANAN (IMAGE UPLOAD) ---
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);
    rightLayout->setAlignment(Qt::AlignTop);

    QLabel* lblCover = new QLabel("Cover Buku");
    lblCover->setStyleSheet("font-weight: 600; color: #2B3674; font-family: 'Segoe UI';");
    rightLayout->addWidget(lblCover);

    // Preview Area
    m_imagePreview = new QLabel("📷\nBelum ada\ngambar");
    m_imagePreview->setFixedSize(200, 280);
    m_imagePreview->setAlignment(Qt::AlignCenter);
    m_imagePreview->setWordWrap(true);
    m_imagePreview->setStyleSheet(
        "border: 2px dashed #A3AED0; border-radius: 15px; "
        "background-color: #F4F7FE; color: #A3AED0; font-weight: bold;"
    );
    rightLayout->addWidget(m_imagePreview);

    // Browse Button
    m_btnSelectImage = new QPushButton("Pilih Cover");
    m_btnSelectImage->setCursor(Qt::PointingHandCursor);
    m_btnSelectImage->setStyleSheet(
        "QPushButton { background-color: #EAEFFC; color: #4318FF; border: none; border-radius: 10px; padding: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #D4DFFC; }"
    );
    rightLayout->addWidget(m_btnSelectImage);

    // Gabungkan Layout
    cardLayout->addLayout(leftLayout, 3); // 60% Width
    cardLayout->addLayout(rightLayout, 2); // 40% Width

    contentLayout->addWidget(card);
    
    // Add stretch content
    contentLayout->addStretch();
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // 4. BUTTONS FRAME (Sticky Bottom)
    QFrame* btnFrame = new QFrame();
    btnFrame->setStyleSheet("background-color: white; border-top: 1px solid #E0E5F2;");
    btnFrame->setFixedHeight(80);
    
    QHBoxLayout* btnLayout = new QHBoxLayout(btnFrame);
    btnLayout->setContentsMargins(30, 0, 30, 0);
    btnLayout->setSpacing(15);
    btnLayout->addStretch(); // Push buttons to right

    // Tombol Batal
    m_btnCancel = new QPushButton("Batal");
    m_btnCancel->setCursor(Qt::PointingHandCursor);
    m_btnCancel->setFixedSize(120, 45);
    m_btnCancel->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #E0E5F2; color: #A3AED0; border-radius: 15px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { border: 1px solid #EE5D50; color: #EE5D50; }"
    );

    // Tombol Simpan
    m_btnSave = new QPushButton(m_editMode ? "Simpan Perubahan" : "Tambah Buku");
    m_btnSave->setCursor(Qt::PointingHandCursor);
    m_btnSave->setFixedSize(180, 45);
    m_btnSave->setStyleSheet(
        "QPushButton { background-color: #4318FF; border: none; color: white; border-radius: 15px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #3311DB; }"
        "QPushButton:pressed { background-color: #2108B7; }"
    );

    btnLayout->addWidget(m_btnCancel);
    btnLayout->addWidget(m_btnSave);

    mainLayout->addWidget(btnFrame);

    // Connect Signals
    connect(m_btnSave, &QPushButton::clicked, this, &AddBookDialog::onSaveClicked);
    connect(m_btnCancel, &QPushButton::clicked, this, &AddBookDialog::onCancelClicked);
    connect(m_btnSelectImage, &QPushButton::clicked, this, &AddBookDialog::onSelectImage);
    connect(m_lineJudul, &QLineEdit::textChanged, this, &AddBookDialog::validateInput);
    connect(m_linePenulis, &QLineEdit::textChanged, this, &AddBookDialog::validateInput);
    connect(m_textGenre, &QTextEdit::textChanged, this, &AddBookDialog::validateInput);
}

// Helper untuk membuat grup input
QWidget* AddBookDialog::createInputGroup(const QString& labelText, QWidget* inputWidget, QLabel* errorLabel)
{
    QWidget* group = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(group);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);
    
    QLabel* label = new QLabel(labelText);
    label->setStyleSheet("color: #2B3674; font-weight: 600; font-size: 13px; font-family: 'Segoe UI';");
    layout->addWidget(label);
    
    layout->addWidget(inputWidget);
    
    if (errorLabel) {
        layout->addWidget(errorLabel);
    }
    
    return group;
}

// --- LOGIC FUNCTIONS (Tetap sama, hanya menyesuaikan nama variabel) ---

void AddBookDialog::loadBookData(Book* book)
{
    if (!book) return;

    m_bookId = book->getId();
    m_lineJudul->setText(book->getJudul());
    m_linePenulis->setText(book->getPenulis());
    m_textGenre->setText(book->getGenre().join(", "));
    m_spinTahun->setValue(book->getTahun());
    m_spinRating->setValue(book->getRating());
    
    QString imagePath = book->getImagePath();
    if (!imagePath.isEmpty() && QFile::exists(imagePath)) {
        m_selectedImagePath = imagePath;
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            m_imagePreview->setPixmap(pixmap.scaled(200, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            // Hapus border dashed jika gambar ada
            m_imagePreview->setStyleSheet("border: 2px solid #E0E5F2; border-radius: 15px; background: white;");
        }
    }
}

bool AddBookDialog::validate()
{
    bool isValid = true;

    if (m_lineJudul->text().trimmed().isEmpty()) {
        m_labelJudulError->show();
        isValid = false;
    } else {
        m_labelJudulError->hide();
    }

    if (m_linePenulis->text().trimmed().isEmpty()) {
        m_labelPenulisError->show();
        isValid = false;
    } else {
        m_labelPenulisError->hide();
    }

    if (m_textGenre->toPlainText().trimmed().isEmpty()) {
        m_labelGenreError->show();
        isValid = false;
    } else {
        m_labelGenreError->hide();
    }

    return isValid;
}

void AddBookDialog::validateInput()
{
    validate();
}

void AddBookDialog::onSaveClicked()
{
    if (!validate()) {
        QMessageBox::warning(this, "Validasi Gagal", "Mohon lengkapi field yang wajib diisi.");
        return;
    }

    QString genreText = m_textGenre->toPlainText().trimmed();
    QStringList genres;
    for (const QString& genre : genreText.split(',')) {
        QString trimmed = genre.trimmed();
        if (!trimmed.isEmpty()) genres.append(trimmed);
    }

    if (genres.isEmpty()) {
        QMessageBox::warning(this, "Validasi Gagal", "Minimal satu genre harus diisi.");
        return;
    }

    m_resultBook = Book(
        m_bookId,
        m_lineJudul->text().trimmed(),
        m_linePenulis->text().trimmed(),
        genres,
        m_spinTahun->value(),
        m_spinRating->value(),
        m_selectedImagePath
    );

    accept(); // Tutup dialog dengan result QDialog::Accepted
}

void AddBookDialog::onCancelClicked()
{
    reject(); // Tutup dialog dengan result QDialog::Rejected
}

void AddBookDialog::onSelectImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Pilih Gambar Cover", QDir::homePath(), "Images (*.png *.jpg *.jpeg *.bmp)"
    );
    
    if (fileName.isEmpty()) return;
    
    // Copy logic (simplified for example)
    QString appPath = QCoreApplication::applicationDirPath();
    QString imagesDir = appPath + "/images";
    QDir dir;
    if (!dir.exists(imagesDir)) dir.mkpath(imagesDir);
    
    QFileInfo fileInfo(fileName);
    QString newFileName = QString("book_%1_%2").arg(QDateTime::currentMSecsSinceEpoch()).arg(fileInfo.fileName());
    QString destPath = imagesDir + "/" + newFileName;
    
    if (QFile::copy(fileName, destPath)) {
        m_selectedImagePath = destPath;
        QPixmap pixmap(destPath);
        if (!pixmap.isNull()) {
            m_imagePreview->setPixmap(pixmap.scaled(200, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            m_imagePreview->setStyleSheet("border: 2px solid #E0E5F2; border-radius: 15px; background: white;");
        }
    } else {
        QMessageBox::warning(this, "Error", "Gagal menyalin gambar!");
    }
}

Book AddBookDialog::getBook() const
{
    return m_resultBook;
}