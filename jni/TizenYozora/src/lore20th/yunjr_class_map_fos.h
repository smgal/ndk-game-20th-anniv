
#ifndef __AY_CLASS_MAP_FOS_H__
#define __AY_CLASS_MAP_FOS_H__

#include "ay_class_map_visible.h"
#include "ay_class_map_tile.h"

namespace akiyu
{
	class MapFos: public MapVisible
	{
	public:
		struct Point
		{
			int x, y;
		};

	public:
		MapFos(int block_w, int block_h)
			: _block_w(block_w)
			, _block_h(block_h)
		{
		}

		virtual ~MapFos()
		{
		}

		void operator <<(const MapTile& map);

		bool makeFos(int x, int y, int x_on_tile, int y_on_tile, Point vertices[]);
		bool isMyEyesReached(int x1, int y1, int x2, int y2);

	protected:
		int  _block_w;
		int  _block_h;
	};

}

#endif // #ifndef __AY_CLASS_MAP_FOS_H__
