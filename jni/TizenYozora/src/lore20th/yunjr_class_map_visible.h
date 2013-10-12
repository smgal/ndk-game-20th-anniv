
#ifndef __AY_CLASS_MAP_VISIBLE_H__
#define __AY_CLASS_MAP_VISIBLE_H__

#define CHECK ((x >= 0) && (x < _w) && (y >= 0) && (y < _h))

namespace akiyu
{
	class MapVisible
	{
	protected:
		enum
		{
			MASK_NONE            = 0x00,
			MASK_BLOCKED         = 0x01,
			MASK_VISIBLE         = 0x02,
			MASK_VISIBLE_BLOCKED = (MASK_BLOCKED | MASK_VISIBLE)
		};

	public:
		MapVisible(void)
			: _map(0)
			, _w(0)
			, _h(0)
			, _pitch(0)
		{
		}

		virtual ~MapVisible(void)
		{
			delete[] _map;
		}

		inline bool isAvaliable(void) const
		{
			return (_map != 0);
		}

		inline bool isBlocked(int x, int y) const
		{
			return (CHECK) ? (_map[y*_pitch+x] & MASK_BLOCKED) == MASK_BLOCKED : false;
		}

		inline bool isVisible(int x, int y) const
		{
			return (CHECK) ? (_map[y*_pitch+x] & MASK_VISIBLE) == MASK_VISIBLE : true;
		}

		inline bool isVisibleAndBlocked(int x, int y) const
		{
			return (CHECK) ? (_map[y*_pitch+x] & MASK_VISIBLE_BLOCKED) == MASK_VISIBLE_BLOCKED : false;
		}

		inline int  getTile(int x, int y) const
		{
			return (CHECK) ? (_map[y*_pitch+x] >> 4) : 0;
		}

		inline void setVisibility(int x, int y)
		{
			if (CHECK)
				_map[y*_pitch+x] |= MASK_VISIBLE;
		}

		void reset(void)
		{
			if (_map)
			{
				for (int y = 0; y < _h; y++)
				for (int x = 0; x < _w; x++)
				{
					_map[y*_pitch+x] &= MASK_BLOCKED;
				}
			}
		}

		void operator <<(const char* map[])
		{
			delete[] _map;

			_w = m_strlen(map[0]);
			_h = 0;

			while (map[_h])
				++_h;

			_pitch = _w;
			_map = new char[_w*+_h];

			for (int y = 0; y < _h; y++)
			for (int x = 0; x < _w; x++)
			{
				switch (map[y][x])
				{
					case ' ': _map[_pitch*y+x] = MASK_NONE; break;
					case '#': _map[_pitch*y+x] = (0x01 << 4) | MASK_BLOCKED; break;
					default : _map[_pitch*y+x] = MASK_BLOCKED; break;
				}
			}
		}

	private:
		int m_strlen(const char* str1)
		{
			const char* str2 = str1;

			while (*str2++)
				;

			return str2-str1-1;
		}

	protected:
		char* _map;
		int   _w;
		int   _h;
		int   _pitch;
	};

}

#undef CHECK

#endif // #ifndef __AY_CLASS_MAP_VISIBLE_H__
