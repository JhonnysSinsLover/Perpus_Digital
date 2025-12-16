#ifndef BOOKSCOLLECTIONPAGE_H
#define BOOKSCOLLECTIONPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QStackedWidget>
#include <QFrame>
#include <QLabel>
#include <QResizeEvent>
#include "../backend/DatabaseManager.h"

class BooksCollectionPage : public QWidget
{
    Q_OBJECT

public:
    explicit BooksCollectionPage(QWidget *parent = nullptr);
    ~BooksCollectionPage();

    void refreshTable();
    int getSelectedBookId();

signals:
    void bookSelected(int bookId);
    void editBookRequested(int bookId);
    void deleteBookRequested(int bookId);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onSearchTextChanged();
    void onFilterChanged();
    void onClearSearch();
    void onEditBook();
    void onDeleteBook();
    void onUndoDelete();
    void onTableSelectionChanged();
    void onToggleView();
    
    // BST Slots
    void onBuildBST();
    void onSearchBST();
    
    // [BARU] Preview Slot
    void onPreviewBook();

private:
    void setupUI();
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createControlSection(QVBoxLayout* containerLayout); 
    void createContentSection(QVBoxLayout* contentLayout);
    void createActionSection(QVBoxLayout* mainLayout); 

    // Helpers
    QFrame* createCardFrame();
    void loadBooksToTable(const std::vector<Book>& books);
    void loadBooksToCards(const std::vector<Book>& books);
    void populateGenreComboBox();

    // Data Cache
    std::vector<Book> m_currentBooks; 

    // UI Elements
    QTableWidget* m_tableBooks;
    QScrollArea* m_contentScrollArea;
    QWidget* m_cardContainer;
    QGridLayout* m_cardLayout;
    QStackedWidget* m_viewStack;

    // Controls
    QLineEdit* m_searchBox;
    QComboBox* m_genreCombo;
    QComboBox* m_sortCombo;
    QLineEdit* m_bstSearchBox; 

    // Buttons
    QPushButton* m_btnEditBook;
    QPushButton* m_btnDeleteBook;
    QPushButton* m_btnUndoDelete;
    QPushButton* m_btnToggleView;
    QPushButton* m_btnBuildBST;

    bool m_isCardView;
};

#endif // BOOKSCOLLECTIONPAGE_H