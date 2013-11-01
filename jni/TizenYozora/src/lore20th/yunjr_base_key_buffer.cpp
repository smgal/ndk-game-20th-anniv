
#include "yunjr_base_key_buffer.h"

#include "util/sm_util_sena.h"

#include "../flat_board/target_dep.h"

////////////////////////////////////////////////////////////////////////////////

namespace
{
	// KeyBuffer는 싱글톤이므로 그에 맞는 위치가 필요할지도..
	static bool           s_isKeyPressed = false;
	static unsigned short s_pressedKey;
	static unsigned long  s_repeatTime;
	const  unsigned long  c_delayTime = 75;
}

namespace
{
	#define CASE(key1, key2) \
		case target::KEY_##key1: key = yunjr::KEY_##key2; break

	yunjr::KEY convertTargetToYunjr(target::KEY avejKey)
	{
		yunjr::KEY key = yunjr::KEY_UNKNOWN;

		switch (avejKey)
		{
			CASE(MENU,    SYS_START);
			CASE(SELECT,  SYS_SELECT);
			CASE(UP,      DIR_UP);
			CASE(LEFT,    DIR_LEFT);
			CASE(DOWN,    DIR_DOWN);
			CASE(RIGHT,   DIR_RIGHT);
			CASE(A,   BTN_A);
			CASE(B,   BTN_B);
			CASE(X,   BTN_C);
			CASE(Y,   BTN_D);
			CASE(L,   BTN_L1);
			CASE(R,   BTN_R1);
			CASE(VOLUP,   VOL_UP);
			CASE(VOLDOWN, VOL_DOWN);

			default:
			{
				if (avejKey >= 'A' && avejKey <= 'Z')
					key = yunjr::KEY(int(yunjr::KEY_A) + (avejKey - 'A'));
				else if (avejKey >= 'a' && avejKey <= 'z')
					key = yunjr::KEY(int(yunjr::KEY_A) + (avejKey - 'a'));
			}
		}

		return key;
	}

	bool OnKeyDown(unsigned short avej_key, unsigned long state)
	{
		yunjr::KEY key = convertTargetToYunjr(target::KEY(avej_key));

		return yunjr::KeyBuffer::getKeyBuffer().setKeyDown(key);
	}

	bool OnKeyUp(unsigned short avej_key, unsigned long state)
	{
		yunjr::KEY key = convertTargetToYunjr(target::KEY(avej_key));

		return yunjr::KeyBuffer::getKeyBuffer().setKeyUp(key);
	}
/*??
	avej::input::EventCallback s_callback =
	{
		OnKeyDown,
		OnKeyUp,
	};
*/
}

////////////////////////////////////////////////////////////////////////////////

//?? 싱글톤임이 보장되어야 함
yunjr::KeyBuffer::KeyBuffer(void)
	: m_key_head_ptr(0)
	, m_key_tail_ptr(0)
{
	sena::memclear(m_key_buffer, sizeof(m_key_buffer));
	sena::memclear(m_key_map, sizeof(m_key_map));
}

yunjr::KeyBuffer::~KeyBuffer(void)
{
}

bool yunjr::KeyBuffer::setKeyDown(Key key)
{
	// auto pressed key 구현
	if (!s_isKeyPressed)
	{
		s_isKeyPressed = true;
		s_pressedKey   = key;
		s_repeatTime   = target::system::getTicks() + c_delayTime*3;
	}

	if (key < KEY_MAX)
		m_key_map[key] = 1;

	return m_pushKeyChar(key);
}

bool yunjr::KeyBuffer::setKeyUp(Key key)
{
	// auto pressed key 구현
	if (s_isKeyPressed)
	{
		s_isKeyPressed = false;
	}

	if (key < KEY_MAX)
		m_key_map[key] = 0;

	return true;
}

bool yunjr::KeyBuffer::isKeyPressed(void)
{
	this->processAutoRepeat();

//??	avej::input::processMessage(s_callback);

	return  (m_key_head_ptr != m_key_tail_ptr);
}

yunjr::KeyBuffer::Key yunjr::KeyBuffer::getKey()
{
	Key key = -1;

	if (isKeyPressed())
	{
		key = m_key_buffer[m_key_head_ptr];
		m_key_head_ptr = m_increasePtr(m_key_head_ptr);
	}

	return key;
}

bool yunjr::KeyBuffer::isKeyPressing(Key key)
{
	if (key >= KEY_MAX)
		return false;

	return (m_key_map[key] > 0);
}

void yunjr::KeyBuffer::processAutoRepeat(void)
{
	unsigned long current_tick = target::system::getTicks();

	if (s_isKeyPressed)
	{
		if (s_repeatTime <= current_tick)
		{
			yunjr::KeyBuffer::getKeyBuffer().setKeyUp(s_pressedKey);
			yunjr::KeyBuffer::getKeyBuffer().setKeyDown(s_pressedKey);
			s_repeatTime = current_tick + c_delayTime;
		}
	}
}

int yunjr::KeyBuffer::m_increasePtr(int ptr)
{
	if (++ptr >= 100)
		ptr -= 100;

	return ptr;
}

bool yunjr::KeyBuffer::m_pushKeyChar(Key key)
{
	if (m_increasePtr(m_key_tail_ptr) != m_key_head_ptr)
	{
		m_key_buffer[m_key_tail_ptr] = key;
		m_key_tail_ptr = m_increasePtr(m_key_tail_ptr);
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////

namespace
{
	sena::auto_ptr<yunjr::KeyBuffer> s_p_key_buffer;
}

yunjr::KeyBuffer& yunjr::KeyBuffer::getKeyBuffer(void)
{
	if (s_p_key_buffer.get() == 0)
	{
		s_p_key_buffer.bind(new yunjr::KeyBuffer);
	}

	return *s_p_key_buffer.get();
}


#if 0 //??
#include "util/sm_util_sena.h"
#include "util/sm_util.h"

#include "hd_base_config.h"
#include "hd_base_key_buffer.h"

#include <avej.h>

#define CLEAR_MEMORY(var) memset(var, 0, sizeof(var));




#endif
