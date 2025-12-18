#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <functional>

/**
 * @brief Collection of sorting algorithms
 * 
 * This header provides various sorting algorithm implementations
 * that can be used with any data type that supports comparison.
 */
namespace Sorting
{
    /**
     * @brief QuickSort algorithm implementation
     * @tparam T Type of elements to sort
     * @tparam Compare Comparison function type
     * @param arr Vector to sort
     * @param compare Comparison function
     * @param left Starting index
     * @param right Ending index
     */
    template<typename T, typename Compare>
    void quickSort(std::vector<T>& arr, int left, int right, Compare compare)
    {
        if (left >= right) return;

        // Partition
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

        // Recursive calls
        quickSort(arr, left, pivotIndex - 1, compare);
        quickSort(arr, pivotIndex + 1, right, compare);
    }

    /**
     * @brief QuickSort with default ascending order
     * @tparam T Type of elements to sort
     * @param arr Vector to sort
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
     * @tparam Compare Comparison function type
     * @param arr Vector to sort
     * @param compare Comparison function
     */
    template<typename T, typename Compare>
    void quickSort(std::vector<T>& arr, Compare compare)
    {
        if (arr.empty()) return;
        quickSort(arr, 0, arr.size() - 1, compare);
    }

    /**
     * @brief Merge Sort algorithm implementation
     * @tparam T Type of elements to sort
     * @param arr Vector to sort
     * @param left Starting index
     * @param right Ending index
     * @param compare Comparison function
     */
    template<typename T>
    void mergeSort(std::vector<T>& arr, int left, int right,
                   std::function<bool(const T&, const T&)> compare)
    {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, compare);
        mergeSort(arr, mid + 1, right, compare);

        // Merge
        std::vector<T> temp;
        int i = left, j = mid + 1;

        while (i <= mid && j <= right) {
            if (compare(arr[i], arr[j])) {
                temp.push_back(arr[i++]);
            } else {
                temp.push_back(arr[j++]);
            }
        }

        while (i <= mid) temp.push_back(arr[i++]);
        while (j <= right) temp.push_back(arr[j++]);

        for (size_t k = 0; k < temp.size(); k++) {
            arr[left + k] = temp[k];
        }
    }

    /**
     * @brief Insertion Sort algorithm (good for small arrays)
     * @tparam T Type of elements to sort
     * @param arr Vector to sort
     * @param compare Comparison function
     */
    template<typename T>
    void insertionSort(std::vector<T>& arr,
                       std::function<bool(const T&, const T&)> compare)
    {
        for (size_t i = 1; i < arr.size(); i++) {
            T key = arr[i];
            int j = i - 1;

            while (j >= 0 && !compare(arr[j], key) && arr[j] != key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

} // namespace Sorting

#endif // SORTING_H
