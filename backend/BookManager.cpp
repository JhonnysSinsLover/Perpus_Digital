#include "BookManager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QDateTime>
#include <algorithm>
#include <set>

BookManager::BookManager()
    : m_bstRoot(nullptr)
{
}

bool BookManager::loadFromJson(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray booksArray = root["books"].toArray();

    m_books.clear();
    for (const QJsonValue& value : booksArray) {
        if (value.isObject()) {
            Book book = Book::fromJson(value.toObject());
            m_books.push_back(book);
        }
    }

    qDebug() << "Loaded" << m_books.size() << "books from" << filePath;
    return true;
}

bool BookManager::saveToJson(const QString& filePath)
{
    QJsonArray booksArray;
    for (const Book& book : m_books) {
        booksArray.append(book.toJson());
    }

    QJsonObject root;
    root["books"] = booksArray;

    QJsonDocument doc(root);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Saved" << m_books.size() << "books to" << filePath;
    return true;
}

Book* BookManager::getBookById(int id)
{
    for (Book& book : m_books) {
        if (book.getId() == id) {
            return &book;
        }
    }
    return nullptr;
}

void BookManager::addBook(const Book& book)
{
    m_books.push_back(book);
}

bool BookManager::removeBook(int id)
{
    // Find the book to save it to undo stack before removing
    Book* bookToDelete = getBookById(id);
    if (bookToDelete) {
        // Push to undo stack (LIFO - Last In First Out)
        m_deletedBooks.push(*bookToDelete);
        qDebug() << "Added to undo stack:" << bookToDelete->getJudul();
    }
    
    auto it = std::remove_if(m_books.begin(), m_books.end(),
                             [id](const Book& book) { return book.getId() == id; });
    
    if (it != m_books.end()) {
        m_books.erase(it, m_books.end());
        return true;
    }
    return false;
}

bool BookManager::updateBook(const Book& book)
{
    Book* existingBook = getBookById(book.getId());
    if (existingBook) {
        *existingBook = book;
        return true;
    }
    return false;
}

// ============================================================================
// SORTING ALGORITHMS - QuickSort
// ============================================================================

void BookManager::quickSortByTitle(bool ascending)
{
    if (m_books.empty()) return;

    auto compare = ascending
        ? [](const Book& a, const Book& b) { 
            return a.getJudul().toLower() < b.getJudul().toLower(); 
          }
        : [](const Book& a, const Book& b) { 
            return a.getJudul().toLower() > b.getJudul().toLower(); 
          };

    quickSort(0, m_books.size() - 1, compare);
}

void BookManager::quickSortByYear(bool ascending)
{
    if (m_books.empty()) return;

    auto compare = ascending
        ? [](const Book& a, const Book& b) { return a.getTahun() < b.getTahun(); }
        : [](const Book& a, const Book& b) { return a.getTahun() > b.getTahun(); };

    quickSort(0, m_books.size() - 1, compare);
}

void BookManager::quickSortByRating(bool ascending)
{
    if (m_books.empty()) return;

    auto compare = ascending
        ? [](const Book& a, const Book& b) { return a.getRating() < b.getRating(); }
        : [](const Book& a, const Book& b) { return a.getRating() > b.getRating(); };

    quickSort(0, m_books.size() - 1, compare);
}

void BookManager::quickSortByAuthor(bool ascending)
{
    if (m_books.empty()) return;

    auto compare = ascending
        ? [](const Book& a, const Book& b) { 
            return a.getPenulis().toLower() < b.getPenulis().toLower(); 
          }
        : [](const Book& a, const Book& b) { 
            return a.getPenulis().toLower() > b.getPenulis().toLower(); 
          };

    quickSort(0, m_books.size() - 1, compare);
}

void BookManager::quickSort(int left, int right,
                           std::function<bool(const Book&, const Book&)> compare)
{
    if (left < right) {
        int pivotIndex = partition(left, right, compare);
        quickSort(left, pivotIndex - 1, compare);
        quickSort(pivotIndex + 1, right, compare);
    }
}

int BookManager::partition(int left, int right,
                          std::function<bool(const Book&, const Book&)> compare)
{
    Book pivot = m_books[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (compare(m_books[j], pivot)) {
            i++;
            std::swap(m_books[i], m_books[j]);
        }
    }
    
    std::swap(m_books[i + 1], m_books[right]);
    return i + 1;
}

// ============================================================================
// SEARCHING ALGORITHMS
// ============================================================================

Book* BookManager::binarySearchByTitle(const QString& title)
{
    if (m_books.empty()) return nullptr;

    int index = binarySearchImpl(
        title.toLower(), 
        0, 
        m_books.size() - 1,
        [](const Book& book) { return book.getJudul().toLower(); }
    );

    return (index != -1) ? &m_books[index] : nullptr;
}

int BookManager::binarySearchImpl(const QString& key, int left, int right,
                                 std::function<QString(const Book&)> getKey) const
{
    while (left <= right) {
        int mid = left + (right - left) / 2;
        QString midKey = getKey(m_books[mid]);

        if (midKey == key) {
            return mid;
        } else if (midKey < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

std::vector<Book> BookManager::searchByGenre(const QString& genre) const
{
    std::vector<Book> result;
    for (const Book& book : m_books) {
        if (book.hasGenre(genre)) {
            result.push_back(book);
        }
    }
    return result;
}

std::vector<Book> BookManager::searchByAuthor(const QString& author) const
{
    std::vector<Book> result;
    QString authorLower = author.toLower();
    
    for (const Book& book : m_books) {
        if (book.getPenulis().toLower().contains(authorLower)) {
            result.push_back(book);
        }
    }
    return result;
}

std::vector<Book> BookManager::searchByYearRange(int minYear, int maxYear) const
{
    std::vector<Book> result;
    for (const Book& book : m_books) {
        int year = book.getTahun();
        if (year >= minYear && year <= maxYear) {
            result.push_back(book);
        }
    }
    return result;
}

std::vector<QString> BookManager::getAllGenres() const
{
    std::set<QString> genreSet;
    
    for (const Book& book : m_books) {
        QStringList genres = book.getGenre();
        for (const QString& genre : genres) {
            genreSet.insert(genre);
        }
    }
    
    return std::vector<QString>(genreSet.begin(), genreSet.end());
}

// ============================================================================
// STACK - Undo Deletion Feature Implementation
// ============================================================================

bool BookManager::undoDelete()
{
    if (m_deletedBooks.empty()) {
        qDebug() << "Undo stack is empty";
        return false;
    }
    
    // Pop from stack (LIFO - Last In First Out)
    Book restoredBook = m_deletedBooks.top();
    m_deletedBooks.pop();
    
    // Add book back to collection
    m_books.push_back(restoredBook);
    
    qDebug() << "Restored book:" << restoredBook.getJudul();
    return true;
}

// ============================================================================
// QUEUE - Book Borrowing Queue Feature Implementation
// ============================================================================

bool BookManager::addToBorrowQueue(const QString& borrowerName, int bookId)
{
    Book* book = getBookById(bookId);
    if (!book) {
        qDebug() << "Book with ID" << bookId << "not found";
        return false;
    }
    
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    BorrowRequest request(borrowerName, bookId, book->getJudul(), currentTime);
    
    // Add to queue (FIFO - First In First Out)
    m_borrowQueue.push(request);
    
    qDebug() << "Added borrow request:" << borrowerName << "for" << book->getJudul();
    return true;
}

BookManager::BorrowRequest BookManager::processNextBorrow()
{
    if (m_borrowQueue.empty()) {
        qDebug() << "Borrow queue is empty";
        return BorrowRequest();
    }
    
    // Get front of queue (FIFO - First In First Out)
    BorrowRequest request = m_borrowQueue.front();
    m_borrowQueue.pop();
    
    qDebug() << "Processed borrow request:" << request.borrowerName << "for" << request.bookTitle;
    return request;
}

std::vector<BookManager::BorrowRequest> BookManager::getAllBorrowRequests() const
{
    std::vector<BorrowRequest> requests;
    std::queue<BorrowRequest> tempQueue = m_borrowQueue;
    
    while (!tempQueue.empty()) {
        requests.push_back(tempQueue.front());
        tempQueue.pop();
    }
    
    return requests;
}

// ============================================================================
// PRIORITY QUEUE - Popular Books by Rating Implementation
// ============================================================================

std::vector<Book> BookManager::getTopRatedBooks(int topN)
{
    if (m_books.empty()) {
        return std::vector<Book>();
    }
    
    // Create max heap (priority queue) based on rating
    auto compareRating = [](const Book& a, const Book& b) {
        return a.getRating() < b.getRating(); // Max heap
    };
    
    std::priority_queue<Book, std::vector<Book>, decltype(compareRating)> maxHeap(compareRating);
    
    // Add all books to priority queue
    for (const Book& book : m_books) {
        maxHeap.push(book);
    }
    
    // Extract top N books
    std::vector<Book> topBooks;
    for (int i = 0; i < topN && !maxHeap.empty(); i++) {
        topBooks.push_back(maxHeap.top());
        maxHeap.pop();
    }
    
    return topBooks;
}

std::vector<Book> BookManager::getPopularBooks(double minRating)
{
    std::vector<Book> popularBooks;
    
    for (const Book& book : m_books) {
        if (book.getRating() >= minRating) {
            popularBooks.push_back(book);
        }
    }
    
    // Sort by rating (descending)
    std::sort(popularBooks.begin(), popularBooks.end(),
              [](const Book& a, const Book& b) {
                  return a.getRating() > b.getRating();
              });
    
    return popularBooks;
}

// ============================================================================
// BINARY SEARCH TREE - Catalog Structure Implementation
// ============================================================================

void BookManager::buildBST()
{
    m_bstRoot = nullptr;
    
    for (const Book& book : m_books) {
        m_bstRoot = insertBST(m_bstRoot, book);
    }
    
    qDebug() << "Built BST with" << m_books.size() << "books";
}

std::shared_ptr<BookManager::BSTNode> BookManager::insertBST(std::shared_ptr<BSTNode> node, const Book& book)
{
    if (!node) {
        return std::make_shared<BSTNode>(book);
    }
    
    // Compare by title (case-insensitive)
    QString currentTitle = node->book.getJudul().toLower();
    QString newTitle = book.getJudul().toLower();
    
    if (newTitle < currentTitle) {
        node->left = insertBST(node->left, book);
    } else if (newTitle > currentTitle) {
        node->right = insertBST(node->right, book);
    }
    // If equal, don't insert duplicate
    
    return node;
}

Book* BookManager::searchBST(const QString& title)
{
    return searchBSTHelper(m_bstRoot, title);
}

Book* BookManager::searchBSTHelper(std::shared_ptr<BSTNode> node, const QString& title)
{
    if (!node) {
        return nullptr;
    }
    
    QString currentTitle = node->book.getJudul().toLower();
    QString searchTitle = title.toLower();
    
    if (searchTitle == currentTitle) {
        return &(node->book);
    } else if (searchTitle < currentTitle) {
        return searchBSTHelper(node->left, title);
    } else {
        return searchBSTHelper(node->right, title);
    }
}

std::vector<Book> BookManager::getBSTInOrder()
{
    std::vector<Book> result;
    inOrderTraversal(m_bstRoot, result);
    return result;
}

void BookManager::inOrderTraversal(std::shared_ptr<BSTNode> node, std::vector<Book>& result)
{
    if (!node) {
        return;
    }
    
    inOrderTraversal(node->left, result);
    result.push_back(node->book);
    inOrderTraversal(node->right, result);
}
