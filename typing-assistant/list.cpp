#include "list.h"
#include "tree.h"
#include <iostream>


using namespace std;

list::list()
{
	first = NULL;
	last = NULL;
	counter = 0;
}

list::~list()
{
	/*Katastrofi tis dunamikis listas diagrafontas tous epimerous komvous tis*/
	if (first != NULL)
	{
		while(first != NULL)
		{
			if (first != last)
			{
				last = last->prev;
				delete last->next;
				last->next = NULL;
			}
			else if (first == last)
			{
				delete last;
				first = NULL;
				last = NULL;
			}
			else
			{
				cout << "Error!\n" << endl;
			}
		}
	}
}

/*Sunartisi pou elegxei an uparxei idi ena gramma(akmi) mesa se mia lista*/
node* list::isin(int myletter)
{
	nodel* pointer = this->first;
	while (pointer != NULL)
	{
		if(myletter == pointer->letter)
		{
			return &(pointer->child);
		}
		pointer = pointer->next;
	}

	return NULL;
}

/*Sunartisi pou dimiourgei enan neo komvo listas mesa sti lista kai tou pernaei ta swsta orismata*/
nodel* list::nnodel(int myletter, node* mparent)
{
	nodel* temp = new nodel;

	if (first == NULL)
	{
		first = temp;
		last = temp;
	}
	else
	{
		last->next = temp;
		temp->prev = last;
		last = temp;
	}

	temp->letter = myletter;
	temp->child.tletter = myletter;
	temp->child.parent = mparent;
	counter++;
	return temp;
}

/*Deprecated. Sunartisi pou diaghafei enan komvo mesa apo tin lista dinontas ton deikti ston komvo*/
int list::dnodel(nodel* mynode)
{
	if (mynode == NULL)
		return 1;
	if (mynode == first && mynode == last)
	{
		first = NULL;
		last = NULL;
	}
	else if (mynode == first)
	{
		first = first->next;
		first->prev = NULL;
	}
	else if (mynode == last)
	{
		last = last->prev;
		last->next = NULL;
	}
	else
	{
		mynode->prev->next = mynode->next;
		mynode->next->prev = mynode->prev;
	}
	delete mynode;
	counter--;
	return 0;
}

nodel::nodel()
{
	next = NULL;
	prev = NULL;
	letter = ' ';
}

nodel::~nodel()
{


}
