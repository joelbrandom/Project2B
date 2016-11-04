//#include "stdafx.h"
#include "store.h"
#include <iostream>
#include <map>
#include <string>

int main()
{
  populateRatingVector("/Users/joel/Documents/School/UMKC Fall 2016/Data Structures/Projects/Project2B/Project2B/ratings.txt");
  populateMap(booksMap, "/Users/joel/Documents/School/UMKC Fall 2016/Data Structures/Projects/Project2B/Project2B/books.txt");
  populateMap(customersMap, "/Users/joel/Documents/School/UMKC Fall 2016/Data Structures/Projects/Project2B/Project2B/customers.txt");

  //for (int i = 0; i < ratingVector.size() / 2; ++i)
    //std::cout << ratingVector[i].customer_ID << ' ' << ratingVector[i].rating << ' ' << ratingVector[i].book_ID << '\n';

  for (std::map<int, std::string>::iterator i = customersMap.begin(); i != customersMap.end(); ++i)
    std::cout << i->first << ' ' << i->second << '\n';

  return 0;
}
