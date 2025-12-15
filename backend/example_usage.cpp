#include "BookManager.h"
#include "Graph.h"
#include <QDebug>

/**
 * @file example_usage.cpp
 * @brief Example demonstrating how to use BookManager and Graph classes
 * 
 * This file shows typical usage patterns for the digital library system.
 * Compile and run this to see the backend in action.
 */

void demonstrateBookManager()
{
    qDebug() << "\n========== BOOKMANAGER DEMO ==========\n";
    
    BookManager manager;
    
    // 1. Load books from JSON
    if (manager.loadFromJson("data/books.json")) {
        qDebug() << "Successfully loaded" << manager.getBookCount() << "books\n";
    }
    
    // 2. Display all books
    qDebug() << "All books:";
    std::vector<Book> books = manager.getAllBooks();
    for (size_t i = 0; i < std::min(books.size(), size_t(5)); i++) {
        qDebug() << books[i].toString();
    }
    qDebug() << "... and" << (books.size() - 5) << "more\n";
    
    // 3. Sort by title
    qDebug() << "\n--- Sorting by title (ascending) ---";
    manager.quickSortByTitle(true);
    books = manager.getAllBooks();
    for (size_t i = 0; i < std::min(books.size(), size_t(5)); i++) {
        qDebug() << books[i].toString();
    }
    
    // 4. Sort by year
    qDebug() << "\n--- Sorting by year (descending) ---";
    manager.quickSortByYear(false);
    books = manager.getAllBooks();
    for (size_t i = 0; i < std::min(books.size(), size_t(5)); i++) {
        qDebug() << books[i].toString();
    }
    
    // 5. Sort by rating
    qDebug() << "\n--- Top rated books ---";
    manager.quickSortByRating(false);
    books = manager.getAllBooks();
    for (size_t i = 0; i < std::min(books.size(), size_t(5)); i++) {
        qDebug() << books[i].toString();
    }
    
    // 6. Binary search (must sort first!)
    qDebug() << "\n--- Binary search for 'Bumi Manusia' ---";
    manager.quickSortByTitle(true); // Sort first for binary search
    Book* found = manager.binarySearchByTitle("Bumi Manusia");
    if (found) {
        qDebug() << "Found:" << found->toString();
    } else {
        qDebug() << "Not found";
    }
    
    // 7. Search by genre
    qDebug() << "\n--- Books in 'Fantasi' genre ---";
    std::vector<Book> fantasyBooks = manager.searchByGenre("Fantasi");
    for (const Book& book : fantasyBooks) {
        qDebug() << book.toString();
    }
    
    // 8. Search by author
    qDebug() << "\n--- Books by 'Tere Liye' ---";
    std::vector<Book> tereLiyeBooks = manager.searchByAuthor("Tere Liye");
    for (const Book& book : tereLiyeBooks) {
        qDebug() << book.toString();
    }
    
    // 9. Search by year range
    qDebug() << "\n--- Books from 2010-2020 ---";
    std::vector<Book> modernBooks = manager.searchByYearRange(2010, 2020);
    qDebug() << "Found" << modernBooks.size() << "books in this range";
    
    // 10. Get all genres
    qDebug() << "\n--- All genres in library ---";
    std::vector<QString> genres = manager.getAllGenres();
    QString genreList;
    for (const QString& genre : genres) {
        genreList += genre + ", ";
    }
    genreList.chop(2);
    qDebug() << genreList;
}

void demonstrateGraph()
{
    qDebug() << "\n\n========== GRAPH DEMO ==========\n";
    
    BookManager manager;
    manager.loadFromJson("data/books.json");
    
    Graph graph;
    
    // 1. Build graph from books
    qDebug() << "Building genre recommendation graph...";
    graph.buildGraph(manager.getAllBooks());
    qDebug() << "Graph built with" << graph.getNodeCount() << "genres and" 
             << graph.getEdgeCount() << "connections\n";
    
    // 2. Display graph structure
    qDebug() << "Graph structure:";
    qDebug() << graph.toString();
    
    // 3. Get connected genres
    QString testGenre = "Drama";
    qDebug() << "\n--- Genres connected to '" << testGenre << "' ---";
    std::set<QString> connected = graph.getConnectedGenres(testGenre);
    for (const QString& genre : connected) {
        qDebug() << "  -" << genre;
    }
    qDebug() << "Total:" << graph.getGenreDegree(testGenre) << "connections";
    
    // 4. Get recommendations
    qDebug() << "\n--- Book recommendations based on 'Fantasi' genre ---";
    std::vector<Book> recommendations = graph.getRecommendation("Fantasi", 
                                                                manager.getAllBooks(), 
                                                                2);
    qDebug() << "Found" << recommendations.size() << "recommendations:";
    for (size_t i = 0; i < std::min(recommendations.size(), size_t(8)); i++) {
        qDebug() << (i+1) << "." << recommendations[i].toString();
        QString genres = recommendations[i].getGenre().join(", ");
        qDebug() << "   Genres:" << genres;
    }
    
    // 5. Find path between genres
    qDebug() << "\n--- Finding path from 'Fantasi' to 'Sejarah' ---";
    std::vector<QString> path = graph.findPath("Fantasi", "Sejarah");
    if (!path.empty()) {
        QString pathStr;
        for (size_t i = 0; i < path.size(); i++) {
            pathStr += path[i];
            if (i < path.size() - 1) pathStr += " → ";
        }
        qDebug() << "Path found:" << pathStr;
    } else {
        qDebug() << "No path found";
    }
    
    // 6. Try different genres
    qDebug() << "\n--- Recommendations for 'Romansa' ---";
    recommendations = graph.getRecommendation("Romansa", 
                                             manager.getAllBooks(), 
                                             1);
    qDebug() << "Top 5 recommendations:";
    for (size_t i = 0; i < std::min(recommendations.size(), size_t(5)); i++) {
        qDebug() << (i+1) << "." << recommendations[i].toString();
    }
}

void demonstrateAdvancedFeatures()
{
    qDebug() << "\n\n========== ADVANCED FEATURES ==========\n";
    
    BookManager manager;
    manager.loadFromJson("data/books.json");
    
    // 1. Add new book
    qDebug() << "--- Adding new book ---";
    Book newBook(99, "Test Book", "Test Author", 
                 QStringList{"Fiksi", "Drama"}, 2024, 4.5);
    manager.addBook(newBook);
    qDebug() << "Book added. Total books:" << manager.getBookCount();
    
    // 2. Get book by ID
    qDebug() << "\n--- Getting book by ID ---";
    Book* book = manager.getBookById(1);
    if (book) {
        qDebug() << "Book #1:" << book->toString();
    }
    
    // 3. Update book
    qDebug() << "\n--- Updating book ---";
    if (book) {
        book->setRating(5.0);
        manager.updateBook(*book);
        qDebug() << "Updated:" << book->toString();
    }
    
    // 4. Remove book
    qDebug() << "\n--- Removing book ---";
    if (manager.removeBook(99)) {
        qDebug() << "Book removed. Total books:" << manager.getBookCount();
    }
    
    // 5. Save to JSON
    qDebug() << "\n--- Saving to JSON ---";
    if (manager.saveToJson("data/books_backup.json")) {
        qDebug() << "Books saved successfully";
    }
}

/**
 * @brief Main function - runs all demonstrations
 * 
 * Uncomment this main function to compile as standalone executable.
 * Otherwise, these functions can be called from MainWindow.
 */
/*
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    demonstrateBookManager();
    demonstrateGraph();
    demonstrateAdvancedFeatures();
    
    qDebug() << "\n========== ALL DEMOS COMPLETE ==========\n";
    
    return 0;
}
*/
