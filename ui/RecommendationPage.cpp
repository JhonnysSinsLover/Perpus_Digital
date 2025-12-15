#include "RecommendationPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QStyle>

RecommendationPage::RecommendationPage(Graph* genreGraph, QWidget *parent)
    : QWidget(parent)
    , m_genreGraph(genreGraph)
{
    setupUI();
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
    
    // Background Abu-abu kebiruan (#F4F7FE)
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

    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void RecommendationPage::createHeaderSection(QVBoxLayout* mainLayout)
{
    QFrame* headerFrame = new QFrame(this);
    headerFrame->setFixedHeight(80);
    headerFrame->setStyleSheet(
        "QFrame { background-color: white; border-bottom: 1px solid #E0E5F2; }"
    );
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(30, 0, 30, 0);
    headerLayout->setSpacing(20);
    
    // Ikon Lampu/Ide
    QLabel* iconLabel = new QLabel("💡", headerFrame);
    iconLabel->setStyleSheet("font-size: 24px; background: transparent;");
    headerLayout->addWidget(iconLabel);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);
    titleLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* titleLabel = new QLabel("Rekomendasi Buku", headerFrame);
    titleLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 24px; font-weight: 700; color: #2B3674; background: transparent;");
    
    QLabel* subLabel = new QLabel("Temukan buku menarik lainnya berdasarkan kesamaan genre.", headerFrame);
    subLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 13px; color: #A3AED0; background: transparent;");
    
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(subLabel);
    headerLayout->addLayout(titleLayout);
    
    headerLayout->addStretch();
    mainLayout->addWidget(headerFrame);
}

QFrame* RecommendationPage::createCardFrame()
{
    QFrame* card = new QFrame(this);
    card->setStyleSheet("background-color: white; border-radius: 16px;");
    
    // Drop Shadow
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 15));
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);
    
    return card;
}

void RecommendationPage::createContentSection(QVBoxLayout* contentLayout)
{
    // --- KARTU 1: INPUT PENCARIAN ---
    QFrame* inputCard = createCardFrame();
    QVBoxLayout* inputLayout = new QVBoxLayout(inputCard);
    inputLayout->setContentsMargins(30, 30, 30, 30);
    inputLayout->setSpacing(20);
    
    // Header Input
    QLabel* lblInputTitle = new QLabel("🔍 Cari Rekomendasi", inputCard);
    lblInputTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    inputLayout->addWidget(lblInputTitle);
    
    // Search Bar & Button Row
    QHBoxLayout* searchRow = new QHBoxLayout();
    searchRow->setSpacing(15);
    
    m_bookTitleInput = new QLineEdit(inputCard);
    m_bookTitleInput->setPlaceholderText("Ketik judul buku favoritmu...");
    m_bookTitleInput->setFixedHeight(45);
    // Style Input Modern (Border Jelas)
    m_bookTitleInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: white;"
        "   border: 1px solid #94A3B8;" /* Border Slate Grey */
        "   border-radius: 10px;"
        "   padding: 0 15px;"
        "   font-size: 14px;"
        "   color: #2B3674;"
        "   font-family: 'Segoe UI';"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #4318FF;"
        "   background-color: #FDFDFD;"
        "}"
    );
    
    m_btnGetRec = new QPushButton("Dapatkan Rekomendasi", inputCard);
    m_btnGetRec->setCursor(Qt::PointingHandCursor);
    m_btnGetRec->setFixedSize(200, 45);
    m_btnGetRec->setStyleSheet(
        "QPushButton { background-color: #4318FF; color: white; border-radius: 10px; font-weight: bold; font-size: 14px; border: none; }"
        "QPushButton:hover { background-color: #3311DB; }"
        "QPushButton:pressed { background-color: #2108B7; }"
    );
    
    searchRow->addWidget(m_bookTitleInput);
    searchRow->addWidget(m_btnGetRec);
    inputLayout->addLayout(searchRow);
    
    // Build Graph Option
    QHBoxLayout* optionRow = new QHBoxLayout();
    QLabel* lblInfo = new QLabel("Tips: Pastikan Graph Database sudah dibangun agar hasil akurat.", inputCard);
    lblInfo->setStyleSheet("color: #A3AED0; font-size: 12px; font-style: italic;");
    
    m_btnBuildGraph = new QPushButton("🔄 Re-Build Graph Database", inputCard);
    m_btnBuildGraph->setCursor(Qt::PointingHandCursor);
    m_btnBuildGraph->setFixedSize(180, 35);
    m_btnBuildGraph->setStyleSheet(
        "QPushButton { background-color: #F4F7FE; color: #4318FF; border: 1px solid #E0E5F2; border-radius: 8px; font-weight: 600; font-size: 12px; }"
        "QPushButton:hover { border: 1px solid #4318FF; background-color: #EAEFFC; }"
    );
    
    optionRow->addWidget(lblInfo);
    optionRow->addStretch();
    optionRow->addWidget(m_btnBuildGraph);
    inputLayout->addLayout(optionRow);
    
    contentLayout->addWidget(inputCard);

    // --- KARTU 2: HASIL REKOMENDASI ---
    QFrame* resultsCard = createCardFrame();
    QVBoxLayout* resultsLayout = new QVBoxLayout(resultsCard);
    resultsLayout->setContentsMargins(30, 30, 30, 30);
    resultsLayout->setSpacing(15);
    
    QLabel* lblResultTitle = new QLabel("🎯 Hasil Rekomendasi", resultsCard);
    lblResultTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 18px; font-weight: 700; color: #2B3674;");
    resultsLayout->addWidget(lblResultTitle);
    
    m_resultsBrowser = new QTextBrowser(resultsCard);
    m_resultsBrowser->setMinimumHeight(450);
    // Hilangkan border default QTextBrowser agar menyatu dengan card
    m_resultsBrowser->setStyleSheet("QTextBrowser { border: none; background-color: transparent; }");
    m_resultsBrowser->setOpenExternalLinks(false);
    
    // Default Empty State HTML
    m_resultsBrowser->setHtml(
        "<div style='text-align: center; margin-top: 50px; font-family: Segoe UI;'>"
        "  <div style='font-size: 40px; margin-bottom: 20px;'>📚</div>"
        "  <div style='font-size: 18px; font-weight: bold; color: #A3AED0;'>Belum ada hasil</div>"
        "  <div style='font-size: 14px; color: #D0D5DD;'>Masukkan judul buku di atas untuk mulai mencari.</div>"
        "</div>"
    );
    
    resultsLayout->addWidget(m_resultsBrowser);
    contentLayout->addWidget(resultsCard);

    // Connect signals
    connect(m_btnGetRec, &QPushButton::clicked, this, &RecommendationPage::onGetRecommendations);
    connect(m_btnBuildGraph, &QPushButton::clicked, this, &RecommendationPage::onBuildGraph);
    
    // Trigger search on Enter key
    connect(m_bookTitleInput, &QLineEdit::returnPressed, this, &RecommendationPage::onGetRecommendations);
}

void RecommendationPage::onBuildGraph()
{
    if (!m_genreGraph) return;

    DatabaseManager& dbManager = DatabaseManager::instance();
    m_genreGraph->buildGraph(dbManager.getAllBooks());
    
    QMessageBox::information(this, "✅ Berhasil",
                            "Graph database berhasil diperbarui!\n"
                            "Algoritma siap memberikan rekomendasi.");
}

void RecommendationPage::onGetRecommendations()
{
    QString bookTitle = m_bookTitleInput->text().trimmed();
    
    if (bookTitle.isEmpty()) {
        QMessageBox::warning(this, "Input Kosong", "Silakan masukkan judul buku terlebih dahulu.");
        m_bookTitleInput->setFocus();
        return;
    }
    
    if (!m_genreGraph) {
        QMessageBox::warning(this, "Error", "Graph belum diinisialisasi!");
        return;
    }
    
    // Find the book
    Book* targetBook = nullptr;
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> allBooks = dbManager.getAllBooks();
    for (Book& book : allBooks) {
        if (book.getJudul().contains(bookTitle, Qt::CaseInsensitive)) {
            targetBook = &book;
            break;
        }
    }
    
    if (!targetBook) {
        m_resultsBrowser->setHtml(
            "<div style='text-align: center; margin-top: 50px; font-family: Segoe UI;'>"
            "  <div style='font-size: 40px; margin-bottom: 20px;'>❌</div>"
            "  <div style='font-size: 18px; font-weight: bold; color: #EE5D50;'>Buku tidak ditemukan</div>"
            "  <div style='font-size: 14px; color: #A3AED0;'>Pastikan ejaan judul buku sudah benar.</div>"
            "</div>"
        );
        return;
    }
    
    // Get recommendations
    QStringList genres = targetBook->getGenre();
    std::vector<Book> recommendations;
    
    if (!genres.isEmpty()) {
        QString primaryGenre = genres.first();
        recommendations = m_genreGraph->getRecommendation(primaryGenre, allBooks, 5);
    }
    
    // --- GENERATE MODERN HTML RESULT ---
    QString html = R"(
        <html>
        <head>
            <style>
                body { font-family: 'Segoe UI', sans-serif; color: #2B3674; }
                .header-box {
                    background-color: #EAEFFC; 
                    border-radius: 12px; 
                    padding: 15px; 
                    border-left: 5px solid #4318FF;
                    margin-bottom: 25px;
                }
                .rec-card {
                    background-color: #F8F9FC;
                    border: 1px solid #E0E5F2;
                    border-radius: 10px;
                    padding: 15px;
                    margin-bottom: 10px;
                }
                .rec-title { font-size: 16px; font-weight: bold; color: #2B3674; margin-bottom: 5px; }
                .rec-meta { font-size: 12px; color: #A3AED0; }
                .badge {
                    background-color: #05CD99; color: white;
                    padding: 2px 8px; border-radius: 4px; font-size: 10px; font-weight: bold;
                }
                .rating { color: #FFB547; font-weight: bold; }
            </style>
        </head>
        <body>
    )";
    
    // 1. Header Info Buku Sumber
    html += QString(
        "<div class='header-box'>"
        "  <div style='font-size: 12px; color: #A3AED0; margin-bottom: 4px;'>Berdasarkan buku:</div>"
        "  <div style='font-size: 20px; font-weight: bold;'>%1</div>"
        "  <div style='font-size: 13px; margin-top: 5px;'>Penulis: <b>%2</b> &nbsp;|&nbsp; Genre: <span style='color: #4318FF;'>%3</span></div>"
        "</div>"
    ).arg(targetBook->getJudul())
     .arg(targetBook->getPenulis())
     .arg(targetBook->getGenre().join(", "));
    
    // 2. List Rekomendasi
    if (recommendations.empty()) {
        html += "<div style='color: #A3AED0; text-align: center; margin-top: 20px;'>Tidak ada rekomendasi lain yang mirip saat ini.</div>";
    } else {
        html += QString("<div style='margin-bottom: 15px; font-weight: bold; font-size: 16px;'>🎯 %1 Rekomendasi Teratas:</div>").arg(recommendations.size());
        
        int count = 1;
        for (const Book& book : recommendations) {
            html += QString(
                "<div class='rec-card'>"
                "  <table width='100%' cellpadding='0' cellspacing='0'>"
                "    <tr>"
                "      <td width='30' valign='top' style='font-size: 20px; font-weight: bold; color: #E0E5F2;'>#%1</td>"
                "      <td>"
                "        <div class='rec-title'>%2</div>"
                "        <div class='rec-meta'>✍️ %3 &nbsp;&nbsp; 📅 %4 &nbsp;&nbsp; <span class='rating'>⭐ %5</span></div>"
                "        <div style='margin-top: 8px;'><span class='badge'>%6</span></div>"
                "      </td>"
                "    </tr>"
                "  </table>"
                "</div>"
            ).arg(count++)
             .arg(book.getJudul())
             .arg(book.getPenulis())
             .arg(book.getTahun())
             .arg(book.getRating(), 0, 'f', 1)
             .arg(book.getGenre().join(", "));
        }
    }
    
    html += "</body></html>";
    m_resultsBrowser->setHtml(html);
}