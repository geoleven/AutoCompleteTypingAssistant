#include "tree.h"
#include "list.h"
#include <iostream>

using namespace std;

tree::tree()
{
	/*Initialize tou root se eidikes times*/
	root.ff = 32767;
	root.tletter = '#';

}

tree::~tree()
{

}


node::node()
{
	/*Initialize tou komvou tou dentrou kai dunamiko allocation tis listas*/
	ff = 0;
	letters = new list;
	tletter = ' ';
	for (int i = 0; i < N; i++)
	{
		top[i] = NULL;
	}
	parent = NULL;
}

node::~node()
{
	/*Diagrafi tis dunamikis listas tou komvou*/
	delete letters;
}

