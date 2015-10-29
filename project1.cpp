#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <stdlib.h>

using namespace std;

main()
{
	int pfd[2];
	int pid;

	int cow = 10;	//Variable to initialize

	char buffer[16];

	if(pipe(pfd) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid = fork();	//0 = child, else its the parent



	// If the current process is the child...
	if (pid == 0)
	{
		cout << "Before writing the value " << &cow << endl;

		cout << "Child cow: " << cow << " " << endl;

		if(write(pfd[1], "hi there", 9) < 0)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}

		cow = 1000;

		cout << "After writing the value " << &cow << endl;
		cout << "Child cow: " << cow << " " << endl;

		return 0;
	}

	// Else if it's the parent...
	else
	{
		waitpid(pid, NULL, 0);

		cout << "Before writing the value " << &cow << endl;
		cout << "Parent cow: " << cow << " " << endl;

		if(read(pfd[0], buffer, 16) < 0)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}

		else
			cout << buffer << endl;

		cout << "After writing the value " << &cow << endl;
		cout << "Parent cow: " << cow << " " << endl;
	}

	// Part 2 execve
	char *argv[] = {"/bin/ls",0};
	char *envp[] = {0};

	pid = fork();	//0 = child, else its the parent

	if(pid == 0)
	{
		if(execve(argv[0], &argv[0], envp) < 0)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}