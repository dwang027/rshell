#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"

using namespace std;

int main()
{
	pid_t pid = getpid();
	int status;
	
	pid = fork();

	char* args[] = {"ls", "-a", 0};
	
	if (pid == 0)
	{
		
		execvp(args[0], args);
		_exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		wait(&status);
	}
	else
	{
		cout << "ERROR: FORK WAS UNSUCCESSFUL\n";
	}


	cout << "Safely returned to main()\n";

	return 0;
}

	
