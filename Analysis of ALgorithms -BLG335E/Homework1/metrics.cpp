// Racha Badreddine
#include "tweet.h"

int binarySearch(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy)
{
    // Binary search 
    int left = 0;
    int right = tweets.size() - 1;

    while (left <= right) {
        // calculate the middle index
        int mid = left + (right - left) / 2;

        // get the right midtweet attribute depending the sortBy parameter
        long long midtweet_attribute;
        if (sortBy == "tweetID") midtweet_attribute = tweets[mid].tweetID;
        else if (sortBy == "retweetCount") midtweet_attribute = tweets[mid].retweetCount;
        else if (sortBy == "favoriteCount") midtweet_attribute = tweets[mid].favoriteCount;
        else throw std::invalid_argument("Invalid value for sortBy");

        // found --> return the index
        if (midtweet_attribute == key) return mid;
        // the target is in the right part
        else if (midtweet_attribute < key) left = mid + 1;
        // the target is in the left part
        else right = mid - 1;
    }

    // not found
    return -1;
}

int countAboveThreshold(const std::vector<Tweet>& tweets, const std::string& metric, int threshold) 
{
    // Since the array is not sorted we can not apply binary search
    // linear search will be applied --> O(n)
    // only values strictly larger than the treshold are considered

    // lambda function to compare according to the attribute needed
    auto comparison = [metric, threshold](const Tweet& tweet1){
        if (metric == "retweetCount") return  (tweet1.retweetCount > threshold) ;
        else if (metric == "favoriteCount") return (tweet1.favoriteCount > threshold);
        else throw std::invalid_argument("wrong value of sortBy");
    };

    // intialize the result
    int result = 0;

    for(int i =0 ; i < tweets.size(); i++){
        // if larger than the attribute needed increase the result by 1
        if(comparison(tweets[i])) result++; 
    }

   return result;
}
