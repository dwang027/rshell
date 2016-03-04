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
	 virtual void set_before(Items* para) = 0;
	 virtual bool execute() = 0;
	
};

//semi-colon connector - ;
class Always: public Connector
{
	public:
	 Always(){};
	 Always(Items* para)
	 {
		before = para;
	 };
	 void set_before(Items* para)
	 {
		before = para;
	 }
	 bool execute()
	 {
		bool perNext;
		perNext = before->execute();
		return true;
	 }
};

//and connector - &&
class Success: public Connector
{
	public:
	 Success(){};
	 Success(Items* para)
	 {
		before = para;
	 };
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

//or connector - ||
class Failure: public Connector
{
	public:
	 Failure(){};
	 Failure(Items* para)
	 {
		before = para;
	 };
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
//composite
class CommandList: public Items
{
	protected:
	 vector<Items*> commLine; //for user input
	public:
	 CommandList(){};
	 void add_com(Items* temp)
	 {
		commLine.push_back(temp); //adds commands from user to vector
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
	 }
};

bool add_com(vector<string> &arr, string var, int &type)
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
	else if (type == -1)
	{
		temp = false;
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
			bool awesome = true;
			CommandList fullLine;
			string temp;
			for (istringstream tString1(commandLine); tString1 >> temp; )
			{
				int type = 0;
				bool detectCon = false;
				vector<string> arr;
				detectCon = add_com(arr, temp, type);
				while (!detectCon && !tString1.eof())
				{
					tString1 >> temp;
					detectCon = add_com(arr, temp, type);
				}
				Items* complCom = new Command(arr);
				if (type == 0 || type == -1) 
				{
					Items* complCon = new Always(complCom);
					fullLine.add_com(complCon);
				}
				else if (type == 2) 
				{
					Items* complCon = new Failure(complCom);
					fullLine.add_com(complCon);
				}
				else if (type == 1) 
				{
					Items* complCon = new Success(complCom);
					fullLine.add_com(complCon);
				}
				
			}
			awesome = fullLine.execute();
		}
	}
	return 0;
}

