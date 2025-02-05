// Racha Badreddine 
#include "tweet.h"

void bubbleSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    // lambda function to dynamically choose the attribute to sort
    auto comparison = [sortBy, ascending](const Tweet& tweet1, const Tweet& tweet2){
        if (sortBy == "tweetID") return ascending ? (tweet1.tweetID > tweet2.tweetID) : (tweet1.tweetID < tweet2.tweetID);
        else if (sortBy == "retweetCount") return ascending ? (tweet1.retweetCount > tweet2.retweetCount) : (tweet1.retweetCount < tweet2.retweetCount);
        else if (sortBy == "favoriteCount") return ascending ? (tweet1.favoriteCount > tweet2.favoriteCount) : (tweet1.favoriteCount < tweet2.favoriteCount);
        else throw std::invalid_argument("wrong value of sortBy");
    };

    // get size
    int size  = tweets.size();

    // first loop 
    for(int i=0; i < size - 1; i++){ 
        // second loop always starting from the first index
        for(int j=0; j < size - i - 1; j++){
            // when current element needs to be changed with the next one swap them:
            //  - the current is stricty larger (stable) and the order is asending
            //  - the current is strictly smaller (stable) and the order is desending
            if(comparison(tweets[j], tweets[j + 1])){
                std::swap(tweets[j], tweets[j + 1]);
            }
        }
    }
    
}

void insertionSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    // lambda function to dynamically choose the attribute to sort
    auto comparison = [sortBy, ascending](const Tweet& tweet1, const Tweet& tweet2){
        if (sortBy == "tweetID") return ascending ? (tweet1.tweetID < tweet2.tweetID) : (tweet1.tweetID > tweet2.tweetID);
        else if (sortBy == "retweetCount") return ascending ? (tweet1.retweetCount < tweet2.retweetCount) : (tweet1.retweetCount > tweet2.retweetCount);
        else if (sortBy == "favoriteCount") return ascending ? (tweet1.favoriteCount < tweet2.favoriteCount) : (tweet1.favoriteCount > tweet2.favoriteCount);
        else throw std::invalid_argument("wrong value of sortBy");
    };
    

    // get size
    int size = tweets.size();

    // first loop
    for (int i = 1; i < size; i++) {
        // get current tweet
        Tweet current = tweets[i];
        // initialize second loop
        int j = i - 1;

        // while the contions are true keep shifting till findin the right place for the tweet
        //  - when the current is strictly larger and the order is descending
        //  - when the current is strictly smaller and the order is ascending
        while (j >= 0 && comparison(current, tweets[j])) {
            tweets[j + 1] = tweets[j];
            j--;
        }
        // insert the current
        tweets[j + 1] = current; 
    }


}

void merge(std::vector<Tweet>& tweets, int left, int mid, int right, const std::string& sortBy, bool ascending) 
{
    // lambda function to dynamically choose the attribute to sort
    auto comparison = [sortBy, ascending](const Tweet& tweet1, const Tweet& tweet2){
        if (sortBy == "tweetID") return ascending ? (tweet1.tweetID <= tweet2.tweetID) : (tweet1.tweetID >= tweet2.tweetID);
        else if (sortBy == "retweetCount") return ascending ? (tweet1.retweetCount <= tweet2.retweetCount) : (tweet1.retweetCount >= tweet2.retweetCount);
        else if (sortBy == "favoriteCount") return ascending ? (tweet1.favoriteCount <= tweet2.favoriteCount) : (tweet1.favoriteCount >= tweet2.favoriteCount);
        else throw std::invalid_argument("wrong value of sortBy");
    };

    // copy the tweets vector to 2, left and right subarrays
    std::vector<Tweet> left_subarray(tweets.begin() + left, tweets.begin() + mid + 1);
    std::vector<Tweet> right_subarray(tweets.begin() + mid + 1, tweets.begin() + right + 1);

    // initialize the indexes of all arrays
    size_t left_index = 0, right_index = 0, index = left;

    // continue until the end of one of the arrays is reached or both
    while(left_index < left_subarray.size() && right_index < right_subarray.size()){
        //  - if the element in the left subarray is smaller (not strictly) and the order is ascending
        //  - if the element in the right subarray is larger (not strictly) and the order is descending
        if(comparison(left_subarray[left_index], right_subarray[right_index])){
            tweets[index++] = left_subarray[left_index++];
        
        // else take the element from right subarray
        }else{
            tweets[index++] = right_subarray[right_index++];
        }
    }

    // if the end of left subarray is not reached
    while (left_index < left_subarray.size()){
        tweets[index++] = left_subarray[left_index++];
    }

    // if the end of right subarray is not reached
    while(right_index < right_subarray.size()){
        tweets[index++] = right_subarray[right_index++];
    }    
}

void mergeSort(std::vector<Tweet>& tweets, int left, int right, const std::string& sortBy, bool ascending) 
{
    // base case, the subarray size is 1
    if (right <= left) return;

    // calculate the mid
    int mid =  left + (right - left) / 2;
    // call for the left subarray
    mergeSort(tweets, left, mid, sortBy, ascending);
    // call for the right subarray
    mergeSort(tweets, mid + 1, right, sortBy, ascending);
    // affter dividing, now merge
    merge(tweets, left, mid, right, sortBy, ascending);

}