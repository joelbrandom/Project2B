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

  populateCustomerVector("/Users/joel/Documents/School/UMKC Fall 2016/Data Structures/Projects/Project2B/Project2B/customers.txt");

  Book a(123, "The dog", true);
  Book b(21, "zCool", true);

  if (b > a)
    std::cout << "B bigger than a!\n";
  else if (a > b)
    std::cout << "a bigger than b\n";

  Book c(123, "The dog", false);
  Book d(21, "zCool", false);

  if (d > c)
    std::cout << "d bigger than c!\n";
  else if (c > d)
    std::cout << "c bigger than d\n";

/*
  int login;
  std::cout << "Log in as:\n";

  while(login != -1)
  {
    std::cin >> login;

    if (login >= 0 && login < customerVector.size())
      std::cout << "Success! You are logged in as" << customerVector[login] << "!\n";
    else
      std::cout << "No user by that id";
  }
  //for (int i = 0; i < ratingVector.size() / 2; ++i)
    //std::cout << ratingVector[i].customer_ID << ' ' << ratingVector[i].rating << ' ' << ratingVector[i].book_ID << '\n';

  for (std::map<int, std::string>::iterator i = customersMap.begin(); i != customersMap.end(); ++i)
    std::cout << i->first << ' ' << i->second << '\n';
*/
  return 0;
}
