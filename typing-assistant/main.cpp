/*_________________________________________________Inclusions and definitions___________________________________________________*/
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include "autcom.h"
/*______________Function libraries___________________*/
#include <unistd.h>
#include <termios.h>


using namespace std;

/*Forward declaration*/
int getchar_silent();
void cap(string*, string*);

int main(int argc, char *argv[])
{
	string path;
	char d[3] = { '-', 'd', '\0'};


	/*______________________________________________Argument reader/Path selector_______________________________________________*/
	/*No path name given (default case)*/
	if (argc < 2)
	{
		/*Path environmental variable retrieval*/
		path = getenv("HOME");

		/*Adding "/" to the end of path if not there*/
		if (path[(path.length() - 1)] != '/')
			path = path + "/";
		path = path + ".dict";
	}
	else if ((argc < 3) && (strcmp(argv[1], d) == 0))
	{
		path = argv[2];
	}
	else
	{
		cout << "Den dwthikan swsta orismata! Exiting." << endl;
		return 1;
	}

	/*Elegxos*/
	cout << "The dictionary path is: " << path << endl;
	cout << "Please wait while loading. . ." << endl;


	/*_____________________________________________File existence and access check______________________________________________*/

	{
	struct stat dictfile;
	if (lstat(path.c_str(), &dictfile) == -1)
	{
		perror(" Failed to get dictionary. Exiting. ");
		return 1;
	}
	if (!S_ISREG(dictfile.st_mode))
	{
		perror(" Specified path is not a file. Exiting. ");
		return 1;
	}
	}


	/*_______________________________________________Input switcher and actions_________________________________________________*/

	char next;
	autcom mac(path);
	string inbuf = "";
	string original = "";
	string text = "";


    while ((next = getchar_silent()) != EOF)
    {
        if (isalpha(next))
        {
            putchar(next);
            original.append(string(1, next));
            inbuf.append(string(1, tolower(next)));
        }
        else if (next == ' ' || next == '\n')
        {
        	/*Otan allazei leksi i grammi prosthetoume i enimerwnoume analoga sto leksiko*/
        	text =  text + original + " ";
        	if (next == ' ')
        		putchar(next);
        	if (inbuf.length() > 0)
        		mac.add(inbuf);
        	inbuf = "";
        	inbuf.clear();
        	original = "";
        	original.clear();
        }
        else
        {
        	switch(next)
        	{
        		case '\t':
        		{
        			string temps;
        			int epiloges = 0;
        			if ((epiloges = (mac.print_top(inbuf, &temps))) == 1)
        			{
        				temps = "";
        				//temps.clear();
        				if((next = getchar_silent()) == EOF)
        					continue;
        				mac.complete(next, inbuf, &temps);
        				cap(&original, &temps);
        				text.append(temps);
        				text.append(" ");
        				cout << text;
        				fflush(stdout);
        				inbuf = "";
        			}
        			else if (epiloges == 0)
        			{
        				/*Periptwsi pou uparxei mono ena suggestion kai ara sumplirwnoume tin leksi*/
        				mac.add(temps);
        				cap(&original, &temps);
        				cout << temps.substr(inbuf.length(), string::npos);
        				text.append(original);
        				text.append(temps.substr(inbuf.length(), string::npos));
        				inbuf = "";
        			}
        			else
        			{
        				break;
        			}
    	        	inbuf = "";
    	        	inbuf.clear();
    	        	temps = "";
    	        	temps.clear();
    	        	original = "";
    	        	original.clear();
        		}
        			break;
        		case VEOF:
        			cout << "\nExiting" << endl;
        			return 0;
        			break;
        		default:
        			continue;
        	}
        }
    }

	return 0;
}


/*__________________________________________________Given intercept function____________________________________________________*/

int getchar_silent()
{
    int ch;
    struct termios oldt, newt;

    /* Retrieve old terminal settings */
    tcgetattr(STDIN_FILENO, &oldt);

    /* Disable canonical input mode, and input character echoing. */
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );

    /* Set new terminal settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    /* Read next character, and then switch to old terminal settings. */
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}


void cap(string* a, string* b)
{
	for(int i = 0; i < (*a).length(); i++)
	{
		if (isupper((*a)[i]))
			(*b)[i] = toupper((*b)[i]);
	}
}
