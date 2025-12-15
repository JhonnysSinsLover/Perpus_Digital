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
    explicit RecommendationPage(Graph* genreGraph, QWidget *parent = nullptr);
    ~RecommendationPage();

private slots:
    void onGetRecommendations();
    void onBuildGraph();

private:
    void setupUI();
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createContentSection(QVBoxLayout* contentLayout);
    
    // Helper untuk membuat Container Kartu
    QFrame* createCardFrame();

    Graph* m_genreGraph;
    
    QLineEdit* m_bookTitleInput;
    QPushButton* m_btnGetRec;
    QPushButton* m_btnBuildGraph;
    QTextBrowser* m_resultsBrowser;
};

#endif // RECOMMENDATIONPAGE_H