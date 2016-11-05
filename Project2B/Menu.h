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
  loggedIn = false;
}

void Menu::login(int ID)
{
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

void Menu::logout()
{
  loggedIn = false;
}

void Menu::show()
{
  while (!loggedIn)
  {
    std::cout << "Please log in to continue. Enter your customer ID:\n";
    std::cin >> loginID;
    login(loginID);
  }
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

}

void Menu::ui_Rate()
{

}

void Menu::ui_Recommendations()
{

}

#endif
