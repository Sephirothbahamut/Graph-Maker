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

int main()
	{
	graph g;

	g.read_file("test.txt", graph::ADJ_VECTOR);
	//run dfa algorithm
	//dfa(root);

	cout << "=========================" << endl;
	_getch();
	{
	window_mode w(&g);
	w.run();
	//w.run();
	}
	cout << "=========================" << endl;
	_getch();
	return 0;
	}

