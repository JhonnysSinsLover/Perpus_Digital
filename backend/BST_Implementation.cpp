/**
 * ============================================================================
 * BINARY SEARCH TREE (BST) IMPLEMENTATION
 * ============================================================================
 * 
 * Implementasi Binary Search Tree untuk katalog buku di Perpustakaan Digital
 * 
 * Struktur Data: Tree (Binary Search Tree)
 * Kegunaan: Menyimpan dan mencari buku berdasarkan judul secara efisien
 * Kompleksitas: 
 * - Search: O(log n) average, O(n) worst case
 * - Insert: O(log n) average, O(n) worst case
 * - Traversal: O(n)
 */

#include "BookManager.h"
#include <QDebug>

// ============================================================================
// BST Node Structure (dari BookManager.h)
// ============================================================================
/*
struct BSTNode {
    Book book;
    std::shared_ptr<BSTNode> left;
    std::shared_ptr<BSTNode> right;
    
    BSTNode(const Book& b) : book(b), left(nullptr), right(nullptr) {}
};
*/

// ============================================================================
// BUILD BST - Membuat Binary Search Tree dari koleksi buku
// ============================================================================

void BookManager::buildBST()
{
    m_bstRoot = nullptr;
    
    for (const Book& book : m_books) {
        m_bstRoot = insertBST(m_bstRoot, book);
    }
    
    qDebug() << "Built BST with" << m_books.size() << "books";
}

// ============================================================================
// INSERT BST - Menambahkan node baru ke dalam BST
// Rekursif: Bandingkan judul, insert ke left/right subtree
// ============================================================================

std::shared_ptr<BookManager::BSTNode> BookManager::insertBST(
    std::shared_ptr<BSTNode> node, 
    const Book& book)
{
    // Base case: jika node kosong, buat node baru
    if (!node) {
        return std::make_shared<BSTNode>(book);
    }
    
    // Compare by title (case-insensitive)
    QString currentTitle = node->book.getJudul().toLower();
    QString newTitle = book.getJudul().toLower();
    
    // Rekursif: insert ke left atau right subtree
    if (newTitle < currentTitle) {
        node->left = insertBST(node->left, book);
    } else if (newTitle > currentTitle) {
        node->right = insertBST(node->right, book);
    }
    // If equal, don't insert duplicate
    
    return node;
}

// ============================================================================
// SEARCH BST - Mencari buku berdasarkan judul exact match
// Kompleksitas: O(log n) average case
// ============================================================================

Book* BookManager::searchBST(const QString& title)
{
    return searchBSTHelper(m_bstRoot, title);
}

Book* BookManager::searchBSTHelper(
    std::shared_ptr<BSTNode> node, 
    const QString& title)
{
    // Base case: node kosong atau tidak ditemukan
    if (!node) {
        return nullptr;
    }
    
    QString currentTitle = node->book.getJudul().toLower();
    QString searchTitle = title.toLower();
    
    // Found: judul sama
    if (searchTitle == currentTitle) {
        return &(node->book);
    } 
    // Search left subtree: search key lebih kecil
    else if (searchTitle < currentTitle) {
        return searchBSTHelper(node->left, title);
    } 
    // Search right subtree: search key lebih besar
    else {
        return searchBSTHelper(node->right, title);
    }
}

// ============================================================================
// PARTIAL SEARCH BST - Mencari buku dengan partial title match
// Menggunakan In-Order Traversal untuk mendapatkan hasil terurut
// ============================================================================

std::vector<Book> BookManager::searchBSTPartial(const QString& partialTitle)
{
    std::vector<Book> results;
    
    if (!m_bstRoot) {
        qDebug() << "BST is empty, cannot perform partial search";
        return results;
    }
    
    // Perform in-order traversal and collect matching books
    searchBSTPartialHelper(m_bstRoot, partialTitle.toLower(), results);
    
    qDebug() << "BST Partial Search found" << results.size() 
             << "books matching:" << partialTitle;
    return results;
}

void BookManager::searchBSTPartialHelper(
    std::shared_ptr<BSTNode> node, 
    const QString& partialTitle, 
    std::vector<Book>& results)
{
    if (!node) {
        return;
    }
    
    // In-order traversal: left -> current -> right
    // Menghasilkan data terurut secara alfabetis
    searchBSTPartialHelper(node->left, partialTitle, results);
    
    // Check if current node's title contains the partial title
    QString currentTitle = node->book.getJudul().toLower();
    if (currentTitle.contains(partialTitle)) {
        results.push_back(node->book);
    }
    
    searchBSTPartialHelper(node->right, partialTitle, results);
}

// ============================================================================
// IN-ORDER TRAVERSAL - Mendapatkan semua buku dalam urutan alfabetis
// Left -> Root -> Right = Sorted Order
// ============================================================================

std::vector<Book> BookManager::getBSTInOrder()
{
    std::vector<Book> result;
    inOrderTraversal(m_bstRoot, result);
    return result;
}

void BookManager::inOrderTraversal(
    std::shared_ptr<BSTNode> node, 
    std::vector<Book>& result)
{
    if (!node) {
        return;
    }
    
    // Left subtree (buku dengan judul lebih kecil)
    inOrderTraversal(node->left, result);
    
    // Current node (root)
    result.push_back(node->book);
    
    // Right subtree (buku dengan judul lebih besar)
    inOrderTraversal(node->right, result);
}

// ============================================================================
// KEUNTUNGAN BINARY SEARCH TREE:
// ============================================================================
// 1. Pencarian efisien: O(log n) untuk data terurut
// 2. In-order traversal menghasilkan data sorted
// 3. Insert dan delete relatif efisien
// 4. Cocok untuk katalog buku yang sering dicari berdasarkan judul
//
// KELEMAHAN:
// 1. Worst case O(n) jika tree tidak seimbang (skewed tree)
// 2. Membutuhkan memory untuk pointer (left, right)
// 3. Perlu rebuild jika ada perubahan data besar
// ============================================================================
