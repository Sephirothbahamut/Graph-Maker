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

#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Dead " << endl;
	debug_depth--;
#endif
	}


void graph::node::AddRef(){} void graph::node::ReleaseRef(){}

std::vector<graph::node*> graph::node::adj_vector()
	{
	std::vector<graph::node*> v; v.reserve(list_arc_from.size());
	for (auto a : list_arc_from) { v.push_back(a->get_to()); }
	return v;
	}

std::vector<graph::arc*> graph::node::adj_arc_vector()
	{
	return std::vector<graph::arc*>(list_arc_from.begin(), list_arc_from.end());
	}

void graph::node::adj_reset() { adj_it = list_arc_from.begin(); iteration_invalidated = false; }
graph::node * graph::node::adj_next()
	{
	if (iteration_invalidated) { return nullptr; }
	else if (adj_it == list_arc_from.end()) { return nullptr; }
	else
		{
		graph::arc* a = *adj_it;
		adj_it++;
		if (a->get_to() != this) { return a->get_to(); }
		else { return a->get_from(); }
		}
	}
graph::arc * graph::node::adj_arc()
	{
	if (iteration_invalidated) { return nullptr; }
	else if (adj_it == list_arc_from.end()) { return nullptr; }
	else { return *(adj_it++); }
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
	set_position(sf::Vector2f(x, y));
	}

void graph::node::set_position(sf::Vector2f pos)
	{
	sprite.setPosition(pos);
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

#include <sstream>
void graph::node::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
	target.draw(sprite);
	}

#include <sstream>
std::string graph::node::id()
	{
	std::stringstream ss; ss << "N-" << this;
	return std::string(ss.str());
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