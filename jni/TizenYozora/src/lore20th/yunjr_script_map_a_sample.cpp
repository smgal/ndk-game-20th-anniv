
#include "yunjr_script_def.h"

////////////////////////////////////////////////////////////////////////////////

DECLARE_MAP(sample)

////////////////////////////////////////////////////////////////////////////////

DEF_s(sample, Identify)

	return (strcmp(sz_map_name, "sample") == 0);

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_v(sample, Load)

	_Map_Init(30, 30)
	//_Map_SetEncounter(1, 10)
	_Map_SetStartPos(15, 15)

	_Map_SetTileAsDefault(TILE_SET_TOWN)

	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow(L"　　　　■■■■■■■■■■■■■■■■■■■■■■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　■円■　　　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　　　　　　　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　　　　　　　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　早　　　　　　　♨♨　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　　■　　♨　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　※　■　　　♨　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　毒毒毒毒　　男■　　♨　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　毒毒　　　　　　　　男　■　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　毒毒　　　．　　　　　■　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　．　　　　■■■■　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　早　　　　　　＋＋＋＋　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　　　男※　＋＋＋＋　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　■　　　　川　　　　　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　■　　　川川川　■　　早　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　■　　　川川　■　　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　■■　　川　　　　　　　　　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　．　　┌┬┬┐　××÷××■　　　　")
	_Map_SetRow(L"　　　　■　　　　．　　　　　├┼┼┤　××　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　　　├┼┼┤　×÷　　　■　　　　")
	_Map_SetRow(L"　　　　■　　　　　　　　　　└┴┴┘　××　　　■　　　　")
	_Map_SetRow(L"　　　　■■■■■■■■■■■■■■■■■■■■■■　　　　")
	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow(L"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_v(sample, Unload)

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_ii(sample, Talk)

	IF ON(15,16) THEN
		_Talk(L"야메로 이런 싸움은 모 야메룽다")
		HALT
	END

	IF ON(18,12) THEN
		_Talk(L"Talk script 2-1")
		_Talk("Talk script 2-2")
		_Talk("Talk script 2-3")
		_Talk("Talk script 2-4")
		HALT
	END

	IF ON(13,11) THEN
		_Talk("Talk script 3-1")
		_Talk("Talk script 3-2")
		_Talk("Talk script 3-3")
		HALT
	END

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_ii(sample, Read)

	IF ON(11,15) THEN
		_Talk("Sign script 1")
	ELSEIF ON(9,8) THEN
		_Talk("Sign script 2")
	ELSEIF ON(19,18) THEN
		_Talk("Sign script 3")
	END

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_ii(sample, Occur)

	IF ON(12,10) THEN
		_Talk("Event script 1")
/*
		_WarpPrevPos()
*/
	ELSEIF ON(16,16) THEN
		IF _Flag_IsSet(_FLAG_BATTLE) THEN
/*
			_Battle_Init()
			_Battle_RegisterEnemy(69)
			_Battle_RegisterEnemy(71)
			_Battle_RegisterEnemy(75)
			_Battle_RegisterEnemy(3)
			_Battle_RegisterEnemy(1)
			_Battle_ShowEnemy()

			_Talk("여기는 Necromancer를 만날 수 있는 이벤트다")
			_PressAnyKey()

			_Battle_Start(true)

			DECLARE(temp)
			ASSIGN(temp, _Battle_Result())

			IF EQUAL(temp, 1) THEN
				_Talk("정상적인 전투 종료")
			ELSE
				_Talk("도망")
			END
*/
		ELSE
			_Map_ChangeTile(17, 16, 7)
			_Map_ChangeTile(17, 17, 7)
			_Map_ChangeTile(17, 18, 7)
			_Flag_Set(_FLAG_BATTLE)
		END
	END

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_ii(sample, Enter)
/*
	IF ON(14,5) THEN
		_Select_Init()
		_Select_Add("새로운 스크립트를 로드하시겠습니까?")
		_Select_Add("예")
		_Select_Add("아니오")
		_Select_Run()

		DECLARE(temp)
		ASSIGN(temp, _Select_Result())

		IF EQUAL(temp, 1) THEN
			_LoadScript("town2.cm2")
		END
	END
*/
ENDDEF
