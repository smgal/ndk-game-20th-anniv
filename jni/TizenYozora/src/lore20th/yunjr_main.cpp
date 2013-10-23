
#include "yunjr_base.h"
#include "yunjr_base_gfx.h"
#include "yunjr_base_font.h"
#include "yunjr_base_text.h"

#include "yunjr_class.h"
#include "yunjr_class_chara.h"
#include "yunjr_class_control_lv1.h"
#include "yunjr_class_control_lv2.h"
#include "yunjr_class_game_state.h"
#include "yunjr_class_map.h"
#include "yunjr_class_map_event.h"
#include "yunjr_class_pc_party.h"

#include "yunjr_res.h"
#include "yunjr_res_string.h"

#include <new>
#include <vector>
#include <string.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// local

////////////////////////////////////////////////////////////////////////////////
// type definition

namespace yunjr
{
	typedef flat_board::FlatBoard<flat_board::PIXELFORMAT_ARGB8888> FlatBoard32;
	typedef unsigned long TargetPixel;
}

////////////////////////////////////////////////////////////////////////////////
// local

namespace yunjr
{
	namespace res_collection
	{
		namespace
		{
			std::vector<Chara*> chara_list;
		}

		std::vector<Chara*>& getCharaList(void)
		{
			return chara_list;
		}
	}

	// for updating
	template <>
	void yunjr::Operator<ControlId, unsigned long>::operator()(ControlId obj)
	{
		(obj.second) ? obj.second->update(param) : (void)0;
	}

	// for rendering
	template <>
	void yunjr::Operator<ControlId, FlatBoard32*>::operator()(ControlId obj)
	{
		*param << param->getDefaultFillRect();
		*param << param->getDefaultBitBlt();

		obj.second->render(*param);
	}
}

////////////////////////////////////////////////////////////////////////////////
// main

namespace
{
	yunjr::ControlWindow* s_p_main_window = 0;

	struct MapCallback
	{
		static void actBlock(int x1, int y1, bool bUseless)
		{
		}
		static void actMove(int x1, int y1, bool bEncounter)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			party.move(x1, y1);
		}
		static void actEvent(int x1, int y1, bool bUseless)
		{
		}
		static void actEnter(int x1, int y1, bool bUseless)
		{
		}
		static void actSign(int x1, int y1, bool bUseless)
		{
		}
		static void actWater(int x1, int y1, bool bUseless)
		{
		}
		static void actSwamp(int x1, int y1, bool bUseless)
		{
		}
		static void actLava(int x1, int y1, bool bUseless)
		{
		}
		static void actTalk(int x1, int y1, bool bUseless)
		{
			/*
			party.face(x1, y1);

			window[WINDOWTYPE_MAP]->setUpdateFlag();
			window[WINDOWTYPE_MAP]->display();
			window[WINDOWTYPE_SUBMAP]->display();

			LoreConsole::getConsole().clear();
			*/
			yunjr::PcParty& party = yunjr::game::object::getParty();

			yunjr::map_event::occur(yunjr::map_event::TYPE_TALK, 0, party.x + x1, party.y + y1);
		}
	};
}

void yunjr::init(const char* sz_id)
{
	// for Android (regarding the life cycle of Activity)
	{
		delete s_p_main_window;
		deleteVector(res_collection::getCharaList());

		yunjr::GameState::getMutableInstance().reset();
	}

	Resource& resrouce = Resource::getMutableInstance();

	{
		s_p_main_window = ControlWindow::newInstance(720, 1280);

		s_p_main_window->addChild(std::make_pair("WAK-01", ControlWaku::newInstance()));
		s_p_main_window->addChild(std::make_pair("MAP-01", ControlMap::newInstance(20*2, 20*2, 216*2, 203*2)));
		s_p_main_window->addChild(std::make_pair("CON-01", ControlConsole::newInstance(267*2, 21*2, 350*2, 201*2, 20*2, 24*2, 26*2, 24*2)));
		s_p_main_window->addChild(std::make_pair("PAN-01", ControlPanel::newInstance()));

		resrouce.setMainWindow(s_p_main_window);
	}

	{
		const Resource& resource = Resource::getInstance();

		ControlConsole* p_console = (ControlConsole*)resource.getMainWindow()->findControl("CON-01");

		if (p_console)
		{
//??			p_console->setText(yunjr::res::TEST_STRING1, yunjr::res::TEST_STRING2, yunjr::res::TEST_STRING3);
		}
	}

	{
		Map& map = game::object::getMap();

		map.act_func[Map::ACT_BLOCK] = &MapCallback::actBlock;
		map.act_func[Map::ACT_MOVE]  = &MapCallback::actMove;
		map.act_func[Map::ACT_EVENT] = &MapCallback::actEvent;
		map.act_func[Map::ACT_ENTER] = &MapCallback::actEnter;
		map.act_func[Map::ACT_SIGN]  = &MapCallback::actSign;
		map.act_func[Map::ACT_WATER] = &MapCallback::actWater;
		map.act_func[Map::ACT_SWAMP] = &MapCallback::actSwamp;
		map.act_func[Map::ACT_LAVA]  = &MapCallback::actLava;
		map.act_func[Map::ACT_TALK]  = &MapCallback::actTalk;
	}

	{
		//game::map::loadFromFile("lore20th/K_DEN2.MAP");
		yunjr::map_event::load("vesper");
	}

	{
		std::vector< ::yunjr::Chara*>& chara_list = yunjr::res_collection::getCharaList();

		chara_list.push_back(Playable::newInstance());
	}

}

void yunjr::done()
{
	delete s_p_main_window;
	s_p_main_window = 0;

	deleteVector(res_collection::getCharaList());
}

bool yunjr::loop(const BufferDesc& buffer_desc)
{
	yunjr::GameState& game_state = yunjr::GameState::getMutableInstance();

	if (game_state.isTerminating())
		return false;

	if (buffer_desc.bits_per_pixel != 32)
		return false;

	Resource::getMutableInstance().setFrameBuffer(&buffer_desc);

	int dest_buffer_pitch = (buffer_desc.bytes_per_line << 3) / buffer_desc.bits_per_pixel;
	FlatBoard32 dest_board((FlatBoard32::Pixel*)buffer_desc.p_start_address, buffer_desc.width, buffer_desc.height, dest_buffer_pitch);

	// Galaxy Note I problem: The default filling value of Android bitmap buffer is not defined.
	{
		static bool is_first = true;

		if (is_first)
		{
			dest_board.fillRect(0, 0, buffer_desc.width, buffer_desc.height, 0xFF000000);
			is_first = false;
		}
	}

	// update
	{
		unsigned long current_tick = 0;

		game_state.update(current_tick);

		s_p_main_window->update(current_tick);
	}

	// render
	{
		s_p_main_window->render(dest_board);
	}

	Resource::getMutableInstance().setFrameBuffer(0);

	return true;
}
