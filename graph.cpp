#include "stdafx.h"
#include "graph.h"


#ifdef DEBUG_GRAPH_BUILDING
unsigned int debug_depth = 0;
#define ME "[GRAPH]: "
#endif

void graph::AddRef() {} void graph::ReleaseRef() {}

graph::node * graph::node_next()
	{
	if (node_it == list_node.end()) { return nullptr; }
	else { return *node_it++; }
	}

void graph::node_reset() { node_it = list_node.begin(); }


graph::graph()
	{}


graph::~graph()
	{
	while (!list_arc.empty())
		{
		remove_arc(list_arc.back());
		}
	while (!list_node.empty())
		{
		remove_node(list_node.back());
		}
	}

void graph::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
	for (auto a : list_arc) { target.draw(*a); }
	for (auto n : list_node) { target.draw(*n); }
	}

graph::node* graph::add_node()
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Creating new node" << endl;
#endif
	graph::node* node = new graph::node(this);
#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Node created" << endl;
	debug_depth--;
#endif
	return node;
	}

bool graph::remove_node(node* n)
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Removing node " << n << endl;
#endif
	auto found = std::find(list_node.begin(), list_node.end(), n);
	if (found != list_node.end())
		{
		list_node.erase(found);
		delete n;
#ifdef DEBUG_GRAPH_BUILDING
		debug_msg << ME << "Node removed" << endl;
		debug_depth--;
#endif
		return true;
		}
#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Node not found" << endl;
	debug_depth--;
#endif
	return false;
	}

graph::arc* graph::add_arc(node* from, node* to, bool directed, bool merge)
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Creating new " << (directed ? "directed " : "") << "arc between " << from << " and " << to << endl;
#endif
	graph::arc* arc;

	if (merge)
		{
		//already exists
		for (graph::arc* a : from->list_arc_from)
			{
			if (a->to == to)
				{
#ifdef DEBUG_GRAPH_BUILDING
				debug_msg << ME << "Arc already exists" << endl;
				debug_depth--;
#endif
				return(a);
				}
			}
		//exists opposite direction
		for (graph::arc* a : from->list_arc_to)
			{
			if (a->from == to)
				{
#ifdef DEBUG_GRAPH_BUILDING
				debug_msg << ME << "Arc turned bidirectional" << endl;
				debug_depth--;
#endif
				a->set_directed(false);
				return(a);
				}
			}
		//not exists
		}
#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Arc created" << endl;
	debug_depth--;
#endif
	return(new graph::arc(this, from, to, directed));
	}

bool graph::remove_arc(arc* a)
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Removing arc " << a << endl;
#endif
	auto found = std::find(list_arc.begin(), list_arc.end(), a);
	if (found != list_arc.end())
		{
		list_arc.erase(found);
		delete a;
#ifdef DEBUG_GRAPH_BUILDING
		debug_msg << ME << "Arc removed" << endl;
		debug_depth--;
#endif
		return true;
		}
#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Arc not found" << endl;
	debug_depth--;
#endif
	return false;
	}

const std::list<graph::node*>& graph::get_nodes()
	{
	return(list_node);
	}
const std::list<graph::arc*>& graph::get_arcs()
	{
	return(list_arc);
	}

// User variables

//node
bool graph::shared_nodes_variable_chk(std::string var_name)
	{
	return(shared_vars_node.find(var_name) != shared_vars_node.end());
	}

void graph::shared_nodes_variable_set(std::string var_name, double default_value)
	{
	for (node* n : list_node)
		{
		n->shr_set(var_name, default_value);
		}
	shared_vars_node[var_name] = default_value;
	}

void graph::shared_nodes_variable_del(std::string var_name)
	{
	for (node* n : list_node)
		{
		n->shr_del(var_name);
		}
	shared_vars_node.erase(var_name);
	}

//arc
bool graph::shared_arcs_variable_chk(std::string var_name)
	{
	return(shared_vars_arc.find(var_name) != shared_vars_arc.end());
	}

void graph::shared_arcs_variable_set(std::string var_name, double default_value)
	{
	for (arc* a : list_arc)
		{
		a->shr_set(var_name, default_value);
		}
	shared_vars_arc[var_name] = default_value;
	}

void graph::shared_arcs_variable_del(std::string var_name)
	{
	for (arc* a : list_arc)
		{
		a->shr_del(var_name);
		}
	shared_vars_arc.erase(var_name);
	}

bool graph::read_file(std::string fname, filetype ftype)
	{
	std::ifstream in;
	in.open(fname, std::ifstream::in);
	char c;
	size_t directed;
	in >> directed;
#ifdef DEBUG_READ_FILE
	switch (directed)
		{
		case 0: cout << "non directed graph" << endl; break;
		case 1: cout << "directed graph" << endl; break;
		case 2: cout << "mixed graph" << endl; break;
		default: cout << "unexpected value" << endl;
		}
#endif
	// SHARED VARIABLES
	size_t variables_amount_node;
	in >> variables_amount_node;

#ifdef DEBUG_READ_FILE
	cout << "there are " << variables_amount_node << " shared node variables" << endl;
#endif
	// nodes
	for (size_t i = 0; i < variables_amount_node; i++)
		{
		std::string name;
		double def;
		std::getline(in, name);
		std::getline(in, name);
		in >> def;
#ifdef DEBUG_READ_FILE
		cout << " [" << name << "]: " << def << endl;
#endif
		shared_nodes_variable_set(name, def);
		}
	//arcs
	size_t variables_amount_arc;
	in >> variables_amount_arc;

#ifdef DEBUG_READ_FILE
	cout << "there are " << variables_amount_arc << " shared arc variables" << endl;
#endif
	for (size_t i = 0; i < variables_amount_arc; i++)
		{
		std::string name;
		double def;
		std::getline(in, name);
		std::getline(in, name);
		in >> def;
#ifdef DEBUG_READ_FILE
		cout << " [" << name << "]: " << def << endl;
#endif
		shared_nodes_variable_set(name, def);
		}

	// GRAPH
	// nodes
	size_t nodes_n;
	in >> nodes_n;
#ifdef DEBUG_READ_FILE
	cout << endl << "there are " << nodes_n << " nodes" << endl;
#endif
	std::vector<node*> nodes;
	for (size_t i = 0; i < nodes_n; i++)
		{
		nodes.push_back(add_node());
		}

	for (size_t i = 0; i < nodes_n; i++)
		{
		char tmp;
		size_t n;
		in >> tmp >> n >> tmp;
#ifdef DEBUG_READ_FILE
		if (tmp != ']' || n != i)
			{
			cout << "error" << endl;
			}
#endif

		float x, y;
		in >> x >> y;
#ifdef DEBUG_READ_FILE
		cout << "NODE " << i << " (" << x << ", " << y << ")";
#endif
		nodes[i]->set_position(x, y);
		size_t shared_amount;
		in >> shared_amount;
		for (size_t j = 0; j < shared_amount; j++)
			{
			std::string name;
			double def;
			std::getline(in, name);
			std::getline(in, name);
			in >> def;
#ifdef DEBUG_READ_FILE
			cout << " [" << name << "]: " << def << endl;
#endif
			nodes[i]->shr_set(name, def);
			}

		size_t neighbours_amount;
		in >> neighbours_amount;
#ifdef DEBUG_READ_FILE
		cout << " has " << neighbours_amount << " neighbours" << endl;
#endif

		for (size_t j = 0; j < neighbours_amount; j++)
			{
			size_t neighbour_index;
			bool directed_p;
			in >> neighbour_index;
			if (directed == 2)
				{
				char c;
				in >> c;
				directed_p = (c == 'd');
				}
			else
				{
				directed_p = directed;
				}
#ifdef DEBUG_READ_FILE
			cout << "    - " << neighbour_index << endl;
#endif
			add_arc(nodes[i], nodes[neighbour_index], directed);
			}
		}


	return(true);
	}

// //// //