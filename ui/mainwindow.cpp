#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "DashboardPage.h"
#include "BooksCollectionPage.h"
#include "AddBookPage.h"
#include <QScreen>
#include <QGuiApplication>
#include <QResizeEvent>
#include "StatisticsPage.h"
#include "RecommendationPage.h"
#include "BorrowQueuePage.h"
#include "PopularBooksPage.h"
#include "AddBookDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
    
    // Initialize database
    DatabaseManager& dbManager = DatabaseManager::instance();
    if (!dbManager.initialize()) {
        showErrorMessage("Database Error", "Failed to initialize database!");
        return;
    }
    
    // Check if database is empty, insert sample books
    std::vector<Book> existingBooks = dbManager.getAllBooks();
    if (existingBooks.empty()) {
        // Try to import from JSON first
        m_currentDataPath = QCoreApplication::applicationDirPath() + "/../data/books.json";
        QFileInfo checkFile(m_currentDataPath);
        
        if (!checkFile.exists()) {
            m_currentDataPath = "data/books.json";
        }
        
        bool dataLoaded = false;
        if (dbManager.importFromJson(m_currentDataPath)) {
            updateStatusBar("📚 Imported data from JSON to database");
            dataLoaded = true;
        } else {
            // Insert sample books if JSON import failed
            if (dbManager.insertSampleBooks()) {
                updateStatusBar("📚 Inserted 15 sample books to database");
                dataLoaded = true;
            } else {
                updateStatusBar("⚠ No data loaded. Use File menu to import books.");
            }
        }
        
        if (dataLoaded) {
            existingBooks = dbManager.getAllBooks();
        }
    }
    
    // Build genre graph and update pages
    if (!existingBooks.empty()) {
        m_genreGraph.buildGraph(existingBooks);
        
        // Update all pages
        if (m_dashboardPage) m_dashboardPage->updateDashboard();
        if (m_collectionPage) m_collectionPage->refreshTable();
        if (m_statisticsPage) m_statisticsPage->updateStatistics();
        
        updateStatusBar(QString("📚 Loaded %1 books from database").arg(existingBooks.size()));
        showSuccessMessage("Database Ready", QString("Successfully loaded %1 books from database!").arg(existingBooks.size()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // Set window properties with responsive sizing
    setWindowTitle("Perpustakaan Digital - Professional Book Management System");
    setMinimumSize(1024, 600);  // Minimum usable size
    resize(1400, 800);  // Default size
    
    // Make window responsive to screen size
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        // If screen is large, use 80% of screen size
        if (screenGeometry.width() > 1600) {
            resize(screenGeometry.width() * 0.8, screenGeometry.height() * 0.85);
        }
    }
    
    // Create central widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create sidebar
    createSidebar();
    mainLayout->addWidget(m_sidebarFrame);
    
    // Create stacked widget for pages
    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(m_stackedWidget, 1);
    
    // Create pages
    createPages();
    
    // Apply styles
    applyStyles();
    
    // Show dashboard by default
    showDashboard();
    
    // Create menu bar
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    QMenu* fileMenu = menuBar->addMenu("&File");
    QAction* loadAction = fileMenu->addAction("📁 Load Data");
    QAction* saveAction = fileMenu->addAction("💾 Save Data");
    fileMenu->addSeparator();
    QAction* refreshAction = fileMenu->addAction("🔄 Refresh");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("❌ Exit");
    
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadData);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveData);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefreshData);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    // Status bar
    statusBar()->showMessage("Ready");
}

void MainWindow::createSidebar()
{
    m_sidebarFrame = new QFrame(this);
    m_sidebarFrame->setMinimumWidth(200);
    m_sidebarFrame->setMaximumWidth(350);
    m_sidebarFrame->setFixedWidth(260);  // Default, will be adjusted in resizeEvent
    m_sidebarFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_sidebarFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #1e3a5f;"  // Professional dark blue
        "   border-right: 3px solid #142d4c;"
        "}"
    );
    
    QVBoxLayout* sidebarLayout = new QVBoxLayout(m_sidebarFrame);
    sidebarLayout->setSpacing(8);
    sidebarLayout->setContentsMargins(15, 25, 15, 25);
    
    // Logo/Title dengan icon - HORIZONTAL LAYOUT
    QFrame* logoFrame = new QFrame(this);
    logoFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #2d5a8c;"
        "   border: 2px solid #3d7ab8;"
        "   border-radius: 12px;"
        "   padding: 12px 15px;"
        "}"
    );
    QHBoxLayout* logoLayout = new QHBoxLayout(logoFrame);
    logoLayout->setSpacing(10);
    logoLayout->setContentsMargins(5, 5, 5, 5);
    
    QLabel* iconLabel = new QLabel("📚", this);
    iconLabel->setStyleSheet(
        "font-size: 36px;"
        "background: transparent;"
        "border: none;"
    );
    iconLabel->setFixedSize(48, 48);
    iconLabel->setAlignment(Qt::AlignCenter);
    
    QLabel* titleLabel = new QLabel("Perpustakaan\nDigital", this);
    titleLabel->setStyleSheet(
        "color: white; "
        "font-size: 16px; "
        "font-weight: bold; "
        "border: none;"
        "line-height: 1.3;"
    );
    titleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    
    logoLayout->addWidget(iconLabel);
    logoLayout->addWidget(titleLabel, 1);
    sidebarLayout->addWidget(logoFrame);
    
    sidebarLayout->addSpacing(25);
    
    // Navigation buttons dengan emoji icons yang tidak akan stretch
    m_btnDashboard = new QPushButton("📊  Dashboard", this);
    
    m_btnCollection = new QPushButton("📚  Koleksi Buku", this);
    
    m_btnAddBook = new QPushButton("➕  Tambah Buku", this);
    
    m_btnStatistics = new QPushButton("📈  Statistik", this);
    
    m_btnRecommendations = new QPushButton("💡  Rekomendasi", this);
    
    m_btnBorrowQueue = new QPushButton("📋  Antrian Peminjaman", this);
    
    m_btnPopularBooks = new QPushButton("⭐  Buku Populer", this);
    
    // Store buttons in list for easy styling
    m_sidebarButtons << m_btnDashboard << m_btnCollection << m_btnAddBook 
                    << m_btnStatistics << m_btnRecommendations 
                    << m_btnBorrowQueue << m_btnPopularBooks;
    
    // Style buttons - MODERN DESIGN dengan border (bukan shadow)
    QString buttonStyle = 
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #e8f4f8;"
        "   border: 2px solid transparent;"
        "   border-left: 4px solid transparent;"
        "   padding: 14px 18px;"
        "   text-align: left;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "   border-radius: 8px;"
        "   margin: 2px 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2d5a8c;"
        "   border-left: 4px solid #3d9bd9;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #245a8f;"
        "}"
        "QPushButton[isActive=\"true\"] {"
        "   background-color: #2d7ab8;"
        "   border: 2px solid #4a9dd9;"
        "   border-left: 4px solid #6fb9e8;"
        "   color: white;"
        "   font-weight: bold;"
        "}";
    
    for (QPushButton* btn : std::as_const(m_sidebarButtons)) {
        btn->setStyleSheet(buttonStyle);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(48);
        btn->setMaximumHeight(52);
        sidebarLayout->addWidget(btn);
    }
    
    sidebarLayout->addStretch();
    
    // Footer dengan border
    QFrame* footerFrame = new QFrame(this);
    footerFrame->setStyleSheet(
        "QFrame {"
        "   background-color: transparent;"
        "   border-top: 2px solid #2d5a8c;"
        "   padding-top: 10px;"
        "}"
    );
    QVBoxLayout* footerLayout = new QVBoxLayout(footerFrame);
    
    QLabel* footerLabel = new QLabel("© 2025 Perpustakaan Digital", this);
    footerLabel->setStyleSheet(
        "color: #8ba4b8; "
        "font-size: 10px; "
        "border: none;"
    );
    footerLabel->setAlignment(Qt::AlignCenter);
    
    QLabel* versionLabel = new QLabel("Version 2.0", this);
    versionLabel->setStyleSheet(
        "color: #6a8ca8; "
        "font-size: 9px; "
        "border: none;"
    );
    versionLabel->setAlignment(Qt::AlignCenter);
    
    footerLayout->addWidget(footerLabel);
    footerLayout->addWidget(versionLabel);
    sidebarLayout->addWidget(footerFrame);
    
    // Connect signals
    connect(m_btnDashboard, &QPushButton::clicked, this, &MainWindow::showDashboard);
    connect(m_btnCollection, &QPushButton::clicked, this, &MainWindow::showBooksCollection);
    connect(m_btnAddBook, &QPushButton::clicked, this, &MainWindow::showAddBook);
    connect(m_btnStatistics, &QPushButton::clicked, this, &MainWindow::showStatistics);
    connect(m_btnRecommendations, &QPushButton::clicked, this, &MainWindow::showRecommendations);
    connect(m_btnBorrowQueue, &QPushButton::clicked, this, &MainWindow::showBorrowQueue);
    connect(m_btnPopularBooks, &QPushButton::clicked, this, &MainWindow::showPopularBooks);
}

void MainWindow::createPages()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    
    // Dashboard
    m_dashboardPage = new DashboardPage(this);
    m_stackedWidget->addWidget(m_dashboardPage);
    
    // Books Collection
    m_collectionPage = new BooksCollectionPage(this);
    connect(m_collectionPage, &BooksCollectionPage::editBookRequested, 
            [this, &dbManager](int bookId) {
                Book book = dbManager.getBookById(bookId);
                if (book.getId() == 0) return;  // Book not found
                
                AddBookDialog dialog(this, &book);
                dialog.setWindowTitle("✏️ Edit Buku");
                
                if (dialog.exec() == QDialog::Accepted) {
                    Book updatedBook = dialog.getBook();
                    updatedBook.setId(bookId);
                    
                    if (dbManager.updateBook(updatedBook)) {
                        m_collectionPage->refreshTable();
                        m_dashboardPage->updateDashboard();
                        m_statisticsPage->updateStatistics();
                        std::vector<Book> books = dbManager.getAllBooks();
                        m_genreGraph.buildGraph(books);
                        showSuccessMessage("✅ Berhasil", 
                                         QString("Buku '%1' berhasil diperbarui!").arg(updatedBook.getJudul()));
                    }
                }
            });
    
    connect(m_collectionPage, &BooksCollectionPage::deleteBookRequested,
            [this, &dbManager](int bookId) {
                Book book = dbManager.getBookById(bookId);
                if (book.getId() == 0) return;  // Book not found
                
                QString bookTitle = book.getJudul();
                
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Konfirmasi Hapus");
                msgBox.setText(QString("Apakah Anda yakin ingin menghapus buku '%1'?").arg(bookTitle));
                msgBox.setInformativeText("Aksi ini tidak dapat dibatalkan.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);
                
                if (msgBox.exec() == QMessageBox::Yes) {
                    if (dbManager.deleteBook(bookId)) {
                        m_collectionPage->refreshTable();
                        m_dashboardPage->updateDashboard();
                        m_statisticsPage->updateStatistics();
                        std::vector<Book> books = dbManager.getAllBooks();
                        m_genreGraph.buildGraph(books);
                        showSuccessMessage("✅ Berhasil", QString("Buku '%1' berhasil dihapus!").arg(bookTitle));
                    }
                }
            });
    
    m_stackedWidget->addWidget(m_collectionPage);
    
    // Add Book
    m_addBookPage = new AddBookPage(this);
    connect(m_addBookPage, &AddBookPage::bookAdded, [this, &dbManager]() {
        m_collectionPage->refreshTable();
        m_dashboardPage->updateDashboard();
        m_statisticsPage->updateStatistics();
        std::vector<Book> books = dbManager.getAllBooks();
        m_genreGraph.buildGraph(books);
    });
    m_stackedWidget->addWidget(m_addBookPage);
    
    // Statistics
    m_statisticsPage = new StatisticsPage(this);
    m_stackedWidget->addWidget(m_statisticsPage);
    
    // Recommendations
    m_recommendationPage = new RecommendationPage(&m_genreGraph, this);
    m_stackedWidget->addWidget(m_recommendationPage);
    
    // Borrow Queue (Queue - FIFO)
    m_borrowQueuePage = new BorrowQueuePage(this);
    m_stackedWidget->addWidget(m_borrowQueuePage);
    
    // Popular Books (Priority Queue - Max Heap)
    m_popularBooksPage = new PopularBooksPage(this);
    m_stackedWidget->addWidget(m_popularBooksPage);
}

void MainWindow::switchPage(int pageIndex)
{
    // Update button states
    for (int i = 0; i < m_sidebarButtons.size(); i++) {
        m_sidebarButtons[i]->setProperty("isActive", i == pageIndex);
        m_sidebarButtons[i]->style()->unpolish(m_sidebarButtons[i]);
        m_sidebarButtons[i]->style()->polish(m_sidebarButtons[i]);
    }
    
    m_stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::showDashboard()
{
    switchPage(0);
    if (m_dashboardPage) {
        m_dashboardPage->updateDashboard();
    }
    updateStatusBar("📊 Dashboard");
}

void MainWindow::showBooksCollection()
{
    switchPage(1);
    updateStatusBar("📚 Koleksi Buku");
}

void MainWindow::showAddBook()
{
    switchPage(2);
    updateStatusBar("➕ Tambah Buku");
}

void MainWindow::showStatistics()
{
    switchPage(3);
    if (m_statisticsPage) {
        m_statisticsPage->updateStatistics();
    }
    updateStatusBar("📈 Statistik");
}

void MainWindow::showRecommendations()
{
    switchPage(4);
    updateStatusBar("💡 Rekomendasi");
}

void MainWindow::showBorrowQueue()
{
    switchPage(5);
    m_borrowQueuePage->refreshQueue();
    updateStatusBar("📋 Antrian Peminjaman (Queue - FIFO)");
}

void MainWindow::showPopularBooks()
{
    switchPage(6);
    m_popularBooksPage->refreshBooks();
    updateStatusBar("⭐ Buku Populer (Priority Queue - Max Heap)");
}

void MainWindow::onLoadData()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Books Data"), "",
        tr("JSON Files (*.json);;All Files (*)"));
    
    if (fileName.isEmpty())
        return;
    
    DatabaseManager& dbManager = DatabaseManager::instance();
    if (dbManager.importFromJson(fileName)) {
        m_currentDataPath = fileName;
        std::vector<Book> books = dbManager.getAllBooks();
        m_genreGraph.buildGraph(books);
        
        // Update all pages
        m_dashboardPage->updateDashboard();
        m_collectionPage->refreshTable();
        m_statisticsPage->updateStatistics();
        
        updateStatusBar(QString("Imported %1 books from %2")
                       .arg(books.size())
                       .arg(QFileInfo(fileName).fileName()));
        showSuccessMessage("Success", 
                          QString("Successfully imported %1 books into database!")
                          .arg(books.size()));
    } else {
        showErrorMessage("Error", "Failed to import data from file!");
    }
}

void MainWindow::onSaveData()
{
    QString fileName = m_currentDataPath;
    
    if (fileName.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this,
            tr("Export Books Data"), "data/books.json",
            tr("JSON Files (*.json);;All Files (*)"));
    }
    
    if (fileName.isEmpty())
        return;
    
    DatabaseManager& dbManager = DatabaseManager::instance();
    std::vector<Book> books = dbManager.getAllBooks();
    
    if (dbManager.exportToJson(fileName)) {
        m_currentDataPath = fileName;
        updateStatusBar(QString("Exported %1 books to %2")
                       .arg(books.size())
                       .arg(QFileInfo(fileName).fileName()));
        showSuccessMessage("Success", "Data exported successfully!");
    } else {
        showErrorMessage("Error", "Failed to export data!");
    }
}

void MainWindow::onRefreshData()
{
    m_dashboardPage->updateDashboard();
    m_collectionPage->refreshTable();
    m_statisticsPage->updateStatistics();
    updateStatusBar("Data refreshed");
}

void MainWindow::applyStyles()
{
    // Main window style
    setStyleSheet(
        "QMainWindow {"
        "   background-color: #f5f6fa;"
        "}"
        "QMenuBar {"
        "   background-color: #34495e;"
        "   color: white;"
        "   padding: 5px;"
        "}"
        "QMenuBar::item {"
        "   background-color: transparent;"
        "   color: white;"
        "   padding: 5px 10px;"
        "}"
        "QMenuBar::item:selected {"
        "   background-color: #3498db;"
        "}"
        "QMenu {"
        "   background-color: white;"
        "   color: #2c3e50;"
        "   border: 1px solid #bdc3c7;"
        "}"
        "QMenu::item:selected {"
        "   background-color: #3498db;"
        "   color: white;"
        "}"
        "QStatusBar {"
        "   background-color: #ecf0f1;"
        "   color: #2c3e50;"
        "   font-size: 13px;"
        "   padding: 5px;"
        "}"
    );
}

void MainWindow::updateStatusBar(const QString& message)
{
    statusBar()->showMessage(message, 5000);
}

void MainWindow::showSuccessMessage(const QString& title, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::showErrorMessage(const QString& title, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    adjustSidebarForWindowSize();
}

void MainWindow::adjustSidebarForWindowSize()
{
    if (!m_sidebarFrame) return;
    
    int windowWidth = width();
    
    // Responsive sidebar width based on window size
    if (windowWidth < 1200) {
        // Small window - narrow sidebar
        m_sidebarFrame->setFixedWidth(220);
        
        // Update button styling for smaller width
        for (QPushButton* btn : std::as_const(m_sidebarButtons)) {
            QFont font = btn->font();
            font.setPointSize(13);
            btn->setFont(font);
            btn->setIconSize(QSize(18, 18));
        }
    } else if (windowWidth < 1600) {
        // Medium window - default sidebar
        m_sidebarFrame->setFixedWidth(260);
        
        for (QPushButton* btn : std::as_const(m_sidebarButtons)) {
            QFont font = btn->font();
            font.setPointSize(14);
            btn->setFont(font);
            btn->setIconSize(QSize(20, 20));
        }
    } else {
        // Large window - wider sidebar
        m_sidebarFrame->setFixedWidth(300);
        
        for (QPushButton* btn : std::as_const(m_sidebarButtons)) {
            QFont font = btn->font();
            font.setPointSize(15);
            btn->setFont(font);
            btn->setIconSize(QSize(22, 22));
        }
    }
}
