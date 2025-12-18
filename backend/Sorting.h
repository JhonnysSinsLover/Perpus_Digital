#ifndef SORTING_H
#define SORTING_H

#include <vector>

/**
 * @brief QuickSort Algorithm Implementation (ONLY)
 * 
 * This header provides ONLY QuickSort algorithm - a highly efficient
 * divide-and-conquer sorting algorithm with O(n log n) average complexity.
 * 
 * Other sorting algorithms (Bubble, Selection, Merge) have been REMOVED
 * for code cleanliness and focus on the most efficient general-purpose algorithm.
 */
namespace Sorting
{
    /**
     * @brief QuickSort algorithm implementation (Core recursive function)
     * @tparam T Type of elements to sort
     * @tparam Compare Comparison function type (supports lambdas)
     * @param arr Vector to sort
     * @param left Starting index
     * @param right Ending index
     * @param compare Comparison function (lambda or function object)
     * 
     * Time Complexity:
     * - Average: O(n log n)
     * - Worst: O(n²) - rare with good pivot selection
     * - Best: O(n log n)
     * 
     * Space Complexity: O(log n) due to recursion stack
     */
    template<typename T, typename Compare>
    void quickSort(std::vector<T>& arr, int left, int right, Compare compare)
    {
        if (left >= right) return;

        // Partition phase
        T pivot = arr[right];
        int i = left - 1;

        for (int j = left; j < right; j++) {
            if (compare(arr[j], pivot)) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[right]);
        int pivotIndex = i + 1;

        // Recursive calls on sub-arrays
        quickSort(arr, left, pivotIndex - 1, compare);
        quickSort(arr, pivotIndex + 1, right, compare);
    }

    /**
     * @brief QuickSort with default ascending order
     * @tparam T Type of elements to sort (must support < operator)
     * @param arr Vector to sort
     * 
     * Usage: Sorting::quickSort(myVector);
     */
    template<typename T>
    void quickSort(std::vector<T>& arr)
    {
        if (arr.empty()) return;
        quickSort(arr, 0, arr.size() - 1, 
                 [](const T& a, const T& b) { return a < b; });
    }

    /**
     * @brief QuickSort with custom comparator (2-parameter convenience version)
     * @tparam T Type of elements
     * @tparam Compare Comparison function type (lambda or function object)
     * @param arr Vector to sort
     * @param compare Comparison function
     * 
     * Usage: 
     * Sorting::quickSort(books, [](const Book& a, const Book& b) { 
     *     return a.getTitle() < b.getTitle(); 
     * });
     */
    template<typename T, typename Compare>
    void quickSort(std::vector<T>& arr, Compare compare)
    {
        if (arr.empty()) return;
        quickSort(arr, 0, arr.size() - 1, compare);
    }

} // namespace Sorting

#endif // SORTING_H
