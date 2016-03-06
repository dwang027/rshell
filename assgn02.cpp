//Diana Wang - Huber Ochoa

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#pragma GCC disgnostic ignored "-Wwrite-strings"

using namespace std;

//base class
class Items
{
	public:
	 Items(){};
	 virtual bool execute()=0;
	 /*This is where we interpret our user-input Command Line. aka We find out what we are dealing with*/
	 bool interpret_line(vector<string> &arr, string var, int &type, bool &testCase, int &testType, bool &grouping)
	 {
		int last = var.size() - 1;
		bool temp;
		if (var == "||")
		{
			type = 2;
			temp = true;
		}
		else if (var == "&&")
		{
			type = 1;
			temp = true;
		}
		else if (var.at(last) == ';')
		{
			string com = var.substr(0, last);
			arr.push_back(com);
			type = 0;
			temp = true;
		}
		else if (var == "#")
		{
			type = -1;
			temp = false;
		}
		else if (type == -1) { temp = false; }
		else if (var == "test" || var == "[" || var == "]") 
		{ 
			testCase = true;
			temp = false;
		}
		else if (var == "-e") 
		{ 
			testType = 0;
			temp = false;
		}
		else if (var == "-f") 
		{ 
			testType = 1;
			temp = false;
		}
		else if (var == "-d")
		{
			testType = 2;
			temp = false;
		}
		else if ((var == "(" )||(var == ")"))
		{
			temp = false;
			grouping = !grouping;
		}
		else 
		{
			arr.push_back(var);
			temp = false;
		}
	
		return temp;
	 }

};

class Command: public Items
{
	protected:
	 vector<string> command;
	public:
	 Command(){};
	 Command(vector<string> temp)
	 {
		command = temp;
	 }
	 void set_command(vector<string> temp)
	 {
		command = temp;
	 }
	 bool execute()
	 {
		//getting process ID
		pid_t pid = getpid();
		int status = 0;
		bool exeRes = true;
		
		int size = command.size();
		
		//starting to convert vector<string> to vector<char*> using const_char
		vector<char*> arr;
		for ( int i = 0; i < size; ++i)
		{
			arr.push_back(const_cast<char*>(command.at(i).c_str()));
		}
		arr.push_back(NULL);
		char** comExec = &arr[0];
		
		//splits process into two, one parent one child
		pid = fork();
		
		//if in child
		if (pid == 0)
		{
			execvp(comExec[0], comExec);
			perror("ERROR: EXECVP() FAILED UNEXPECTEDLY");
			exit(1);
		}
		//if in parent, wait for child
		else if (pid > 0)
		{
			wait(&status);
		}
		else 
		{ 
			cout << "ERROR: FORK WAS UNSUCCESSFUL\n";
			exit(127); 
		}

		if (status == 0){exeRes = true;}
		else {exeRes = false;}

		return exeRes;
	 }
};

class Connector: public Items
{
	protected:
	 Items* before;
	public:
	 Connector(){};
	 virtual int truesNeeded() = 0;
	 virtual void set_before(Items* para) = 0;
	 virtual bool execute() = 0;
	
};

//Semi-colon connector - ;
class Always: public Connector
{
	public:
	 Always(){};
	 Always(Items* para)
	 {
		before = para;
	 };
	 int truesNeeded() { return 0; }
	 void set_before(Items* para)
	 {
		before = para;
	 }
	 bool execute()
	 {
		bool perNext;
		perNext = before->execute();
		return perNext;
	 }
};

//And connector - &&
class Success: public Connector
{
	public:
	 Success(){};
	 Success(Items* para)
	 {
		before = para;
	 };
	 int truesNeeded() { return 2; }
	 void set_before(Items* para)
	 {
		before = para;
	 }
	 bool execute()
	 {
		bool perNext;
		perNext = before->execute();
		return perNext;
	 }
};

//Or connector - ||
class Failure: public Connector
{
	public:
	 Failure(){};
	 Failure(Items* para)
	 {
		before = para;
	 };
	 int truesNeeded() { return 1; }
	 void set_before(Items* para)
	 {
		before = para;
	 }
	 bool execute()
	 {
		bool perNext;
		perNext = before->execute();
		return !perNext;
	 }

};


//Composite
class CommandList: public Items
{
	protected:
	 vector<Items*> commLine; //For user input
	public:
	 CommandList(){};
	 
	 void add_com(Items* temp)
	 {
		commLine.push_back(temp); //Adds commands from user to vector
	 }

	 void parse_commLine(string commandLine)
	 {
		string temp;
		bool grouping = false;
		for (istringstream tString1(commandLine); tString1 >> temp; )//This is where the main parsing takes place
		{
			int type = 0;
			bool detectCon = false;
			bool testCase = false;
			int testType = 0;
			vector<string> arr;
			detectCon = interpret_line(arr, temp, type, testCase, testType, grouping);
			while (!detectCon && !tString1.eof())
			{
				tString1 >> temp;
				detectCon = interpret_line(arr, temp, type, testCase, testType, grouping);
			}

			Items* complCom;
				

			if (testCase)
			{
				cout << "This test case is type " << testType << endl;
				cout << arr.at(0) << endl;
				type = 3;
				// This is where a test object will be created. comlCom will be set equal to the object 
				// and it will be inserted to our commandline. In this case, I made type = 3, so my 
				// code won't try and make a full command with a complCom that is incomplete. In other circumstances,
				// this will not be needed. testType indicates what type of "test" it will be, 
				// "-e, -f, or -d", and corresponds to "0, 1, and 2" respectively.
			}
			else { complCom = new Command(arr); }
				

			if (type == 0 || type == -1) 
			{
				Items* complCon = new Always(complCom);
				this->add_com(complCon);
			}
			else if (type == 2) 
			{
				Items* complCon = new Failure(complCom);
				this->add_com(complCon);
			}
			else if (type == 1) 
			{
				Items* complCon = new Success(complCom);
				this->add_com(complCon);
			}
		}
		
		return;

	 }

	 bool execute()
	 {
		bool permission = true;
		for (int i = 0; i < commLine.size(); ++i)
		{
			if (permission)
			{
				permission = commLine.at(i)->execute();
			}
			else
			{
				permission = true;
			}
		}
		return permission;
	 }
};

/*class Grouping: public Items
{
	protected:
	 vector<Items*> comGroup;
	public:
	 Grouping(){};
	 void add_com(Items* temp)
	 {
		comGroup.push_back(temp);
	 }
	 
	 bool execute()
	 {
		int truesSoFar = 0;
		int truesNeeded = comGroup.at(0)->truesNeeded();
		bool permission = true;
		for (int i = 0; i < commGroup.size(); ++i)
		{
			if (permission)
			{
				permission = comGroup.at(i)->execute();
			}
			else { permission = true; }
			if (permission) { truesSoFar += 1; }
		}
		
		if (truesSoFar >= truesNeeded) { permission = true; }
		else { permission = false; }

		return permission;
	 }
};*/

int main()
{
	string commandLine = "start";

	while (commandLine != "exit")
	{
		cout << "$ ";
		getline(cin ,commandLine);

		if(commandLine != "exit")
		{
			bool awesome = true;
			CommandList fullLine;
			fullLine.parse_commLine(commandLine);
			awesome = fullLine.execute();
		}
	}
	return 0;
}

