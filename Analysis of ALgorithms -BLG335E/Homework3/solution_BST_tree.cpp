// Student Name: Racha Badreddine
// Student ID: 150210928
#include <iostream> 
#include <stdlib.h>
#include <iomanip>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

///////////////////// Global Variable /////////////////////////
vector<string> all_publishers; // Added this a global variable to store publishers while reading the file
vector<string> all_games; // Added this a global variable to store publishers while reading the file
bool print_flag = true;	// To control printing best seller (i do not print it after sorting Data)
bool store_game_name = false; // Since it was not clear what to store after sorting data i hold both cases

/////////////////// Player ///////////////////
class publisher
{
public:
	string name;
	float na_sales;
	float eu_sales;
	float others_sales;
};

/////////////////// Node ///////////////////
class Node{
	public:
		publisher key;
		Node *parent, *left, *right;	// color attribute and its method are deleted here
	
		Node(publisher);
		~Node();	
};

/////////////////// BST-Tree ///////////////////
class BST_tree{
	private:
		Node* root;
		void inorder_traversal(Node*, int[3]);
	public:	
        publisher* best_seller[3];	
		stack<string> tree_deep_stack;

        Node* get_root();

		Node* BST_insert(Node* root, Node* ptr);
		void insertValue(vector<string>);
        void find_best_seller(); 

		Node* search(string pub_name);	// Added search function
		void deleteTreeHelper(Node*);	// To delete nodes constructed
		
        BST_tree();
		~BST_tree();
};

void print_best_sellers(int year, publisher* temp_publisher[3]){
    cout.precision(5);
    cout<< "End of the "+to_string(year)+" Year"<<endl;
    cout<< "Best seller in North America: "+temp_publisher[0]->name+" - "<<temp_publisher[0]->na_sales<<" million"<<endl;
    cout<< "Best seller in Europe: "+temp_publisher[1]->name+" - "<<temp_publisher[1]->eu_sales<<" million"<<endl;
    cout<< "Best seller rest of the World: "+temp_publisher[2]->name+" - "<<temp_publisher[2]->others_sales<<" million"<<endl;
}

// Function to create Binary search tee Based on the publisher name 
// At the same time calls other functions to print best seller each Decade when data is sorted by year
BST_tree generate_BST_tree_from_csv(string file_name){
    
    BST_tree temp_BSTtree;

    // open file
    std::fstream publishers_file;
    publishers_file.open(file_name, std::ios::in);

    // File not opened --> return empty tree
    if (!publishers_file.is_open()) {
		std::cerr << "File could not be opened!" << std::endl;
		return temp_BSTtree;
    }

    std::string line;
	int ref_year = 1990;	// To print after the reference year (works as a treshhold)

    getline(publishers_file, line);	// discard first line 

    while (getline(publishers_file, line)) {
        stringstream ss(line);	// get line as string stream

        string data;	// to store each attribute
		vector<string> alldata;	// to store needed attributes for sonstruction
		int year ;

		while(getline(ss, data, ',')){
			alldata.push_back(data);
		}
		// to store data for random search
		if(!store_game_name)
			all_publishers.push_back(alldata[3]);
		else 
			all_games.push_back(alldata[0]);

		year = stoi(alldata[2]);	// get year of release

		// Find best seller each decade only when we are reading the data file sorted by year
		if(print_flag&& year > ref_year){
			temp_BSTtree.find_best_seller();	// Call member function to find best seller
			print_best_sellers(ref_year, temp_BSTtree.best_seller);	// Print best seller
			ref_year +=10;	// update the reference year to the next decade
		}

		temp_BSTtree.insertValue(alldata);	// Insert value read
    }
	// To print the data for last decade
	if(print_flag){
		temp_BSTtree.find_best_seller();
		print_best_sellers(ref_year, temp_BSTtree.best_seller);
	}

	// Close file
    publishers_file.close();

    return temp_BSTtree;
}
string sort_data(string fname, string order_attribute);
void get_random_keys(vector<string>& random_publisher, int n);

////////////////////////////////////////////
//----------------- MAIN -----------------//
////////////////////////////////////////////
int main(int argc, char* argv[]){

	string fname = argv[1];	
	
	// Runtime for insertion
    auto start = std::chrono::high_resolution_clock::now();	
    BST_tree BSTtree = generate_BST_tree_from_csv(fname);
	auto finish = std::chrono::high_resolution_clock::now();

	// calculate time with microseconds
    cout << "Time Taken to insert all data to BST: "<< std::chrono::duration_cast<std::chrono::microseconds>(finish-start).count() << "µs\n";

	// 50 Random Search 
	vector<string> random_keys;	// vector to hold the randomly found 50 publishers
	get_random_keys(random_keys, 50);

	long long total_search_time = 0;	// To store cumulative runtime of search

	// calculate runtime for each search operation in nanoseconds
	for(const string& publisher : random_keys){
		start = std::chrono::high_resolution_clock::now();
		BSTtree.search(publisher);
		finish = std::chrono::high_resolution_clock::now();
    	total_search_time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	}

	cout << "Average time for 50 random searches: " << total_search_time/50.0 <<" ns\n";	// take the average runtime

	cout << "############################ Sorted Part ########################################\n";

	// Order Data Based on game name -- CHECK THE ERROR Returning null
	print_flag = false; // Do not print best seller now

	//string order_attribute = "Name";	// With this we can control the attribute by choosing the game name or publisher name I used both for my report
	string order_attribute = "Publisher";

	// The next line can be commented depending on the need
	//if(order_attribute == "Name") store_game_name = true; // make the global variable to true to control some functions(in case i want to store game name)


	fname = sort_data(fname, order_attribute);	// Sort the data based on the attribut chosen
	if(fname == "NONE") return EXIT_FAILURE;	// Error while reading or writing to the new file

	
	// Runtime for insertion
    start = std::chrono::high_resolution_clock::now();
	BST_tree Bst_for_oredered_data = generate_BST_tree_from_csv(fname);
	finish = std::chrono::high_resolution_clock::now();

	// calculate time with microseconds
    cout << "Time Taken to insert all sorted data to BST: "<< std::chrono::duration_cast<std::chrono::microseconds>(finish-start).count() << "µs\n";

	if(store_game_name){
		random_keys.clear();
		get_random_keys(random_keys, 50);
	}
	// 50 Random Search (using the same 50 random publisher found before)
	total_search_time = 0.0;
	// calculate runtime for each search operation in nanoseconds
	for(const string& key : random_keys){
		start = std::chrono::high_resolution_clock::now();
		Bst_for_oredered_data.search(key);
		finish = std::chrono::high_resolution_clock::now();
    	total_search_time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	}
	
	cout << "Average time for 50 random searches in Sorted Data: " << total_search_time/50.0 <<" ns\n";

	return EXIT_SUCCESS;
}


/////////////////// Node ///////////////////

Node::Node(publisher key){
	this->key = key;
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}
Node::~Node() {
	// No pointer here -> No need to destruct anything
}

// Member function to get the root
Node* BST_tree::get_root(){
    return this->root;
}

// Member function to insert the new created node to the right place
Node* BST_tree::BST_insert(Node* root, Node* ptr){
    Node* temp = root;

	// tree is empty -> new node will be the root
	if (!root) {
		root = ptr;
		this->root = root;  
		return root;
	}


	while(temp){
		// If the key is smaller than the current one
		if(ptr->key.name < temp->key.name){
			if(temp->left)	temp = temp->left;	// Go left if leftchild exists
			else {
				temp->left = ptr;				// leftchild does not exist --> Add node here
				ptr->parent = temp;				// 2 pointers are updated for (child-parent) relation
				break;
			} 
			
		// If the key is larger than the current one
		}else if(ptr->key.name > temp->key.name){
			if(temp->right) temp = temp->right;	// Go right if rightchild exists
			else{
				temp->right = ptr;				// Add node
				ptr->parent = temp;
				break;
			}
		}else{
			// Found --> update cumulative
			temp->key.na_sales += ptr->key.na_sales;
			temp->key.eu_sales += ptr->key.eu_sales;
			temp->key.others_sales += ptr->key.others_sales;
			break;
		}
	}

	return this->root;	//return the root
}

// Member function to get data read -> create node -> call insert function
void BST_tree::insertValue(vector<string> n) {
	// Since it was not clear if we need to construct tree after sorting based on game name or publisher name
	// This function holds both cases

	publisher curr_publisher;

	if(!store_game_name){	// To store the publisher in our node as it is supposed to be 
		curr_publisher.name = n[3];
		curr_publisher.na_sales = stof(n[4]);
		curr_publisher.eu_sales = stof(n[5]);
		curr_publisher.others_sales = stof(n[6]);
	}else{
		curr_publisher.name = n[0]; // stores game name instead of publisher name
		curr_publisher.na_sales = 0;
		curr_publisher.eu_sales = 0;
		curr_publisher.others_sales = 0;
	}

	Node* new_node = new Node(curr_publisher);
	BST_insert(this->get_root(), new_node);

}

// Method to print best seller through inorder traversal (the traversal was not sepecified so i chose this)
void BST_tree::find_best_seller(){
	// Actually we could keep track of the best seller while readign but ok let's traverse -> O(n)

	// array to hold xurrent best seller while traversing
	float Best_data[3] = {0.0f, 0.0f, 0.0f}; // 0 -> na, 1-> eu, 2->other

	stack<Node*> traversal;	// stack for iterative inorder traversal

	Node* temp = this->get_root();

	while(temp || !traversal.empty()){

		// go as much as possible
		while(temp){
			traversal.push(temp);
			temp = temp->left;
		}

		
		temp = traversal.top();
		traversal.pop();

		// make updates if needed
		if(temp->key.na_sales > Best_data[0]){
			best_seller[0] = &(temp->key);
			Best_data[0] = temp->key.na_sales;
		} 
		if(temp->key.eu_sales > Best_data[1]){
			best_seller[1] = &(temp->key);
			Best_data[1] = temp->key.eu_sales;
		} 
		if(temp->key.others_sales > Best_data[2]){
			best_seller[2] = &(temp->key);
			Best_data[2] = temp->key.others_sales;
		} 

		// go right
		temp = temp->right;
	}
	 

}

// Member function that performs Binary Search 
Node* BST_tree::search(string pub_name){
	Node* temp = this->root;

	// empty tree case
	if (!temp) { 
		return nullptr;
	}


	while(temp){
		// if target is smaller that current key
		if(pub_name < temp->key.name){

			if(temp->left)	temp = temp->left;	// Go left
			else return nullptr;
			
		// if target is larger that current key
		}else if(pub_name > temp->key.name){
			if(temp->right) temp = temp->right;	//Go right
			else return nullptr;
		}else{
			// Found 
			return temp;
		}
	}

	// Not found
	return nullptr;
}


BST_tree::BST_tree(){
	this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
}


BST_tree::~BST_tree(){
	deleteTreeHelper(this->get_root());

	for (int i = 0; i < 3; ++i) {
        best_seller[i] = nullptr;
    }
}

// Helper function to destruct the tree
void BST_tree::deleteTreeHelper(Node* node){
	if (node == nullptr)
      return;

    deleteTreeHelper(node->left);       //delete the left subtree from memory
    deleteTreeHelper(node->right);      //delete the right subtree from memory

    delete node;    //delete the current node from memory
}


string sort_data(string fname, string order_attribute){
	// open file
    std::fstream publishers_file;
    publishers_file.open(fname, std::ios::in);

    // File not opened --> return empty tree
    if (!publishers_file.is_open()) {
		std::cerr << "File could not be opened! No sorted part :)" << std::endl;
		return "NONE";
    }
	vector<string> lines;
	std::string line;

	// discard first line 
	getline(publishers_file, line);
	string header = line;

	while (getline(publishers_file, line)){
		lines.push_back(line);
	}

	// Sort data based on attribute chosen
	if(order_attribute == "Name")
		stable_sort(lines.begin(), lines.end());	
	else if(order_attribute == "Publisher")
		stable_sort(lines.begin(), lines.end(), [](const string& a, const string& b) {
			auto get_field = [](const string& line, int index) {
				std::stringstream ss(line);
				std::string field;
				for (int i = 0; i <= index && std::getline(ss, field, ','); ++i);
				return field;
			};
			return get_field(a, 3) < get_field(b, 3);  // Compare Publisher names
		});


	// open file
	string output_file_name = "sorted_data_by_" + order_attribute + ".csv";
	std::fstream output_file;
	output_file.open(output_file_name, std::ios::out);

	// File not opened 
	if (!output_file.is_open()) {
	std::cerr << "File could not be opened!" << std::endl;
	return "NONE";
	}

	// writing first line
	output_file << header;

	std::stringstream data;

	// iterating throught the vector and storing it in a string stream
	for(const string& line : lines)
		data << line <<'\n';

	// dump all data to the file
	output_file << data.rdbuf();

	// close the file
	output_file.close();

	return output_file_name;
	
}

void get_random_keys(vector<string>& random_publisher, int n){
	if(!store_game_name){
		int size = all_publishers.size();
		for(int i = 0; i < n; i++){
			int random_num = rand() % size;
			random_publisher.push_back(all_publishers[random_num]);
		}
	}else{
		int size = all_games.size();
		for(int i = 0; i < n; i++){
			int random_num = rand() % size;
			random_publisher.push_back(all_games[random_num]);
		}
	}
}