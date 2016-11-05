#pragma once


//#include<store.h>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include "Binary_Search_Tree.h"
//#include <omp.h>
using namespace std;

struct Rating
{
	int customer_ID;
	int rating;
	int book_ID;

	// constructors for Rating
	Rating (int custID, int rate, int bookID)
	{
		customer_ID = custID;
		rating = rate;
		book_ID = bookID;
	}
	Rating ()
	{
	}
};

struct Book
{
	int isbn;
	string title;
	bool sortByISBN;

	// constructors for Book
	Book (int a, string b, bool c)
	{
		isbn = a;
		title = b;
		sortByISBN = c;
	}
	Book ()
	{
	}

	// Overload > and < operators for comparing books
	// check if we're sorting by ISBN or title to tell whether we should
	// compare ISBN or title
	bool operator>(const Book& compare) const
	{
		if (this->sortByISBN)
			return (this->isbn > compare.isbn);
		else
			return (this->title > compare.title);
	}
	bool operator<(const Book& compare) const
	{
		if (this->sortByISBN)
			return (this->isbn < compare.isbn);
		else
			return (this->title < compare.title);
	}
};

struct Weight
{
	int customer_ID, weight;

	// Weight constructors
	Weight (int a, int b)
	{
		customer_ID = a;
		weight = b;
	}
	Weight ()
	{
	}

	// overload operators < and >, so we can compare weights and see which is greater
	bool operator>(const Weight& compare) const
	{
		return (this->weight > compare.weight);
	}
	bool operator<(const Weight& compare) const
	{
		return (this->weight < compare.weight);
	}
};

// create booksByISBN and booksByTitle binary search trees
// We need two binary search trees, so we can search by ISBN and title
Binary_Search_Tree<string> booksByISBN, booksByTitle;

// populate Books binary search trees from specified source file
void populateBooksBST(const string& source)
{
	ifstream ifs(source);
	if (ifs)
	{
		string isbn, title, line;

		while (getline(ifs, line))
		{
			// match and store (ISBN) and then (title)
			// Title can contain any characters, so match with anything after the comma and space
			regex re("([0-9]+), (.*)");
			smatch match;
			if (regex_search(line, match, re) && match.size() > 1)
			{
				isbn = match.str(1);
				title = match.str(2);
				// Create string that is the title followed by a comma, a space, and then ISBN number
				// then store in booksByTitle, so we can sort by title
				string title_ISBN = match.str(2) + ", " + match.str(1);
				booksByTitle.insert(title_ISBN);
				booksByISBN.insert(line);
			}
		}
	}
}

vector<Rating> ratingVector;
vector<string> customerVector;
map<int, string> booksMap, customersMap;

// Store ratings into a vector
void populateRatingVector(const string& source)
{
	ifstream ifs(source);
	if (ifs)
	{
		int cID, user_rating, book;
		string line;

		while (getline(ifs, line))
		{
			// Store and match (number), (number), (number)
			// they are: customer_ID, rating, book_ID
			regex re("([0-9]+), ([0-9]+), ([0-9]+)");
			smatch match;
			if (regex_search(line, match, re) && match.size() > 1)
			{
				cID = stoi(match.str(1));
				user_rating = stoi(match.str(2));
				book = stoi(match.str(3));
				Rating rating(cID, user_rating, book);
				ratingVector.push_back(rating);
			}
		}
		ifs.close();
	}
}

void populateCustomerVector(const string& source)
{
	ifstream ifs(source);
	if (ifs)
	{
		string name, line;

		while (getline(ifs, line))
		{
			// We only match the customer's name as their ID will be the same as their index in the vector
			// as in: customer ID n will be customerVector[n]
			regex re(", (.*)");
			smatch match;
			if (regex_search(line, match, re) && match.size() > 1)
			{
				name = match.str(1);
				customerVector.push_back(name);
			}
		}
	}
}

// Generalized function to populate maps
// Works for customersMap and booksMap
// this will likely be removed
void populateMap(map<int, string>& map, const string& source)
{
	ifstream ifs(source);
	if (ifs)
	{
		int id;
		string name, line;

		while (getline(ifs, line))
		{
			regex re("([0-9]+), (.*)");
			smatch match;
			if (regex_search(line, match, re) && match.size() > 1)
			{
				id = stoi(match.str(1));
				name = match.str(2);
				map[id] = name;
			}
		}
		ifs.close();
	}
}

// Below is the start of a parallel function, we will add and change things to parallel later
// !!!Most things below will likely be removed!!!

/*
void ppp()
{
	omp_set_num_threads(3);
	int MAX = 12, count = 0;
	#pragma omp parallel for
	for (int i = 0; i < MAX; ++i)
	{
		#pragma omp atomic
			++count;
	}
	std::cout << count << '\n';
}
*/


/*
	Below are the beginnings of possible functions to be used
	(what used to be in main())

	// File I/O
	ifstream fin("books.txt");
	ofstream fout("books_output.txt");
	ifstream fin("customers.txt");
	ofstream fout("customers_output.txt");
	ifstream fin("ratings.txt");
	ofstream fout("ratings_output.txt");

	//store Ratings into vector need to adjust it a liitle bit
	vector<Rating>list_rating;
	int i = 0;
	//


	map<string, string> books; // h
	string book_title; // h
	string ISBN; // h

	map<int, string> customers; //h
	int ID; // h
	string Name; // h
	map<string, string>::iterator V;// h
	map<int, string>::iterator K;// h

								 // read in isbn and book title from the file and store it into map
								 // can put it into separate function and call it when you need to store data (cpp)
								 // need to ignore commas
	while (fin >> ISBN >> book_title)
	{

		books[ISBN] = book_title;

	}

*/

//store Ratings into vector need to adjust it a liitle bit
vector<Rating>list_rating;
int i = 0;
//


map<string, string> books; // h
string book_title; // h
string ISBN; // h

map<int, string> customers; //h
int ID; // h
string Name; // h
map<string, string>::iterator V;// h
map<int, string>::iterator K;// h

//function that finds book title by isbn number, if it is not there it will return Error (cpp)
// You can find the second from a map using:
// map_variable["ISBN"]
string find_a_title_by_isbn(string new_isbn)
{

	for (map<string, string>::iterator V = books.begin(); V != books.end(); V++)
	{
		if (V->first == new_isbn)
		{
			return V->second;

		}
		//return "Error";
	}
	return "Error";
}

/*
	// can make it a function to read in id and names from the file (cpp)
	while (fin >> ID >> Name)
	{
		customers[ID] = Name;

	}
*/

//function that finds book title by isbn number, if it is not there it will return Error (cpp)
string find_name_by_id(int new_id)
{

	for (map<int, string>::iterator K = customers.begin(); K != customers.end(); K++)
	{
		if (K->first == new_id)
		{
			return K->second;

		}
		//return "Error";
	}
	return "Error: not found";
}


/*for (map<string, int> ::iterator V = m2.begin(); V != m2.end(); V++)
{
fout << V->first << ",  " << V->second << endl;
}
system("pause");
return 0;
}
*/
