#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "../backend/Book.h"

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr, Book* editBook = nullptr);
    ~AddBookDialog();
    
    Book getBook() const;

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void onSelectImage();
    void validateInput();

private:
    void setupUI();
    void loadBookData(Book* book);
    bool validate();

    // Helper untuk membuat input field dengan label modern
    QWidget* createInputGroup(const QString& labelText, QWidget* inputWidget, QLabel* errorLabel = nullptr);

    // Form fields
    QLineEdit* m_lineJudul;
    QLineEdit* m_linePenulis;
    QTextEdit* m_textGenre;
    QSpinBox* m_spinTahun;
    QDoubleSpinBox* m_spinRating;
    
    // Image Handling
    QLineEdit* m_lineImagePath; // Hidden path storage
    QPushButton* m_btnSelectImage;
    QLabel* m_imagePreview; // Preview container
    
    // Error labels
    QLabel* m_labelJudulError;
    QLabel* m_labelPenulisError;
    QLabel* m_labelGenreError;
    
    // Action buttons
    QPushButton* m_btnSave;
    QPushButton* m_btnCancel;
    
    // Data logic
    int m_bookId;
    bool m_editMode;
    QString m_selectedImagePath;
    Book m_resultBook;
};

#endif // ADDBOOKDIALOG_H