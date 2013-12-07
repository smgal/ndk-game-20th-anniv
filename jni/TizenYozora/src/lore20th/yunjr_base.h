
#ifndef __YUNJR_BASE_H__
#define __YUNJR_BASE_H__

#include "yunjr_z_config.h"

#include "util/sm_util_sena.h"

#include <cassert>
#include <vector>
#include <utility>

namespace yunjr
{
	enum ENDBATTLE
	{
		ENDBATTLE_NONE,
		ENDBATTLE_LOSE,
		ENDBATTLE_WIN,
		ENDBATTLE_RUN_AWAY,
	};

	enum GAMESTATE
	{
		GAMESTATE_MAP,
		GAMESTATE_BATTLE,
	};

	enum EXITCODE
	{
		EXITCODE_BY_USER,
		EXITCODE_BY_ACCIDENT,
		EXITCODE_BY_ENEMY,
		EXITCODE_BY_FORCE,
	};

	enum BATTLERESULT
	{
		BATTLERESULT_EVADE,
		BATTLERESULT_WIN,
		BATTLERESULT_LOSE,
	};
}

namespace yunjr
{
	typedef flat_board::FlatBoard<flat_board::PIXELFORMAT_ARGB8888> FlatBoard32;
	using flat_board::Color32;

	template <typename T>
	void deleteVector(std::vector<T*>& container)
	{
		typename std::vector<T*>::iterator item = container.begin();

		for ( ; item != container.end(); ++item)
			delete *item;

		container.clear();
	}

	template <typename T1, typename T2>
	void deleteVectorForPair(std::vector<std::pair<T1, T2*> >& container)
	{
		typename std::vector<std::pair<T1, T2*> >::iterator item = container.begin();

		for ( ; item != container.end(); ++item)
			delete item->second;

		container.clear();
	}

	template <typename T, typename Param1>
	struct Operator
	{
		Param1 param;

		Operator(Param1 in_param)
			: param(in_param) {	}

		void operator()(T p_obj);
	};
}

#endif // #ifndef __YUNJR_BASE_H__
