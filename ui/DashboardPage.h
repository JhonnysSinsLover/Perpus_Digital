#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include "../backend/DatabaseManager.h"

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

    void updateDashboard();

private:
    void setupUI();
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createStatCards(QVBoxLayout* mainLayout);
    void createRecentBooksTable(QVBoxLayout* mainLayout);
    void applyStyles();
    
    // Helper visual components
    QWidget* createCompactStatCard(const QString& title, const QString& value, 
                                   const QString& icon, const QString& iconColorBg, bool showTrend = false);
    
    // UI Elements pointers
    QLineEdit* m_searchBar;
    QPushButton* m_notificationBtn;
    QPushButton* m_profileBtn;
    
    QLabel* m_lblTotalBooks;
    QLabel* m_lblTotalAuthors;
    QLabel* m_lblTotalGenres;
    QLabel* m_lblAvgRating;
    
    QTableWidget* m_recentBooksTable;
};

#endif // DASHBOARDPAGE_H