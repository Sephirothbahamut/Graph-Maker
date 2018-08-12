#pragma once
class variable
	{
	public:
		bool hidden;
		double value;

		variable(double value = 0, bool hidden = false);
		~variable();
	};

