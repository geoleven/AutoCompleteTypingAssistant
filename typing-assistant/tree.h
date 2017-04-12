#ifndef TREE_H_
#define TREE_H_

//#include "list.h"
//#include <string>

#define N 6

using namespace std;

class list;

class node {
	private:

	public:
		node();
		~node();
		list* letters;
		int tletter;
		int ff;				/*frequence factor*/
		node* top[N];
		node* parent;
};

class tree {
	private:

	public:
		tree();
		~tree();
		node root;
};


#endif /* TREE_H_ */
