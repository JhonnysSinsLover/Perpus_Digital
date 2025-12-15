#ifndef TEST_BACKEND_H
#define TEST_BACKEND_H

#include "BookManager.h"
#include "Graph.h"
#include "Sorting.h"
#include "Searching.h"
#include <QDebug>

/**
 * @brief Simple test suite for backend classes
 */
class TestBackend
{
public:
    static void runAllTests()
    {
        qDebug() << "========== RUNNING BACKEND TESTS ==========\n";
        
        testBook();
        testBookManager();
        testGraph();
        testSorting();
        testSearching();
        
        qDebug() << "\n========== ALL TESTS COMPLETE ==========";
    }

private:
    static void testBook()
    {
        qDebug() << "TEST: Book class";
        
        Book book(1, "Test Book", "Test Author", 
                  QStringList{"Genre1", "Genre2"}, 2024, 4.5);
        
        Q_ASSERT(book.getId() == 1);
        Q_ASSERT(book.getJudul() == "Test Book");
        Q_ASSERT(book.hasGenre("Genre1"));
        Q_ASSERT(!book.hasGenre("Genre3"));
        
        qDebug() << "  ✓ Book creation and getters work";
        qDebug() << "  ✓ hasGenre() works";
        
        // Test JSON conversion
        QJsonObject json = book.toJson();
        Book book2 = Book::fromJson(json);
        Q_ASSERT(book2.getId() == book.getId());
        Q_ASSERT(book2.getJudul() == book.getJudul());
        
        qDebug() << "  ✓ JSON serialization works\n";
    }

    static void testBookManager()
    {
        qDebug() << "TEST: BookManager class";
        
        BookManager manager;
        
        // Add books
        for (int i = 1; i <= 5; i++) {
            Book book(i, QString("Book %1").arg(i), "Author", 
                     QStringList{"Genre"}, 2020 + i, 4.0 + i * 0.1);
            manager.addBook(book);
        }
        
        Q_ASSERT(manager.getBookCount() == 5);
        qDebug() << "  ✓ addBook() works";
        
        // Get book by ID
        Book* book = manager.getBookById(3);
        Q_ASSERT(book != nullptr);
        Q_ASSERT(book->getId() == 3);
        qDebug() << "  ✓ getBookById() works";
        
        // Sort by year
        manager.quickSortByYear(true);
        std::vector<Book> books = manager.getAllBooks();
        Q_ASSERT(books[0].getTahun() <= books[1].getTahun());
        qDebug() << "  ✓ quickSortByYear() works";
        
        // Sort by title
        manager.quickSortByTitle(true);
        books = manager.getAllBooks();
        Q_ASSERT(books[0].getJudul() <= books[1].getJudul());
        qDebug() << "  ✓ quickSortByTitle() works";
        
        // Binary search (on sorted data)
        Book* found = manager.binarySearchByTitle("Book 3");
        Q_ASSERT(found != nullptr);
        Q_ASSERT(found->getJudul() == "Book 3");
        qDebug() << "  ✓ binarySearchByTitle() works";
        
        // Remove book
        bool removed = manager.removeBook(3);
        Q_ASSERT(removed == true);
        Q_ASSERT(manager.getBookCount() == 4);
        qDebug() << "  ✓ removeBook() works\n";
    }

    static void testGraph()
    {
        qDebug() << "TEST: Graph class";
        
        Graph graph;
        
        // Add edges
        graph.addEdge("GenreA", "GenreB");
        graph.addEdge("GenreB", "GenreC");
        
        Q_ASSERT(graph.hasEdge("GenreA", "GenreB"));
        Q_ASSERT(graph.hasEdge("GenreB", "GenreA")); // Undirected
        qDebug() << "  ✓ addEdge() and hasEdge() work";
        
        Q_ASSERT(graph.getGenreDegree("GenreB") == 2);
        qDebug() << "  ✓ getGenreDegree() works";
        
        // Test with books
        BookManager manager;
        Book book1(1, "Book1", "Author", QStringList{"A", "B"}, 2020, 4.5);
        Book book2(2, "Book2", "Author", QStringList{"B", "C"}, 2021, 4.6);
        manager.addBook(book1);
        manager.addBook(book2);
        
        Graph bookGraph;
        bookGraph.buildGraph(manager.getAllBooks());
        
        Q_ASSERT(bookGraph.hasEdge("A", "B"));
        Q_ASSERT(bookGraph.hasEdge("B", "C"));
        qDebug() << "  ✓ buildGraph() works";
        
        // Test recommendations
        std::vector<Book> recs = bookGraph.getRecommendation("A", 
                                                             manager.getAllBooks(), 
                                                             2);
        Q_ASSERT(recs.size() > 0);
        qDebug() << "  ✓ getRecommendation() works";
        
        // Test path finding
        std::vector<QString> path = bookGraph.findPath("A", "C");
        Q_ASSERT(!path.empty());
        Q_ASSERT(path[0] == "A");
        Q_ASSERT(path[path.size()-1] == "C");
        qDebug() << "  ✓ findPath() works\n";
    }

    static void testSorting()
    {
        qDebug() << "TEST: Sorting algorithms";
        
        std::vector<int> arr = {5, 2, 8, 1, 9, 3};
        
        // Test QuickSort
        std::vector<int> arr1 = arr;
        Sorting::quickSort(arr1, 0, arr1.size() - 1,
                          [](int a, int b) { return a < b; });
        Q_ASSERT(arr1[0] == 1);
        Q_ASSERT(arr1[arr1.size()-1] == 9);
        qDebug() << "  ✓ QuickSort works";
        
        // Test MergeSort
        std::vector<int> arr2 = arr;
        Sorting::mergeSort(arr2, 0, arr2.size() - 1,
                          [](int a, int b) { return a < b; });
        Q_ASSERT(arr2[0] == 1);
        Q_ASSERT(arr2[arr2.size()-1] == 9);
        qDebug() << "  ✓ MergeSort works\n";
    }

    static void testSearching()
    {
        qDebug() << "TEST: Searching algorithms";
        
        std::vector<int> sortedArr = {1, 3, 5, 7, 9, 11, 13};
        
        // Test Binary Search
        int idx = Searching::binarySearch(sortedArr, 7,
                                         [](int a, int b) { return a < b; });
        Q_ASSERT(idx == 3);
        qDebug() << "  ✓ Binary Search works";
        
        // Test Linear Search
        std::vector<int> unsortedArr = {5, 2, 8, 1, 9};
        idx = Searching::linearSearch(unsortedArr, 8);
        Q_ASSERT(idx == 2);
        qDebug() << "  ✓ Linear Search works";
        
        // Test Jump Search
        idx = Searching::jumpSearch(sortedArr, 9);
        Q_ASSERT(idx == 4);
        qDebug() << "  ✓ Jump Search works\n";
    }
};

#endif // TEST_BACKEND_H
