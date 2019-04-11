// GraphMaker.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <conio.h>

//import graph
#include "graph.h"
#include "window_mode.h"
using std::cout;
using std::cin;
using std::endl;

void dfa(graph::node* n)
	{
	if (!(n->shr_get("visited")))
		{
		n->shr_set("visited", true);
		auto adj = n->get_adj_node();
		for (auto ad : adj)
			{
			dfa(ad);
			}
		}
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

int main()
	{
	graph g;

	g.read_file("test.txt", graph::ADJ_VECTOR);
	//run dfa algorithm
	//dfa(root);

	cout << "=========================" << endl;
	char c;
	while (true)
		{
		cin >> c;
		switch (c)
			{
			case 'q': goto OUT;
			case 'n': g.add_node(); break;
			case 't': test(g); break;
			case 'w': window_mode w(&g); w.run();  break;
			}
		}
	OUT:

	/*_getch();
	{
	window_mode w(&g);
	w.run();
	}*/
	cout << "=========================" << endl;
	_getch();
	return 0;
	}

