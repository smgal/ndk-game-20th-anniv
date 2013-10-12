
#ifndef __YUNJR_BASE_GFX_H__
#define __YUNJR_BASE_GFX_H__

#include "yunjr_base.h"
#include "yunjr_base_font.h"
#include "yunjr_base_text.h"

#include "yunjr_res.h"

namespace yunjr
{
	namespace gfx
	{
		void fillRect(unsigned long* p_dest_32, int w, int h, int dest_pitch, unsigned long color, unsigned long alpha);
		void bitBlt(unsigned long* p_dest_32, int w, int h, int dest_pitch, unsigned long* p_sour_32, int sour_pitch, unsigned long alpha, const flat_board::Optional<unsigned long>& chroma_key);

		void drawTile(FlatBoard32& dest_board, int x, int y, TileId tile_id, int id_offset = 0);
	}

	namespace gfx
	{
		struct TextBoard
		{
			typedef unsigned long Pixel;

			Pixel* m_p_buffer;
			int    m_width;
			int    m_height;
			int    m_pitch;

			TextBoard(Pixel* p_buffer, int width, int height, int pitch);

			void renderTextFx(int pos_x, int pos_y, const Text& text, Color32 color1, Color32 color2);
			void renderText(int pos_x, int pos_y, const Text& text, Color32 color1, Color32 color2);
			void renderText(int pos_x, int pos_y, const std::vector<yunjr::GlyphInfo*>& glyph_info, Color32 color1, Color32 color2);
		};
	}
}

#endif // #ifndef __YUNJR_BASE_GFX_H__
