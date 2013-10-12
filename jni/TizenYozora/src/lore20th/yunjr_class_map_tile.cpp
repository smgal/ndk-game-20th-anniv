
#include "ay_class_map_tile.h"

namespace
{
	int strlen(const char* str1)
	{
		const char* str2 = str1;

		while (*str2++)
			;

		return str2-str1-1;
	}
}

akiyu::MapTile::MapTile(const char* map_data[])
	: m_size_x(0)
	, m_size_y(0)
	, m_pitch(0)
	, m_p_data(0)
{
	this->init(map_data);
}

akiyu::MapTile::~MapTile(void)
{
	delete[] m_p_data;
}

void akiyu::MapTile::init(const char* map_data[])
{
	m_size_x = strlen(map_data[0]);
	m_size_y = 0;

	while (map_data[m_size_y])
		++m_size_y;

	m_pitch = m_size_x;

	m_p_data = new MapData[m_pitch * m_size_y];

	for (int j = 0; j < m_size_y; j++)
	{
		MapData* p_map = m_p_data;

		p_map += j * m_pitch;

		for (int i = 0; i < m_size_x; i++)
		{
			if      (map_data[j][i] == '>')
				p_map->tile = 16+1;
			else if (map_data[j][i] == '<')
				p_map->tile = 16+2;
			else if (map_data[j][i] != ' ')
				p_map->tile = 1;
			else
				p_map->tile = 0;

			++p_map;
		}
	}

	for (int j = 0; j < m_size_y; j++)
	{
		MapData* p_map = m_p_data;

		p_map += j * m_pitch;

		for (int i = 0; i < m_size_x; i++)
		{
			if ((p_map->tile > 0) && (p_map->tile <= 16))
			{
				int tile = 0;

				if (isWall(i, j-1))
					tile += 1;
				if (isWall(i+1, j))
					tile += 2;
				if (isWall(i, j+1))
					tile += 4;
				if (isWall(i-1, j))
					tile += 8;

				p_map->tile = tile + 1;
			}
			++p_map;
		}
	}
}

bool akiyu::MapTile::isWall(int x, int y) const
{
	if ((x < 0) || (x >= m_size_x) || (y < 0) || (y >= m_size_y))
		return false;

	MapData* p_map = m_p_data;

	p_map += y * m_pitch + x;

	return (p_map->tile > 0) && (p_map->tile <= 16);
}

bool akiyu::MapTile::isMoveable(int x, int y) const
{
	if ((x < 0) || (x >= m_size_x) || (y < 0) || (y >= m_size_y))
		return false;

	MapData* p_map = m_p_data;

	p_map += y * m_pitch + x;

	return (p_map->tile == 0) || (p_map->tile > 16);
}

int  akiyu::MapTile::getTile(int x, int y) const
{
	if ((x < 0) || (x >= m_size_x) || (y < 0) || (y >= m_size_y))
		return false;

	MapData* p_map = m_p_data;

	p_map += y * m_pitch + x;

	return p_map->tile;
}

int  akiyu::MapTile::getSizeX(void) const
{
	return m_size_x;
}

int  akiyu::MapTile::getSizeY(void) const
{
	return m_size_y;
}
