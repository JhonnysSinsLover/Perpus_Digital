#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include "Book.h"
#include <vector>
#include <stack>
#include <queue>
#include <QString>
#include <QJsonDocument>
#include <functional>
#include <memory>

/**
 * @brief Manager class for handling book collections
 * 
 * This class manages a collection of books with features including:
 * - Loading from JSON file
 * - Sorting algorithms (QuickSort)
 * - Searching algorithms (Binary Search)
 * - CRUD operations
 */
class BookManager
{
public:
    /**
     * @brief Default constructor
     */
    BookManager();

    /**
     * @brief Load books from JSON file
     * @param filePath Path to the JSON file
     * @return true if successful, false otherwise
     */
    bool loadFromJson(const QString& filePath);

    /**
     * @brief Save books to JSON file
     * @param filePath Path to save the JSON file
     * @return true if successful, false otherwise
     */
    bool saveToJson(const QString& filePath);

    /**
     * @brief Get all books in the collection
     * @return Vector of all books
     */
    std::vector<Book> getAllBooks() const { return m_books; }

    /**
     * @brief Get book by ID
     * @param id Book ID
     * @return Pointer to book if found, nullptr otherwise
     */
    Book* getBookById(int id);

    /**
     * @brief Add a new book to the collection
     * @param book Book to add
     */
    void addBook(const Book& book);

    /**
     * @brief Remove book by ID
     * @param id Book ID to remove
     * @return true if removed, false if not found
     */
    bool removeBook(int id);

    /**
     * @brief Update existing book
     * @param book Updated book data
     * @return true if updated, false if not found
     */
    bool updateBook(const Book& book);

    /**
     * @brief Get number of books in collection
     * @return Number of books
     */
    size_t getBookCount() const { return m_books.size(); }

    /**
     * @brief Clear all books from collection
     */
    void clear() { m_books.clear(); }

    // Sorting methods
    /**
     * @brief Sort books by title using QuickSort
     * @param ascending True for ascending order, false for descending
     */
    void quickSortByTitle(bool ascending = true);

    /**
     * @brief Sort books by year using QuickSort
     * @param ascending True for ascending order, false for descending
     */
    void quickSortByYear(bool ascending = true);

    /**
     * @brief Sort books by rating using QuickSort
     * @param ascending True for ascending order, false for descending
     */
    void quickSortByRating(bool ascending = true);

    /**
     * @brief Sort books by author using QuickSort
     * @param ascending True for ascending order, false for descending
     */
    void quickSortByAuthor(bool ascending = true);

    // Searching methods
    /**
     * @brief Binary search for book by title (requires sorted data)
     * @param title Title to search for
     * @return Pointer to found book, nullptr if not found
     */
    Book* binarySearchByTitle(const QString& title);

    /**
     * @brief Linear search for books by genre
     * @param genre Genre to search for
     * @return Vector of books with matching genre
     */
    std::vector<Book> searchByGenre(const QString& genre) const;

    /**
     * @brief Linear search for books by author
     * @param author Author to search for
     * @return Vector of books by the author
     */
    std::vector<Book> searchByAuthor(const QString& author) const;

    /**
     * @brief Search books by year range
     * @param minYear Minimum year
     * @param maxYear Maximum year
     * @return Vector of books within the year range
     */
    std::vector<Book> searchByYearRange(int minYear, int maxYear) const;

    /**
     * @brief Get all unique genres from all books
     * @return Vector of unique genre names
     */
    std::vector<QString> getAllGenres() const;

    // ============================================================================
    // STACK - Undo Deletion Feature
    // ============================================================================
    
    /**
     * @brief Undo last book deletion
     * Restores the last deleted book from the undo stack (LIFO)
     * @return true if undo successful, false if stack is empty
     */
    bool undoDelete();
    
    /**
     * @brief Check if undo is available
     * @return true if there are deleted books that can be restored
     */
    bool canUndo() const { return !m_deletedBooks.empty(); }
    
    /**
     * @brief Get size of undo stack
     * @return Number of books that can be restored
     */
    size_t getUndoStackSize() const { return m_deletedBooks.size(); }
    
    /**
     * @brief Clear undo stack
     */
    void clearUndoStack() { while (!m_deletedBooks.empty()) m_deletedBooks.pop(); }

    // ============================================================================
    // QUEUE - Book Borrowing Queue Feature
    // ============================================================================
    
    /**
     * @brief Structure for borrowing request
     */
    struct BorrowRequest {
        QString borrowerName;
        int bookId;
        QString bookTitle;
        QString requestTime;
        
        BorrowRequest() = default;
        BorrowRequest(const QString& name, int id, const QString& title, const QString& time)
            : borrowerName(name), bookId(id), bookTitle(title), requestTime(time) {}
    };
    
    /**
     * @brief Add borrower to queue
     * @param borrowerName Name of borrower
     * @param bookId ID of book to borrow
     * @return true if added successfully
     */
    bool addToBorrowQueue(const QString& borrowerName, int bookId);
    
    /**
     * @brief Process next borrowing request (FIFO)
     * @return BorrowRequest that was processed, or empty if queue is empty
     */
    BorrowRequest processNextBorrow();
    
    /**
     * @brief Check if borrow queue is empty
     * @return true if queue is empty
     */
    bool isBorrowQueueEmpty() const { return m_borrowQueue.empty(); }
    
    /**
     * @brief Get size of borrow queue
     * @return Number of pending borrow requests
     */
    size_t getBorrowQueueSize() const { return m_borrowQueue.size(); }
    
    /**
     * @brief Get all pending borrow requests (without removing)
     * @return Vector of all requests in queue
     */
    std::vector<BorrowRequest> getAllBorrowRequests() const;
    
    /**
     * @brief Clear borrow queue
     */
    void clearBorrowQueue() { while (!m_borrowQueue.empty()) m_borrowQueue.pop(); }

    // ============================================================================
    // PRIORITY QUEUE - Popular Books by Rating
    // ============================================================================
    
    /**
     * @brief Get top N books by rating using priority queue (Max Heap)
     * @param topN Number of top books to retrieve
     * @return Vector of top rated books
     */
    std::vector<Book> getTopRatedBooks(int topN = 5);
    
    /**
     * @brief Get books with rating above threshold
     * @param minRating Minimum rating threshold
     * @return Vector of books with rating >= minRating
     */
    std::vector<Book> getPopularBooks(double minRating = 4.5);

    // ============================================================================
    // BINARY SEARCH TREE - Catalog Structure (Optional)
    // ============================================================================
    
    /**
     * @brief Node structure for BST
     */
    struct BSTNode {
        Book book;
        std::shared_ptr<BSTNode> left;
        std::shared_ptr<BSTNode> right;
        
        BSTNode(const Book& b) : book(b), left(nullptr), right(nullptr) {}
    };
    
    /**
     * @brief Build BST from current books (sorted by title)
     */
    void buildBST();
    
    /**
     * @brief Search book in BST by title (exact match)
     * @param title Title to search
     * @return Pointer to book if found, nullptr otherwise
     */
    Book* searchBST(const QString& title);
    
    /**
     * @brief Search books in BST by partial title (flexible, case-insensitive)
     * Uses in-order traversal to find all books that match the partial title
     * @param partialTitle Partial title to search (can be lowercase, uppercase, or incomplete)
     * @return Vector of books that match the partial title
     */
    std::vector<Book> searchBSTPartial(const QString& partialTitle);
    
    /**
     * @brief Get BST in-order traversal (sorted books)
     * @return Vector of books in sorted order
     */
    std::vector<Book> getBSTInOrder();
    
    /**
     * @brief Check if BST is built
     * @return true if BST exists
     */
    bool hasBST() const { return m_bstRoot != nullptr; }
    
    /**
     * @brief Clear BST
     */
    void clearBST() { m_bstRoot = nullptr; }

private:
    std::vector<Book> m_books;  ///< Collection of books
    
    // Stack for undo deletion (LIFO)
    std::stack<Book> m_deletedBooks;
    
    // Queue for borrowing requests (FIFO)
    std::queue<BorrowRequest> m_borrowQueue;
    
    // Binary Search Tree root
    std::shared_ptr<BSTNode> m_bstRoot;

    /**
     * @brief QuickSort implementation
     * @param left Left index
     * @param right Right index
     * @param compare Comparison function
     */
    void quickSort(int left, int right, 
                   std::function<bool(const Book&, const Book&)> compare);

    /**
     * @brief Partition for QuickSort
     * @param left Left index
     * @param right Right index
     * @param compare Comparison function
     * @return Partition index
     */
    int partition(int left, int right,
                  std::function<bool(const Book&, const Book&)> compare);

    /**
     * @brief Binary search implementation
     * @param key Search key
     * @param left Left index
     * @param right Right index
     * @param getKey Function to extract key from book
     * @return Index of found book, -1 if not found
     */
    int binarySearchImpl(const QString& key, int left, int right,
                         std::function<QString(const Book&)> getKey) const;

    // BST helper functions
    /**
     * @brief Insert node into BST recursively
     * @param node Current node
     * @param book Book to insert
     * @return New node pointer
     */
    std::shared_ptr<BSTNode> insertBST(std::shared_ptr<BSTNode> node, const Book& book);
    
    /**
     * @brief Search in BST recursively (exact match)
     * @param node Current node
     * @param title Title to search
     * @return Pointer to book if found, nullptr otherwise
     */
    Book* searchBSTHelper(std::shared_ptr<BSTNode> node, const QString& title);
    
    /**
     * @brief Search in BST recursively with partial match (flexible)
     * @param node Current node
     * @param partialTitle Partial title to search
     * @param results Vector to store matching books
     */
    void searchBSTPartialHelper(std::shared_ptr<BSTNode> node, const QString& partialTitle, std::vector<Book>& results);
    
    /**
     * @brief In-order traversal of BST
     * @param node Current node
     * @param result Result vector to fill
     */
    void inOrderTraversal(std::shared_ptr<BSTNode> node, std::vector<Book>& result);
};

#endif // BOOKMANAGER_H
