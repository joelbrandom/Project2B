#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "Binary_Search_Tree.h"
#include <queue>
#include <omp.h>
#include <list>
using namespace std;

struct Rating
{
	int customer_ID;
	int rating;
	string book_ID;

	// constructors for Rating
	Rating(int a, int b, string c)
	{
		customer_ID = a;
		rating = b;
		book_ID = c;
	}
	Rating (int a, string b)
	{
		//customer_ID = custID;
		rating = a;
		book_ID = b;
	}
	Rating(string a)
	{
		book_ID = a;
	}
	Rating ()
	{
	}

	bool operator<(const Rating& compare) const
	{
		return (this->book_ID < compare.book_ID);
	}
	bool operator>(const Rating& compare) const
	{
		return (this->book_ID > compare.book_ID);
	}
	bool operator==(const Rating& compare) const
	{
		return (this->book_ID == compare.book_ID);
	}
	void operator=(const Rating* assignment)
	{
		this->book_ID = assignment->book_ID;
		this->rating = assignment->rating;
	}
	void operator=(const Rating& assignment)
	{
		this->book_ID = assignment.book_ID;
		this->rating = assignment.rating;
	}

	friend ostream& operator<<(ostream& os, const Rating& r);
};

ostream& operator<<(ostream& os, const Rating& r)
{
	os << r.customer_ID << ", " << r.rating << ", " << r.book_ID;
	return os;
}

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
	Book(int a, string b)
	{
		isbn = a;
		title = b;
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
	Weight(int a)
	{
		customer_ID = a;
		weight = 0;
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

// loginIDRatings is a vector of all the logged in user's ratings
// this makes it so we can use parallel programming easier
// in the recommendation function
vector<Rating> loginIDRatings;
vector<Binary_Search_Tree<Rating>> ratingBST;
vector<string> customerVector;
priority_queue<Weight> weightQueue;
list<string> recommendationsList;

// Populate the binary search trees of ratings with the ratings from specified file
void populateRatingBST(const string& source, const int& loginID)
{
	while (ratingBST.size() < customerVector.size())
	{
		Binary_Search_Tree<Rating> bst;
		ratingBST.push_back(bst);
	}

	ifstream ifs(source);
	if (ifs)
	{
		int cust_ID, user_rating;
		string book, line;

		while (getline(ifs, line))
		{
			regex re("([0-9]+), ([0-9]+), ([0-9]+)");
			smatch match;
			if (regex_search(line, match, re) && match.size() > 1)
			{
				cust_ID = stoi(match.str(1));
				user_rating = stoi(match.str(2));
				book = match.str(3);
				Rating rating(cust_ID, user_rating, book);
				ratingBST[cust_ID].insert(rating);
				if (cust_ID == loginID)
					loginIDRatings.push_back(rating);
			}
		}
	}
}

// Populate the customer vector so it contains
// all customers from specified file (customers.txt)
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

// Save newly made ratings into the ratings.txt
void saveNewRatings(const string& source)
{
	ofstream ofs(source);
	if (ofs.is_open())
	{
		for (int i = 0; i < ratingBST.size(); ++i)
		{
			ofs << ratingBST[i].to_string_noNull();
		}
		ofs.close();
	}
}
