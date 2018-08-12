#include "stdafx.h"
#include <SFML\Graphics.hpp>

#include "window_mode.h"

// =================================================== SELECTION

void window_mode::select(graph::node * n)
	{
	selected_nodes.insert(n);
	n->selected = true;
	n->set_color(color_node_selected, color_node_selected_outline);
	}

void window_mode::deselect(graph::node * n)
	{
	selected_nodes.erase(n);
	n->selected = false;
	n->set_color(color_node_idle, color_node_idle_outline);
	}

void window_mode::deselect_all()
	{
	while (!selected_nodes.empty())
		{
		deselect(*selected_nodes.begin());
		}
	}

void window_mode::set_mode(mode_type mode)
	{
	this->mode = mode;
	switch (mode)
		{
		case idle:
			action_mouse_pressed = &window_mode::action_mouse_pressed_idle;
			action_mouse_moved = &window_mode::action_mouse_moved_idle;
			action_mouse_released = &window_mode::action_none;
			action_draw = &window_mode::action_draw_none;
			break;

		case selecting:
			action_mouse_pressed = &window_mode::action_none;
			action_mouse_moved = &window_mode::action_mouse_moved_selecting;
			action_mouse_released = &window_mode::action_mouse_released_selecting;
			action_draw = &window_mode::action_draw_selecting;
			break;

		case dragging:
			action_mouse_pressed = &window_mode::action_none;
			action_mouse_moved = &window_mode::action_mouse_moved_dragging;
			action_mouse_released = &window_mode::action_mouse_released_dragging;
			action_draw = &window_mode::action_draw_dragging;
			break;
		}
	}

// =================================================== ACTIONS AND MODES
void window_mode::action_none(sf::Event &event)
	{
	}
void window_mode::action_draw_none()
	{
	}

// =================================================== idle
void window_mode::action_mouse_pressed_idle(sf::Event & event)
	{
	if (event.mouseButton.button == sf::Mouse::Left)
		{
		//if inside a node
		if (hover != nullptr)
			{
			if (hover->selected)
				{
				if (ctrl)
					{//deselect that node
					deselect(hover);
					}
				//enter move mode with all selected nodes
				}
			else
				{//if it is not selected
				if (!ctrl)
					{//deselect all and select that
					deselect_all();
					}
				//in both cases select that node
				select(hover);
				}
			select_x1 = event.mouseButton.x;
			select_y1 = event.mouseButton.y;
			select_x2 = event.mouseButton.x;
			select_y2 = event.mouseButton.y;

			set_mode(dragging);
			}
		else
			{
			//else run selection
			if (!ctrl)
				{
				deselect_all();
				}
			select_x1 = event.mouseButton.x;
			select_y1 = event.mouseButton.y;
			select_x2 = event.mouseButton.x;
			select_y2 = event.mouseButton.y;
			set_mode(selecting);
			}
		}
	}

void window_mode::action_mouse_moved_idle(sf::Event & event)
	{
	for (auto n : g->get_nodes())
		{
		float dist = sqrt(
			pow(n->sprite.getPosition().x - event.mouseMove.x, 2)
			+
			pow(n->sprite.getPosition().y - event.mouseMove.y, 2)
		);

		if (dist < n->sprite.getRadius() + 4)
			{//if a node is under the mouse
			hover = n;
			return; // goto END;
			}
		}
	hover = nullptr;
	//END:;
	}

// =================================================== dragging
void window_mode::action_mouse_moved_dragging(sf::Event &event)
	{
	select_x2 = event.mouseMove.x;
	select_y2 = event.mouseMove.y;
	}

void window_mode::action_mouse_released_dragging(sf::Event &event)
	{
	select_x2 = event.mouseButton.x;
	select_y2 = event.mouseButton.y;
	float dx = select_x2 - select_x1;
	float dy = select_y2 - select_y1;

	for (auto n : selected_nodes)
		{
		n->set_position(n->sprite.getPosition().x + dx, n->sprite.getPosition().y + dy);
		}
	set_mode(idle);
	}


void window_mode::action_draw_dragging()
	{
	float dx = select_x2 - select_x1;
	float dy = select_y2 - select_y1;
	for (auto n : selected_nodes)
		{
		ghost_node.setRadius(n->sprite.getRadius());
		ghost_node.setOrigin(n->sprite.getOrigin());

		ghost_node.setPosition(n->sprite.getPosition().x + dx, n->sprite.getPosition().y + dy);
		window.draw(ghost_node);
		}
	}

// =================================================== selecting
void window_mode::action_mouse_moved_selecting(sf::Event &event)
	{
	select_x2 = event.mouseMove.x;
	select_y2 = event.mouseMove.y;
	}

void window_mode::action_mouse_released_selecting(sf::Event &event)
	{
	select_x2 = event.mouseButton.x;
	select_y2 = event.mouseButton.y;
	float x1 = std::min(select_x1, select_x2);
	float y1 = std::min(select_y1, select_y2);
	float x2 = std::max(select_x1, select_x2);
	float y2 = std::max(select_y1, select_y2);


	for (auto n : g->get_nodes())
		{
		float x = n->sprite.getPosition().x;
		float y = n->sprite.getPosition().y;

		if (x > x1 && x < x2 && y > y1 && y < y2)
			{
			select(n);
			}
		}

	set_mode(idle);
	}


void window_mode::action_draw_selecting()
	{
	sf::RectangleShape sel_rect;
	sel_rect.setPosition(std::min(select_x1, select_x2), std::min(select_y1, select_y2));
	sel_rect.setSize(sf::Vector2f(std::abs(select_x1 - select_x2), std::abs(select_y1 - select_y2)));
	sel_rect.setOutlineColor(sf::Color(0, 180, 255, 255));
	sel_rect.setOutlineThickness(1);
	sel_rect.setFillColor(sf::Color(0, 180, 255, 100));
	window.draw(sel_rect);
	}

// =================================================== CONSTRUCTOR
window_mode::window_mode(graph* g) : g(g)
	{
	set_mode(idle);

	color_node_idle = sf::Color(255, 255, 255, 180);
	color_node_idle_outline = sf::Color(255, 255, 255, 255);
	color_node_selected = sf::Color(0, 255, 80, 180);
	color_node_selected_outline = sf::Color(0, 255, 80, 255);

	ghost_node.setFillColor(sf::Color(0, 255, 80, 50));
	ghost_node.setOutlineColor(sf::Color(0, 255, 80, 125));
	ghost_node.setOutlineThickness(2);
	}
window_mode::~window_mode()
	{
	}
;

int window_mode::run()
	{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(sf::VideoMode(800, 600), "Graph Maker", sf::Style::Default, settings);
	window.setKeyRepeatEnabled(false);

	// Start the game loop
	while (window.isOpen())
		{
		cout << ctrl << endl;
		// Process events
		sf::Event event;
		if (window.waitEvent(event))
			{
			// Close window: exit
			switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					break;

				//======================================= KEYBOARD =======================================
				case sf::Event::KeyPressed:
					switch (event.key.code)
						{
						case sf::Keyboard::Add: case sf::Keyboard::Subtract:
							/*int change = 1 * ((event.key.code == sf::Keyboard::Add) - (event.key.code == sf::Keyboard::Subtract));
							for (auto n : selected_nodes)
								{
								n->change_size(change);
								}*/
							break;

						case sf::Keyboard::LControl: case sf::Keyboard::RControl:
							cout << "pippo";
							ctrl = true;
							break;
						}
					break;

				case sf::Event::KeyReleased:
					switch (event.key.code)
						{
						case sf::Keyboard::LControl: case sf::Keyboard::RControl:
							ctrl = false;
							break;
						}
					break;

				//======================================= MOUSE =======================================
				case sf::Event::MouseButtonPressed:
					(this->*action_mouse_pressed)(event);
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
						{
						(this->*action_mouse_released)(event);
						}
					break;
				case sf::Event::MouseMoved:
					(this->*action_mouse_moved)(event);
					break;

				}
			}

		// CLS
		window.clear();

		// DRAW
		for (auto a : g->get_arcs())
			{
			window.draw(a->sprite);
			}
		for (auto n : g->get_nodes())
			{
			window.draw(n->sprite);
			}

		(this->*action_draw)();

		window.display();
		}


	return 0;
	}
