
#ifndef __AY_CLASS_MAP_TILE_H__
#define __AY_CLASS_MAP_TILE_H__

namespace akiyu
{
	struct MapData
	{
		int tile;
	};

	class MapTile
	{
	public:
		MapTile(const char* mapData[]);
		virtual ~MapTile(void);

		void init(const char* mapData[]);
		int  getSizeX(void) const;
		int  getSizeY(void) const;

		bool isWall(int x, int y) const;
		bool isMoveable(int x, int y) const;

		int  getTile(int x, int y) const;

	private:
		int m_size_x;
		int m_size_y;
		int m_pitch;

		MapData* m_p_data;
	};

	class MapIterator
	{
	public:
		struct Coord
		{
			int x, y;

			Coord() {}
			Coord(int _x, int _y): x(_x), y(_y) {}
		};

		struct Area
		{
			int w, h;

			Area() {}
			Area(int _w, int _h): w(_w), h(_h) {}
		};

		struct Desc
		{
			Coord map_index;
			Coord real_pos;
		};

		MapIterator(Coord offset, Area tile_size, Area clip_rect)
			: m_is_valid(true)
			, m_offset(offset)
			, m_tile_size(tile_size)
			, m_clip_rect(clip_rect)
		{
			m_map_index      = Coord(- offset.x / tile_size.w, - (offset.y + (tile_size.h-1)) / tile_size.h);
			m_out.map_index  = m_map_index;
			m_out.real_pos   = Coord(m_out.map_index.x * tile_size.w + offset.x, m_out.map_index.y * tile_size.h + offset.y);
		}

		virtual ~MapIterator()
		{
		}

		inline bool isEndOfLoop(void) const
		{
			return (!m_is_valid);
		}

		inline const Desc& getDesc(void) const
		{
			return m_out;
		}

		MapIterator& operator++(void)
		{
			m_out.map_index.x++;
			m_out.real_pos.x += m_tile_size.w;

			if (m_out.real_pos.x > m_clip_rect.w)
			{
				m_out.map_index.x = m_map_index.x;
				m_out.real_pos.x  = m_out.map_index.x * m_tile_size.w + m_offset.x;

				m_out.map_index.y++;
				m_out.real_pos.y += m_tile_size.h;

				if (m_out.real_pos.y > m_clip_rect.h)
					m_is_valid = false;
			}

			return (*this);
		}

	private:
		bool  m_is_valid;
		Coord m_map_index;
		Coord m_offset;
		Area  m_tile_size;
		Area  m_clip_rect;

		Desc  m_out;
	};

}

#endif // #ifndef __AY_CLASS_MAP_TILE_H__
