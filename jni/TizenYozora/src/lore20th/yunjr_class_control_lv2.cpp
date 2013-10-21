
#include "yunjr_base.h"
#include "yunjr_base_gfx.h"

#include "yunjr_class.h"
#include "yunjr_class_control_lv2.h"
#include "yunjr_class_chara.h"

#include "yunjr_res_game.h"

#include <algorithm>
#include <string.h>

namespace yunjr
{
	// ScaleTraits
	template <int Scale>
	struct ScaleTraits
	{
		template <typename Pixel>
		void expandW(Pixel*& p_d, Pixel*& p_s);

		template <typename Pixel>
		void expandH(Pixel*& p_d, int src_ppl, int dst_ppl);
	};

	template <>
	struct ScaleTraits<2>
	{
		enum { SCALE = 2 };
		enum { GUIDE_WIDTH  = 9 * TILE_W };
		enum { GUIDE_HEIGHT = 9 * TILE_H };

		template <typename Pixel>
		static inline void expandW(Pixel*& p_d, Pixel*& p_s)
		{
			*p_d++ = *p_s;
			*p_d++ = *p_s++;
		}

		template <typename Pixel>
		static inline void expandH(Pixel* p_d, int src_ppl, int dst_ppl)
		{
			memcpy(p_d + dst_ppl, p_d, src_ppl * sizeof(Pixel) * SCALE);
		}
	};

	template <>
	struct ScaleTraits<3>
	{
		enum { SCALE = 3 };
		enum { GUIDE_WIDTH  = 9 * TILE_W };
		enum { GUIDE_HEIGHT = 9 * TILE_H };

		template <typename Pixel>
		static inline void expandW(Pixel*& p_d, Pixel*& p_s)
		{
			*p_d++ = *p_s;
			*p_d++ = *p_s;
			*p_d++ = *p_s++;
		}

		template <typename Pixel>
		static inline void expandH(Pixel* p_d, int src_ppl, int dst_ppl)
		{
			memcpy(p_d + dst_ppl, p_d, src_ppl * sizeof(Pixel) * SCALE);
			// if you apply alpha attenuation to this line, interlace effect occurs.
			memcpy(p_d + dst_ppl*2, p_d, src_ppl * sizeof(Pixel) * SCALE);
		}
	};
}


yunjr::ControlMap::ControlMap()
{
}

struct MapCallback
{
	static void actBlock(int x1, int y1, bool bUseless)
	{
	}
	static void actMove(int x1, int y1, bool bEncounter)
	{
	}
	static void actEvent(int x1, int y1, bool bUseless)
	{
	}
	static void actEnter(int x1, int y1, bool bUseless)
	{
	}
	static void actSign(int x1, int y1, bool bUseless)
	{
	}
	static void actWater(int x1, int y1, bool bUseless)
	{
	}
	static void actSwamp(int x1, int y1, bool bUseless)
	{
	}
	static void actLava(int x1, int y1, bool bUseless)
	{
	}
	static void actTalk(int x1, int y1, bool bUseless)
	{
	}
};

yunjr::ControlMap* yunjr::ControlMap::newInstance(int x, int y, int width, int height)
{
	enum { SCALE = MAP_SCALE_RATIO };
	
	struct AttributeMap: public Visible::Attribute
	{
		struct { int x, y; } pos;
		struct { int width, height; } size;

		AttributeMap(int x, int y, int width, int height)
		{
			pos.x = x;
			pos.y = y;
			size.width = width;
			size.height = height;

			int buffer_w = ScaleTraits<SCALE>::GUIDE_WIDTH;
			int buffer_h = ScaleTraits<SCALE>::GUIDE_HEIGHT;

			auto_buffer.bind(new FlatBoard32::Pixel[buffer_w * buffer_h]);
			new (&map_board) FlatBoard32(auto_buffer.get(), buffer_w, buffer_h, buffer_w);
		}

		auto_ptr<FlatBoard32::Pixel[]> auto_buffer;
		FlatBoard32 map_board;
	};

	struct ShapeMap: public Visible::Shape
	{
		virtual void render(Visible* p_this, FlatBoard32& dest_board) const
		{
			AttributeMap& attribute = *((AttributeMap*)p_this->getAttribute());

			attribute.map_board << gfx::bitBlt;

			int buffer_width = attribute.map_board.getBufferDesc().width;
			int buffer_height = attribute.map_board.getBufferDesc().height;
			int buffer_x = (dest_board.getBufferDesc().width - buffer_width) >> 1;
			int buffer_y = (dest_board.getBufferDesc().height - buffer_height) >> 1;

			{
				// map
				attribute.map_board.fillRect(0, 0, buffer_width, buffer_height, 0xFF103050);

				int NUM_MAP_X = (ScaleTraits<SCALE>::GUIDE_WIDTH + (TILE_W-1)) / TILE_W;
				int NUM_MAP_Y = (ScaleTraits<SCALE>::GUIDE_HEIGHT + (TILE_H-1)) / TILE_H;

				int map_offset_x;
				int map_offset_y;

				Resource::getCurrentMapPos(map_offset_x, map_offset_y);

				int display_offset_x = - map_offset_x % TILE_W;
				int display_offset_y = - map_offset_y % TILE_H;

				map_offset_x = map_offset_x / TILE_W - NUM_MAP_Y / 2;
				map_offset_y = map_offset_y / TILE_H - NUM_MAP_X / 2;

				for (int y = 0; y <= NUM_MAP_Y; y++)
				for (int x = 0; x <= NUM_MAP_X; x++)
				{
					int x_pos = x * TILE_W + 0;
					int y_pos = y * TILE_H + 0;

					int ix_map = res::game::map(x + map_offset_x, y + map_offset_y);
					ix_map += NUM_TILE_ID_GROUND_W * res::game::map.type;

					gfx::drawTile(attribute.map_board, x_pos + display_offset_x, y_pos + display_offset_y, TILE_ID_GROUND, ix_map);
				}

				// playable
				std::vector<Chara*>& chara_list = yunjr::res_collection::getCharaList();
				std::for_each(chara_list.begin(), chara_list.end(), Operator<Chara*, FlatBoard32*>(&attribute.map_board));
			}

			const BufferDesc* p_buffer_desc = Resource::getInstance().getFrameBuffer();

			if (p_buffer_desc)
			{
				int src_w = attribute.map_board.getBufferDesc().width;
				int src_h = attribute.map_board.getBufferDesc().height;
				int src_p = src_w; // packed buffer

				int dst_w = p_buffer_desc->width;
				int dst_h = p_buffer_desc->height;
				int dst_p = (p_buffer_desc->bytes_per_line << 3) / p_buffer_desc->bits_per_pixel;

				FlatBoard32::Pixel* p_src = attribute.auto_buffer.get();
				FlatBoard32::Pixel* p_dst = (FlatBoard32::Pixel*)p_buffer_desc->p_start_address;

				p_dst += attribute.pos.y * dst_p + attribute.pos.x;

				// assert(...);

				int offset_y = (attribute.size.height - ScaleTraits<SCALE>::SCALE * buffer_height) / 4 * 2;

				for (int y = 0; y < src_h; y++)
				{
					int revised_y = ScaleTraits<SCALE>::SCALE * y + offset_y;

					if (revised_y >= attribute.size.height)
					{
						break;
					}
					if (revised_y >= 0)
					{
						FlatBoard32::Pixel* p_s = p_src + y * src_p;
						FlatBoard32::Pixel* p_d = p_dst + revised_y * dst_p;
						FlatBoard32::Pixel* p_m = p_d;

						for (int x = 0; x < src_w; x++)
							ScaleTraits<SCALE>::expandW(p_d, p_s);

						ScaleTraits<SCALE>::expandH(p_m, src_p, dst_p);
					}
				}
			}
		}
	};

	struct UpdateMap: public Visible::Update
	{
		virtual bool update(Visible* p_this, unsigned long tick)
		{
			std::vector<Chara*>& chara_list = yunjr::res_collection::getCharaList();
			std::for_each(chara_list.begin(), chara_list.end(), Operator<Chara*, unsigned long>(tick));

			Playable* p_chara = (Playable*)chara_list[0];
			assert(p_chara);
			Resource::setCurrentMapPos(p_chara->getPosX(), p_chara->getPosY());

			return true;
		}
	};

	ControlMap* p_map = new ControlMap();

	*p_map << new AttributeMap(x, y, width, height) << new ShapeMap() << new UpdateMap();

	return p_map;
}
