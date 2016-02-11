//Diana Wang - Huber Ochoa

#include <iostream>
#include <string>
#include <cstdlib>

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
	 string commandN;
	 string arguementN;
	public:
	 Command()
	 {
		commandN = "none";
		arguementN = "none";
	 };
	 Command(string comName, string argName)
	 {
		commandN = comName;
		arguementN = argName;
	 }
	 void set_command(string comName)
	 {
		commandN = comName;
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
	return 0;
}

