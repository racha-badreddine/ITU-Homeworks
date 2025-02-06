#include "methods.h"

int getMax(std::vector<Item>& items, const std::string& attribute)
{
    // This function will find the max element linearly O(n)
    // lambda function to compare according to the attribute needed --> Only age will be used in the scenario
    auto comparison = [attribute](const Item& item1, const Item& item2){
        if (attribute == "age") return  (item1.age > item2.age) ;
        else if (attribute == "type") return (item1.type > item2.type);
        else if (attribute == "origin") return (item1.origin > item2.origin);
        else if (attribute == "rarityScore") return (item1.rarityScore > item2.rarityScore);
        else throw std::invalid_argument("wrong attribute");
    };

    // start with first as max
    Item max_item = items[0];

    // Iterate over all elements and compare according to the attribute given
    for(int i = 1; i < items.size();i++){
        if(comparison(items[i], max_item)){
            max_item = items[i];
        }
    }

    if (attribute == "age") return  max_item.age ;
    else if (attribute == "type") return max_item.type;
    else if (attribute == "origin") return max_item.origin;

    // According to scenario rarity score will not be used since it is a double and calculated later but i added it in case, (int conversion)
    return max_item.rarityScore;
}

// min = age - ageWindow
// max = age + ageWindow
// rarityScore = (1 - probability) * (1 + item.age/ageMax)
void calculateRarityScores(std::vector<Item>& items, int ageWindow)
{
    // Given the items are sorted 
    for (int i = 0; i < items.size(); i++) {
        // Define the age window bounds
        int min_age = items[i].age - ageWindow;
        int max_age = items[i].age + ageWindow;

        int similar_items = 0;
        int count_total = 0;

        // Find the starting index within the window
        int min_index = i;
        while (min_index >= 0 && items[min_index].age >= min_age) {
            min_index--;
        }
        // get first index included in Range
        min_index++; 

        // Count items within the window starting from min index
        for (int j = min_index; j < items.size(); j++) {
            // since items are sorted, we break when we exceed the max
            if (items[j].age > max_age) break;
            // increase count total 
            count_total++;
            // If same type and origin add as similar item
            if (items[j].type == items[i].type && items[j].origin == items[i].origin) {
                similar_items++;
            }
        }

        // Calculate probabily based on the equation given
        double prob = (count_total > 0) ? (static_cast<double>(similar_items) / count_total) : 0.0;

        // Compute the rarity score
        // sorted array -> max in last element
        double age_max = items[items.size() - 1].age; 

        items[i].rarityScore = (1 - prob) * (1 + (static_cast<double>(items[i].age) / age_max));
    }
}
