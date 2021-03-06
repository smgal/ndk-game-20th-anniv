
#ifndef __YUNJR_Z_CONFIG_H__
#define __YUNJR_Z_CONFIG_H__

#include "../flat_board/flat_board.h"
#include "../flat_board/target_dep.h"

namespace yunjr
{
	struct BufferDesc
	{
		void* p_start_address;
		int   width;
		int   height;
		int   bytes_per_line;
		int   bits_per_pixel;
	};

	void init(const char* sz_id);
	void done(void);
	bool loop(const BufferDesc& buffer_desc);

	typedef unsigned long TargetPixel;
}

namespace yunjr
{
	const int MAP_SCALE_RATIO = 2;

	/*
	 * MAP_SCROLL_IN_PIXELS * m = TILE_W
	 * MAP_SCROLL_IN_PIXELS * n = TILE_H
	 * (m and n must be are integers)
	 */
	const int MAP_SCROLL_IN_PIXELS = 4;

	const int DEFAULT_FONT_SIZE = 28;
	const int DEFAULT_FONT_ASCENDER = 26;
	const int DEFAULT_FONT_BTBD = (16 * 2); // Baseline to Baseline Distance
	const int DEFAULT_BATTLE_BTBD = (20 * 2);

	const int TILE_W = 24;
	const int TILE_H = 24;
}

#endif
