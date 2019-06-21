#include "stdafx.h"
#include "arc.h"

#ifdef DEBUG_GRAPH_BUILDING
extern unsigned int debug_depth;
#define ME "[A-" << this << " (" << from << ", "<< to << ")]: "
#endif

void graph::arc::update_position()
	{
	float fx = from->sprite.getPosition().x;
	float fy = from->sprite.getPosition().y;
	float fr = from->sprite.getRadius();
	float tx = to->sprite.getPosition().x;
	float ty = to->sprite.getPosition().y;
	float tr = to->sprite.getRadius();

	auto th = std::atan2(ty - fy, tx - fx); //angle of to with from as origin

	const float PI = 3.141592654f;

	if (owner->arc_style == triangular)
		{
		float angle_variation = 15/*deg*/ * 2 * PI / 360;
		//from-to
		connection[0].position.x = fx + (fr * cos(th + angle_variation));
		connection[0].position.y = fy + (fr * sin(th + angle_variation));
		connection[1].position.x = fx + (fr * cos(th - angle_variation));
		connection[1].position.y = fy + (fr * sin(th - angle_variation));
		connection[2].position.x = tx - (tr * cos(th));
		connection[2].position.y = ty - (tr * sin(th));
		if (!directed)
			{
			//to-from
			connection[3].position.x = tx - (tr * cos(th + angle_variation));
			connection[3].position.y = ty - (tr * sin(th + angle_variation));
			connection[4].position.x = tx - (tr * cos(th - angle_variation));
			connection[4].position.y = ty - (tr * sin(th - angle_variation));
			connection[5].position.x = fx + (fr * cos(th));
			connection[5].position.y = fy + (fr * sin(th));
			}
		}
	else
		{
		float angle_variation = 90/*deg*/ * 2 * PI / 360;
		float size = 3;

		//UP SIDE
		connection[0].position.x = fx + (fr * cos(th));
		connection[0].position.y = fy + (fr * sin(th));
		connection[1].position.x = connection[0].position.x + (size * cos(th + angle_variation));
		connection[1].position.y = connection[0].position.y + (size * sin(th + angle_variation));
		connection[3].position.x = tx - (tr * cos(th));
		connection[3].position.y = ty - (tr * sin(th));
		connection[2].position.x = connection[3].position.x + (size * cos(th + angle_variation));
		connection[2].position.y = connection[3].position.y + (size * sin(th + angle_variation));

		//DOWN SIDE

		connection[4].position.x = fx + (fr * cos(th));
		connection[4].position.y = fy + (fr * sin(th));
		connection[5].position.x = connection[4].position.x - (size * cos(th + angle_variation));
		connection[5].position.y = connection[4].position.y - (size * sin(th + angle_variation));
		connection[7].position.x = tx - (tr * cos(th));
		connection[7].position.y = ty - (tr * sin(th));
		connection[6].position.x = connection[7].position.x - (size * cos(th + angle_variation));
		connection[6].position.y = connection[7].position.y - (size * sin(th + angle_variation));
		}

	float avg_x = (fx + tx) / 2;
	float avg_y = (fy + ty) / 2;
	sprite.setPosition(avg_x, avg_y);
	}

void graph::arc::update_color()
	{
	if (owner->arc_style == triangular)
		{
		connection[0].color = connection[1].color = from->sprite.getOutlineColor();
		connection[2].color = to->sprite.getOutlineColor();
		connection[3].color = connection[4].color = to->sprite.getOutlineColor();
		connection[5].color = from->sprite.getOutlineColor();
		}
	else
		{
		connection[0].color = connection[1].color = from->sprite.getOutlineColor();
		connection[1].color.a = 0;
		connection[2].color = connection[3].color = to->sprite.getOutlineColor();
		connection[2].color.a = 0;

		connection[4].color = connection[5].color = from->sprite.getOutlineColor();
		connection[5].color.a = 0;
		connection[6].color = connection[7].color = to->sprite.getOutlineColor();
		connection[6].color.a = 0;
		}


	sprite.setFillColor(sf::Color(255, 255, 255, 180));
	sprite.setOutlineColor(sf::Color(255, 255, 255, 255));
	sprite.setOutlineThickness(2);

	}

graph::arc::arc(graph * owner, node* from, node* to, bool directed)
	{
	this->owner = owner;
	owner->list_arc.push_back(this);

	this->from = from;
	this->to = to;
	this->directed = directed;

#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Borning" << endl;
#endif

	if (directed)
		{
		from->add_arc_from(this);
		to->add_arc_to(this);
		}
	else
		{
		from->add_arc_from(this);
		from->add_arc_to(this);
		to->add_arc_from(this);
		to->add_arc_to(this);
		}

	set_style(owner->arc_style);

	sprite.setSize({8, 8});
	sprite.setOrigin({ 4, 4 });

	update_position();
	update_color();

	// User variables
	for (auto i : owner->shared_vars_arc)
		{
		shr_set(i.first, i.second);
		}

	//style
	set_style(owner->arc_style);


#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Born" << endl;
	debug_depth--;
#endif
	}


graph::arc::~arc()
	{
#ifdef DEBUG_GRAPH_BUILDING
	debug_depth++;
	debug_msg << ME << "Dying" << endl;
#endif

	if (directed)
		{
		from->remove_arc_from(this);
		to->remove_arc_to(this);
		}
	else
		{
		from->remove_arc_from(this);
		from->remove_arc_to(this);
		to->remove_arc_from(this);
		to->remove_arc_to(this);
		}

#ifdef DEBUG_GRAPH_BUILDING
	debug_msg << ME << "Dead" << endl;
	debug_depth--;
#endif
	}

void graph::arc::AddRef() {} void graph::arc::ReleaseRef() {}

void graph::arc::set_directed(bool directed)
	{
	if (this->directed != directed)
		{
		this->directed = directed;
		if (directed)
			{
			from->remove_arc_to(this);
			to->remove_arc_from(this);
			connection[1].color = sf::Color::Red;
			}
		else
			{
			from->add_arc_to(this);
			to->add_arc_from(this);
			connection[1].color = sf::Color::Green;
			}
		}
	}

void graph::arc::swap()
	{
	if (directed)
		{
		from->remove_arc_from(this);
		to->remove_arc_to(this);
		node* tmp = from;
		from = to;
		to = tmp;
		from->add_arc_from(this);
		to->add_arc_to(this);

		auto s = connection[0];
		connection[0] = connection[1];
		connection[1] = s;
		}
	}

graph::node * graph::arc::get_from()
	{
	return from;
	}
graph::node * graph::arc::get_to()
	{
	return to;
	}

bool graph::arc::get_directed()
	{
	return directed;
	}

void graph::arc::set_style(arc_style_type style)
	{
	switch (style)
		{
		case arc_style_type::triangular:
			connection.setPrimitiveType(sf::Triangles);
			connection.resize(6);
			break;
		case arc_style_type::linear:
			connection.setPrimitiveType(sf::Quads);
			connection.resize(8);
			break;
		}
	update_position();
	update_color();
	}

void graph::arc::set_color(sf::Color fill, sf::Color outline)
	{
	sprite.setFillColor(fill);
	sprite.setOutlineColor(outline);
	}

void graph::arc::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
	target.draw(connection);
	target.draw(sprite);
	}

// User variables

std::string graph::arc::id()
	{
	std::stringstream ss; ss << "S-" << this;
	return std::string(ss.str());
	}

//personal
bool graph::arc::chk(std::string name)
	{
	return(vars.find(name) != vars.end());
	}
void graph::arc::set(std::string name, double val)
	{
	vars[name] = val;
	}
double graph::arc::get(std::string name)
	{
	return(vars[name]);
	}
void graph::arc::del(std::string name)
	{
	vars.erase(name);
	}

//shared
bool graph::arc::shr_chk(std::string name)
	{
	return(shared_vars.find(name) != vars.end());
	}
void graph::arc::shr_set(std::string name, double val)
	{
	shared_vars[name] = val;
	}
double graph::arc::shr_get(std::string name)
	{
	return(shared_vars[name]);
	}
void graph::arc::shr_del(std::string name)
	{
	shared_vars.erase(name);
	}

// //// //