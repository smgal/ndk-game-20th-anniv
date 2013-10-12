
#ifndef __YUNJR_BASE_FONT_H__
#define __YUNJR_BASE_FONT_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include <vector>

namespace yunjr
{
	struct GlyphInfo
	{
		int left;
		int top;
		int x_advance;
		int y_advance;

		struct
		{
			int width;
			int height;
			int bytes_per_line;
			unsigned char* p_buffer;
		} glyph;
	};

	struct FontAttrib
	{
		unsigned int size;
		bool is_italic;
		bool is_bold;
	};

	struct KerningPair
	{
		wchar_t first;
		wchar_t second;
	};
}

namespace yunjr
{
	class Font
	{
	public:
		Font(const char* sz_file_name);
		Font(const void* p_buffer, int buffer_size);
		~Font();

		void set(const FontAttrib& font_attrib);
		void get(FontAttrib& out_font_attrib) const;
		void get(const KerningPair& kerning_pair, int& current_x) const;

		bool load(unsigned long code, GlyphInfo*& p_out_glyph_info) const;
		void unload(GlyphInfo*& p_glyph_info) const;

		bool load(const wchar_t* sz_text, int text_length, std::vector<GlyphInfo*>& out_glyph_info_list) const;
		void unload(std::vector<GlyphInfo*>& glyph_info_list) const;

	private:
		struct Impl;
		Impl* p_impl;
	};

} // namespace sm2d


#endif // #ifndef __YUNJR_BASE_FONT_H__
