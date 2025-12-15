#ifndef POPULARBOOKSPAGE_H
#define POPULARBOOKSPAGE_H

#include <QWidget>
#include <QTableWidget>
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
    QWidget* createInputGroup(const QString& labelText, QWidget* inputWidget);
    
    void loadBooksToTable(const std::vector<Book>& books);
    
    // UI Elements
    QTableWidget* m_tableBooks;
    QSpinBox* m_topNInput;
    QDoubleSpinBox* m_minRatingInput;
    QPushButton* m_btnShowTopN;
    QPushButton* m_btnFilterRating;
    QLabel* m_resultLabel;
};

#endif // POPULARBOOKSPAGE_H