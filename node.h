#pragma once
#include "graph.h"


class graph::node
	{
	friend graph;

	private:
		graph * owner;

		std::set<arc*> list_arc_to;
		std::set<arc*> list_arc_from;

		// User variables
		std::unordered_map<std::string, double> shared_vars;
		std::unordered_map<std::string, double> vars;
		// //// //

		void add_arc_from(arc* a);
		void add_arc_to(arc* a);
		void remove_arc_from(arc* a);
		void remove_arc_to(arc* a);

		node(graph * owner);
		~node();

		std::vector<arc*> iteration_vector_arc;
		std::vector<node*> iteration_vector_node;
		bool iteration_invalidated = true;

	public:
		sf::CircleShape sprite;
		bool selected = false;


		std::vector<node*> get_adj_node();
		std::vector<arc*> get_adj_arc();

		void set_color(sf::Color fill, sf::Color outline);
		void set_position(float x, float y);
		void set_position(sf::Vector2f pos);
		void set_size(float size);
		void change_size(float change);

		// User variables
		//personal
		bool chk(std::string name);
		void set(std::string name, double value = 0);
		double get(std::string name);
		void del(std::string name);

		//shared
		bool shr_chk(std::string name);
		void shr_set(std::string name, double value);
		double shr_get(std::string name);

	private: void shr_del(std::string name);
			 // //// //
	public:

	};