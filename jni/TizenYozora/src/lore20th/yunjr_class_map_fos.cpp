
#include "ay_class_map_fos.h"

namespace
{
	template <typename type>
	type abs(type a)
	{
		return (a > 0) ? a : -a;
	}

	struct Rect
	{
		int x1, y1, x2, y2;
	};
}

void akiyu::MapFos::operator <<(const MapTile& map)
{
	delete[] this->_map;

	this->_w     = map.getSizeX();
	this->_h     = map.getSizeY();
	this->_pitch = this->_w;
	this->_map   = new char[this->_w * this->_h];

	for (int y = 0; y < this->_h; y++)
	for (int x = 0; x < this->_w; x++)
		this->_map[this->_pitch*y+x] = (map.isWall(x,y)) ? MASK_BLOCKED : MASK_NONE;
}

namespace
{
#	define QAZ(i,p1,p2,p3,p4) case i: v[0].x = b.p1; v[0].y = b.p2; v[3].x = b.p3; v[3].y = b.p4; break;

	bool _makeShadow(int x, int y, const Rect& b, akiyu::MapFos::Point v[], int& st)
	{
		st  = (x > b.x2) ? 0 : (x < b.x1) ? 2 : 1;
		st += (y > b.y2) ? 0 : (y < b.y1) ? 6 : 3;

		switch (st)
		{
			QAZ(0, x1,y2,x2,y1)
			QAZ(1, x1,y2,x2,y2)
			QAZ(2, x1,y1,x2,y2)
			QAZ(3, x2,y1,x2,y2)
			QAZ(5, x1,y1,x1,y2)
			QAZ(6, x1,y1,x2,y2)
			QAZ(7, x1,y1,x2,y1)
			QAZ(8, x1,y2,x2,y1)
			default: return false;
		}

		v[1].x = x + (v[0].x - x) * 15*4;
		v[1].y = y + (v[0].y - y) * 15*4;
		v[2].x = x + (v[3].x - x) * 15*4;
		v[2].y = y + (v[3].y - y) * 15*4; 

		return true;
	}

#	undef QAZ
}

bool akiyu::MapFos::makeFos(int x, int y, int on_tile_x, int on_tile_y, Point vertices[])
{
	Rect box = { on_tile_x * _block_w, on_tile_y * _block_h, on_tile_x * _block_w + _block_w, on_tile_y * _block_h + _block_h };

	int status;
	if (!_makeShadow(x, y, box, vertices, status))
		return false;

	this->isMyEyesReached(x, y, vertices[1].x, vertices[1].y);
	this->isMyEyesReached(x, y, vertices[2].x, vertices[2].y);

	return true;
}

bool akiyu::MapFos::isMyEyesReached(int x1, int y1, int x2, int y2)
{
	if ((abs(x1-x2) <= _block_w) && (abs(y1-y2) <= _block_h))
	{
		this->setVisibility(x1 / _block_w, y1 / _block_h);
		return true;
	}

	int dx = x2 - x1;
	int dy = y2 - y1;

	int dir_x = (dx < 0) ? -1 : 1;
	int dir_y = (dy < 0) ? -1 : 1;

	{
		if (dir_x < 0)
			dx = -dx;

		if (dir_y < 0)
			dy = -dy;

		bool qaz = (dx >= dy);

		if (!qaz)
		{
			dx ^= dy;
			dy ^= dx;
			dx ^= dy;
		}

		int dir       = 2 * dy - dx;
		int inc_to_R  = 2 * dy;
		int inc_to_UR = 2 * (dy - dx);
		int x         = x1;
		int y         = y1;

		int prev_x    = x / _block_w;
		int prev_y    = y / _block_h;

		int& major_axis = (qaz) ? x : y;
		int& minor_axis = (qaz) ? y : x;
		int& major_inc  = (qaz) ? dir_x : dir_y;
		int& minor_inc  = (qaz) ? dir_y : dir_x;

		while ((x != x2) || (y != y2))
		{
			major_axis += major_inc;

			if (dir <= 0)
				dir += inc_to_R;
			else
			{
				dir += inc_to_UR;
				minor_axis += minor_inc;
			}

			if ((prev_x != x / _block_w) || (prev_y != y / _block_h))
			{
				int new_x = x / _block_w;
				int new_y = y / _block_h;

				if ((new_x < 0) || (new_x >= _w) || (new_y < 0) || (new_y >= _h))
					return true;

				this->setVisibility(new_x, new_y);

				if (this->isBlocked(new_x, new_y))
					return false;

				prev_x = new_x;
				prev_y = new_y;
			}
		}
	}

	return true;
}
