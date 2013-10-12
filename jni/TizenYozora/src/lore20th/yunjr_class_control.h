
#ifndef __YUNJR_CLASS_CONTROL_H__
#define __YUNJR_CLASS_CONTROL_H__

#include "yunjr_base.h"
#include "yunjr_base_gfx.h"

#include "yunjr_class.h"

#include <algorithm>
#include <string.h>

namespace yunjr
{
	struct Control: public Visible
	{
		struct Pos
		{
			int x;
			int y;

			Pos()
				: x(0)
				, y(0)
			{
			}

			Pos(int in_x, int in_y)
				: x(in_x)
				, y(in_y)
			{
			}
		};

		struct Size
		{
			int width;
			int height;

			Size()
				: width(0)
				, height(0)
			{
			}

			Size(int in_width, int in_height)
				: width(in_width)
				, height(in_height)
			{
			}
		};

		//??
		//virtual Pos getPos(void) const;
		//virtual Size getSize(void) const;
	};

	typedef std::pair<ResId, Control*> ControlId;
}

#endif // #ifndef __YUNJR_CLASS_CONTROL_H__
