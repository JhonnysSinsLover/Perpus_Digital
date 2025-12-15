#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QGraphicsDropShadowEffect> // Penting untuk efek modern
#include "../backend/DatabaseManager.h"

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

    void updateStatistics();

private:
    void setupUI();
    void applyStyles();
    
    // Helper untuk membuat Container Kartu Putih dengan Shadow
    QFrame* createCardFrame();
    
    // Helper untuk Summary Card (Icon + Text)
    QFrame* createStatCard(const QString& icon, const QString& title, 
                           const QString& value, const QString& color);
    
    // Chart widgets containers
    QFrame* m_genreChartFrame;
    QFrame* m_ratingChartFrame;
    QFrame* m_yearChartFrame;
    
    // Labels untuk update nilai summary
    QLabel* m_lblTotalBooks;
    QLabel* m_lblTotalGenres;
    QLabel* m_lblAvgRating;
};

#endif // STATISTICSPAGE_H