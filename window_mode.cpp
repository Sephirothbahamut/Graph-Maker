#include "stdafx.h"
#include <SFML\Graphics.hpp>

#include "window_mode.h"

void window_mode::snap_coords(sf::Vector2f &source)
	{
	source.x = std::round(source.x / grid_x) * grid_x;
	source.y = std::round(source.y / grid_y) * grid_y;
	}

void window_mode::snap_selecting()
	{
	select_x1 = std::round(select_x1 / grid_x) * grid_x;
	select_x2 = std::round(select_x2 / grid_x) * grid_x;
	select_y1 = std::round(select_y1 / grid_y) * grid_y;
	select_y2 = std::round(select_y2 / grid_y) * grid_y;
	}

sf::Vector2i window_mode::mouse_on_world(int x, int y)
	{
	window.setView(view_work_area);
	return sf::Vector2i(window.mapPixelToCoords(sf::Vector2i(x, y)));
	}

// =================================================== ========= ==================================
// =================================================== Selection ==================================
// =================================================== ========= ==================================

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

void window_mode::set_select_coords(int x, int y)
	{
	select_x1 = x;
	select_y1 = y;
	select_x2 = x;
	select_y2 = y;
	}

graph::node* window_mode::get_hover(int x, int y)
	{
	for (auto n : g->get_nodes())
		{
		float dist = sqrt(
			pow(n->sprite.getPosition().x - x, 2)
			+
			pow(n->sprite.getPosition().y - y, 2)
		);

		if (dist < n->sprite.getRadius() + 4)
			{//if a node is under the mouse
			hover = n;
			return(hover); // goto END;
			}
		}
	hover = nullptr;
	return(hover);
	//END:;
	}

// =================================================== ======== ==================================
// =================================================== Graphics ==================================
// =================================================== ======== ==================================

void window_mode::view_move(int x_offset, int y_offset)
	{
	view_work_area.move(sf::Vector2f(x_offset, y_offset));
	view_offset_x += x_offset;
	view_offset_y += y_offset;
	
	graphics_gui_text_offset.setString(std::to_string(view_offset_x) + ", " + std::to_string(view_offset_y));
	update_draw_grid();
	}

void window_mode::view_zoom(float zoom_offset)
	{
	if ((zoom_offset < 0 && view_zoom_value > 0.8) || (zoom_offset > 0 && view_zoom_value < 16))
		{
		view_zoom_value += zoom_offset * 0.2;
		graphics_gui_text_zoom.setString(std::to_string(view_zoom_value));
		view_work_area.setSize(window.getSize().x * view_zoom_value, window.getSize().y * view_zoom_value);

		if (view_zoom_value < 0.8) { view_zoom_value = 0.8; }
		else
		if (view_zoom_value > 16) { view_zoom_value = 16; }
		}
	}

void window_mode::update_draw_grid()
	{
	unsigned int max_x = window.getSize().x + (2 * grid_x); // +2 to ad a square outside on both sides to see it partially when view is not snapping to the grid
	unsigned int max_y = window.getSize().y + (2 * grid_y); // +2 to ad a square outside on both sides to see it partially when view is not snapping to the grid

	unsigned int index = 0;
	unsigned int size = ((max_x / grid_x) + (max_y / grid_y) + 2) * 2;
	if (size != draw_grid.getVertexCount())
		{
		draw_grid.resize(size);
		}

	//hor lines
	for (unsigned int y = 0; y < max_y; y += grid_y)
		{
		draw_grid[index] = sf::Vector2f(0 - grid_x, y - grid_y); // -1 to move the first square left outside and have the last right outside
		draw_grid[index].color = sf::Color(255, 255, 255, 100);
		index++;
		draw_grid[index] = sf::Vector2f(max_x - grid_x, y - grid_y); // -1 to move the first square left outside and have the last right outside
		draw_grid[index].color = sf::Color(255, 255, 255, 100);
		index++;
		}

	//vert lines
	for (unsigned int x = 0; x < max_x; x += grid_x)
		{
		draw_grid[index] = sf::Vector2f(x - grid_x, 0 - grid_y);
		draw_grid[index].color = sf::Color(255, 255, 255, 100);
		index++;
		draw_grid[index] = sf::Vector2f(x - grid_x, max_y - grid_y);
		draw_grid[index].color = sf::Color(255, 255, 255, 100);
		index++;
		}

	//snap
	for (index = 0; index < size; index++)
		{
		draw_grid[index].position.x += view_offset_x - (view_offset_x % grid_x);
		draw_grid[index].position.y += view_offset_y - (view_offset_y % grid_y);
		}
	}

// =================================================== ================= ==================================
// =================================================== ACTIONS AND MODES ==================================
// =================================================== ================= ==================================
void window_mode::set_mode(mode_type mode)
	{
	clicking = false;
	this->mode = mode;
	switch (mode)
		{
		case idle:
			action_event = &window_mode::action_event_idle;
			action_draw = &window_mode::action_draw_none;
			break;

		case selecting:
			action_event = &window_mode::action_event_selecting;
			action_draw = &window_mode::action_draw_selecting;
			break;

		case dragging:
			action_event = &window_mode::action_event_dragging;
			action_draw = &window_mode::action_draw_dragging;
			break;

		case dragging_view:
			action_event = &window_mode::action_event_dragging_view;
			action_draw = &window_mode::action_draw_none;
			break;

		case add_node:
			action_event = &window_mode::action_event_add_node;
			action_draw = &window_mode::action_draw_add_node;
			break;

		case add_arc:
			action_event = &window_mode::action_event_add_arc;
			action_draw = &window_mode::action_draw_add_arc;
			break;
		}
	}

void window_mode::action_draw_none(){}

// =================================================== idle
void window_mode::action_event_idle(sf::Event & event)
	{
	switch (event.type)
		{
		// KEYBOARD
		case sf::Event::KeyPressed:
			switch (event.key.code)
				{
				case sf::Keyboard::Delete:
					for (auto n : selected_nodes)
						{
						std::cout << "Removing node: [" << n << "];" << std::endl;
						g->remove_node(n);
						}
					selected_nodes.clear();
					break;

				case sf::Keyboard::N:
					set_mode(add_node);
					break;

				case sf::Keyboard::A:
					set_mode(add_arc);
					break;
				}
			break;

		// MOUSE
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					action_mouse_pressed_idle(event);
					break;

				case sf::Mouse::Right:
					select_x1 = event.mouseButton.x;
					select_y1 = event.mouseButton.y;
					set_mode(dragging_view);
					break;
				}
			break;
		}
	}

void window_mode::action_mouse_pressed_idle(sf::Event & event)
	{
	if (event.mouseButton.button == sf::Mouse::Left)
		{
		//if inside a node
		sf::Vector2i m = mouse_on_world(event.mouseButton.x, event.mouseButton.y);
		if (get_hover(m.x, m.y) != nullptr)
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

			set_select_coords(m.x, m.y);
			set_mode(dragging);
			}
		else
			{
			//else run selection
			if (!ctrl)
				{
				deselect_all();
				}

			set_select_coords(m.x, m.y);
			set_mode(selecting);
			}
		}
	}

// =================================================== selecting
void window_mode::action_event_selecting(sf::Event &event)
	{
	switch (event.type)
		{
		// MOUSE
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				{
				sf::Vector2i m = mouse_on_world(event.mouseButton.x, event.mouseButton.y);
				select_x2 = m.x;
				select_y2 = m.y;
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
			break;
		case sf::Event::MouseMoved:
			if (true)
				{
				sf::Vector2i m = mouse_on_world(event.mouseMove.x, event.mouseMove.y);
				select_x2 = m.x;
				select_y2 = m.y;
				}
			break;

		}
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

// =================================================== dragging
void window_mode::action_event_dragging(sf::Event &event)
	{
	switch (event.type)
		{
		// MOUSE
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				{
				sf::Vector2i m = mouse_on_world(event.mouseButton.x, event.mouseButton.y);
				select_x2 = m.x;
				select_y2 = m.y;
				float dx = select_x2 - select_x1;
				float dy = select_y2 - select_y1;

				for (auto n : selected_nodes)
					{
					sf::Vector2f new_pos(n->sprite.getPosition().x + dx, n->sprite.getPosition().y + dy);
					if (!alt)
						{
						snap_coords(new_pos);
						}
					n->set_position(new_pos);
					}
				set_mode(idle);
				}
			break;
		case sf::Event::MouseMoved:
			if (true)
				{
				sf::Vector2i m = mouse_on_world(event.mouseMove.x, event.mouseMove.y);
				select_x2 = m.x;
				select_y2 = m.y;
				}
			break;

		}
	}

void window_mode::action_draw_dragging()
	{
	float dx = select_x2 - select_x1;
	float dy = select_y2 - select_y1;
	for (auto n : selected_nodes)
		{
		ghost_node.setRadius(n->sprite.getRadius());
		ghost_node.setOrigin(n->sprite.getOrigin());

		sf::Vector2f new_pos(n->sprite.getPosition().x + dx, n->sprite.getPosition().y + dy);
		if (!alt)
			{
			snap_coords(new_pos);
			}

		ghost_node.setPosition(new_pos);
		window.draw(ghost_node);
		}
	}

// =================================================== dragging_view
void window_mode::action_event_dragging_view(sf::Event &event)
	{
	switch (event.type)
		{
		// MOUSE
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Right)
				{
				set_mode(idle);
				}
			break;

		case sf::Event::MouseMoved:
			if (true)
				{
				select_x2 = event.mouseMove.x;
				select_y2 = event.mouseMove.y;

				int dx = (select_x1 - select_x2);
				int dy = (select_y1 - select_y2);

				view_move(dx, dy);

				select_x1 = select_x2;
				select_y1 = select_y2;
				}
			break;

		}
	}

// =================================================== add_node

void window_mode::action_event_add_node(sf::Event & event)
	{
	switch (event.type)
		{
		// KEYBOARD
		case sf::Event::KeyReleased:
			switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					set_mode(idle);
					break;
				}
			break;

		// MOUSE
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				{
				sf::Vector2i m = mouse_on_world(event.mouseButton.x, event.mouseButton.y);
				select_x1 = m.x;
				select_y1 = m.y;
				snap_selecting();

				graph::node* n = g->add_node();
				n->set_position(select_x1, select_y1);

				if (!ctrl)
					{
					set_mode(idle);
					}
				}
			else
				{
				set_mode(idle);
				}
			break;
		case sf::Event::MouseMoved:
			if (true)
				{
				sf::Vector2i m = mouse_on_world(event.mouseMove.x, event.mouseMove.y);
				select_x1 = m.x;
				select_y1 = m.y;
				snap_selecting();
				}
			break;

		}
	}

void window_mode::action_draw_add_node()
	{
	ghost_node.setRadius(16);
	ghost_node.setOrigin(16, 16);

	ghost_node.setPosition(select_x1, select_y1);
	window.draw(ghost_node);
	}

// =================================================== add_arc

void window_mode::action_event_add_arc(sf::Event & event)
	{
	switch (event.type)
		{
		// KEYBOARD
		case sf::Event::KeyReleased:
			switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					set_mode(idle);
					break;
				}
			break;

		// MOUSE
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
				{
				sf::Vector2i m = mouse_on_world(event.mouseButton.x, event.mouseButton.y);
				if (get_hover(m.x, m.y) != nullptr)
					{
					set_select_coords(m.x, m.y);
					clicking = true;
					}
				else
					{
					set_mode(idle);
					}
				}
			break;

		case sf::Event::MouseMoved:
			if (true)
				{
				sf::Vector2i m = mouse_on_world(event.mouseMove.x, event.mouseMove.y);
				select_x2 = m.x;
				select_y2 = m.y;
				}
			break;

		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				{
				sf::Vector2i m = mouse_on_world(event.mouseButton.x, event.mouseButton.y);
				graph::node * prev = hover;
				if (get_hover(m.x, m.y) != nullptr && prev != nullptr)
					{
					g->add_arc(prev, hover);
					}

				if (!ctrl)
					{
					set_mode(idle);
					}
				}
			break;

		}
	}

void window_mode::action_draw_add_arc()
	{
	if (clicking)
		{
		sf::VertexArray line(sf::LinesStrip, 2);
		line[0].position.x = select_x1;
		line[0].position.y = select_y1;
		line[0].color = sf::Color(0, 255, 80, 125);

		line[1].position.x = select_x2;
		line[1].position.y = select_y2;
		line[1].color = sf::Color(0, 255, 80, 125);

		window.draw(line);
		}
	}

// =================================================== ===================== ==================================
// =================================================== End Actions and Modes ==================================
// =================================================== ===================== ==================================
// =================================================== CONSTRUCTOR
window_mode::window_mode(graph* g) : g(g)
	{
	if (!graphics_font_default.loadFromFile("arial.ttf"))
		{
		std::cout << "something went really wrong" << std::endl;
		}

	set_mode(idle);

	graphics_gui_text_offset = sf::Text();
	graphics_gui_text_zoom = sf::Text();
	graphics_gui_text_offset.setFont(graphics_font_default);
	graphics_gui_text_zoom.setFont(graphics_font_default);
	graphics_gui_text_offset.setCharacterSize(12);
	graphics_gui_text_zoom.setCharacterSize(12);
	graphics_gui_text_offset.setPosition(32, 48);
	graphics_gui_text_zoom.setPosition(32, 32);

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

int window_mode::run()
	{

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	settings.sRgbCapable = false;
	window.create(sf::VideoMode(800, 600), "Graph Maker", sf::Style::Default, settings);
	//window.setKeyRepeatEnabled(false);

	update_draw_grid();
	view_work_area = window.getDefaultView();
	view_gui = window.getDefaultView();
	view_zoom_value = 1;
	view_zoom(0);

	draw();

	// Start the game loop
	while (window.isOpen())
		{
		// Process events
		sf::Event event;
		if (window.pollEvent(event))
			{
			switch (event.type)
				{
				// Window control
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::Resized:
					if (true)
						{
						view_work_area.setSize(event.size.width, event.size.height);
						view_gui.setSize(event.size.width, event.size.height);
						update_draw_grid();
						}
					break;

				// KEYBOARD
				case sf::Event::KeyPressed:
					switch (event.key.code)
						{
						case sf::Keyboard::LControl: case sf::Keyboard::RControl:
							ctrl = true;
							break;
						case sf::Keyboard::LAlt: case sf::Keyboard::RAlt:
							alt = true;
							break;

						case sf::Keyboard::Left:
							if (true)
								{
								view_move(-12, 0);
								}
							break;
						case sf::Keyboard::Right:
							if (true)
								{
								view_move(12, 0);
								}
							break;
						case sf::Keyboard::Up:
							if (true)
								{
								view_move(0, -12);
								}
							break;
						case sf::Keyboard::Down:
							if (true)
								{
								view_move(0, 12);
								}
							break;
						case sf::Keyboard::Add:
							if (ctrl)
								{
								view_zoom(0.2);
								}
							break;
						case sf::Keyboard::Subtract:
							if (ctrl)
								{
								view_zoom(-0.2);
								}
							break;
						case sf::Keyboard::Comma: //TODO remove, debug only
							for (int i = 0; i < 5000; i++)
								{
								graph::node* n = g->add_node();
								n->set_position(12, 12);
								}
							break;
						}
					break;

				case sf::Event::KeyReleased:
					switch (event.key.code)
						{
						case sf::Keyboard::LControl: case sf::Keyboard::RControl:
							ctrl = false;
							set_mode(idle);
							break;
						case sf::Keyboard::LAlt: case sf::Keyboard::RAlt:
							alt = false;
							break;
						}
					break;

				//MOUSE
				case sf::Event::MouseWheelScrolled:
					if (true)
						{
						int delta = event.mouseWheelScroll.delta;

						if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
							{
							if (!ctrl)
								{
								view_zoom(-delta);
								}
							else
								{
								view_work_area.rotate(delta * 10);
								}
							}
						else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
							{
							if (!ctrl)
								{
								view_work_area.rotate(delta * 10);
								}
							else
								{
								view_work_area.zoom(1 - (delta * 0.2));
								}
							}
						}
					break;

				case sf::Event::MouseMoved:
					break;
				}
			(this->*action_event)(event);
			}

		draw();
		}


	return 0;
	}

void window_mode::draw()
	{
	// CLS
	window.clear();

	// DRAW
	window.setView(view_work_area);

	window.draw(draw_grid);

	for (auto a : g->get_arcs())
		{
		window.draw(a->sprite);
		window.draw(a->selectable_sprite);
		}
	for (auto n : g->get_nodes())
		{
		window.draw(n->sprite);
		}

	(this->*action_draw)();

	sf::CircleShape c(20, 30);
	c.setPosition(30, 30);
	c.setFillColor(sf::Color::Green);
	window.draw(c);

	window.setView(view_gui);

	sf::CircleShape d(20, 30);
	d.setPosition(50, 30);
	d.setFillColor(sf::Color::Red);
	window.draw(d);
	window.draw(graphics_gui_text_zoom);
	window.draw(graphics_gui_text_offset);

	window.display();
	}