#ifndef BORROWQUEUEPAGE_H
#define BORROWQUEUEPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QDateEdit>
#include <QDate>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "../backend/DatabaseManager.h"
#include <queue>

struct BorrowRequest {
    int id;
    QString namaPeminjam;
    QString bukuDipinjam;
    QDate tanggalPinjam;
};

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
    void onEnqueueClicked();
    void onProcessNextClicked();
    void onClearQueueClicked();

private:
    void setupUI();
    
    // UI Building Blocks
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createContentSection(QVBoxLayout* contentLayout);
    
    // Helpers
    QFrame* createCardFrame();
    QWidget* createInputGroup(const QString& labelText, QWidget* inputWidget);
    void loadQueueToTable();
    void updateQueueTable();
    
    // UI Elements
    QTableWidget* m_tableQueue;
    QLineEdit* m_borrowerNameInput;
    QLineEdit* m_lineNamaPeminjam;
    QLineEdit* m_lineBukuDipinjam;
    QComboBox* m_bookCombo;
    QDateEdit* m_dateEdit;
    
    QPushButton* m_btnAddToBorrow;
    QPushButton* m_btnEnqueue;
    QPushButton* m_btnProcessNext;
    QPushButton* m_btnClearQueue;
    
    QLabel* m_queueSizeLabel;
    QLabel* m_lblQueueSize;
    
    std::queue<BorrowRequest> m_queue;
    int m_nextBorrowId;
};

#endif // BORROWQUEUEPAGE_H