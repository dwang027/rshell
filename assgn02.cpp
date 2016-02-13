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

class Items
{
	public:
	 Items(){};
	 virtual bool execute() = 0;
};

class Command: public Items
{
	protected:
	 vector<string> command;
	public:
	 Command(){};
	 void set_command(vector<string> temp)
	 {
		command = temp;
	 }
	 bool execute()
	 {

		pid_t pid = getpid();
		int status = 0;
		bool exeRes = true;

		int size = command.size();
		vector<char*> arr;
		for ( int i = 0; i < size; ++i)
		{
			arr.push_back(const_cast<char*>(command.at(i).c_str()));
		}
		arr.push_back(NULL);
		char** comExec = &arr[0];

		pid = fork();
		
		if (pid == 0)
		{
			execvp(comExec[0], comExec);
			perror("ERROR: EXECVP() FAILED UNEXPECTEDLY");
			exit(1);
		}
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
		//execute Before Item, then assess whether next item should
		//execute. return true for yes, false for no
	 }
};

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
		perNext = !perNext;
		return perNext;
	 }

};

class CommandList: public Items
{
	protected:
	 vector<Items*> commLine;
	public:
	 CommandList(){};
	 void add_com(Items* temp)
	 {
		commLine.push_back(temp);
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
			if (!permission)
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
				Items* complCom = new Command();
				Items* complCon;
				complCom->set_command(arr);
				if (type == 0 || type == -1) {complCon = new Always();}
				else if (type == 2) {complCon = new Failure();}
				else if (type == 1) {complCon = new Success();}
				complCon->set_before(complCom);

			}
		}
	}
	return 0;
}

