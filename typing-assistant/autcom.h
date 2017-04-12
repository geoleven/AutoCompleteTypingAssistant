#ifndef AUTCOM_H_
#define AUTCOM_H_

#include "tree.h"
#include <string>

using namespace std;

class autcom {
	private:
		void sortt(string*);
		string leaf_word(node*);
		int insert(string);
		int osave(string, string);
		string mpath;
	public:
		autcom(string);
		~autcom();
		tree trie;
		int print_top(string, string*);
		int return_top(string, string*);
		int add(string);
		int update_top(node*, node*);
		int complete(int, string, string*);
};


#endif /* AUTCOM_H_ */
