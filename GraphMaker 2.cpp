// GraphMaker.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <thread>

//import graph
#include "graph.h"
#include "window_mode.h"
using std::cout;
using std::cin;
using std::endl;


void dfa(graph::node* n)
	{
	std::cout << "entering " + n->id() << std::endl;
	if (n->shr_get("visited") == 0)
		{
		n->shr_set("visited", 1);
		n->adj_reset(); //resets the adj internal iterator
		graph::node* next = n->adj_next();
		while (next != nullptr)
			{
			dfa(next);
			next = n->adj_next();
			}
		}
	std::cout << "exiting " + n->id() << std::endl;
	}
void dfa_wrapper(graph &g)
	{
	g.shared_nodes_variable_set("visited", 0);
	g.node_reset();
	dfa(g.node_next());
	}

void test(graph& g) 
	{
	for (int j = 0; j < 5; j++) {
		std::set<graph::node*> sel;
		for (int i = 0; i < 1000; i++)
			{
			sel.insert(g.add_node());
			}
		/*for (auto n : sel)
			{
			g.remove_node(n);
			}*/
		}
	}

#include "script_console.h"

int main()
	{
	

	graph g;

	//g.read_file("test.txt", graph::ADJ_VECTOR);

	window_mode win(&g);

	std::thread winthread(&window_mode::run, &win);
	//run dfa algorithm
	//dfa(root);

	cout << "=========================" << endl;
	std::string input;
	while (true)
		{
		std::cin >> input;
		/*if (input == "w" || input == "window_mode" || input == "window mode")
			{
			window_mode win(&g);
			win.run();
			}
		else*/ if (input == "s" || input == "script")
			{
			bool success;
			script_console c(success, &g);
			if (!success)
				{
				std::cout << "Failed at creating the console." << std::endl;
				return 0;
				}
			c.open();
			}
		else if (input == "sf" || input == "script_file")
			{
			bool success;
			script_console c(success, &g);
			if (!success)
				{
				std::cout << "Failed at creating the console." << std::endl;
				return 0;
				}

			std::cin >> input;
			c.load(input);
			}
		else if (input == "t") { dfa_wrapper(g); }
		else if (input == "h" || input == "help")
			{
			std::cout << "Commands list:\n"
				<< "'w' or 'window_mode' to open graphics interface.\n"
				<< "'s' or 'script' to write a script.\n"
				<< "'h' or 'help' to show this message.\n"
				<< "'hs' or 'help script' to show help about scripting.\n"
				<< "'q', 'x', 'quit' or 'exit' to close the program." << std::endl;
			}
		else if (input == "hs" || input == "help script")
			{
			std::cout << "Help about scripting:\n"
				<< "A script must have a 'void script(){}' function from which execution will start.\n"
				<< "Other funcitons can be declared in the global scope.\n"
				<< "Anything declared in the global scope will be available across multiple script calls.\n"
				<< "To end a script type '$$' in the last line." << std::endl;
			}
		else if (input == "q" || input == "x" || input == "quit" || input == "exit")
			{
			goto OUT;
			}
		else { std::cout << "Write 'h' or 'help' for a list of commands." << std::endl; }
		}
	OUT:

	cout << "=========================" << endl;
	_getch();
	return 0;
	}

