#ifndef GRAPH_H
#define GRAPH_H

#include "Book.h"
#include <QString>
#include <QStringList>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

/**
 * @brief Graph data structure for book genre recommendations
 * 
 * This class implements an adjacency list representation of a graph
 * where nodes are genres and edges represent relationships between genres.
 * Used to provide book recommendations based on genre connections.
 */
class Graph
{
public:
    /**
     * @brief Default constructor
     */
    Graph();

    /**
     * @brief Add an edge between two genres
     * @param genre1 First genre
     * @param genre2 Second genre (undirected graph)
     */
    void addEdge(const QString& genre1, const QString& genre2);

    /**
     * @brief Remove an edge between two genres
     * @param genre1 First genre
     * @param genre2 Second genre
     */
    void removeEdge(const QString& genre1, const QString& genre2);

    /**
     * @brief Check if an edge exists between two genres
     * @param genre1 First genre
     * @param genre2 Second genre
     * @return true if edge exists, false otherwise
     */
    bool hasEdge(const QString& genre1, const QString& genre2) const;

    /**
     * @brief Build graph from a collection of books
     * Automatically creates connections between genres that appear together
     * @param books Vector of books to analyze
     */
    void buildGraph(const std::vector<Book>& books);

    /**
     * @brief Get recommended books based on a specific genre
     * Uses BFS to traverse connected genres and find related books
     * @param genre Starting genre for recommendations
     * @param books Collection of all available books
     * @param maxDepth Maximum depth of genre connections to explore (default 2)
     * @return Vector of recommended books sorted by relevance
     */
    std::vector<Book> getRecommendation(const QString& genre,
                                        const std::vector<Book>& books,
                                        int maxDepth = 2) const;

    /**
     * @brief Get all genres connected to a specific genre
     * @param genre Genre to get connections for
     * @return Set of connected genres
     */
    std::set<QString> getConnectedGenres(const QString& genre) const;

    /**
     * @brief Get all genres in the graph
     * @return Vector of all genre names
     */
    std::vector<QString> getAllGenres() const;

    /**
     * @brief Get number of connections for a specific genre
     * @param genre Genre to check
     * @return Number of connected genres
     */
    int getGenreDegree(const QString& genre) const;

    /**
     * @brief Find shortest path between two genres using BFS
     * @param startGenre Starting genre
     * @param endGenre Target genre
     * @return Vector of genres representing the path (empty if no path exists)
     */
    std::vector<QString> findPath(const QString& startGenre,
                                   const QString& endGenre) const;

    /**
     * @brief Clear all data from the graph
     */
    void clear();

    /**
     * @brief Get total number of genres in the graph
     * @return Number of genres (nodes)
     */
    size_t getNodeCount() const { return m_adjacencyList.size(); }

    /**
     * @brief Get total number of connections in the graph
     * @return Number of edges
     */
    size_t getEdgeCount() const;

    /**
     * @brief Print graph structure (for debugging)
     * @return QString representation of the graph
     */
    QString toString() const;

private:
    /// Adjacency list representation: genre -> set of connected genres
    std::map<QString, std::set<QString>> m_adjacencyList;

    /**
     * @brief Breadth-First Search to explore connected genres
     * @param startGenre Starting genre
     * @param maxDepth Maximum depth to explore
     * @return Map of genre to depth level
     */
    std::map<QString, int> bfs(const QString& startGenre, int maxDepth) const;

    /**
     * @brief Calculate relevance score for a book based on genre connections
     * @param book Book to score
     * @param genreDepths Map of genre to depth from BFS
     * @return Relevance score (higher is more relevant)
     */
    double calculateRelevance(const Book& book,
                             const std::map<QString, int>& genreDepths) const;
};

// ============================================================================
// INLINE IMPLEMENTATIONS
// ============================================================================

inline Graph::Graph()
{
}

inline void Graph::addEdge(const QString& genre1, const QString& genre2)
{
    if (genre1 == genre2) return; // No self-loops
    
    m_adjacencyList[genre1].insert(genre2);
    m_adjacencyList[genre2].insert(genre1); // Undirected graph
}

inline void Graph::removeEdge(const QString& genre1, const QString& genre2)
{
    auto it1 = m_adjacencyList.find(genre1);
    if (it1 != m_adjacencyList.end()) {
        it1->second.erase(genre2);
    }

    auto it2 = m_adjacencyList.find(genre2);
    if (it2 != m_adjacencyList.end()) {
        it2->second.erase(genre1);
    }
}

inline bool Graph::hasEdge(const QString& genre1, const QString& genre2) const
{
    auto it = m_adjacencyList.find(genre1);
    if (it != m_adjacencyList.end()) {
        return it->second.find(genre2) != it->second.end();
    }
    return false;
}

inline std::set<QString> Graph::getConnectedGenres(const QString& genre) const
{
    auto it = m_adjacencyList.find(genre);
    if (it != m_adjacencyList.end()) {
        return it->second;
    }
    return std::set<QString>();
}

inline std::vector<QString> Graph::getAllGenres() const
{
    std::vector<QString> genres;
    for (const auto& pair : m_adjacencyList) {
        genres.push_back(pair.first);
    }
    return genres;
}

inline int Graph::getGenreDegree(const QString& genre) const
{
    auto it = m_adjacencyList.find(genre);
    if (it != m_adjacencyList.end()) {
        return it->second.size();
    }
    return 0;
}

inline void Graph::clear()
{
    m_adjacencyList.clear();
}

inline size_t Graph::getEdgeCount() const
{
    size_t count = 0;
    for (const auto& pair : m_adjacencyList) {
        count += pair.second.size();
    }
    return count / 2; // Undirected graph, so divide by 2
}

// ============================================================================
// COMPLEX IMPLEMENTATIONS
// ============================================================================

inline void Graph::buildGraph(const std::vector<Book>& books)
{
    clear();

    // Create connections between genres that appear together in books
    for (const Book& book : books) {
        QStringList genres = book.getGenre();
        
        // Connect every genre with every other genre in the same book
        for (int i = 0; i < genres.size(); i++) {
            for (int j = i + 1; j < genres.size(); j++) {
                addEdge(genres[i], genres[j]);
            }
        }
    }
}

inline std::map<QString, int> Graph::bfs(const QString& startGenre, int maxDepth) const
{
    std::map<QString, int> genreDepths;
    std::queue<std::pair<QString, int>> queue;
    std::set<QString> visited;

    queue.push({startGenre, 0});
    visited.insert(startGenre);
    genreDepths[startGenre] = 0;

    while (!queue.empty()) {
        auto [currentGenre, depth] = queue.front();
        queue.pop();

        if (depth >= maxDepth) continue;

        auto it = m_adjacencyList.find(currentGenre);
        if (it != m_adjacencyList.end()) {
            for (const QString& neighbor : it->second) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    genreDepths[neighbor] = depth + 1;
                    queue.push({neighbor, depth + 1});
                }
            }
        }
    }

    return genreDepths;
}

inline double Graph::calculateRelevance(const Book& book,
                                       const std::map<QString, int>& genreDepths) const
{
    double score = 0.0;
    QStringList bookGenres = book.getGenre();

    for (const QString& genre : bookGenres) {
        auto it = genreDepths.find(genre);
        if (it != genreDepths.end()) {
            // Lower depth = higher relevance
            // Score: 10 for depth 0, 5 for depth 1, 2 for depth 2, etc.
            score += 10.0 / (it->second + 1);
        }
    }

    // Bonus for rating
    score += book.getRating() * 0.5;

    return score;
}

inline std::vector<Book> Graph::getRecommendation(const QString& genre,
                                                  const std::vector<Book>& books,
                                                  int maxDepth) const
{
    // Find all connected genres using BFS
    std::map<QString, int> genreDepths = bfs(genre, maxDepth);

    // Score and collect books that match connected genres
    std::vector<std::pair<Book, double>> scoredBooks;

    for (const Book& book : books) {
        double score = calculateRelevance(book, genreDepths);
        if (score > 0) {
            scoredBooks.push_back({book, score});
        }
    }

    // Sort by score (descending)
    std::sort(scoredBooks.begin(), scoredBooks.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    // Extract books
    std::vector<Book> recommendations;
    for (const auto& pair : scoredBooks) {
        recommendations.push_back(pair.first);
    }

    return recommendations;
}

inline std::vector<QString> Graph::findPath(const QString& startGenre,
                                           const QString& endGenre) const
{
    if (startGenre == endGenre) {
        return {startGenre};
    }

    std::map<QString, QString> parent;
    std::queue<QString> queue;
    std::set<QString> visited;

    queue.push(startGenre);
    visited.insert(startGenre);
    parent[startGenre] = "";

    while (!queue.empty()) {
        QString current = queue.front();
        queue.pop();

        if (current == endGenre) {
            // Reconstruct path
            std::vector<QString> path;
            QString node = endGenre;
            while (!node.isEmpty()) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        auto it = m_adjacencyList.find(current);
        if (it != m_adjacencyList.end()) {
            for (const QString& neighbor : it->second) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    queue.push(neighbor);
                }
            }
        }
    }

    return {}; // No path found
}

inline QString Graph::toString() const
{
    QString result;
    for (const auto& pair : m_adjacencyList) {
        result += pair.first + ": ";
        for (const QString& neighbor : pair.second) {
            result += neighbor + ", ";
        }
        if (!pair.second.empty()) {
            result.chop(2); // Remove trailing ", "
        }
        result += "\n";
    }
    return result;
}

#endif // GRAPH_H
