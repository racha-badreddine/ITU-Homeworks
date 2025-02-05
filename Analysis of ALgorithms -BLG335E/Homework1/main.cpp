// Racha Badreddine
#include "tweet.h"
#include <chrono> // added for calculating time

int main() 
{
    // read data and store it in a vector
    std::string size = "5k.csv";
    std::string file_name = "./data/sizes/tweets" + size;  
    std::vector<Tweet> data = readTweetsFromFile(file_name);

    //start time for sort
    const auto start{std::chrono::steady_clock::now()};

    //sort the data in ascending order

    std::string key = "retweetCount";

    // bubble sort 
    //bubbleSort(data, key, true);

    //insertion sort
    //insertionSort(data, key, true);

    //merge sort
    mergeSort(data, 0, data.size() - 1,  key, true);

    //start time for metrics
    //const auto start{std::chrono::steady_clock::now()};

    // applying binary search after sorting the data !
    //long long id = 1773335;
    //std::cout << binarySearch(data, id, key) << '\n';

    //finding above treshhold
    //int threshold = 250;
    //countAboveThreshold(data, key, threshold);

    // stop time
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};

    // write to file
    std::string sort = "merge_Sort";
    writeTweetsToFile("./" + sort + "_output" + size, data);

    // calculate time
    std::cout << "Time spent: "<< elapsed_seconds.count() << "s\n";
}
