#include "autcom.h"
#include "list.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>

#define OVERWRITE 1
#define SE 0

using namespace std;

autcom::autcom(string path)
{
	mpath = path;
	insert(path);
}

/*Eisagwgi leksewn apo to leksiko*/
int autcom::insert(string path)
{

	ifstream inp(path.c_str(), ifstream::in);
	if ( inp.is_open() )
	{
		string temp;
		while (!inp.eof())
		{
			istream& inps = inp;
			inps >> temp;
			if (temp != "\t" && temp != " " && temp != "\n" && temp != "")
				add(temp);
			temp = "";
			temp.clear();
		}
		inp.close();
		cout << "Dictionary loaded!" << endl;
	}
	else
	{
		inp.close();
		cout << "To input file den anoikse!" << endl;
		return 0;
	}
	return 1;
}

/*Antikatastasi paliou leksikou me neo an ayto einai defined*/
autcom::~autcom()
{
	if (OVERWRITE)
	{
		string path = mpath + "new";
		if(remove(mpath.c_str()))
			cout << "Original file cannot be replaced for some reason." << endl;
		else
			if (rename(path.c_str(), mpath.c_str()))
				cout << "Original file cannot be replaced for some reason." << endl;
	}
}

/*Save leksewn sto neo leksiko*/
int autcom::osave(string path, string word)
{
	path = path + "new";
	ofstream otp(path.c_str(), ios::app);
	otp << word;
	otp << "\n";
	otp.close();
	return 0;
}

/*Sunartisi pou elegxei an uparxei mia leksi sto dentro kai an oxi tin prosthetei. Kanei episis update ta ff*/
int autcom::add(string word)
{
	node* troot= &(trie.root);
	node* temp = NULL;
	int next_char;
	unsigned int counter = 0;
	if (word.length() <= 0 || word == " " || word == "")
	{
		/*Gia debugging*/
		//cout << "Oups!" << endl;
		return -1;
	}
	while (counter < word.length())
	{
		next_char = word[counter];
		temp = troot;
		if (!isalpha(next_char))
		{

		}
		else if (!(troot = troot->letters->isin(next_char)))
		{
			temp->letters->nnodel(next_char, temp);
			troot = temp;
			continue;
		}
		counter++;
	}
	temp = troot;
	if (!(troot = troot->letters->isin('@')))
	{
		temp->letters->nnodel('@', temp);
		temp = temp->letters->isin('@');
		if(osave(mpath, word))
			perror("\nError on saving file! \n");
	}
	else
		temp = troot;
	(temp->ff)++;
	update_top(temp, temp);

	return 1;
}

/*Anadromiko update twn ff sto dentro*/
int autcom::update_top(node* mynode, node* node_with_new_f)
{
	if (mynode == NULL)
		return -1;
	if (mynode->tletter == '@')
		update_top(mynode->parent, node_with_new_f);
	else
	{
		int temp = -1;
		int exists = 0;
		for(int i = 0; i < N; i++)
		{
			if (mynode->top[i] == NULL)
			{
				temp = i;
				break;
			}
			else if (mynode->top[i]->ff <= node_with_new_f->ff)
			{
				if (temp == -1)
					temp = i;
				else
					if(mynode->top[i]->ff <= mynode->top[temp]->ff)
						temp = i;
			}
		}
		for (int i = 0; i < N; i++)
			if (mynode->top[i] == node_with_new_f)
				exists = 1;
		if (temp != -1 && !exists)
			mynode->top[temp] = node_with_new_f;
		if (mynode->tletter != '#')
			update_top(mynode->parent, node_with_new_f);
	}
	return 0;
}

/*Sorting twn suggestions pou tha provlithoun*/
void autcom::sortt(string* table)
{
	int len = N;
	int swapped;
	do
	{
		swapped = 0;
		for (int i = 1; i < len; i++)
		{
			if (table[i-1] > table[i])
			{
				string temp;
				temp = table[i];
				table[i] = table[i-1];
				table[i-1] = temp;
				swapped = 1;
			}
		}
	}while(!swapped);
}

/*Epistrofi tis leksis pou antiproswpeuei ena fullo*/
string autcom::leaf_word(node* mynode)
{
	string temp = "";
	if (mynode == NULL)
		return "^";
	if (mynode->tletter != '@')
	{
		perror ( "Error encountered! " );
		cout << "letter was" << string(1,mynode->tletter) << endl;
		return " ";
	}
	node* pointer = mynode->parent;
	while (pointer->tletter != '#')
	{
		temp.insert(0, string(1, pointer->tletter));
		pointer = pointer->parent;
	}
	return temp;
}

/*Epistrofi se buffer twn top N suggestions*/
int autcom::return_top(string word, string* temp)
{
	node* troot = &(trie.root);
	unsigned int counter = 0;
	int next_char = word[0];
	while (counter < word.length())
	{
		next_char = word[counter];
		if (!(troot = troot->letters->isin(next_char)))
			return 0;
		counter++;
	}

	counter = 0;

	for(int i = 0; i < N; i++)
	{
		temp[i] = leaf_word(troot->top[i]);
		if (temp[i] != "^")
			counter++;
	}

	sortt(temp);
	return counter;
}

/*Ektupwsi ton top N suggestions ektos an ayto einai mono ena opote epistrofi se buffer*/
int autcom::print_top(string inbuf, string* output)
{
	string suggestions[N];
	int resultsn = 0;
	if ((resultsn = return_top(inbuf, suggestions)) == 1)
	{
		for (int i = 0; i < N; i++)
		{
			if (suggestions[i] != "^")
			{
				(*output) = suggestions[i];
				break;
			}
		}
		return 0;
	}
	else if (resultsn == 0)
	{
		//perror ( "\nThere no suggestions yet available! " );
		//cerr << "\nThere no suggestions yet available!";
		//cout << endl;
		return -1;
	}
	else
	{
		if (SE)
			cerr.tie(&cout);
		cerr << endl;
		for(int i = 0; i < N; i++)
			if (suggestions[i] != "^")
				cerr << suggestions[i] << " ";
		cout << endl;
	}
	return 1;
}

/*Sumplirwsi leksis vasi mias epilogis pou ekane o xristis an ayti einai egkuri*/
int autcom::complete(int choice, string inbuf, string* output)
{
	string suggestions[N];
	return_top(inbuf, suggestions);
	if (!isdigit(choice) || choice == '0')
		return 2;
	choice = (choice - '0') - 1;
	int legch = -1;
	for(int i = 0; i < N; i++)
	{
		if (suggestions[i] != "^")
			legch++;
		if (choice == legch)
		{
			choice = i;
			break;
		}
	}

	if (suggestions[choice] != "^")
	{
		(*output) = suggestions[choice];
		add(suggestions[choice]);
	}
	return 0;
}

