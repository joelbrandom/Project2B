#include "Menu.h"
#include "store.h"
#include <iostream>

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
}

void Menu::show()
{
  if (!loggedIn)
  {
    std::cout << "Please log in to continue. Enter your customer ID:\n";
    std::cin >> loginID;
    login(loginID);
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
