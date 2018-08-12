#pragma once

#include <vector>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
using std::cout;
using std::cin;
using std::endl;

//#define DEBUG_GRAPH_BUILDING
//#define DEBUG_READ_FILE

#ifdef DEBUG_GRAPH_BUILDING
using std::endl;
#define debug_msg \
	for(unsigned int i = 0; i<debug_depth; i++) \
		{ \
		std::cout << "    "; \
		} \
	std::cout
#endif

class graph
	{
	public:

		enum arc_style_type { triangular, linear };
		arc_style_type arc_style = linear;

		typedef unsigned short int node_id;
		typedef unsigned short int arc_id;

		class node;
		class arc;
		#include "node.h"
		#include "arc.h"

		friend node;
		friend arc;

	private:
		std::list<node*> list_node;
		std::list<arc*> list_arc;

		// User variables
		std::unordered_map<std::string, double> shared_vars_node;
		std::unordered_map<std::string, double> shared_vars_arc;
		// //// //


	public:
		graph();
		~graph();

		node* add_node();
		void remove_node(node* n);

		arc* add_arc(node* from, node* to, bool directed = false, bool merge = true);
		void remove_arc(arc* a);


		//iterate
		const std::list<node*>& get_nodes();
		const std::list<arc*>& get_arcs();

		// User variables
		bool shared_nodes_variable_chk(std::string var_name);
		void shared_nodes_variable_set(std::string var_name, double default_value = 0);
		void shared_nodes_variable_del(std::string var_name);
		bool shared_arcs_variable_chk(std::string var_name);
		void shared_arcs_variable_set(std::string var_name, double default_value = 0);
		void shared_arcs_variable_del(std::string var_name);
		// //// //

		//read file
		enum filetype
			{
			ADJ_VECTOR,
			ADJ_MATRIX
			};

		bool read_file(std::string fname, filetype ftype);

	};