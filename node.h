#pragma once
#include "graph.h"


class graph::node : public sf::Drawable
	{
	friend class graph;
	friend class window_mode;

	private:
		graph* owner;

		std::set<arc*> list_arc_to;
		std::set<arc*> list_arc_from;

		// User variables
		std::unordered_map<const std::string*, double> shared_vars;
		std::unordered_map<std::string, double> vars;
		// //// //

		void add_arc_from(arc* a);
		void add_arc_to(arc* a);
		void remove_arc_from(arc* a);
		void remove_arc_to(arc* a);

		node(graph * owner);
		~node();

	public:
		void AddRef(); void ReleaseRef(); //required by angelscript
		sf::CircleShape sprite;
		bool selected = false;


		std::vector<node*> adj_vector();
		std::vector<arc*> adj_arc_vector();
		std::set<arc*>::iterator adj_it;
		bool iteration_invalidated = true;

		void adj_reset();
		node* adj_next();
		arc* adj_arc();

		void set_color(sf::Color fill, sf::Color outline);
		void set_position(float x, float y);
		void set_position(sf::Vector2f pos);
		void set_size(float size);
		void change_size(float change); 
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// User variables
		std::string id();
		//personal
		bool chk(std::string name);
		void set(std::string name, double value = 0);
		double get(std::string name);
		void del(std::string name);

		//shared
		bool shr_chk(std::string name);
		void shr_set(std::string name, double value);
		double shr_get(std::string name);

	private:
		void graph_shr_set(const std::string* s, double value);
		void graph_shr_del(const std::string* s);
			 // //// //
	public:

	};