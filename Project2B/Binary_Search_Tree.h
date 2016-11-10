#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "Binary_Tree.h"
#include <vector>
#include <list>
#include <algorithm>
#include <omp.h>
#include "store.h"

template<typename Item_Type>
class Binary_Search_Tree : public Binary_Tree<Item_Type>
{
public:
	// Constructor
	Binary_Search_Tree() : Binary_Tree<Item_Type>() {}

	// Public Member Functions
	virtual bool insert(const Item_Type& item);

	virtual bool erase(const Item_Type& item);

	const Item_Type* min() const;

	const Item_Type* min(BTNode<Item_Type>* local_root) const;

	const Item_Type* max() const;

	const Item_Type* max(BTNode<Item_Type>* local_root) const;

	const Item_Type* find(const Item_Type& target) const;

	// Search for an item. You do not need to specify the target's entire name.
	void search(const Item_Type& target, std::vector<Item_Type>& matches) const;

	void makeListOfRecs(const Binary_Search_Tree<Item_Type>& compare, std::list<std::string>& list) const;

	const std::vector<Item_Type> searchWrapper(const Item_Type& target) const;

	const std::list<std::string> makeListOfRecsWrapper(const Binary_Search_Tree<Item_Type>& compare) const;

private:

	// Private Member Functions
	virtual bool insert(BTNode<Item_Type>*& local_root,
		const Item_Type& item);

	virtual bool erase(BTNode<Item_Type>*& local_root,
		const Item_Type& item);

	const Item_Type* find(BTNode<Item_Type>* local_root,
		const Item_Type& target) const;

	void search(BTNode<Item_Type>* local_root, const Item_Type& target, std::vector<Item_Type>& matches) const;

	void makeListOfRecs(BTNode<Item_Type>* local_root, const Binary_Search_Tree<Item_Type>& compare, std::list<std::string>& list) const;

	virtual void replace_parent(
		BTNode<Item_Type>*& old_root,
		BTNode<Item_Type>*& local_root);

}; // End binary search tree

template<typename Item_Type>
const Item_Type* Binary_Search_Tree<Item_Type>::min() const {
	return min(this->root);

}

template<typename Item_Type>
const Item_Type* Binary_Search_Tree<Item_Type>::min(BTNode<Item_Type>* local_root) const{
	if (local_root == NULL)
		return NULL;

		if (local_root->left == NULL)
			return &(local_root->data);
		 return min(local_root->left);
}

template<typename Item_Type>
const Item_Type* Binary_Search_Tree<Item_Type>::max() const {
	return max(this->root);

}

template<typename Item_Type>
const Item_Type* Binary_Search_Tree<Item_Type>::max(BTNode<Item_Type>* local_root) const{
	if (local_root == NULL)
		return NULL;

		if (local_root->right == NULL)
			return &(local_root->data);
		return max(local_root->right);
}



template<typename Item_Type>
const Item_Type* Binary_Search_Tree<Item_Type>::find(
	const Item_Type& item) const {
		return find(this->root, item);
}

template<typename Item_Type>
const Item_Type* Binary_Search_Tree<Item_Type>::find(BTNode<Item_Type>* local_root, const Item_Type& target) const {
		if (local_root == NULL)
			return NULL;
		if (target < local_root->data)
			return find(local_root->left, target);
		else if (local_root->data < target)
			return find(local_root->right, target);
		else
			return &(local_root->data);
}

template<typename Item_Type>
void Binary_Search_Tree<Item_Type>::search(const Item_Type& target, std::vector<Item_Type>& matches) const
{
	return search(this->root, target);
}

template<typename Item_Type>
void Binary_Search_Tree<Item_Type>::search(BTNode<Item_Type>* local_root, const Item_Type& target, std::vector<Item_Type>& matches) const
{
	if (local_root == NULL)
		return;
	// We may be able to use the below code to match without being case sensitive
	//std::string check = local_root->data.substr(0, target.length());
	//if (std::transform(target.begin(), target.end(), target.begin(), ::tolower) == std::transform(check.begin(), check.end(), check.begin(), ::tolower))

	// If target matches the beginning of the node, push it into matches list
	// and then search the left and right subtrees
	// If target does not match, compare and search left subtree if target is less than the node's data
	// or search right subtree if target is greater than the node's data
	if (target == local_root->data.substr(0, target.length()))
	{
		matches.push_back(local_root->data);
		#pragma omp parallel sections
		{
			#pragma omp section
			search(local_root->left, target, matches);
			#pragma omp section
			search(local_root->right, target, matches);
		}
	}
	else if (target < local_root->data)
		return search(local_root->left, target, matches);
	else if (target > local_root->data)
		return search(local_root->right, target, matches);
}

// Wrapper function for search()
// start searching at root, and go from there
template<typename Item_Type>
const std::vector<Item_Type> Binary_Search_Tree<Item_Type>::searchWrapper(const Item_Type& target) const
{
	std::vector<Item_Type> matches;
	search(this->root, target, matches);
	return matches;
}

template<typename Item_Type>
void Binary_Search_Tree<Item_Type>::makeListOfRecs(const Binary_Search_Tree<Item_Type>& compare, std::list<std::string>& list) const
{
	return makeListOfRecs(this->root, compare, list);
}

template<typename Item_Type>
void Binary_Search_Tree<Item_Type>::makeListOfRecs(BTNode<Item_Type>* local_root, const Binary_Search_Tree<Item_Type>& compare, std::list<std::string>& list) const
{
	if (local_root == NULL || list.size() == 10)
		return;
	if (compare.find(local_root->data.book_ID) == NULL)
	{
		//list.push_back(get_data().book_ID);
		//std::string title = *booksByISBN.find(std::to_string(get_data().book_ID));
		std::string theISBN = local_root->data.book_ID;
		//(booksByISBN.find(local_root->data.book_ID));
		std::vector<std::string> v = booksByISBN.searchWrapper(theISBN);
		if (v.size() != 0)
			list.push_back(booksByISBN.searchWrapper(theISBN)[0]);
	}
	makeListOfRecs(local_root->left, compare, list);
	makeListOfRecs(local_root->right, compare, list);
}

template<typename Item_Type>
const std::list<std::string> Binary_Search_Tree<Item_Type>::makeListOfRecsWrapper(const Binary_Search_Tree<Item_Type>& compare) const
{
	std::list<std::string> listOfRecs;
	makeListOfRecs(this->root, compare, listOfRecs);
	return listOfRecs;
}

template<typename Item_Type>
bool Binary_Search_Tree<Item_Type>::insert(const Item_Type& item) {
		return insert(this->root, item);
}

template<typename Item_Type>
bool Binary_Search_Tree<Item_Type>::insert(BTNode<Item_Type>*& local_root,const Item_Type& item) {
		if (local_root == NULL) {
			local_root = new BTNode<Item_Type>(item);
			return true;
		}
		else {
			if (item < local_root->data)
				return insert(local_root->left, item);
			else if (local_root->data < item)
				return insert(local_root->right, item);
			else
				return false;
		}
}


template<typename Item_Type>
bool Binary_Search_Tree<Item_Type>::erase(const Item_Type& item) {
		return erase(this->root, item);
}

template<typename Item_Type>
bool Binary_Search_Tree<Item_Type>::erase(BTNode<Item_Type>*& local_root, const Item_Type& item) {
	if (local_root == NULL) {
		return false;
	}
	else {
		if (item < local_root->data)
			return erase(local_root->left, item);
		else if (local_root->data < item)
			return erase(local_root->right, item);
		else {
			BTNode<Item_Type>* old_root = local_root;
			if (local_root->left == NULL) {
				local_root = local_root->right;
			}
			else if (local_root->right == NULL) {
				local_root = local_root->left;
			}
			else {
				replace_parent(old_root, old_root->left);
			}
			delete old_root;
			return true;
		}
	}
}

template<typename Item_Type>
void Binary_Search_Tree<Item_Type>::replace_parent(BTNode<Item_Type>*& old_root, BTNode<Item_Type>*& local_root) {
		if (local_root->right != NULL) {
			replace_parent(old_root, local_root->right);
		}
		else {
			old_root->data = local_root->data;
			old_root = local_root;
			local_root = local_root->left;
		}
}



#endif
