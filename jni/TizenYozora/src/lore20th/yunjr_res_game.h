
#ifndef __YUNJR_RES_GAME_H__
#define __YUNJR_RES_GAME_H__

#include "hd_class_pc_party.h"
#include "hd_class_map.h"

namespace yunjr
{
	namespace res
	{
		namespace game
		{
			extern hadar::PcParty party;
			extern hadar::Map map;

			bool loadMap(const char* sz_map_file_name);
		}
	}
}

#endif
