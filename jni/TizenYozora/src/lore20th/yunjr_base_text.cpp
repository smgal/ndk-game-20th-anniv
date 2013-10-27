
#include "yunjr_base.h"
#include "yunjr_base_text.h"
#include "yunjr_base_text_impl.h"

#include "util/sm_util_sena.h"

#include <new>
#include <string.h>

yunjr::Text::Text(const char* sz_text_utf8)
{
	new (this) Text(sz_text_utf8, 0x7FFFFFFF);
}

yunjr::Text::Text(const char* sz_text_utf8, size_t length)
	: p_impl(0)
{
	// revise string legnth parameter
	{
		size_t real_length = 0;

		const char* p = sz_text_utf8;

		while (*p++ && real_length < length)
			++real_length;

		length = real_length;
	}

	sena::auto_ptr<wchar_t[]> wchar_buffer(new wchar_t[length+1]);

	const char* p_src     = sz_text_utf8;
	wchar_t*    p_dst     = wchar_buffer.get();
	wchar_t*    p_dst_end = p_dst + length;

	while (*p_dst && p_dst < p_dst_end)
		*p_dst++ = wchar_t(*p_src++);

	*p_dst = 0;

	p_impl = new Impl(wchar_buffer.get(), length);
}

yunjr::Text::Text(const wchar_t* sz_text)
	: p_impl(0)
{
	size_t length = 0;

	const wchar_t* p = sz_text;

	while (*p++)
		++length;

	p_impl = new Impl(sz_text, length);
}

yunjr::Text::Text(const wchar_t* sz_text, size_t length)
	: p_impl(new Impl(sz_text, length))
{
}

yunjr::Text::~Text()
{
	delete p_impl;
}
