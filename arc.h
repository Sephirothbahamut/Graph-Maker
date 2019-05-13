#pragma once
#include "graph.h"

class graph::arc : public sf::Drawable
	{
	friend graph;

	private:
		graph * owner;

		node* from;
		node* to;
		bool directed;

		// User variables
		std::unordered_map<std::string, double> shared_vars;
		std::unordered_map<std::string, double> vars;
		// //// //

		void update_position();
		void update_color();

		arc(graph * owner, node* from, node* to, bool directed);
		~arc();


	public:
		sf::VertexArray sprite;
		sf::RectangleShape selectable_sprite;
		bool selected = false;

		void set_directed(bool directed);
		void swap();

		node* get_from();
		node* get_to();
		bool get_directed();

		void set_style(graph::arc_style_type style);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

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