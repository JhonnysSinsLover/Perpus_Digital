#ifndef RECOMMENDATIONPAGE_H
#define RECOMMENDATIONPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include "../backend/DatabaseManager.h"
#include "Graph.h"

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
    
    // Helpers
    QFrame* createCardFrame();
    void displayRecommendations(const std::vector<Book>& books);
    void clearRecommendationGrid();

    Graph* m_genreGraph;
    
    // UI Elements
    QLineEdit* m_bookTitleInput;
    QPushButton* m_btnGetRec;
    QPushButton* m_btnBuildGraph;
    
    // Container untuk Hasil
    QWidget* m_resultContainer;
    QGridLayout* m_resultGrid;
    QLabel* m_lblResultStatus; // Untuk menampilkan pesan "Tidak ditemukan" atau "Hasil untuk..."
};

#endif // RECOMMENDATIONPAGE_H