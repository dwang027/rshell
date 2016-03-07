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
#include <sys/stat.h>
#pragma GCC disgnostic ignored "-Wwrite-strings"

using namespace std;

//base class
class Items
{
	public:
	 Items(){};
	 virtual bool execute()=0;
	 virtual int truesNeeded()=0;
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
		else if (var == "(" )
		{
			temp = false;
			grouping = true;
		}
		else if (var == ")")
		{
			temp = true;
			type = 0;
		}
			
		else 
		{
			arr.push_back(var);
			temp = false;
		}
	
		return temp;
	 }

};

class Test: public Items
{
    private:
     int flag;
     string path;
    
    public:
     Test(){};
     Test(int f, string p)
     {
         flag = f;
         path = p;
     }
     int truesNeeded() { return 0; } 
     bool execute()
     {
         struct stat checkFile;
         int solution = stat(path.c_str(), &checkFile);
         
         if (flag == 0)
         {
             return solution;
         }
         else if (flag == 1)
         {
             //short hand if/else magic syntax
             //if it is a regular file, return true, otherwise no bueno
             if (S_ISREG(checkFile.st_mode)){
                 cout << "(True)" << endl;
                 return true;
             }
             else{
                 cout << "(False)" << endl;
                 return false;
             }
             
         }
         else if (flag == 2)
         {
             if(S_ISDIR(checkFile.st_mode)){
                 cout << "(True)" << endl;
                 return true;
             }
             else{
                 cout << "(False)" << endl;
                 return false;
             }
         }
         else
         {
            cout << "WE HAVE A PROBLEM GUYS." << endl;    
         }
     }
};

class Command: public Items
{
	protected:
	 vector<string> command;
	public:
	 Command(){};
	 int truesNeeded() {return 0;}
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

class Grouping: public Items
{
	public:
	 vector<Items*> comGroup;
	 Grouping(){};
	 int truesNeeded() { return 0; }

	 void addGroupcom(Items* temp)
	 {
		cout << "here?" << endl;
		comGroup.push_back(temp);
	 }

	 int size() { return comGroup.size(); }

	 void parse(istringstream &inStream)
	 {
		cout << "parsing group" << endl;
		string temp;
		bool pendingGroup = false;
		Items* groupCom;
		while (!inStream.eof() )//This is where the main parsing takes place
		{
			Items* complCom;
			int type = 0;
			bool detectCon = false;
			bool testCase = false;
			int testType = 0;
			bool grouping = false;
			vector<string> arr;
			while (!detectCon && !inStream.eof() && !grouping)
			{
				inStream >> temp;
				detectCon = interpret_line(arr, temp, type, testCase, testType, grouping);
				cout << temp << endl;
			}
			if (grouping)
			{
				Grouping* groupUp;
				groupUp->parse(inStream);
				pendingGroup = true;
				type = 3;
				groupCom = groupUp;
			}


			if (testCase && !pendingGroup)
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
			else if (!testCase && !pendingGroup){ cout << "maybe here?" << endl; complCom = new Command(arr); }
			else if (grouping) { complCom = groupCom; }
			cout << "made it out?" << endl;


			if (type == 0 || type == -1) 
			{
				Items* complCon = new Always(complCom);
				this->addGroupcom(complCon);
				cout << "type 0" << endl;
			}
			else if (type == 2) 
			{
				Items* complCon = new Failure(complCom);
				this->addGroupcom(complCon);
				cout << "type 2" << endl;
			}
			else if (type == 1) 
			{
				cout << "poop" << endl;
				Items* complCon = new Success(complCom);
				cout << "pop" << endl;
				comGroup.push_back(complCon);
				cout << "type 1" << endl;
			}

			if (this->size() >= 2) { return;}
		}
		
		return;

	 }
	 
	 bool execute()
	 {
		int truesSoFar = 0;
		int truesNeeded = comGroup.at(0)->truesNeeded();
		bool permission = true;
		for (int i = 0; i < comGroup.size(); ++i)
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
};


//Composite
class CommandList: public Items
{
	protected:
	 vector<Items*> commLine; //For user input
	public:
	 CommandList(){};
	 int truesNeeded() { return 0;}
	 
	 void add_com(Items* temp)
	 {
		commLine.push_back(temp); //Adds commands from user to vector
	 }

	 void parse_commLine(string commandLine)
	 {
		cout << "parsing command line" << endl;
		string temp;
		bool pendingGroup = false;
		Items* groupCom;
		for (istringstream sStreamIn(commandLine); sStreamIn >> temp; )//This is where the main parsing takes place
		{
			Items* complCom;
			int type = 0;
			bool grouping = false;
			bool detectCon = false;
			bool testCase = false;
			int testType = 0;
			vector<string> arr;
			detectCon = interpret_line(arr, temp, type, testCase, testType, grouping);
			while (!detectCon && !sStreamIn.eof() && !grouping)
			{
				sStreamIn >> temp;
				detectCon = interpret_line(arr, temp, type, testCase, testType, grouping);
				cout << temp << endl;
			}
			
			if (grouping)
			{
				Grouping* groupUp;
				groupUp->parse(sStreamIn);
				pendingGroup = true;
				type = 3;
				groupCom = groupUp;
			}

			if (testCase && !pendingGroup)
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
			else if (!testCase && !pendingGroup){ complCom = new Command(arr); }
			else if (pendingGroup) { complCom = groupCom; }
				

			if (type == 0 || type == -1) 
			{
				Items* complCon = new Always(complCom);
				this->add_com(complCon);
				cout << "type 0" << endl;
			}
			else if (type == 2) 
			{
				Items* complCon = new Failure(complCom);
				this->add_com(complCon);
				cout << "type 2" << endl;
			}
			else if (type == 1) 
			{
				Items* complCon = new Success(complCom);
				this->add_com(complCon);
				cout << "type 1" << endl;
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

