//Diana Wang - Huber Ochoa

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#pragma GCC disgnostic ignored "-Wwrite-strings"

using namespace std;

/*class Items
{
	public:
	 Items(){};
	 virtual bool execute() = 0;
};

class Command: public Items
{
	protected:
	 char* command[];
	public:
	 Command()
	 {
		
	 };
	 Command(char* para[])
	 {
		command[] = para[];
	 }
	 void set_command(char* para[])
	 {
		command[] = para[];
	 }
	 bool execute()
	 { //finish this function, Huber }
};

class Connector: public Items
{
	protected:
	 Item* before;
	public:
	 Connector(){};
	 virtual void set_before(Item* para) = 0;
	 virtual bool execute() = 0;
	
};

class Always: public Connector()
{
	public:
	 Always(){};
	 Always(Item* para)
	 {
		before = para;
	 }
	 void set_before(Item* para)
	 {
		before = para;
	 }
	 bool execute()
	 {
		//execute Before Item, then assess whether next item should
		//execute. return true for yes, false for no
	 }
};

class Succes: public Connector()
{
	public:
	 Succes(){};
	 Success(Item* para)
	 {
		before = para;
	 }
	 void set_before(Item* para)
	 {
		before = para;
	 }
	 bool execute()
	 {
		//same as above execute
	 }
};

class Failure: public Connector()
{
	public:
	 Failure(){};
	 Failure(Item* para)
	 {
		before = para;
	 }
	 void set_before(Item* para)
	 {
		before = para;
	 }
	 bool execute()
	 {
		//figure it out
	 }

};*/

bool add_com(vector<string> &arr, string var)
{
	int last = var.size() - 1;
	bool temp;
	if (var == "||")
	{
		arr.push_back("Failure type");
		temp = true;
	}
	else if (var == "&&")
	{
		arr.push_back("Success type");
		temp = true;
	}
	else if (var.at(last) == ';')
	{
		string com = var.substr(0, last);
		arr.push_back(com);
		arr.push_back("Always type");
		temp = true;
	}
	else if (var == "#")
	{
		arr.push_back("Rest Was Comment");
		temp = true;
	}
	else 
	{
		arr.push_back(var);
		temp = false;
	}
	
	return temp;
}

int main()
{
	string commandLine = "start";

	while (commandLine != "exit")
	{
		cout << "$ ";
		getline(cin ,commandLine);

		if(commandLine != "exit")
		{
			string temp;
			vector< vector<string> > master;
			for (istringstream tString1(commandLine); tString1 >> temp; )
			{
				
				bool detectCon = false;
				vector<string> arr;
				detectCon = add_com(arr, temp);
				while (!detectCon)
				{
					tString1 >> temp;
					detectCon = add_com(arr, temp);
				}
				master.push_back(arr);
			}
			for (int i = 0; i < master.size(); ++i)
			{
				for (int j = 0; j < master.at(i).size(); ++j)
				{
					cout << master.at(i).at(j) << ' ';
				}
				cout << endl;
			}
		}
	}
	return 0;
}

