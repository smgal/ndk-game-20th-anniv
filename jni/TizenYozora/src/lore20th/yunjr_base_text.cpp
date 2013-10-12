
#include "yunjr_base.h"
#include "yunjr_base_text.h"
#include "yunjr_base_text_impl.h"

#include <string.h>

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
