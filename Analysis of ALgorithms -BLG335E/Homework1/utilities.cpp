// Racha Badreddine
#include "tweet.h"

std::vector<Tweet> readTweetsFromFile(const std::string& filename) 
{
    // open file
    std::fstream tweets_file;
    tweets_file.open(filename, std::ios::in);

    // File not opened --> return empty vector
    if (!tweets_file.is_open()) {
    std::cerr << "File could not be opened!" << std::endl;
    return {};
    }

    // vector to hole all tweets
    std::vector<Tweet> tweets;
    std::string line;

    // discard first line 
    getline(tweets_file, line);

    while(getline(tweets_file, line)){
        // create Tweet object for data read
        Tweet curr_tweet;

        // find position of the 2 commas
        int position1 = line.find(',');
        int position2 = line.find(',', position1 + 1);

        // extract the data from the line: long long, int, int
        curr_tweet.tweetID = std::stoll(line.substr(0, position1));
        curr_tweet.retweetCount = std::stoi(line.substr(position1 + 1, position2 - position1 - 1));
        curr_tweet.favoriteCount = std::stoi(line.substr(position2 + 1));


        tweets.push_back(curr_tweet);
    }

    // closing the file
    tweets_file.close();

    return tweets;
    
}

void writeTweetsToFile(const std::string& filename, const std::vector<Tweet>& tweets) 
{
    // open file
    std::fstream output_file;
    output_file.open(filename, std::ios::out);

    // File not opened 
    if (!output_file.is_open()) {
    std::cerr << "File could not be opened!" << std::endl;
    return ;
    }

    // writign first line
    output_file << "tweetID,retweet_count,favorite_count\n";

    std::stringstream data;

    // iterating throught the vector and storing it in a string stream
    for(const Tweet& tweet : tweets){
        data << tweet.tweetID << ',' << tweet.retweetCount << ',' << tweet.favoriteCount <<'\n';
    }

    // dump all data to the file
    output_file << data.rdbuf();

    // close the file
    output_file.close();
}