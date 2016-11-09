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
  void ui_Search2();
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
// We also want to save the new ratings to ratings.txt!
void Menu::logout()
{
	saveNewRatings("ratings.txt");
	loggedIn = false;
}

void Menu::show()
{
	populateCustomerVector("customers.txt");
	//populateRatingVector("ratings.txt");
	populateRatingBST("ratings.txt");
	populateBooksBST("books.txt");
  // While not logged in, ask user to log in and attempt login
  while (!loggedIn)
  {
    std::cout << "Please log in to continue. Enter your customer ID:\n";
    std::cin.clear();
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

    std::cin.clear();
    std::cin >> choice;

    switch (choice)
    {
      case 0:
        ui_Search2();
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
  std::vector<string> matches;
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
      std::cout << "There were multiple matches for your search. Please select your desired book by entering the corresponding number:\n";

    // Output all matches
    // Not yet included: we will have a number next to each match like 5: title
    // then the user can select which of the matches they meant or none at all
    // once they select the correct book, they can then rate it
    //for (std::list<std::string>::const_iterator it = matches.begin(); it != matches.end(); ++it)
    //  std::cout << *it << '\n';
    for (int i = 0; i < matches.size(); ++i)
      std::cout << i << ": " << matches.at(i) << '\n';

    if (matches.size() > 1)
    {
      int select;
      std::cin >> select;
      // We want select to be between 0 and matches.size()-1 inclusive, so this will allow
      // the user to select again if their input is not one of the options
      while (select < 0 || select >= matches.size())
      {
        std::cout << "Error: There is no book corresponding to the number: " << select << '\n';
        std::cout << "Please try again:\n";
        std::cin >> select;
      }

      if (select >= 0 && select < matches.size())
      {
        std::cout << "You selected: " << matches.at(select) << '\n';
        // Ask user to rate the book between 1-5 inclusive
        // If user has already rated book, provide them with their current rating
        std::string targetISBN;
        int ratingCheck;
        if (searchByISBN)
        {
          // If we searched by ISBN, the result with be (ISBN, Title)
          // so we want to grab the ISBN by getting the first 9 characters
          // of the match
          targetISBN = matches.at(select).substr(0, 9);
          // The rating check will search for the book in the corresponding
          // vector<Rating> to the logged in customer's ID.
          ratingCheck = searchRatingVector(theRatingVector[loginID], targetISBN);
        }
        else
        {
          // If we searched by title, the match result will be (Title, ISBN)
          // so to grab the ISBN, we want to find the last space, and then
          // get the next 9 characters, starting with the position after the space
          targetISBN = matches.at(select).substr(matches.at(select).find_last_of(" ") + 1, 9);
          ratingCheck = searchRatingVector(theRatingVector[loginID], targetISBN);
        }
        // Initialize inputRating as -1, so we know what it is
        // and that it currently won't be accepted
        int inputRating = -1;
        if (ratingCheck == -1)
        {
          // If ratingCheck returns -1, then the user has not rated the book
          std::cout << "You have not rated this book. What would you like to rate it (1-5 inclusive)?\n";
          while (inputRating <= 0 || inputRating > 5)
          {
            std::cin.clear();
            std::cin >> inputRating;
            std::cout << "Error: you tried to rate the book a value outside of 1-5 inclusive. Please try again:\n";
          }
          Rating newRating(inputRating, std::stoi(targetISBN));
          theRatingVector[loginID].push_back(newRating);
        }
        else
        {
          // This else covers when the user has rated the book and may want
          // to update their rating
          std::cout << "You have already rated this book (current rating: " << theRatingVector[loginID][ratingCheck].rating << ").\n";
          std::cout << "You can now change your rating if you wish:\n";
          while (inputRating <= 0 || inputRating > 5)
          {
            std::cin.clear();
            std::cin >> inputRating;
            if (inputRating <= 0 || inputRating > 5)
              std::cout << "Error: you tried to rate the book a value outisde of 1-5 inclusive.\nPlease try again:\n";
          }
          theRatingVector[loginID][ratingCheck].rating = inputRating;
        }
      }
    }
  }
  else
  {
    std::cout << "There were no matches for your search.\n";
  }
}

// This is testing to see how it works with ratings in a vector of BSTs
void Menu::ui_Search2()
{
	std::string target;
	bool searchByISBN;
	std::vector<string> matches;
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
			std::cout << "There were multiple matches for your search. Please select your desired book by entering the corresponding number:\n";

		// Output all matches
		// Not yet included: we will have a number next to each match like 5: title
		// then the user can select which of the matches they meant or none at all
		// once they select the correct book, they can then rate it
		//for (std::list<std::string>::const_iterator it = matches.begin(); it != matches.end(); ++it)
		//  std::cout << *it << '\n';
		for (int i = 0; i < matches.size(); ++i)
			std::cout << i << ": " << matches.at(i) << '\n';

		if (matches.size() > 1)
		{
			int select;
			std::cin >> select;
			// We want select to be between 0 and matches.size()-1 inclusive, so this will allow
			// the user to select again if their input is not one of the options
			while (select < 0 || select >= matches.size())
			{
				std::cout << "Error: There is no book corresponding to the number: " << select << '\n';
				std::cout << "Please try again:\n";
				std::cin >> select;
			}

			if (select >= 0 && select < matches.size())
			{
				std::cout << "You selected: " << matches.at(select) << '\n';
				// Ask user to rate the book between 1-5 inclusive
				// If user has already rated book, provide them with their current rating
				std::string targetISBN;
				Rating ratingCheck;
				bool found = false;
				if (searchByISBN)
				{
					// If we searched by ISBN, the result with be (ISBN, Title)
					// so we want to grab the ISBN by getting the first 9 characters
					// of the match
					targetISBN = matches.at(select).substr(0, 9);
					Rating targetRating(std::stoi(targetISBN));
					// The rating check will search for the book in the corresponding
					// vector<Rating> to the logged in customer's ID.
					if (ratingBST[loginID].find(targetRating) != NULL)
					{
						ratingCheck = ratingBST[loginID].find(targetRating);
						found = true;
					}
				}
				else
				{
					// If we searched by title, the match result will be (Title, ISBN)
					// so to grab the ISBN, we want to find the last space, and then
					// get the next 9 characters, starting with the position after the space
					targetISBN = matches.at(select).substr(matches.at(select).find_last_of(" ") + 1, 9);
					Rating targetRating(std::stoi(targetISBN));
					if (ratingBST[loginID].find(targetRating) != NULL)
					{
						ratingCheck = ratingBST[loginID].find(targetRating);
						found = true;
					}
				}
				// Initialize inputRating as -1, so we know what it is
				// and that it currently won't be accepted
				int inputRating = -1;
				if (!found)
				{
					// If user has not rated the book
					std::cout << "You have not rated this book. What would you like to rate it (1-5 inclusive)?\n";
					while (inputRating <= 0 || inputRating > 5)
					{
						std::cin.clear();
						std::cin >> inputRating;
						std::cout << "Error: you tried to rate the book a value outside of 1-5 inclusive. Please try again:\n";
					}
					Rating newRating(inputRating, std::stoi(targetISBN));
					ratingBST[loginID].insert(newRating);
				}
				else
				{
					// This else covers when the user has rated the book and may want
					// to update their rating
					std::cout << "You have already rated this book (current rating: " << ratingCheck.rating << ").\n";
					std::cout << "You can now change your rating if you wish:\n";
					while (inputRating <= 0 || inputRating > 5)
					{
						std::cin.clear();
						std::cin >> inputRating;
						if (inputRating <= 0 || inputRating > 5)
							std::cout << "Error: you tried to rate the book a value outisde of 1-5 inclusive.\nPlease try again:\n";
					}
					Rating newRating(inputRating, std::stoi(targetISBN));
					ratingBST[loginID].erase(std::stoi(targetISBN));
					ratingBST[loginID].insert(newRating);
				}
			}
		}
	}
	else
	{
		std::cout << "There were no matches for your search.\n";
	}
}

void Menu::ui_Rate()
{
  // MOVE RATING CODE HERE EVENTUALLY
}

void Menu::ui_Recommendations()
{

}

#endif
