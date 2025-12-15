#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QTextBrowser>
#include <QGraphicsDropShadowEffect>
#include "../backend/DatabaseManager.h"
#include "../backend/Book.h"
#include <vector>

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

    void updateStatistics();

private:
    void setupUI();
    
    QWidget* createSummaryCard(const QString& title, const QString& value, const QString& iconPath, const QString& color);
    
    void loadStatistics();
    void updateSummary(const std::vector<Book*>& books);
    void updateGenreChart(const std::vector<Book*>& books);
    void updateRatingChart(const std::vector<Book*>& books);
    void updateYearChart(const std::vector<Book*>& books);
    
    QWidget* m_lblTotalBooks;
    QWidget* m_lblTotalGenres;
    QWidget* m_lblAvgRating;
    QWidget* m_lblLatestYear;
    
    QTextBrowser* m_browserGenreChart;
    QTextBrowser* m_browserRatingChart;
    QTextBrowser* m_browserYearChart;
};

#endif // STATISTICSPAGE_H