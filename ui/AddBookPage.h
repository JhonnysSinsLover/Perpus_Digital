#ifndef ADDBOOKPAGE_H
#define ADDBOOKPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "../backend/DatabaseManager.h"

class AddBookPage : public QWidget
{
    Q_OBJECT

public:
    explicit AddBookPage(QWidget *parent = nullptr);
    ~AddBookPage();

signals:
    void bookAdded();

private slots:
    void onAddBook();
    void onClearForm();
    void validateInput();
    void onBrowseImage();

private:
    void setupUI();
    // Helper methods untuk membagi layout
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createFormSection(QVBoxLayout* contentLayout);
    
    // Helper untuk membuat input field dengan label di atasnya
    QWidget* createInputGroup(const QString& labelText, QWidget* inputWidget, QLabel* errorLabel = nullptr);

    bool validate();
    void clearForm();
    int generateNewId();
    
    // Form widgets
    QLineEdit* m_lineJudul;
    QLineEdit* m_linePenulis;
    QTextEdit* m_textGenre;
    QSpinBox* m_spinTahun;
    QDoubleSpinBox* m_spinRating;
    
    // Image Handling
    QLineEdit* m_lineImagePath; // Hidden or Read-only
    QPushButton* m_btnBrowseImage;
    QLabel* m_labelImagePreview;
    
    // Error Labels
    QLabel* m_labelJudulError;
    QLabel* m_labelPenulisError;
    QLabel* m_labelGenreError;
    
    // Action Buttons
    QPushButton* m_btnAdd;
    QPushButton* m_btnClear;
    
    QString m_selectedImagePath;
};

#endif // ADDBOOKPAGE_H