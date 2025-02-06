#include "methods.h"

std::vector<Item> countingSort(std::vector<Item>& items, const std::string& attribute, bool ascending)
{
    // Counting sort function works for age, type, origin 
    // rarity not supported, double!
    auto get_attribute = [attribute](const Item& item){
        if (attribute == "age") return  item.age;
        else if (attribute == "type") return item.type ;
        else if (attribute == "origin") return item.origin;
        else if (attribute == "rarityScore") throw std::invalid_argument("rarityScore attribute is double!");
        else throw std::invalid_argument("wrong attribute");
    };

    // First we need to find the max element in the array
    int Range = getMax(items, attribute);

    // create counting array
    std::vector<int> counting_array(Range + 1, 0);

    // create output array
    std::vector<Item> sorted_items(items.size());

    // count the occurence of the specific attribute of each item
    for(const Item& item : items){
        counting_array[get_attribute(item)] +=1;
    }

    // For ascending we find the indexes starting from left
    if (ascending) {
        for (int i = 1; i <= Range; i++) {
            counting_array[i] += counting_array[i - 1];
        }
    // For descending we find them by starting from right
    } else {
        for (int i = Range - 1; i >= 0; i--) {
            counting_array[i] += counting_array[i + 1];
        }
    }

    // Building the sorted  array
    for(int i = items.size() - 1; i>=0; i--){
        int key = get_attribute(items[i]);
        sorted_items[counting_array[key] - 1] = items[i];
        counting_array[key]--;
    }

    return sorted_items;
}

// Function to heapify a subtree rooted with node i in the array of items
void heapify(std::vector<Item>& items, int n, int i, bool descending)
{
    // Here it is a mistake, descending is ascending according to the tests    
    auto compare = [descending](double first, double second) {
        return descending ? first < second : first > second;
    };

    // get left child index
    int left = 2*i + 1;
    // get right child index
    int right = 2*i + 2;
    // start with root as largest
    int largest = i;

    // left child is considered as larger if  larger or smaller depending on descending or ascending order
    if(left < n && compare(items[left].rarityScore , items[i].rarityScore)){
        largest = left;
    }

    // right child is taken if larger or smaller depending on descending or ascending order
    if(right < n && compare(items[right].rarityScore , items[largest].rarityScore)){
        largest = right;
    }

    // if one of the children was larger or smaller depending on descending or ascending order it is swapped with root
    if(largest != i){
        std::swap(items[i], items[largest]);
        // recursive call for the heapify function
        heapify(items, n, largest, descending);
    } 
    // If the root was larger or smaller depending on descending or ascending order the recursion stops
}

// Function to perform heap sort on rarityScore scores of items
std::vector<Item> heapSortByRarity(std::vector<Item>& items, bool descending)
{
    // take the size of the array
    int n = items.size();

    // we first construct the heap using bottom-up construction
    for(int i = n / 2 - 1; i >=0 ;i--){
        heapify(items, n , i, descending);
    }

    // now we apply the heap sort algorithm
    for (int i = n - 1; i > 0; i--) {
        std::swap(items[0], items[i]);
        heapify(items, i, 0, descending);
    }

    return items;
}
