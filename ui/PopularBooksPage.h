#ifndef POPULARBOOKSPAGE_H
#define POPULARBOOKSPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "../backend/DatabaseManager.h"

/**
 * @brief Page for displaying popular books using Priority Queue (Max Heap)
 * Updated to use Modern Card Grid Layout
 */
class PopularBooksPage : public QWidget
{
    Q_OBJECT

public:
    explicit PopularBooksPage(QWidget *parent = nullptr);
    ~PopularBooksPage();

    void refreshBooks();

private slots:
    void onShowTopN();
    void onFilterByRating();

private:
    void setupUI();
    
    // UI Helpers
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createContentSection(QVBoxLayout* contentLayout);
    QFrame* createCardFrame();
    
    // Fungsi baru untuk menampilkan kartu
    void displayBooksAsCards(const std::vector<Book>& books);
    void clearGrid();
    
    // UI Elements
    QGridLayout* m_booksGrid; // Pengganti Table
    QSpinBox* m_topNInput;
    QDoubleSpinBox* m_minRatingInput;
    QPushButton* m_btnShowTopN;
    QPushButton* m_btnFilterRating;
    QLabel* m_resultLabel;
};

#endif // POPULARBOOKSPAGE_H