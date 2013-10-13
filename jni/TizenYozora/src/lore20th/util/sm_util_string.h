
#ifndef __SM_UTIL_STRING_H
#define __SM_UTIL_STRING_H

#pragma warning(disable: 4996)

#include "sm_util_sena.h"

//#include <string.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// definition

#if 1
#define CT_ASSERT(x, msg) \
	typedef int __CT_ASSERT ## msg ## __ [(x) ? 1 : -1];
#else
#define CT_ASSERT(x, msg)
#endif

#if defined(_WIN32)
#	define SPRINTF _snprintf
#else
#	define SPRINTF snprintf
#endif

////////////////////////////////////////////////////////////////////////////////
// namespace

namespace smutil
{

class IntToStr
{
	char m_s[32];
public:
	IntToStr(int value)
	{
		SPRINTF(m_s, 32, "%d", value);
	}
	const char* operator()(void)
	{
		return m_s;
	}
};

template <unsigned int MAX_STRLEN>
class basic_string
{
private:
	char m_string[MAX_STRLEN+1];

public:
	basic_string(void)
	{
		m_string[0] = 0;
	}
	basic_string(const char* lpsz)
	{      
		sena::strncpy(m_string, lpsz, MAX_STRLEN);
	}

	operator const char*() const
	{
		return m_string;
	}

	const basic_string& operator=(const char* lpsz)
	{      
		sena::strncpy(m_string, lpsz, MAX_STRLEN);
		return *this;
	}

	const basic_string& operator+=(const char* lpsz)
	{      
		sena::strncat(m_string, lpsz, MAX_STRLEN);
		return *this;
	}

	template <unsigned int MAX_STRLEN>
	friend bool operator==(const basic_string<MAX_STRLEN>& str1, const basic_string<MAX_STRLEN>& str2)
	{      
		return sena::strndiff(str1.m_string, str2.m_string, MAX_STRLEN);
	}

	bool operator==(const basic_string<MAX_STRLEN>& str) const
	{      
		return sena::strndiff(m_string, str.m_string, MAX_STRLEN);
	}

	bool operator<(const char* lpsz) const
	{
		return (sena::strncmp(m_string, lpsz, MAX_STRLEN) < 0);
	}

	void copyToFront(const basic_string& lpsz)
	{      
		char m_temp[MAX_STRLEN+1];
		sena::strncpy(m_temp, m_string, MAX_STRLEN);
		sena::strncpy(m_string, lpsz.m_string, MAX_STRLEN);
		sena::strncat(m_string, m_temp, MAX_STRLEN);
	}
};

typedef basic_string<255> string;
typedef basic_string<511> long_string;
typedef basic_string<1023> long_long_string;

}

#endif // #ifndef __SM_UTIL_STRING_H
