#include "methods.h"
#include <chrono>

void calculate_runtime();
std::vector<Item> RunTimeOf_Counting_Sort(std::string dataset,std::vector<Item> items, bool ascending );
void RunTime_RarityScores(std::string dataset,std::vector<Item>& items, int window);
std::vector<Item> RunTimeOf_Heap_Sort(std::string dataset,std::vector<Item> items, bool ascending);

int main() 
{
  // Function that calculates the runtime of all algorithms for all datasets provided
  // outputs written in the output_files Folder

  //calculate_runtime();
  
  // Step1 : Read the items 
  std::vector<Item> items = readItemsFromFile("data/items_l.csv");

  // Step2 : Sort the items based on their age using counting sort algorithm
  std::vector<Item> sorted = countingSort(items, "age", true);

  // write the sorted items (with rarity 0)
  writeItemsToFile("data/items_l_sorted.csv", sorted);

  // Step3: Calculating the rarity score for sorted Items using 50 as age window (example)
  calculateRarityScores(sorted, 50);

  // Writing the sorted items(based on age) with rarity score calculated
  writeItemsToFile("data/items_l_rarity.csv", sorted);

  // Step4 : Sort the items based on their Rarity score using the Heap Sort algorithm
  sorted = heapSortByRarity(sorted, true);

  //Writing the sorted items (based on rarity score)
  writeItemsToFile("data/items_l_rarity_sorted.csv", sorted);

  return 0;
}

// Function to calculate RunTime of counting sort for both ascending and descending order
std::vector<Item> RunTimeOf_Counting_Sort(std::string dataset,std::vector<Item> items, bool ascending ){

  std::fstream output;
  output.open("./data/output_files/counting_sort_RunTimes.txt", std::ios::app);

  // Runtime
  const auto start{std::chrono::steady_clock::now()};
  std::vector<Item> sorted = countingSort(items, "age", ascending);
  const auto end{std::chrono::steady_clock::now()};

  const std::chrono::duration<double> elapsed_seconds{end - start};

  std::string order = ascending ? "ascending" : "descending";
  // calculate time
  output << "Time spent For couting Sort " + order + " for " + dataset + " is: "<< elapsed_seconds.count() << "s\n";

  writeItemsToFile("./data/output_files/" + dataset + "_soted" + order + "_by_age.csv", sorted);

  output.close();

  return sorted;
}

// Function to calculate RunTime of rarity score calc
void RunTime_RarityScores(std::string dataset,std::vector<Item>& items, int window){
  std::fstream output;
  output.open("./data/output_files/RarityCalc_RunTimes.txt", std::ios::app);

  
    // Runtime
    const auto start{std::chrono::steady_clock::now()};
    calculateRarityScores(items, window);
    const auto end{std::chrono::steady_clock::now()};

    const std::chrono::duration<double> elapsed_seconds{end - start};

  // calculate time
  output << "Time spent For Rarity Score with window  " + std::to_string(window) + " for " + dataset + " is: "<< elapsed_seconds.count() << "s\n";

  writeItemsToFile("./data/output_files/" + dataset + "_calc_rarity_with_wind_" + std::to_string(window) +".csv", items);

  output.close();



}

// Function to calculate RunTime of heap sort for both ascending and descending order
std::vector<Item> RunTimeOf_Heap_Sort(std::string dataset,std::vector<Item> items, bool ascending){
  std::fstream output;
  output.open("./data/output_files/HeapSort_RunTimes.txt", std::ios::app);
  // Runtime
  const auto start{std::chrono::steady_clock::now()};
  std::vector<Item> sorted = heapSortByRarity(items, ascending);
  const auto end{std::chrono::steady_clock::now()};

  const std::chrono::duration<double> elapsed_seconds{end - start};

  std::string order = ascending ? "ascending" : "descending";
  // calculate time
  output << "Time spent For Heap Sort " + order + " for " + dataset + " is: "<< elapsed_seconds.count() << "s\n";

  writeItemsToFile("./data/output_files/" + dataset + "_soted" + order + "_by_rarity_hS.csv", sorted);
  output.close();

  return sorted;

}

void calculate_runtime(){
  std::vector<std::string> datasets = {"items_l", "items_m", "items_s"};

  
  for(auto& dataset: datasets){
    // Read items 
    std::vector<Item> items = readItemsFromFile("data/" + dataset + ".csv");

    // Step1 : Calculating Run time for Counting Sort --> According to Age (Scenario) 
    std::vector<Item> sorted_asc = RunTimeOf_Counting_Sort(dataset, items, true);
    std::vector<Item> sorted_desc = RunTimeOf_Counting_Sort(dataset, items, false);

    // Step2: Calculating Rarity Score for different age windows
    std::vector<int> age_windows = {20,50,100, 200, 500, 1000};
    for(auto& window : age_windows){ 
      RunTime_RarityScores(dataset,sorted_asc, window);
    }

    // step3 : Run time for Heap Sort   
    std::vector<Item> sortedwithrar = readItemsFromFile("./data/output_files/" + dataset + "_calc_rarity_with_wind_50.csv");
    std::vector<Item> sortedByRarityAsc = RunTimeOf_Heap_Sort(dataset,sortedwithrar, true);

    sortedwithrar = readItemsFromFile("./data/output_files/" + dataset + "_calc_rarity_with_wind_50.csv");
    std::vector<Item> sortedByRarityDesc = RunTimeOf_Heap_Sort(dataset,sortedwithrar, false);


  }
  
}