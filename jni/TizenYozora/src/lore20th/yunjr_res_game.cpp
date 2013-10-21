
#include "yunjr_base.h"

#include "yunjr_res_game.h"

hadar::PcParty yunjr::res::game::party;
hadar::Map yunjr::res::game::map;

bool yunjr::res::game::loadMap(const char* sz_map_file_name)
{
	target::file_io::StreamReadFile file("lore20th/K_DEN2.MAP");

	int buffer_size = file.getSize();

	if (buffer_size <= 0)
		return false;

	map.clearData();

	/*
		TYPE_TOWN:   'DEN*.MAP', 'K_DEN1.MAP', 'PYRAMID1.MAP', 'K_DEN2.MAP (last)'
		TYPE_KEEP:   'KEEP*.MAP'
		TYPE_GROUND: 'GROUND*.MAP'
		TYPE_DEN:    'DEN*.MAP', 'K_DEN2.MAP'
	*/
	map.setType(hadar::Map::TYPE_DEN);

	unsigned char byte;

	file.read(&byte, 1);
	map.width  = byte;

	file.read(&byte, 1);
	map.height = byte;

	for (int y = 0; y < map.height; y++)
	{
		file.read(&map.data[y][0], map.width);
	}
/*
	map.act_func[hadar::Map::ACT_BLOCK] = &MapCallback::actBlock;
	map.act_func[hadar::Map::ACT_MOVE]  = &MapCallback::actMove;
	map.act_func[hadar::Map::ACT_EVENT] = &MapCallback::actEvent;
	map.act_func[hadar::Map::ACT_ENTER] = &MapCallback::actEnter;
	map.act_func[hadar::Map::ACT_SIGN]  = &MapCallback::actSign;
	map.act_func[hadar::Map::ACT_WATER] = &MapCallback::actWater;
	map.act_func[hadar::Map::ACT_SWAMP] = &MapCallback::actSwamp;
	map.act_func[hadar::Map::ACT_LAVA]  = &MapCallback::actLava;
	map.act_func[hadar::Map::ACT_TALK]  = &MapCallback::actTalk;
*/
	return true;
}
