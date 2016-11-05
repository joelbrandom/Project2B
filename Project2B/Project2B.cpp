//#include "stdafx.h"
//#include "store.h"
#include <iostream>
#include <map>
#include <string>
#include "Binary_Search_Tree.h"
#include <queue>
#include "Menu.h"

int main()
{
  populateRatingVector("ratings.txt");

  populateCustomerVector("customers.txt");

  populateBooksBST("books.txt");

  //std::cout << booksByTitle << '\n';

  Menu menu;
  menu.show();

/*
  std::priority_queue<Weight> weights;

  Weight w1(0, 5), w2(3, 9), w3(0, 0), w4(3, -4), w5(6, 23);
  weights.push(w1);
  weights.push(w2);
  weights.push(w3);
  weights.push(w4);
  weights.push(w5);

  while(!weights.empty())
  {
    std::cout << weights.top().weight << '\n';
    weights.pop();
  }
*/

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
