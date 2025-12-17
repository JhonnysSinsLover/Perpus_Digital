#ifndef BOOKPREVIEWDIALOG_H
#define BOOKPREVIEWDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLineEdit>
#include <QTextEdit> // Ganti QLabel sinopsis dengan TextEdit read-only agar bisa scroll
#include "../backend/Book.h"
#include "../backend/DatabaseManager.h"

/**
 * @brief Dialog for previewing book details and borrowing
 * Updated with Modern UI/UX consistent with Collection Page
 */
class BookPreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookPreviewDialog(const Book& book, QWidget *parent = nullptr);
    ~BookPreviewDialog();

signals:
    void bookBorrowed(int bookId, const QString& borrowerName);

private slots:
    void onBorrowBook();

private:
    void setupUI();
    void loadImage(); // [BARU] Load gambar dengan fallback placeholder
    QColor getPastelColor(const QString& text); // [BARU] Generator warna
    
    // Helper UI
    QWidget* createInfoRow(const QString& icon, const QString& label, const QString& value, bool isBadge = false);
    
    Book m_book;
    
    // UI Elements
    QLabel* m_lblCoverImage;
    QLabel* m_lblTitle;
    QLabel* m_lblAuthor;
    
    // Detail containers
    QWidget* m_detailsContainer;
    
    QPushButton* m_btnBorrow;
    QPushButton* m_btnClose;
};

#endif // BOOKPREVIEWDIALOG_H