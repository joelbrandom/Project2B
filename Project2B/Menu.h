#ifndef Menu_h
#define Menu_h

#include "store.h"

/* MENU_H
  Log in (enter ID)
    OPTIONS:
      Search for book & rate
      Rate a book not rated
      View book recommendations by relevance
*/

class Menu
{
private:
  int loginID;
  bool loggedIn;
public:
  Menu();
  void login(int);
  void logout();
  void show();
  void ui_Search();
  void ui_Rate();
  void ui_Recommendations();
};

Menu::Menu()
{
  // loggedIn should initialize as false and only be true once user logs in
  loggedIn = false;
}

void Menu::login(int ID)
{
  // Check if specified ID is between 0 and customerVector.size()-1 inclusive
  // If so, then they are logged in as the user with the specified ID
  // If not, report error and try again
  if (ID >= 0 && ID < customerVector.size())
  {
    loginID = ID;
    loggedIn = true;
    std::cout << "Success! You are now logged in as " << customerVector[loginID] << "!\n";
  }
  else
  {
    std::cout << "Error! No customer found with ID: " << ID << ".\n";
  }
}

// To log a customer out, we just need to change their loggedIn value from true to false
void Menu::logout()
{
  loggedIn = false;
}

void Menu::show()
{
  // While not logged in, ask user to log in and attempt login
  while (!loggedIn)
  {
    std::cout << "Please log in to continue. Enter your customer ID:\n";
    std::cin >> loginID;
    login(loginID);
  }
  // While logged in, provide customer menu
  while (loggedIn)
  {
    int choice;
    std::cout << "Choose one of the options below by inputting the corresponding number:\n";
    std::cout << "0: Search for a book\n";
    std::cout << "1: Rate a book\n";
    std::cout << "2: View book recommendations\n";
    std::cout << "3: Log out\n";

    std::cin >> choice;

    switch (choice)
    {
      case 0:
        ui_Search();
        break;
      case 1:
        ui_Rate();
        break;
      case 2:
        ui_Recommendations();
        break;
      case 3:
        logout();
        break;
    }
  }
}

void Menu::ui_Search()
{
  std::string target;
  bool searchByISBN;
  std::list<string> matches;
  // Search by ISBN when searchByISBN is true (1), search by title when searchByISBN is false (0)
  std::cout << "Do you want to search by ISBN (1) or title (0)?\n";
  std::cin >> searchByISBN;
  std::cout << "Please enter your search:\n";
  // cin.ignore() to clear stream
  std::cin.ignore();
  // Use getline instead of cin as titles may have spaces
  std::getline(std::cin, target);
  // if searching by ISBN, search the booksByISBN binary search tree
  // otherwise, search the booksByTitle binary search tree
  if (searchByISBN)
    matches = booksByISBN.searchWrapper(target);
  else
    matches = booksByTitle.searchWrapper(target);

  // if the matches list contains any matches
  if (matches.size() > 0)
  {
    // If only one match, use singular phrasing
    // if more then one match, use plural phrasing
    if (matches.size() == 1)
      std::cout << "Here is the match for your search:\n";
    else
      std::cout << "Here are the matches for your search:\n";

    // Output all matches
    // Not yet included: we will have a number next to each match like 5: title
    // then the user can select which of the matches they meant or none at all
    // once they select the correct book, they can then rate it
    for (std::list<std::string>::const_iterator it = matches.begin(); it != matches.end(); ++it)
      std::cout << *it << '\n';
  }
  else
  {
    std::cout << "There were no matches for your search.\n";
  }
}

void Menu::ui_Rate()
{

}

void Menu::ui_Recommendations()
{

}

#endif
