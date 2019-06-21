#pragma once
class graph_component
	{
	friend class graph;
	friend class window_mode;

	bool selected = false;

	public:
		graph_component();
		~graph_component();
	};

