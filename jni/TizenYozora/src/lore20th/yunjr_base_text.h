
#ifndef __YUNJR_BASE_TEXT_H__
#define __YUNJR_BASE_TEXT_H__

namespace yunjr
{
	struct Text
	{
	public:
		Text(const char* sz_text_utf8);
		Text(const char* sz_text_utf8, size_t length);

		Text(const wchar_t* sz_text);
		Text(const wchar_t* sz_text, size_t length);

		~Text();

		struct Impl;

	private:
		Impl* p_impl;
	};

}

#endif // #ifndef __YUNJR_BASE_FONT_H__
