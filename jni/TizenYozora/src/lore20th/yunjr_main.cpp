
#include "yunjr_base.h"
#include "yunjr_base_gfx.h"
#include "yunjr_base_font.h"
#include "yunjr_base_text.h"

#include "yunjr_class.h"
#include "yunjr_class_console.h"
#include "yunjr_class_chara.h"
#include "yunjr_class_control_lv1.h"
#include "yunjr_class_control_lv2.h"
#include "yunjr_class_game_state.h"
#include "yunjr_class_map.h"
#include "yunjr_class_map_event.h"
#include "yunjr_class_pc_party.h"
#include "yunjr_class_pc_player.h"
#include "yunjr_class_pc_functor.h"

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

	void detectGameOver(void)
	{
		sena::vector<yunjr::shared::PcPlayer>& player_list = yunjr::game::object::getPlayerList();

		// 전처리
		sena::for_each(player_list.begin(), player_list.end(), FnCheckCondition<yunjr::shared::PcPlayer>());
/*??
		// 실제 생존해 있는 숫자를 확인
		int num_alive = NUM_OF_CONSCIOUS_PLAYER(player_list);

		if (num_alive == 0)
			proccessGameOver(EXITCODE_BY_ACCIDENT);
*/
	}

	struct MapCallback
	{
		static void actBlock(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			party.face(x1, y1);
		}
		static void actMove(int x1, int y1, bool bEncounter)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			party.move(x1, y1);
/* //??
			window[WINDOWTYPE_MAP]->setUpdateFlag();

			int num_updated;

			num_updated = sena::for_each(player.begin(), player.end(), FnTimeGoes<PcPlayer*>()).Result();

			// player의 디스플레이 된 수치에 변화가 생겼다면
			if (num_updated > 0)
			{
				window[WINDOWTYPE_STATUS]->setUpdateFlag();
			}

			detectGameOver();
			
			// 독심술을 사용 중이라면 사용 시간을 감소 시킴
			if (party.ability.mind_control > 0)
				--party.ability.mind_control;

			if (bEncounter)
			{
				if (smutil::random(party.encounter*20) == 0)
					encounterEnemy();
			}
*/

		}
		static void actEvent(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();
/* //??
			party.move(x1, y1);

			window[WINDOWTYPE_MAP]->setUpdateFlag();

			LoreConsole::getConsole().clear();

			map_event::occur(map_event::TYPE_EVENT, 0, party.x, party.y);
*/
		}
		static void actEnter(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			yunjr::LoreConsole::getConsole().clear();

			yunjr::map_event::occur(yunjr::map_event::TYPE_ENTER, 0, party.x + x1, party.y + y1);
		}
		static void actSign(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			party.face(x1, y1);

			yunjr::LoreConsole& console = yunjr::LoreConsole::getConsole();

			console.clear();
			console.setTextColorIndex(7);
			console.write("표지판에는 다음과 같이 적혀 있었다.");
			console.write("");

			yunjr::map_event::occur(yunjr::map_event::TYPE_SIGN, 0, party.x + x1, party.y + y1);
		}
		static void actWater(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();
/* //??
			if (party.ability.walk_on_water > 0)
			{
				--party.ability.walk_on_water;
				party.move(x1, y1);
				window[WINDOWTYPE_MAP]->setUpdateFlag();

				if (smutil::random(party.encounter*30) == 0)
					encounterEnemy();
			}
			else
			{
				party.face(x1, y1);
			}
*/
		}
		static void actSwamp(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			actMove(x1, y1, false);

			// walkOnSwamp에 대한 처리
			if (party.ability.walk_on_swamp > 0)
			{
				--party.ability.walk_on_swamp;
			}
			else
			{
				yunjr::LoreConsole& console = yunjr::LoreConsole::getConsole();
				console.clear();
				console.setTextColorIndex(13);
				console.write("일행은 독이 있는 늪에 들어갔다 !!!");
				console.write("");

				sena::vector<yunjr::shared::PcPlayer>& player_list = yunjr::game::object::getPlayerList();

				int num_updated = sena::for_each(player_list.begin(), player_list.end(), FnEnterSwamp<yunjr::shared::PcPlayer>()).Result();

				console.display();

				// player의 디스플레이 된 수치에 변화가 생겼다면
				if (num_updated > 0)
				{
					//?? window[WINDOWTYPE_STATUS]->setUpdateFlag();
				}
			}

			detectGameOver();
		}

		static void actLava(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			actMove(x1, y1, false);

			{
				yunjr::LoreConsole& console = yunjr::LoreConsole::getConsole();
				console.clear();
				console.setTextColorIndex(12);
				console.write("일행은 용암지대로 들어섰다 !!!");
				console.write("");

				sena::vector<yunjr::shared::PcPlayer>& player_list = yunjr::game::object::getPlayerList();

				int num_updated = sena::for_each(player_list.begin(), player_list.end(), FnEnterLava<yunjr::shared::PcPlayer>()).Result();

				console.display();

				//?? window[WINDOWTYPE_STATUS]->setUpdateFlag();
			}

			detectGameOver();
		}
		static void actTalk(int x1, int y1, bool bUseless)
		{
			yunjr::PcParty& party = yunjr::game::object::getParty();

			party.face(x1, y1);
/*
			window[WINDOWTYPE_MAP]->setUpdateFlag();
			window[WINDOWTYPE_MAP]->display();
			window[WINDOWTYPE_SUBMAP]->display();
*/
			yunjr::LoreConsole::getConsole().clear();

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

		s_p_main_window->addChild(std::make_pair("WAKU",   ControlWaku::newInstance()));
		s_p_main_window->addChild(std::make_pair("MAP",    ControlMap::newInstance(20*2, 20*2, 216*2, 203*2)));
		s_p_main_window->addChild(std::make_pair("CONSOL", ControlConsole::newInstance(267*2, 21*2, 350*2, 201*2, 20*2, 24*2, 26*2, 24*2)));
		s_p_main_window->addChild(std::make_pair("STATUS", ControlStatus::newInstance(21*2, 251*2, 596*2, 90*2)));
		s_p_main_window->addChild(std::make_pair("PANEL",  ControlPanel::newInstance()));

		resrouce.setMainWindow(s_p_main_window);
	}

	{
		const Resource& resource = Resource::getInstance();

		ControlConsole* p_console = (ControlConsole*)resource.getMainWindow()->findControl("CONSOL");

		if (p_console)
		{
			p_console->setText
			(
				L"April is the cruelest month, breeding",
				L"Lilacs out of the dead land, mixing",
				L"Memory and desire, sirring"
			);
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
		yunjr::map_event::load("sample");
	}

	{
		std::vector< ::yunjr::Chara*>& chara_list = yunjr::res_collection::getCharaList();

		chara_list.push_back(Playable::newInstance());
	}

	// party 초기화
	{
		yunjr::PcParty& party = yunjr::game::object::getParty();

		// 외부에서 이 값이 수정되어져야 함
		party.x = -1;
		party.y = -1;

		party.ability.can_use_ESP = false;
		party.ability.can_use_special_magic = false;
	}

	// player 초기화
	{
		sena::vector<shared::PcPlayer>& player_list = game::object::getPlayerList();

		{
			shared::PcPlayer p_player(new PcPlayer());

			p_player->setDefault(0);

			p_player->setName("SMgal");
			p_player->class_ = 8;
			p_player->level[0] = 1;
			p_player->level[1] = 1;
			p_player->level[2] = 1;
			p_player->reviseAttribute();

			player_list.push_back(p_player);
		}

		// 임시 party 멤버들
		{
			shared::PcPlayer p_player(new PcPlayer());

			p_player->setDefault(0);

			p_player->setName("Hercules");
			p_player->hp = 17;
			p_player->sp = 5;
			p_player->esp = 5;

			player_list.push_back(p_player);
		}

		{
			shared::PcPlayer p_player(new PcPlayer());

			p_player->setDefault(0);

			p_player->setName("Betelgeuse");
			p_player->gender = PcPlayer::GENDER_FEMALE;
			p_player->hp = 7;
			p_player->sp = 17;
			p_player->esp = 5;

			player_list.push_back(p_player);
		}

		{
			shared::PcPlayer p_player(new PcPlayer());

			p_player->setDefault(0);

			p_player->setName("Cominus");
			p_player->hp = 10;
			p_player->sp = 11;
			p_player->esp = 3;
			p_player->class_ = 2;

			player_list.push_back(p_player);
		}
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
