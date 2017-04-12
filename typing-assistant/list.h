#ifndef LIST_H_
#define LIST_H_

#include "tree.h"
//#include <string>

using namespace std;

class nodel {
	private:

	public:
		nodel();
		~nodel();
		nodel* next;
		nodel* prev;
		int letter;
		node child;
};

class list {
	private:
	int counter;
	public:
		list();
		~list();
		int dnodel(nodel*);
		nodel* nnodel(int, node*);
		nodel* first;
		nodel* last;
		node* isin(int);
};



#endif /* LIST_H_ */
