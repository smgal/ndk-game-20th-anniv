 
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

			current_input_info.is_touched = false;
			current_input_info.key_down_flag = 0;
			current_input_info.key_pressed_flag = 0;
		}

		void update(unsigned long tick)
		{
			target::InputDevice* p_input_device = target::getInputDevice();

			if (p_input_device)
			{
				current_input_info = p_input_device->update();
				current_input_info.key_down_flag = 0;

				if (current_input_info.is_touched)
				{
					const int TOUCH_W = 125;
					const int TOUCH_H = 125;

					static const struct TouchToKey
					{
						int x, y;
						target::KEY key;
					} TOUCH_TO_KEY[] =
					{
						{        0,                    0, target::KEY_SELECT },
						{     1120, 720-int(TOUCH_H*3.5), target::KEY_A      },
						{     1120,                    0, target::KEY_B      },
						{     1120,        720-TOUCH_H*2, target::KEY_UP     },
						{     1120,        720-TOUCH_H*1, target::KEY_DOWN   },
						{ 50+125*0,            720-125*1, target::KEY_LEFT   },
						{ 50+125*1,            720-125*1, target::KEY_RIGHT  }
					};

					const TouchToKey* p = &TOUCH_TO_KEY[0];
					const TouchToKey* p_end = p + sizeof(TOUCH_TO_KEY) / sizeof(TOUCH_TO_KEY[0]);

					int x = current_input_info.touch_pos.x;
					int y = current_input_info.touch_pos.y;

					for ( ; p < p_end; ++p)
					{
						if (x >= p->x && x < p->x + TOUCH_W && y >= p->y && y < p->y + TOUCH_H)
						{
							current_input_info.key_pressed_flag |= p->key;
						}
					}
				}
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
