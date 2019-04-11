#pragma once
#include <set>
#include <iomanip>
#include "graph.h"

class window_mode
	{
	graph* g;
	sf::RenderWindow window;

	unsigned short int grid_x = 32;
	unsigned short int grid_y = 32;
	void snap_coords(sf::Vector2f &source);
	void snap_selecting();

	sf::Vector2i mouse_on_world(int x, int y);
	float select_x1;
	float select_y1;
	float select_x2;
	float select_y2;

	float drag_offset_x;
	float drag_offset_y;

	graph::node* hover = nullptr;
	bool ctrl = false;
	bool alt = false;
	bool clicking = false;

	// =================================================== ========= ==================================
	// =================================================== Selection ==================================
	// =================================================== ========= ==================================
	std::set<graph::node*> selected_nodes;
	void select(graph::node* n);
	void deselect(graph::node* n);
	void deselect_all();
	void set_select_coords(int x, int y);
	graph::node* get_hover(int x, int y);

	// =================================================== ======== ==================================
	// =================================================== Graphics ==================================
	// =================================================== ======== ==================================
	sf::Font graphics_font_default;
	sf::Text graphics_gui_text_zoom;
	sf::Text graphics_gui_text_offset;

	sf::View view_work_area;
	sf::View view_gui;

	float view_zoom_value;
	int view_offset_x;
	int view_offset_y;
	void view_move(int x_offset, int y_offset);
	void view_zoom(float zoom_offset);


	sf::VertexArray draw_grid = sf::VertexArray(sf::Lines, 0);
	void update_draw_grid();

	sf::Color color_node_idle;
	sf::Color color_node_idle_outline;
	sf::Color color_node_selected;
	sf::Color color_node_selected_outline;
	sf::CircleShape ghost_node;
	sf::Texture hover_details;

	// =================================================== ================= ==================================
	// =================================================== Actions and Modes ==================================
	// =================================================== ================= ==================================
	enum mode_type { idle, selecting, dragging, dragging_view, add_node, add_arc };
	mode_type mode = idle;
	void set_mode(mode_type mode);

	//action pointers
	void (window_mode::*action_event)(sf::Event &event);
	void (window_mode::*action_draw)();
	void action_draw_none();

	//idle
	void action_event_idle(sf::Event &event);
	void action_mouse_pressed_idle(sf::Event &event);

	//selecting
	void action_event_selecting(sf::Event &event);
	void action_draw_selecting();

	//dragging
	void action_event_dragging(sf::Event &event);
	void action_draw_dragging();

	//dragging_view
	void action_event_dragging_view(sf::Event &event);

	//add_node
	void action_event_add_node(sf::Event &event);
	void action_draw_add_node();

	//add_arc
	void action_event_add_arc(sf::Event &event);
	void action_mouse_pressed_add_arc(sf::Event &event);
	void action_mouse_moved_add_arc(sf::Event &event);
	void action_mouse_released_add_arc(sf::Event &event);
	void action_draw_add_arc();

	// =================================================== ===================== ==================================
	// =================================================== End Actions and Modes ==================================
	// =================================================== ===================== ==================================

	public:
		window_mode(graph* g);
		~window_mode();

		int run();

	private:
		void draw();
	};