#pragma once
#include "graph.h"

class graph::arc : public sf::Drawable
	{
	friend class graph;
	friend class window_mode;

	private:
		graph* owner;

		node* from;
		node* to;
		bool directed;

		// User variables
		std::unordered_map<const std::string*, double> shared_vars;
		std::unordered_map<std::string, double> vars;
		// //// //

		void update_position();
		void update_color();

		arc(graph * owner, node* from, node* to, bool directed);
		~arc();


	public:
		void AddRef(); void ReleaseRef(); //required by angelscript

		sf::VertexArray connection;
		sf::RectangleShape sprite;
		bool selected = false;

		void set_directed(bool directed);
		void swap();

		node* get_from();
		node* get_to();
		bool get_directed();

		void set_style(graph::arc_style_type style);
		void set_color(sf::Color fill, sf::Color outline);
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