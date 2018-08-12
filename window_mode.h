#pragma once
#include <set>
#include "graph.h"

class window_mode
	{
	graph* g;
	sf::RenderWindow window;

	int grid_x = 32;
	int grid_y = 32;


	float select_x1;
	float select_y1;
	float select_x2;
	float select_y2;

	float drag_offset_x;
	float drag_offset_y;

	graph::node* hover = nullptr;
	bool ctrl = false;

	//                                                          SELECTION
	std::set<graph::node*> selected_nodes;
	void select(graph::node* n);
	void deselect(graph::node* n);
	void deselect_all();

	//                                                          GRAPHICS
	sf::Color color_node_idle;
	sf::Color color_node_idle_outline;
	sf::Color color_node_selected;
	sf::Color color_node_selected_outline;
	sf::CircleShape ghost_node;
	sf::Texture hover_details;

	//                                                          ACTIONS AND MODES
	enum mode_type { idle, selecting, dragging, add_node };
	mode_type mode = idle;
	void set_mode(mode_type mode);

	//action pointers
	void (window_mode::*action_event)(sf::Event &event);

	void (window_mode::*action_mouse_pressed)(sf::Event &event);
	void (window_mode::*action_mouse_moved)(sf::Event &event);
	void (window_mode::*action_mouse_released)(sf::Event &event);
	void (window_mode::*action_draw)();

	//actions
	void action_none(sf::Event &event); // placeholder
	void action_draw_none(); // placeholder

	//idle
	void action_mouse_pressed_idle(sf::Event &event);
	void action_mouse_moved_idle(sf::Event &event);

	//dragging
	void action_mouse_moved_dragging(sf::Event &event);
	void action_mouse_released_dragging(sf::Event &event);
	void action_draw_dragging();

	void action_mouse_moved_selecting(sf::Event &event);
	void action_mouse_released_selecting(sf::Event &event);
	void action_draw_selecting();


	public:
		window_mode(graph* g);
		~window_mode();

		int run();
	};