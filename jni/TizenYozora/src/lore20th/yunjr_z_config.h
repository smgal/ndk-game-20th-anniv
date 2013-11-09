
#ifndef __YUNJR_Z_CONFIG_H__
#define __YUNJR_Z_CONFIG_H__

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
	const int DEFAULT_FONT_BTBD = (16 * 2); // Baseline to Baseline Distance

	const int TILE_W = 24;
	const int TILE_H = 24;
}

#endif
