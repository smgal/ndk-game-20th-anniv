 
#ifndef __YUNJR_CLASS_GAME_STATE_H__
#define __YUNJR_CLASS_GAME_STATE_H__

#include "yunjr_base.h"

namespace yunjr
{
	struct GameState
	{
		typedef target::KEY Key;

		bool is_terminating;
		target::InputUpdateInfo current_input_info;

		GameState()
			: is_terminating(false)
		{
		}

		void reset(void)
		{
			is_terminating = false;
		}

		void update(unsigned long tick)
		{
			target::InputDevice* p_input_device = target::getInputDevice();

			if (p_input_device)
			{
				current_input_info = p_input_device->update();
				current_input_info.key_down_flag = 0;
			}
			else
			{
				current_input_info.is_touched = false;
				current_input_info.key_pressed_flag = 0;
				current_input_info.key_down_flag = 0;
			}
		}

		void terminate(void)
		{
			is_terminating = true;
		}

		void setKeyPressed(Key key)
		{
			current_input_info.key_pressed_flag |= key;
		}

		bool isTerminating(void) const
		{
			return is_terminating;
		}

		bool checkKeyDown(Key key) const
		{
			return ((current_input_info.key_down_flag & key) != 0);
		}

		bool checkKeyPressed(Key key) const
		{
			return ((current_input_info.key_pressed_flag & key) != 0);
		}

		static const GameState& getInstance(void);
		static GameState& getMutableInstance(void);
	};
}

#endif // #ifndef __YUNJR_CLASS_GAME_STATE_H__
