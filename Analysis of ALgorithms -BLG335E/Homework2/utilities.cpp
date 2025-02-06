#include "methods.h"

std::vector<Item> readItemsFromFile(const std::string& filename) 
{
    // open file
    std::fstream items_file;
    items_file.open(filename, std::ios::in);

    // File not opened --> return empty vector
    if (!items_file.is_open()) {
    std::cerr << "File could not be opened!" << std::endl;
    return {};
    }

    // vector to hole all Items
    std::vector<Item> Items;
    std::string line;

    // discard first line 
    getline(items_file, line);

    while (getline(items_file, line)){
        Item curr_item;

        // find position of the 3 commas
        int position1 = line.find(',');
        int position2 = line.find(',', position1 + 1);
        int position3 = line.find(',', position2 + 1);

        // extract the data from the line: int, int, int, double
        curr_item.age = std::stoi(line.substr(0, position1));
        curr_item.type = std::stoi(line.substr(position1 + 1, position2 - position1 - 1));
        curr_item.origin = std::stoi(line.substr(position2 + 1, position3 - position2 - 1));
        curr_item.rarityScore = std::stod(line.substr(position3 + 1));

        // push the current item to the vector
        Items.push_back(curr_item);
    }
    
    // closing the file
    items_file.close();

    return Items;
}

void writeItemsToFile(const std::string& filename, const std::vector<Item>& items) 
{
    // open file
    std::fstream output_file;
    output_file.open(filename, std::ios::out);

    // File not opened 
    if (!output_file.is_open()) {
    std::cerr << "File could not be opened!" << std::endl;
    return ;
    }

    // writing first line
    output_file << "age,type,origin,rarity\n";

    std::stringstream data;

    // iterating throught the vector and storing it in a string stream
    for(const Item& item : items)
        data << item.age << ',' << item.type << ',' << item.origin << ',' << item.rarityScore <<'\n';

    // dump all data to the file
    output_file << data.rdbuf();

    // close the file
    output_file.close();
}
