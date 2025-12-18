#ifndef SEARCHING_H
#define SEARCHING_H

#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>

/**
 * @brief Collection of searching algorithms
 * 
 * This header provides various searching algorithm implementations
 * for different use cases and data structures.
 */
namespace Searching
{
    /**
     * @brief Binary Search algorithm (requires sorted array)
     * @tparam T Type of elements
     * @param arr Sorted vector to search in
     * @param key Value to search for
     * @param compare Comparison function
     * @return Index of found element, -1 if not found
     */
    template<typename T>
    int binarySearch(const std::vector<T>& arr, const T& key,
                     std::function<bool(const T&, const T&)> compare)
    {
        int left = 0;
        int right = arr.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (arr[mid] == key) {
                return mid;
            } else if (compare(arr[mid], key)) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1;
    }

    /**
     * @brief Binary Search with custom key extractor
     * @tparam T Type of elements
     * @tparam K Type of key
     * @param arr Sorted vector to search in
     * @param key Key to search for
     * @param getKey Function to extract key from element
     * @return Index of found element, -1 if not found
     */
    template<typename T, typename K>
    int binarySearchByKey(const std::vector<T>& arr, const K& key,
                          std::function<K(const T&)> getKey)
    {
        int left = 0;
        int right = arr.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            K midKey = getKey(arr[mid]);

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

    /**
     * @brief Linear Search algorithm
     * @tparam T Type of elements
     * @param arr Vector to search in
     * @param key Value to search for
     * @return Index of found element, -1 if not found
     */
    template<typename T>
    int linearSearch(const std::vector<T>& arr, const T& key)
    {
        for (size_t i = 0; i < arr.size(); i++) {
            if (arr[i] == key) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @brief Linear Search with predicate
     * @tparam T Type of elements
     * @tparam Predicate Predicate function type
     * @param arr Vector to search in
     * @param predicate Function that returns true for matching element
     * @return Index of found element, -1 if not found
     */
    template<typename T, typename Predicate>
    int linearSearchIf(const std::vector<T>& arr, Predicate predicate)
    {
        for (size_t i = 0; i < arr.size(); i++) {
            if (predicate(arr[i])) {
                return i;
            }
        }
        return -1;
    }

    /**
     * @brief Find all matching elements
     * @tparam T Type of elements
     * @tparam Predicate Predicate function type
     * @param arr Vector to search in
     * @param predicate Function that returns true for matching elements
     * @return Vector of indices of all matching elements
     */
    template<typename T, typename Predicate>
    std::vector<int> findAll(const std::vector<T>& arr, Predicate predicate)
    {
        std::vector<int> results;
        for (size_t i = 0; i < arr.size(); i++) {
            if (predicate(arr[i])) {
                results.push_back(i);
            }
        }
        return results;
    }

} // namespace Searching

#endif // SEARCHING_H
