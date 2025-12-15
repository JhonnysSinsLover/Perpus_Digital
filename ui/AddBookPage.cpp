#include "AddBookPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QDate>
#include <QScrollArea>
#include <QStyle>
#include <QGraphicsDropShadowEffect>

AddBookPage::AddBookPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    // Validasi awal: sembunyikan pesan error
    m_labelJudulError->hide();
    m_labelPenulisError->hide();
    m_labelGenreError->hide();
}

AddBookPage::~AddBookPage()
{
}

void AddBookPage::setupUI()
{
    // 1. Layout Utama
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 2. Header
    createHeaderSection(mainLayout);

    // 3. Scroll Area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    // Background Abu-abu kebiruan (#F4F7FE) agar kontras dengan Card Putih
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #F4F7FE; border: none; }"
        "QScrollBar:vertical { background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );

    QWidget* contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");
    contentWidget->setStyleSheet("#contentWidget { background-color: #F4F7FE; }");
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    // 4. Form Section
    createFormSection(contentLayout);

    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void AddBookPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(80);
    headerFrame->setStyleSheet(
        "QFrame { background-color: white; border-bottom: 1px solid #E0E5F2; }"
    );
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(20);
    
    QLabel* iconLabel = new QLabel("➕", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent; color: #4318FF;");
    headerLayout->addWidget(iconLabel);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);
    titleLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* titleLabel = new QLabel("Tambah Buku Baru", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674; background: transparent;");
    
    QLabel* subLabel = new QLabel("Masukkan detail buku untuk menambah koleksi perpustakaan.", headerFrame);
    subLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 13px; color: #A3AED0; background: transparent;");
    
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(subLabel);
    headerLayout->addLayout(titleLayout);
    
    headerLayout->addStretch();
    
    // Profile Icon
    QPushButton* profileBtn = new QPushButton("👤", headerFrame);
    profileBtn->setFixedSize(45, 45);
    profileBtn->setCursor(Qt::PointingHandCursor);
    profileBtn->setStyleSheet(
        "QPushButton { background-color: #112D4E; border: none; border-radius: 22px; color: white; font-size: 20px; }"
        "QPushButton:hover { background-color: #2B3674; }"
    );
    headerLayout->addWidget(profileBtn);
    
    mainLayout->addWidget(headerFrame);
}

void AddBookPage::createFormSection(QVBoxLayout* contentLayout)
{
    // Container Kartu Putih
    QFrame* formCard = new QFrame(this);
    formCard->setObjectName("formCard");
    
    // Efek Bayangan
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 15));
    shadowEffect->setOffset(0, 4);
    formCard->setGraphicsEffect(shadowEffect);
    
    // --- STYLESHEET LENGKAP DENGAN FIX PANAH SPINBOX ---
    QString cardStyle = 
        "#formCard { background-color: white; border-radius: 16px; }"
        
        /* 1. Style Input Text (Border Lebih Gelap #94A3B8) */
        "QLineEdit, QTextEdit {"
        "   background-color: white;"
        "   border: 1px solid #94A3B8;"  /* Border Slate Grey agar terlihat di background putih */
        "   border-radius: 8px;"
        "   padding: 10px 12px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "   font-family: 'Segoe UI';"
        "}"
        "QLineEdit:focus, QTextEdit:focus {"
        "   border: 2px solid #4318FF;"
        "   background-color: #FDFDFD;"
        "}"
        
        /* 2. Style SpinBox (Tahun & Rating) */
        "QSpinBox, QDoubleSpinBox {"
        "   background-color: white;"
        "   border: 1px solid #94A3B8;"
        "   border-radius: 8px;"
        "   padding: 10px 12px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "   font-family: 'Segoe UI';"
        "}"
        "QSpinBox:focus, QDoubleSpinBox:focus {"
        "   border: 2px solid #4318FF;"
        "}"
        
        /* 3. FIX TOMBOL PANAH (UP/DOWN BUTTONS) */
        "QSpinBox::up-button, QDoubleSpinBox::up-button {"
        "   subcontrol-origin: border;"
        "   subcontrol-position: top right;"
        "   width: 24px;"
        "   border-left: 1px solid #94A3B8;" /* Garis pemisah tombol */
        "   border-bottom: 1px solid #94A3B8;"
        "   background-color: #F1F5F9;" /* Abu sangat muda */
        "   border-top-right-radius: 8px;" /* Rounded corner mengikuti input */
        "}"
        "QSpinBox::down-button, QDoubleSpinBox::down-button {"
        "   subcontrol-origin: border;"
        "   subcontrol-position: bottom right;"
        "   width: 24px;"
        "   border-left: 1px solid #94A3B8;"
        "   background-color: #F1F5F9;"
        "   border-bottom-right-radius: 8px;"
        "}"
        "QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,"
        "QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {"
        "   background-color: #E2E8F0;" /* Efek hover */
        "}"
        
        /* 4. MENGGAMBAR SEGITIGA SECARA MANUAL (Trik CSS Border) */
        /* Panah Atas */
        "QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {"
        "   width: 0px; height: 0px;"
        "   border-left: 4px solid transparent;"
        "   border-right: 4px solid transparent;"
        "   border-bottom: 5px solid #475569;" /* Warna Panah (Slate 600) */
        "}"
        /* Panah Bawah */
        "QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {"
        "   width: 0px; height: 0px;"
        "   border-left: 4px solid transparent;"
        "   border-right: 4px solid transparent;"
        "   border-top: 5px solid #475569;" /* Warna Panah */
        "}"
        /* Tekan Tombol: Panah jadi biru */
        "QSpinBox::up-arrow:pressed, QDoubleSpinBox::up-arrow:pressed {"
        "   border-bottom: 5px solid #4318FF;"
        "}"
        "QSpinBox::down-arrow:pressed, QDoubleSpinBox::down-arrow:pressed {"
        "   border-top: 5px solid #4318FF;"
        "}"
        ;

    formCard->setStyleSheet(cardStyle);
    
    QHBoxLayout* cardLayout = new QHBoxLayout(formCard);
    cardLayout->setContentsMargins(40, 40, 40, 40);
    cardLayout->setSpacing(50);
    
    // --- KOLOM KIRI (INPUT FIELDS) ---
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(20);
    
    // 1. Judul
    m_lineJudul = new QLineEdit();
    m_lineJudul->setPlaceholderText("Contoh: Belajar C++ Modern");
    m_lineJudul->setMinimumHeight(45);
    
    m_labelJudulError = new QLabel("Judul wajib diisi");
    m_labelJudulError->setStyleSheet("color: #EE5D50; font-size: 12px; font-weight: 500; background: transparent;");
    
    leftLayout->addWidget(createInputGroup("Judul Buku *", m_lineJudul, m_labelJudulError));

    // 2. Penulis
    m_linePenulis = new QLineEdit();
    m_linePenulis->setPlaceholderText("Nama Penulis");
    m_linePenulis->setMinimumHeight(45);
    
    m_labelPenulisError = new QLabel("Penulis wajib diisi");
    m_labelPenulisError->setStyleSheet("color: #EE5D50; font-size: 12px; font-weight: 500; background: transparent;");
    
    leftLayout->addWidget(createInputGroup("Penulis *", m_linePenulis, m_labelPenulisError));

    // 3. Row: Tahun & Rating
    QHBoxLayout* rowYearRating = new QHBoxLayout();
    rowYearRating->setSpacing(20);
    
    m_spinTahun = new QSpinBox();
    m_spinTahun->setRange(1800, 2100);
    m_spinTahun->setValue(QDate::currentDate().year());
    m_spinTahun->setMinimumHeight(45);
    m_spinTahun->setAlignment(Qt::AlignCenter);
    
    m_spinRating = new QDoubleSpinBox();
    m_spinRating->setRange(0.0, 5.0);
    m_spinRating->setSingleStep(0.1);
    m_spinRating->setMinimumHeight(45);
    m_spinRating->setAlignment(Qt::AlignCenter);
    
    rowYearRating->addWidget(createInputGroup("Tahun Terbit", m_spinTahun));
    rowYearRating->addWidget(createInputGroup("Rating (0-5)", m_spinRating));
    leftLayout->addLayout(rowYearRating);

    // 4. Genre
    m_textGenre = new QTextEdit();
    m_textGenre->setPlaceholderText("Pisahkan dengan koma. Contoh: Fiksi, Sains, Sejarah");
    m_textGenre->setMinimumHeight(100);
    
    m_labelGenreError = new QLabel("Genre wajib diisi");
    m_labelGenreError->setStyleSheet("color: #EE5D50; font-size: 12px; font-weight: 500; background: transparent;");
    
    leftLayout->addWidget(createInputGroup("Genre *", m_textGenre, m_labelGenreError));

    // --- KOLOM KANAN (IMAGE UPLOAD) ---
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);
    rightLayout->setAlignment(Qt::AlignTop);

    QLabel* lblImage = new QLabel("Cover Buku");
    lblImage->setStyleSheet("color: #2B3674; font-weight: 600; font-size: 14px; background: transparent;");
    rightLayout->addWidget(lblImage);

    // Preview Container
    m_labelImagePreview = new QLabel("📷\nPreview Cover");
    m_labelImagePreview->setFixedSize(240, 320);
    m_labelImagePreview->setAlignment(Qt::AlignCenter);
    m_labelImagePreview->setStyleSheet(
        "border: 2px dashed #4318FF; border-radius: 12px; "
        "background-color: #F8F9FC; color: #4318FF; font-weight: bold; font-size: 14px;"
    );
    rightLayout->addWidget(m_labelImagePreview);

    // Tombol Browse
    m_btnBrowseImage = new QPushButton("Pilih Gambar");
    m_btnBrowseImage->setCursor(Qt::PointingHandCursor);
    m_btnBrowseImage->setFixedHeight(45);
    m_btnBrowseImage->setStyleSheet(
        "QPushButton { background-color: #EAEFFC; color: #4318FF; border: none; border-radius: 10px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #D4DFFC; }"
    );
    rightLayout->addWidget(m_btnBrowseImage);
    
    // Hidden Path
    m_lineImagePath = new QLineEdit();
    m_lineImagePath->setVisible(false);
    rightLayout->addWidget(m_lineImagePath);

    // Spacer
    rightLayout->addStretch();
    
    // Tombol Aksi
    m_btnAdd = new QPushButton("✅ Simpan Buku");
    m_btnAdd->setCursor(Qt::PointingHandCursor);
    m_btnAdd->setFixedHeight(50);
    m_btnAdd->setStyleSheet(
        "QPushButton { background-color: #4318FF; color: white; border-radius: 10px; font-size: 15px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #3311DB; }"
        "QPushButton:pressed { background-color: #2108B7; }"
    );
    rightLayout->addWidget(m_btnAdd);
    
    m_btnClear = new QPushButton("🔄 Reset Form");
    m_btnClear->setCursor(Qt::PointingHandCursor);
    m_btnClear->setFixedHeight(50);
    m_btnClear->setStyleSheet(
        "QPushButton { background-color: white; border: 1px solid #94A3B8; color: #64748B; border-radius: 10px; font-size: 15px; font-weight: bold; }"
        "QPushButton:hover { border: 1px solid #EE5D50; color: #EE5D50; }"
    );
    rightLayout->addWidget(m_btnClear);

    // Gabungkan Layout
    cardLayout->addLayout(leftLayout, 6); 
    cardLayout->addLayout(rightLayout, 4); 
    
    contentLayout->addWidget(formCard);

    // --- CONNECT SIGNALS ---
    connect(m_btnAdd, &QPushButton::clicked, this, &AddBookPage::onAddBook);
    connect(m_btnClear, &QPushButton::clicked, this, &AddBookPage::onClearForm);
    connect(m_btnBrowseImage, &QPushButton::clicked, this, &AddBookPage::onBrowseImage);
    connect(m_lineJudul, &QLineEdit::textChanged, this, &AddBookPage::validateInput);
    connect(m_linePenulis, &QLineEdit::textChanged, this, &AddBookPage::validateInput);
    connect(m_textGenre, &QTextEdit::textChanged, this, &AddBookPage::validateInput);
}

// Helper: Buat Grup Input
QWidget* AddBookPage::createInputGroup(const QString& labelText, QWidget* inputWidget, QLabel* errorLabel)
{
    QWidget* group = new QWidget();
    group->setStyleSheet("background-color: transparent;"); 
    
    QVBoxLayout* layout = new QVBoxLayout(group);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);
    
    QLabel* label = new QLabel(labelText);
    label->setStyleSheet("color: #2B3674; font-weight: 600; font-size: 14px; font-family: 'Segoe UI'; background: transparent;");
    layout->addWidget(label);
    
    layout->addWidget(inputWidget);
    
    if (errorLabel) {
        layout->addWidget(errorLabel);
    }
    
    return group;
}

// --- LOGIC FUNCTIONS (TETAP SAMA) ---

bool AddBookPage::validate()
{
    bool isValid = true;
    if (m_lineJudul->text().trimmed().isEmpty()) { m_labelJudulError->show(); isValid = false; } else { m_labelJudulError->hide(); }
    if (m_linePenulis->text().trimmed().isEmpty()) { m_labelPenulisError->show(); isValid = false; } else { m_labelPenulisError->hide(); }
    if (m_textGenre->toPlainText().trimmed().isEmpty()) { m_labelGenreError->show(); isValid = false; } else { m_labelGenreError->hide(); }
    return isValid;
}

void AddBookPage::validateInput() { validate(); }

void AddBookPage::onAddBook()
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
    int newId = generateNewId();
    Book newBook(newId, m_lineJudul->text().trimmed(), m_linePenulis->text().trimmed(), genres, m_spinTahun->value(), m_spinRating->value(), m_selectedImagePath);
    DatabaseManager::instance().addBook(newBook);
    QMessageBox::information(this, "Berhasil", QString("Buku '%1' berhasil disimpan!").arg(newBook.getJudul()));
    clearForm();
    emit bookAdded();
}

void AddBookPage::onClearForm() { clearForm(); }

void AddBookPage::clearForm()
{
    m_lineJudul->clear(); m_linePenulis->clear(); m_textGenre->clear();
    m_spinTahun->setValue(QDate::currentDate().year()); m_spinRating->setValue(0.0);
    m_lineImagePath->clear(); m_selectedImagePath.clear();
    m_labelImagePreview->clear(); m_labelImagePreview->setText("📷\nPreview Cover");
    m_labelJudulError->hide(); m_labelPenulisError->hide(); m_labelGenreError->hide();
}

void AddBookPage::onBrowseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Pilih Gambar Cover Buku", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (!fileName.isEmpty()) {
        m_selectedImagePath = fileName;
        m_lineImagePath->setText(fileName);
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            m_labelImagePreview->setPixmap(pixmap.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

int AddBookPage::generateNewId()
{
    std::vector<Book> books = DatabaseManager::instance().getAllBooks();
    if (books.empty()) return 1;
    int maxId = 0;
    for (const Book& book : books) if (book.getId() > maxId) maxId = book.getId();
    return maxId + 1;
}