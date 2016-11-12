#ifndef Menu_h
#define Menu_h

#include "store.h"
#include <cmath> // abs()
#include <math.h> // pow()
#include <limits>

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
	void safeBoolInput(bool&);
	void safeIntInputRange(int&, const int, const int);
	void login(int);
	void logout();
	void show();
	void ui_Search();
	void ui_Recommendations();
};

// Menu constructor, default
Menu::Menu()
{
	// loggedIn should initialize as false and only be true once user logs in
	loggedIn = false;
}

// Function for bool input
// Will only allow the user to input 0 or 1,
// otherwise, will report error and ask user to try again
// parameter a is the var to be inputted
void Menu::safeBoolInput(bool& a)
{
	std::cin >> a;
	while (!std::cin)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		std::cout << "Error: you didn't enter valid input. Please try again:\n";
		std::cin >> a;
	}
}

// Function for int input
// Will allow user to input any integer as long as it falls in the specified range
// will report error and ask to try again when not in range/not integer
// parameter a: var to be inputted
// parameter from: start of range (inclusive)
// parameter to: end of range (inclusive)
void Menu::safeIntInputRange(int& a, const int from, const int to)
{
	std::cin >> a;
	while (!std::cin || a < from || a > to)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
		std::cout << "Error: you didn't enter a valid number. Please try again:\n";
		std::cin >> a;
	}
}

// login function for Menu class
// This runs populateRatingBST() and populateBooksBST() after successful login
// parameter ID: customer ID specified by user
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
		populateRatingBST("ratings.txt", loginID);
		populateBooksBST("books.txt");
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

// Function to display UI menu
void Menu::show()
{
	populateCustomerVector("customers.txt");
	// While not logged in, ask user to log in and attempt login
	while (!loggedIn)
	{
		std::cout << "Please log in to continue. Enter your customer ID:\n";
		safeIntInputRange(loginID, 0, customerVector.size() - 1);
		login(loginID);
	}
	// While logged in, provide customer menu
	while (loggedIn)
	{
		int choice;
		std::cout << "Choose one of the options below by inputting the corresponding number:\n";
		std::cout << "0: Search for a book and rate\n";
		std::cout << "1: View your top 10 book recommendations\n";
		std::cout << "2: Log out\n";

		safeIntInputRange(choice, 0, 2);

		switch (choice)
		{
		case 0:
			ui_Search();
			break;
		case 1:
			ui_Recommendations();
			break;
		case 2:
			logout();
			break;
		}
	}
}

// This is testing to see how it works with ratings in a vector of BSTs
void Menu::ui_Search()
{
	std::string target;
	bool searchByISBN;
	std::vector<string> matches;
	int select = 0;
	// Search by ISBN when searchByISBN is true (1), search by title when searchByISBN is false (0)
	std::cout << "Do you want to search by ISBN (1) or title (0)?\n";

	safeBoolInput(searchByISBN);
	std::cout << "Please enter your search (case sensitive):\n";
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
		if (matches.size() > 1)
			std::cout << "There were multiple matches for your search. Please select your desired book by entering the corresponding number:\n";

		// Output all matches
		// We will have a number next to each match like 5: title
		// then the user can select which of the matches they meant
		// once they select the correct book, they can then rate it
		if (matches.size() > 1)
			for (int i = 0; i < matches.size(); ++i)
				std::cout << i << ": " << matches.at(i) << '\n';

		if (matches.size() > 1)
		{

			// We want select to be between 0 and matches.size()-1 inclusive, so this will allow
			// the user to select again if their input is not one of the options
			safeIntInputRange(select, 0, matches.size() - 1);
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
				Rating targetRating(targetISBN);
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
				Rating targetRating(targetISBN);
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
				// If user has not rated the book, let them rate it
				std::cout << "You have not rated this book. What would you like to rate it (1-5 inclusive)?\n";
				safeIntInputRange(inputRating, 1, 5);
				Rating newRating(loginID, inputRating, targetISBN);
				ratingBST[loginID].insert(newRating);
			}
			else
			{
				// This else covers when the user has rated the book and may want
				// to update their rating, they can keep the same rating by
				// simply entering the same value
				std::cout << "You have already rated this book (current rating: " << ratingCheck.rating << ").\n";
				std::cout << "You can now change your rating if you wish (1-5 inclusive):\n";
				safeIntInputRange(inputRating, 1, 5);
				Rating newRating(loginID, inputRating, targetISBN);
				ratingBST[loginID].erase(targetISBN);
				ratingBST[loginID].insert(newRating);
			}
		}
	}
	else
	{
		std::cout << "There were no matches for your search.\n";
	}
}

void Menu::ui_Recommendations()
{
	// Create 4 threads for more optimal parallelism
	omp_set_num_threads(4);
	#pragma omp parallel for
	for (int i = 0; i < ratingBST.size(); ++i)
	{
		// i = customer being compared to
		// j = index of logged in user's rating vector
		if (i == loginID)
			++i;
		if (i >= customerVector.size())
			break;
		Weight weight(i);
		for (int j = 0; j < loginIDRatings.size(); ++j)
		{
			if (ratingBST[i].find(loginIDRatings[j].book_ID) != NULL)
			{
				// Algorithm to weight each user by similarity
				// Weight = (4 - |your rating - their rating|)^2
				// This will give 0 when there is the max difference in rating ((4 - (5 - 1))^2 = 0)
				// and will give largest value when same score is given ((4 - (5 - 5))^2 = 16)
				weight.weight += pow((4 - std::abs((loginIDRatings[j].rating - ratingBST[i].find(loginIDRatings[j].book_ID)->rating))), 2.0);
			}
		}
		weightQueue.push(weight);
	}
	// makeListOfRecs() will stop after 10 recommendations have been added
	// In the case that we never reach 10 recommendations, we will stop
	// once the weightQueue is empty (once all possibilities have been tried)
	while (!weightQueue.empty())
	{
		recommendationsList = ratingBST[weightQueue.top().customer_ID].makeListOfRecsWrapper(ratingBST[loginID]);
		weightQueue.pop();
	}
	// Output the list of recommendations
	for (std::list<std::string>::const_iterator it = recommendationsList.begin(); it != recommendationsList.end(); ++it)
		std::cout << *it << '\n';
}

#endif
