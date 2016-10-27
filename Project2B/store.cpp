
#pragma once


#include<store.h>
#include <fstream>
#include <iostream>
#include <string>
#include<map>
#include<vector>
using namespace std;


int main()
{
	// File I/O
	ifstream fin("books.txt");
	ofstream fout("books_output.txt");
	ifstream fin("customers.txt");
	ofstream fout("customers_output.txt");
	ifstream fin("ratings.txt");
	ofstream fout("ratings_output.txt");

	struct Rating
	{
		int custmer_ID;
		int rating;
		int book_ID;

	};
	//store Ratings into vector need to adjust it a liitle bit, do not remember how to store struct into vector
	vector<Rating>list_rating;
	int i = 0;
	// 
	while (fin >> Rating)
	{
		list_rating[i] = Rating;
		i++;
	}

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
	//function that finds book title by isbn number, if it is not there it will return Error (cpp)
	string find_a_title_by_isbn(string new_isbn)
	{

		for (map<string, string>::iterator V = books.begin(); V != books.end(); V++)
		{
			if (V->first == new_isbn)
			{
				return V->second;

			}
			return "Error";
		}
	}

}
// can make it a function to read in id and names from the file (cpp)
while (fin >> ID >> Name)
{
	customers[ID] = Name;

}
//function that finds book title by isbn number, if it is not there it will return Error (cpp)
string find_name_by_id(int new_id)
{

	for (map<int, string>::iterator K = customers.begin(); K != customers.end(); K++)
	{
		if (K->first == new_id)
		{
			return K->second;

		}
		return "Error";
	}
}


/*for (map<string, int> ::iterator V = m2.begin(); V != m2.end(); V++)
{
fout << V->first << ",  " << V->second << endl;
}
system("pause");
return 0;*/
}





