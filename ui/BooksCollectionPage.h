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
#include <QGraphicsDropShadowEffect>
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

private slots:
    void onSearchTextChanged();
    void onSearchByGenre();
    void onSearchByAuthor();
    void onClearSearch();
    void onSortByTitle();
    void onSortByYear();
    void onSortByRating();
    void onSortByAuthor();
    void onEditBook();
    void onDeleteBook();
    void onUndoDelete();
    void onTableSelectionChanged();
    void onToggleView();
    void onBuildBST();
    void onSearchBST();

private:
    void setupUI();
    void createHeaderSection(QVBoxLayout* mainLayout);
    void createControlSection(QVBoxLayout* contentLayout);
    void createContentSection(QVBoxLayout* contentLayout);
    void createActionSection(QVBoxLayout* contentLayout);
    
    // Helpers
    QFrame* createCardFrame();
    
    void loadBooksToTable(const std::vector<Book>& books);
    void loadBooksToCards(const std::vector<Book>& books);
    void populateGenreComboBox();
    
    // UI Elements
    QTableWidget* m_tableBooks;
    QScrollArea* m_cardScrollArea;
    QWidget* m_cardContainer;
    QGridLayout* m_cardLayout;
    
    QLineEdit* m_searchBox;
    QComboBox* m_genreCombo;
    QLineEdit* m_authorSearch;
    
    QPushButton* m_btnEditBook;
    QPushButton* m_btnDeleteBook;
    QPushButton* m_btnUndoDelete;
    QPushButton* m_btnToggleView;
    QPushButton* m_btnBuildBST;
    QPushButton* m_btnSearchBST;
    QStackedWidget* m_viewStack;
    QLineEdit* m_bstSearchBox;
    
    // View state
    bool m_isCardView;
    bool m_sortAscending;
    QString m_lastSortColumn;
};

#endif // BOOKSCOLLECTIONPAGE_H