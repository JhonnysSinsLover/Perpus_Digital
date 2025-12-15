#ifndef BORROWQUEUEPAGE_H
#define BORROWQUEUEPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "../backend/DatabaseManager.h"

/**
 * @brief Page for managing book borrowing queue (FIFO) - Modern UI
 */
class BorrowQueuePage : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowQueuePage(QWidget *parent = nullptr);
    ~BorrowQueuePage();

    void refreshQueue();

private slots:
    void onAddToBorrow();
    void onProcessNext();
    void onClearQueue();

private:
    void setupUI();
    
    // UI Building Blocks
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createContentSection(QVBoxLayout* contentLayout);
    
    // Helpers
    QFrame* createCardFrame();
    QWidget* createInputGroup(const QString& labelText, QWidget* inputWidget);
    void loadQueueToTable();
    
    // UI Elements
    QTableWidget* m_tableQueue;
    QLineEdit* m_borrowerNameInput;
    QComboBox* m_bookCombo;
    
    QPushButton* m_btnAddToBorrow;
    QPushButton* m_btnProcessNext;
    QPushButton* m_btnClearQueue;
    
    QLabel* m_queueSizeLabel;
};

#endif // BORROWQUEUEPAGE_H