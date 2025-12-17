#ifndef BOOKCARDWIDGET_H
#define BOOKCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include "../backend/Book.h" 

class BookCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookCardWidget(const Book& book, QWidget *parent = nullptr, bool showEditDelete = true);
    
    int getBookId() const { return m_book.getId(); }
    void updateData(const Book& book);
    void setEditDeleteVisible(bool visible);

signals:
    void editRequested(int bookId);
    void deleteRequested(int bookId);
    void cardClicked(int bookId);
    void previewRequested(int bookId);

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    
    // [BARU] Menangani resize agar overlay selalu penuh
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupUI();
    void applyStyles();
    void refreshContent(); 
    void loadImage();
    QColor getPastelColor(const QString& text);

    Book m_book;
    
    // UI Components
    QWidget* m_imageContainer;
    QLabel* m_imageLabel;
    QFrame* m_hoverOverlay;
    
    QLabel* m_titleLabel;
    QLabel* m_authorLabel;
    QLabel* m_ratingLabel;
    QLabel* m_yearLabel;
    QLabel* m_genreLabel;
    
    QPushButton* m_editBtn;
    QPushButton* m_deleteBtn;
};

#endif // BOOKCARDWIDGET_H