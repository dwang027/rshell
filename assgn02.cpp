//Diana Wang - Huber Ochoa

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/type.h>
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
	 { /*finish this function, Huber*/ }
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

};



int main()
{
	string commandLine;
	while (commandLine != "exit")
	{
		
	}
	return 0;
}

