
#ifndef __YUNJR_RES_H__
#define __YUNJR_RES_H__

#include "yunjr_base.h"

#include <vector>

namespace yunjr
{
	struct ResId
	{
		/*
			RES ID (8888)
			[ verfication | resource type | type | index ]

			1: verification byte
				1: resource type [image]
					1: type [tile]
					2: type [character]
				2: resource type [window]

				3: resource type [event]
		*/

		typedef unsigned long U32;

		enum Tag
		{
			// Verification bits
			// - MSB 00: invalid
			// - MSB 01: resource tag
			// - MSB 10: reserved
			// - MSB 11: string
			TAG_VERIFICATION_RES = 0x40000000UL,
			TAG_TYPE_IMAGE       = 0x01UL,
			TAG_TYPE_IMAGE_UI    = 0x01UL,
			TAG_TYPE_IMAGE_TILE  = 0x02UL,
			TAG_TYPE_IMAGE_CHARA = 0x03UL,
			TAG_TYPE_EVENT       = 0x02UL,

			TAG_VERIFICATION_STR = 0xC0000000UL,

			//?? where?
			TAG_VERIFICATION_MASK = 0xC0000000UL,
		};

		enum
		{
			MASK_TYPE    = 0x00FF0000UL,
			MASK_DETAIL  = 0x0000FF00UL,
			MASK_INDEX   = 0x000000FFUL,

			SHIFT_TYPE   = 16,
			SHIFT_DETAIL = 8,
			SHIFT_INDEX  = 0
		};

		U32 native_id;

		ResId()
			: native_id(0)
		{
		}

		ResId(U32 type, U32 detail, U32 index)
		{
			assert(type < 0x100);
			assert(detail < 0x100);
			assert(index < 0x100);

			native_id = TAG_VERIFICATION_RES | (type << SHIFT_TYPE) | (detail << SHIFT_DETAIL) | (index << SHIFT_INDEX);
		}

		ResId(const char* sz_string)
		{
			/* resource id from a string

			msb                                  lsb
			 |                                    |
			 VV 11111 22222 33333 44444 55555 66666
			
			 VV: verification tag
			 11111: 1st character
			 22222: 2nd character
			 33333: 3rd character
			 44444: 4th character
			 55555: 5th character
			 66666: 6th character

			5-bit composition from a character

			  0: etc
			  1: 'A' or 'a'
			 26: 'Z' or 'z'
			 27: '1'
			 28: '2'
			 29: '3'
			 30: '4'
			 31: '-' or '_'
			*/

			U32 encode[6] = { 0, 0, 0, 0, 0, 0 };

			for (int i = 0; i < 6; i++)
			{
				int character = *sz_string++;

				if (character == 0)
					break;

				U32& code = encode[i];

				if ((character >= 'A') && (character <= 'Z'))
					code = character - 'A' + 1;
				else if ((character >= 'a') && (character <= 'z'))
					code = character - 'a' + 1;
				else if ((character >= '1') && (character <= '4'))
					code = character - '1' + 27;
				else if ((character == '-') || (character == '_'))
					code = 31;
				else
					code = 0;
			}

			native_id = TAG_VERIFICATION_STR | (encode[0] << 25) | (encode[1] << 20) | (encode[2] << 15) | (encode[3] << 10) | (encode[4] << 5) | (encode[5]);
		}

		inline U32 getType(void) const
		{
			return (native_id & MASK_TYPE) >> SHIFT_TYPE;
		}

		inline U32 getDetail(void) const
		{
			return (native_id & MASK_DETAIL) >> SHIFT_DETAIL;
		}

		inline U32 getIndex(void) const
		{
			return (native_id & MASK_INDEX) >> SHIFT_INDEX;
		}
	};

	inline bool operator==(ResId a, ResId b)
	{
		return (a.native_id == b.native_id);
	}

	inline bool operator!=(ResId a, ResId b)
	{
		return (a.native_id != b.native_id);
	}

	///////////////////////////////////////////////////////////////////////////////

	template <typename IdType>
	struct TileBase
	{
		struct Bounds
		{
			int x1, y1, x2, y2;
		};

		struct Offset
		{
			int x, y;
		};

		IdType id_res;
		Bounds bounds;
		Offset origin;

		TileBase()
		{
		}

		TileBase
		(
			IdType        in_id_res,
			const Bounds& in_bounds
		)
			: id_res(in_id_res)
			, bounds(in_bounds)
		{
			origin.x = 0;
			origin.y = 0;
		}

		TileBase
		(
			IdType        in_id_res,
			const Bounds& in_bounds,
			const Offset& in_origin
		)
			: id_res(in_id_res)
			, bounds(in_bounds)
			, origin(in_origin)
		{
		}
	};

	typedef TileBase<ResId> Tile;

	///////////////////////////////////////////////////////////////////////////////

	const int NUM_TILE_ID_GROUND_W = (1344 / TILE_W);
	const int NUM_TILE_ID_GROUND_H = (96 / TILE_H);
	const int NUM_TILE_ID_GROUND = NUM_TILE_ID_GROUND_W * NUM_TILE_ID_GROUND_H;
	const int NUM_TILE_ID_PLAYER = (672 / TILE_W) * (24 / TILE_H);

	enum TileId
	{
		TILE_ID_MIN    = 0,
		TILE_ID_GROUND = TILE_ID_MIN,
		TILE_ID_PLAYER = TILE_ID_GROUND + NUM_TILE_ID_GROUND,
		TILE_ID_MAX    = TILE_ID_PLAYER + NUM_TILE_ID_PLAYER
	};

	///////////////////////////////////////////////////////////////////////////////

	class Font;

	struct Control;
	struct ControlWindow;
	struct Chara;

	namespace resource
	{
		void init();

		void setCurrentMapPos(int x, int y);
		void getCurrentMapPos(int& x, int& y);

		void setCurrentFont(shared_ptr<yunjr::Font> p_font);
		shared_ptr<yunjr::Font> getCurrentFont(void);

		void setMainWindow(yunjr::ControlWindow* p_window);
		yunjr::ControlWindow* getMainWindow(void);

		void setFrameBuffer(const BufferDesc* p_buffer);
		const BufferDesc* getFrameBuffer(void);

		const Tile& getTile(TileId tile_id, int id_offset = 0);
		const FlatBoard32& getResimage(ResId res_id);

		std::vector<Chara*>& getCharaList(void);
	}
}

#endif
