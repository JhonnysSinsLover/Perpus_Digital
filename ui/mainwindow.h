#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFrame>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "../backend/DatabaseManager.h"
#include "Graph.h"

// Forward declarations
class DashboardPage;
class BooksCollectionPage;
class AddBookPage;
class StatisticsPage;
class RecommendationPage;
class BorrowQueuePage;
class PopularBooksPage;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief Main application window with sidebar navigation
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation
    void showDashboard();
    void showBooksCollection();
    void showAddBook();
    void showStatistics();
    void showRecommendations();
    void showBorrowQueue();
    void showPopularBooks();
    
    // Actions
    void onSaveData();
    void onLoadData();
    void onRefreshData();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUI();
    void createSidebar();
    void createPages();
    void switchPage(int pageIndex);
    void applyStyles();
    void updateStatusBar(const QString& message);
    void showSuccessMessage(const QString& title, const QString& message);
    void showErrorMessage(const QString& title, const QString& message);
    void adjustSidebarForWindowSize();
    
    Ui::MainWindow *ui;
    
    // Backend components
    Graph m_genreGraph;
    QString m_currentDataPath;
    
    // UI Components
    QWidget* m_centralWidget;
    QFrame* m_sidebarFrame;
    QStackedWidget* m_stackedWidget;
    
    // Sidebar buttons
    QPushButton* m_btnDashboard;
    QPushButton* m_btnCollection;
    QPushButton* m_btnAddBook;
    QPushButton* m_btnStatistics;
    QPushButton* m_btnRecommendations;
    QPushButton* m_btnBorrowQueue;
    QPushButton* m_btnPopularBooks;
    QList<QPushButton*> m_sidebarButtons;
    
    // Pages
    DashboardPage* m_dashboardPage;
    BooksCollectionPage* m_collectionPage;
    AddBookPage* m_addBookPage;
    StatisticsPage* m_statisticsPage;
    RecommendationPage* m_recommendationPage;
    BorrowQueuePage* m_borrowQueuePage;
    PopularBooksPage* m_popularBooksPage;
};

#endif // MAINWINDOW_H
