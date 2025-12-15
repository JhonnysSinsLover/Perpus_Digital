#ifndef RECOMMENDATIONPAGE_H
#define RECOMMENDATIONPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "../backend/DatabaseManager.h"
#include "Graph.h"

/**
 * @brief Page for book recommendations (Modern UI)
 */
class RecommendationPage : public QWidget
{
    Q_OBJECT

public:
    explicit RecommendationPage(QWidget *parent = nullptr);
    explicit RecommendationPage(Graph* genreGraph, QWidget *parent = nullptr);
    ~RecommendationPage();

private slots:
    void onSearchClicked();
    void onBuildGraphClicked();
    void onRefreshClicked();

private:
    void setupUI();
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createContentSection(QVBoxLayout* contentLayout);
    
    // Helper untuk membuat Container Kartu
    QFrame* createCardFrame();

    Graph* m_genreGraph;
    
    QLineEdit* m_bookTitleInput;
    QLineEdit* m_lineSearchBook;
    QPushButton* m_btnGetRec;
    QPushButton* m_btnBuildGraph;
    QPushButton* m_btnSearch;
    QPushButton* m_btnRefresh;
    QTextBrowser* m_resultsBrowser;
    QTextBrowser* m_browserResults;
};

#endif // RECOMMENDATIONPAGE_H