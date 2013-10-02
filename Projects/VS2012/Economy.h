#pragma once
class Economy
{
public:
	enum Resources
	{
		People = 0,
		Wood,
		Food,
		Magic,
		Equipment,
		Influence,

		ResourcesCount
	};

	Economy(void);
	~Economy(void);
};

