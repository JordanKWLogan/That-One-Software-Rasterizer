#pragma once

#include <stdint.h>

// i should drop my math lib here
struct IPoint2D
{
	using TYPE = int32_t;
	union
	{
		struct
		{
			TYPE x;
			TYPE y;
		};
		TYPE v[2];
	};
};

struct Point4D
{
	using TYPE = float;
	union
	{
		struct
		{
			TYPE x;
			TYPE y;
			TYPE z;
			TYPE w;
		};
		TYPE v[4];
	};
};
