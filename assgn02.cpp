//Diana Wang - Huber Ochoa

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Items
{
	public:
	 Items(){};
	 virtual void execute() = 0;
};

class Command: public Items
{
	private:
	 string commandN;
	 string arguementN;
	public:
	 Command()
	 {
		commandN = "none";
		arguementN = "none";
	 };
	 Command(string comName; string argName)
	 {
		commandN = comName;
		arguementN = argName;
	 }
	 void set_aregument
	 void execute()
	 { /*finish this function, Huber*/ }
};

class Connector: public Items
{
	
};

int main()
{
	return 0;
}

