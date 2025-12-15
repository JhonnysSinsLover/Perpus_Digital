#include "RecommendationPage.h"
#include "BookCardWidget.h" // Pastikan file ini ada (dari fitur Koleksi Buku)
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

RecommendationPage::RecommendationPage(Graph* genreGraph, QWidget *parent)
    : QWidget(parent)
    , m_genreGraph(genreGraph)
{
    setupUI();
    
    // Otomatis build graph saat halaman dibuat agar user tidak perlu klik manual
    // Kecuali ada data baru, tombol manual tetap disediakan.
    QTimer::singleShot(500, this, [this](){
        if(m_genreGraph) m_genreGraph->buildGraph(DatabaseManager::instance().getAllBooks());
    });
}

RecommendationPage::~RecommendationPage()
{
}

void RecommendationPage::setupUI()
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
    
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #F4F7FE; border: none; }"
        "QScrollBar:vertical { background: #F4F7FE; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #C4CDD5; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );

    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: #F4F7FE;");
    
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(25);

    // 4. Content Section
    createContentSection(contentLayout);

    contentLayout->addStretch(); // Push content ke atas
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void RecommendationPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(80);
    headerFrame->setStyleSheet("background-color: white; border-bottom: 1px solid #E0E5F2;");
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(20);
    
    QLabel* iconLabel = new QLabel("💡", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent; border: none;");
    headerLayout->addWidget(iconLabel);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);
    titleLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* titleLabel = new QLabel("Rekomendasi Buku", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 22px; font-weight: 700; color: #2B3674; border: none;");
    
    QLabel* subLabel = new QLabel("Temukan buku menarik lainnya menggunakan algoritma Graph Connectivity.", headerFrame);
    subLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 13px; color: #A3AED0; border: none;");
    
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(subLabel);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    
    mainLayout->addWidget(headerFrame);
}

QFrame* RecommendationPage::createCardFrame()
{
    QFrame* card = new QFrame(this);
    card->setStyleSheet("background-color: white; border-radius: 16px; border: 1px solid #E0E5F2;");
    return card;
}

void RecommendationPage::createContentSection(QVBoxLayout* contentLayout)
{
    // --- SECTION 1: SEARCH INPUT ---
    QFrame* inputCard = createCardFrame();
    QVBoxLayout* inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(30, 30, 30, 30);
    inputLayout->setSpacing(20);
    
    QLabel* lblInputTitle = new QLabel("🔍 Cari Berdasarkan Buku Favorit", inputCard);
    lblInputTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #2B3674; border: none;");
    inputLayout->addWidget(lblInputTitle);
    
    QHBoxLayout* searchRow = new QHBoxLayout();
    searchRow->setSpacing(15);
    
    m_bookTitleInput = new QLineEdit(inputCard);
    m_bookTitleInput->setPlaceholderText("Ketik judul buku (misal: 'Bumi Manusia')...");
    m_bookTitleInput->setFixedHeight(45);
    m_bookTitleInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: white;"
        "   border: 1px solid #94A3B8;"
        "   border-radius: 10px;"
        "   padding: 0 15px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "}"
        "QLineEdit:focus { border: 2px solid #4318FF; }"
    );
    
    m_btnGetRec = new QPushButton("Analisa Graph", inputCard);
    m_btnGetRec->setCursor(Qt::PointingHandCursor);
    m_btnGetRec->setFixedSize(160, 45);
    m_btnGetRec->setStyleSheet(
        "QPushButton { background-color: #4318FF; color: white; border-radius: 10px; font-weight: bold; font-size: 14px; border: none; }"
        "QPushButton:hover { background-color: #3311DB; }"
    );
    
    searchRow->addWidget(m_bookTitleInput);
    searchRow->addWidget(m_btnGetRec);
    inputLayout->addLayout(searchRow);
    
    // Info & Refresh Graph
    QHBoxLayout* optionRow = new QHBoxLayout();
    m_btnBuildGraph = new QPushButton("⚡ Refresh Graph Data", inputCard);
    m_btnBuildGraph->setCursor(Qt::PointingHandCursor);
    m_btnBuildGraph->setStyleSheet("QPushButton { color: #A3AED0; font-weight: 600; border: none; text-align: left; } QPushButton:hover { color: #4318FF; }");
    
    optionRow->addWidget(m_btnBuildGraph);
    optionRow->addStretch();
    inputLayout->addLayout(optionRow);
    
    contentLayout->addWidget(inputCard);

    // --- SECTION 2: RESULTS GRID ---
    // Label Status Hasil
    m_lblResultStatus = new QLabel("", this);
    m_lblResultStatus->setStyleSheet("font-size: 16px; font-weight: 700; color: #2B3674; margin-top: 10px; margin-bottom: 5px;");
    m_lblResultStatus->setVisible(false);
    contentLayout->addWidget(m_lblResultStatus);

    // Container Grid
    m_resultGrid = new QGridLayout();
    m_resultGrid->setSpacing(20);
    m_resultGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    contentLayout->addLayout(m_resultGrid);

    // Connects
    connect(m_btnGetRec, &QPushButton::clicked, this, &RecommendationPage::onGetRecommendations);
    connect(m_btnBuildGraph, &QPushButton::clicked, this, &RecommendationPage::onBuildGraph);
    connect(m_bookTitleInput, &QLineEdit::returnPressed, this, &RecommendationPage::onGetRecommendations);
}

void RecommendationPage::onBuildGraph()
{
    if (!m_genreGraph) return;
    DatabaseManager& dbManager = DatabaseManager::instance();
    m_genreGraph->buildGraph(dbManager.getAllBooks());
    QMessageBox::information(this, "Success", "Graph Connectivity berhasil dibangun ulang!");
}

void RecommendationPage::clearRecommendationGrid()
{
    QLayoutItem* item;
    while ((item = m_resultGrid->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    m_lblResultStatus->setVisible(false);
}

void RecommendationPage::onGetRecommendations()
{
    QString bookTitle = m_bookTitleInput->text().trimmed();
    clearRecommendationGrid(); // Bersihkan hasil sebelumnya
    
    if (bookTitle.isEmpty()) {
        QMessageBox::warning(this, "Input Kosong", "Silakan masukkan judul buku terlebih dahulu.");
        return;
    }
    
    if (!m_genreGraph) return;
    
    // 1. Cari Buku Sumber di Database
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> allBooks = dbManager.getAllBooks();
    Book* targetBook = nullptr;
    
    for (Book& book : allBooks) {
        if (book.getJudul().compare(bookTitle, Qt::CaseInsensitive) == 0 || 
            book.getJudul().contains(bookTitle, Qt::CaseInsensitive)) {
            targetBook = &book;
            break;
        }
    }
    
    if (!targetBook) {
        m_lblResultStatus->setText("❌ Buku tidak ditemukan dalam database.");
        m_lblResultStatus->setStyleSheet("color: #EE5D50; font-weight: bold; font-size: 16px;");
        m_lblResultStatus->setVisible(true);
        return;
    }
    
    // 2. Logika Graph: Dapatkan Tetangga (Rekomendasi)
    QStringList genres = targetBook->getGenre();
    std::vector<Book> recommendations;
    
    QString triggerGenre = "Tidak Spesifik";
    if (!genres.isEmpty()) {
        triggerGenre = genres.first(); // Menggunakan genre utama sebagai node penghubung
        // Mengambil 4 rekomendasi teratas
        recommendations = m_genreGraph->getRecommendation(triggerGenre, allBooks, 4);
    }
    
    // 3. Tampilkan Hasil
    if (recommendations.empty()) {
        m_lblResultStatus->setText(QString("Tidak ada rekomendasi lain untuk genre '%1'.").arg(triggerGenre));
        m_lblResultStatus->setStyleSheet("color: #A3AED0; font-weight: bold; font-size: 16px;");
        m_lblResultStatus->setVisible(true);
    } else {
        // Update Status Label
        m_lblResultStatus->setText(QString("📚 Karena Anda menyukai '%1' (%2):")
                                   .arg(targetBook->getJudul())
                                   .arg(triggerGenre));
        m_lblResultStatus->setStyleSheet("color: #2B3674; font-weight: 700; font-size: 18px;");
        m_lblResultStatus->setVisible(true);
        
        displayRecommendations(recommendations);
    }
}

void RecommendationPage::displayRecommendations(const std::vector<Book>& books)
{
    // LOGIKA GRID RESPONSIVE (Sama seperti BooksCollectionPage)
    // Agar kartu tidak melar aneh jika cuma ada 1 hasil.
    
    int viewportWidth = this->width(); // Perkiraan lebar
    int minCardWidth = 220; 
    int spacing = 20;

    // Reset Grid Columns Stretch
    for (int i = 0; i < m_resultGrid->columnCount(); ++i) {
        m_resultGrid->setColumnStretch(i, 0);
    }

    // Hitung kapasitas kolom (misal: layar muat 4 kolom)
    int numCols = (viewportWidth - 60) / (minCardWidth + spacing);
    if (numCols < 1) numCols = 1;

    // Paksa Grid punya 'numCols' slot agar layout konsisten
    for (int i = 0; i < numCols; ++i) {
        m_resultGrid->setColumnStretch(i, 1);
    }

    // Render Kartu
    for (int i = 0; i < books.size(); ++i) {
        // Menggunakan BookCardWidget (Reusability)
        BookCardWidget* card = new BookCardWidget(books[i], this);
        
        // Setup Size Policy agar mengisi slot grid
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        
        int row = i / numCols;
        int col = i % numCols;
        
        m_resultGrid->addWidget(card, row, col);
    }
}