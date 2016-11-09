#include "stdafx.h"
#include <iostream>
#include <map>
#include <string>
#include "Binary_Search_Tree.h"
#include <queue>
#include "Menu.h"


int main()
{
  // populating the vectors and binary search trees
  // these functions will be moved later on to clean up main()
  populateCustomerVector("customers.txt");
  populateRatingVector("ratings.txt");
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
  return 0;
}
