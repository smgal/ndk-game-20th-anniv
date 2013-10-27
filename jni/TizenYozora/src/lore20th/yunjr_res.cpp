
#include "yunjr_base.h"
#include "yunjr_base_font.h"

#include "yunjr_res.h"


#include <vector>
#include <memory>

using yunjr::FlatBoard32;
using yunjr::TargetPixel;

namespace
{
	template <class T>
	class auto_deletor
	{
	public:
		auto_deletor(T* p_object = 0)
			: m_p_object(p_object)
		{
		}

		~auto_deletor(void)
		{
			delete[] m_p_object;
		}

		void operator()(T* p_object)
		{
			m_p_object = p_object;
		}

	private:
		T* m_p_object;

	};

	struct ResImage
	{
		FlatBoard32 res_ui[2];
		FlatBoard32 res_movable[1];
		FlatBoard32 res_tile[2];

		auto_deletor<FlatBoard32::Pixel> auto_release_ui[2];
		auto_deletor<FlatBoard32::Pixel> auto_release_movable[1];
		auto_deletor<FlatBoard32::Pixel> auto_release_tile[1];

		ResImage()
		{
			// load resource from file
			{
				const struct
				{
					const char* sz_res_file_name;
					FlatBoard32& ref_res;
					auto_deletor<FlatBoard32::Pixel>& ref_auto_release;
				} RES_LIST[] =
				{
					{ "lore20th/lore_tile_8.tga", res_tile[0], auto_release_tile[0] },
					{ "lore20th/lore_sprite_8.tga", res_movable[0], auto_release_movable[0] },
					{ "lore20th/lore_waku.tga", res_ui[0], auto_release_ui[0] },
					{ "lore20th/lore_waku_mask.tga", res_ui[1], auto_release_ui[1] },
				};

				for (unsigned long index = 0; index < sizeof(RES_LIST) / sizeof(RES_LIST[0]); index++)
				{
					unsigned char* p_buffer = 0;
					int buffer_size = 0;

					{
						target::file_io::StreamReadFile file(RES_LIST[index].sz_res_file_name);

						buffer_size = file.getSize();

						if (buffer_size > 0)
						{
							p_buffer = new unsigned char[buffer_size];
							file.read(p_buffer, buffer_size);
						}
					}

					if (p_buffer)
					{
						TargetPixel* p_buffer_target = 0;
						int          buffer_width;
						int          buffer_height;

						flat_board::loadTGA<TargetPixel>(p_buffer, buffer_size, p_buffer_target, buffer_width, buffer_height);

						delete[] p_buffer;

						new (&RES_LIST[index].ref_res) FlatBoard32(static_cast<FlatBoard32::Pixel*>(p_buffer_target), buffer_width, buffer_height, buffer_width);
						RES_LIST[index].ref_auto_release(p_buffer_target);
					}
				}
			}
		}
	};

	yunjr::auto_ptr<ResImage> s_res_image;
	yunjr::Tile s_tiles[yunjr::TILE_ID_MAX];
}

namespace
{
	const yunjr::BufferDesc* s_ref_current_buffer = 0;
	struct yunjr::ControlWindow* s_ref_p_main_window = 0;
	yunjr::shared_ptr<yunjr::Font> s_current_font;
	int s_ref_current_map_pos_x = 0;
	int s_ref_current_map_pos_y = 0;

	std::vector<yunjr::Chara*> s_chara_list;
}

#define MAKE_TILE_ID(tile_id, offset) yunjr::TileId(int(tile_id) + offset)

void yunjr::resource::init(void)
{
	if (s_res_image.get() == 0)
	{
		s_res_image.bind(new ResImage);
	}

	// tile
	{
		const ResId res_id(ResId::TAG_TYPE_IMAGE, ResId::TAG_TYPE_IMAGE_TILE, 0);

		int offset = 0;

		for (int y = 0; y < 96; y += TILE_H)
		for (int x = 0; x < 1344; x += TILE_W)
		{
			Tile::Bounds bounds = { x, y, x + TILE_W, y + TILE_H };

			new (&s_tiles[MAKE_TILE_ID(TILE_ID_GROUND, offset++)]) Tile(res_id, bounds);
		}
	}

	// chara
	{
		const ResId res_id(ResId::TAG_TYPE_IMAGE, ResId::TAG_TYPE_IMAGE_CHARA, 0);

		int offset = 0;

		for (int y = 0; y < 24; y += TILE_H)
		for (int x = 0; x < 672; x += TILE_W)
		{
			Tile::Bounds bounds = { x, y, x + TILE_W, y + TILE_H };

			new (&s_tiles[MAKE_TILE_ID(TILE_ID_PLAYER, offset++)]) Tile(res_id, bounds);
		}
	}

	// font
	{
		target::file_io::StreamReadFile file("lore20th/NanumBarunGothic.ttf");

		int buffer_size = file.getSize();

		if (buffer_size > 0)
		{
			unsigned char* p_buffer = new unsigned char[buffer_size];
			file.read(p_buffer, buffer_size);

			shared_ptr<yunjr::Font> p_font(new Font(p_buffer, buffer_size));

			delete[] p_buffer;

			{
				FontAttrib font_attrib;
				p_font->get(font_attrib);

				font_attrib.size = 28;
				p_font->set(font_attrib);
			}

			resource::setCurrentFont(p_font);
		}
	}
}

void yunjr::resource::setCurrentMapPos(int x, int y)
{
	s_ref_current_map_pos_x = x;
	s_ref_current_map_pos_y = y;
}

void yunjr::resource::getCurrentMapPos(int& x, int& y)
{
	x = s_ref_current_map_pos_x;
	y = s_ref_current_map_pos_y;
}

void yunjr::resource::setCurrentFont(shared_ptr<yunjr::Font> p_font)
{
	s_current_font = p_font;
}

yunjr::shared_ptr<yunjr::Font> yunjr::resource::getCurrentFont(void)
{
	return s_current_font;
}

void yunjr::resource::setMainWindow(yunjr::ControlWindow* p_main_window)
{
	s_ref_p_main_window = p_main_window; 
}

yunjr::ControlWindow* yunjr::resource::getMainWindow(void)
{
	return s_ref_p_main_window; 
}

void yunjr::resource::setFrameBuffer(const BufferDesc* p_buffer)
{
	s_ref_current_buffer = p_buffer;
}

const yunjr::BufferDesc* yunjr::resource::getFrameBuffer(void)
{
	return s_ref_current_buffer;
}

const yunjr::Tile& yunjr::resource::getTile(yunjr::TileId tile_id, int id_offset)
{
	return s_tiles[MAKE_TILE_ID(tile_id, id_offset)];
}

const FlatBoard32& yunjr::resource::getResimage(ResId res_id)
{
	assert(res_id.getType() == ResId::TAG_TYPE_IMAGE);
	
	switch (res_id.getDetail())
	{
	case ResId::TAG_TYPE_IMAGE_UI:
		return s_res_image->res_ui[res_id.getIndex()];
	case ResId::TAG_TYPE_IMAGE_TILE:
		return s_res_image->res_tile[0];
	case ResId::TAG_TYPE_IMAGE_CHARA:
	default:
		return s_res_image->res_movable[0];
	}
}

std::vector<yunjr::Chara*>& yunjr::resource::getCharaList(void)
{
	return s_chara_list;
}
