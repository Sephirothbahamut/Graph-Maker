#include "stdafx.h"
#include "node.h"

#ifdef DEBUG_GRAPH_BUILDING
extern unsigned int debug_depth;
#define ME "[N-" << this << "]: "
#endif

void graph::node::add_arc_from(arc* a)
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Adding arc from here (" << a->from << ", " << a->to << ")" << endl;
	debug_depth--;
#endif
	list_arc_from.insert(a);
	iteration_invalidated = true;
	}

void graph::node::add_arc_to(arc* a)
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Adding arc to   here (" << a->from << ", " << a->to << ")" << endl;
	debug_depth--;
#endif
	list_arc_to.insert(a);
	iteration_invalidated = true;
	}

void graph::node::remove_arc_from(arc* a)
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Removing arc (" << a->from << ", " << a->to << ")" << endl;
	debug_depth--;
#endif
	list_arc_from.erase(a);
	iteration_invalidated = true;
	}

void graph::node::remove_arc_to(arc* a)
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Removing arc (" << a->from << ", " << a->to << ")" << endl;
	debug_depth--;
#endif
	list_arc_to.erase(a);
	iteration_invalidated = true;
	}

graph::node::node(graph * owner)
	{
	this->owner = owner;
	owner->list_node.push_back(this);

	set_size(16);
	sprite.setFillColor(sf::Color(255, 255, 255, 180));
	sprite.setOutlineColor(sf::Color(255, 255, 255, 255));
	sprite.setOutlineThickness(2);

	// User variables
	for (auto i : owner->shared_vars_node)
		{
		shr_set(i.first, i.second);
		}


#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Born " << endl;
	debug_depth--;
#endif
	}

graph::node::~node()
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Dying" << endl;
#endif
	while (!list_arc_from.empty())
		{
#ifdef DEBUG_GRAPH_BUILDING
		debug_msg << ME << "Killing arc " << *(list_arc_from.begin()) << endl;
#endif
		owner->remove_arc(*list_arc_from.begin());
		}
	while (!list_arc_to.empty())
		{
#ifdef DEBUG_GRAPH_BUILDING
		debug_msg << ME << "Killing arc " << *(list_arc_to.begin()) << endl;
#endif
		owner->remove_arc(*list_arc_to.begin());
		}
	owner->list_node.remove(this);

#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Died " << this << endl;
	debug_depth--;
#endif
	}


std::vector<graph::node*> graph::node::get_adj_node()
	{
	if (iteration_invalidated)
		{
		iteration_vector_node.clear();
		for (arc* a : list_arc_from)
			{
			iteration_vector_node.push_back(a->to);
			}
		}
	return(iteration_vector_node);
	}

std::vector<graph::arc*> graph::node::get_adj_arc()
	{
	if (iteration_invalidated)
		{
		iteration_vector_arc.clear();
		for (arc* a : list_arc_from)
			{
			iteration_vector_arc.push_back(a);
			}
		}
	return(iteration_vector_arc);
	}

void graph::node::set_color(sf::Color fill, sf::Color outline)
	{
	sprite.setFillColor(fill);
	sprite.setOutlineColor(outline);
	for (auto a : list_arc_from)
		{
		a->update_color();
		}
	for (auto a : list_arc_to)
		{
		a->update_color();
		}
	}

void graph::node::set_position(float x, float y)
	{
	sprite.setPosition(x, y);
	for (auto a : list_arc_from)
		{
		a->update_position();
		}
	for (auto a : list_arc_to)
		{
		a->update_position();
		}
	}

void graph::node::set_size(float size)
	{
	sprite.setRadius(size);
	sprite.setOrigin(size, size);
	sprite.setPointCount(abs(size));
	for (auto a : list_arc_from)
		{
		a->update_position();
		}
	for (auto a : list_arc_to)
		{
		a->update_position();
		}
	}

void graph::node::change_size(float change)
	{
	set_size(sprite.getRadius() + change);
	}

//personal
bool graph::node::chk(std::string name)
	{
	return(vars.find(name) != vars.end());
	}
void graph::node::set(std::string name, double val)
	{
	vars[name] = val;
	}
double graph::node::get(std::string name)
	{
	return(vars[name]);
	}
void graph::node::del(std::string name)
	{
	vars.erase(name);
	}

//shared
bool graph::node::shr_chk(std::string name)
	{
	return(shared_vars.find(name) != vars.end());
	}
void graph::node::shr_set(std::string name, double val)
	{
	shared_vars[name] = val;
	}
double graph::node::shr_get(std::string name)
	{
	return(shared_vars[name]);
	}
void graph::node::shr_del(std::string name)
	{
	shared_vars.erase(name);
	}

// //// //