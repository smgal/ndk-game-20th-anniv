
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

	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow("　　　　＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　＝纓＝　　　　　　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　　　　　　　　　　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　　　　　　　　　　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　霙　　　　　　　⇒⇒　　　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　　＝　　⇒　　　　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　『　＝　　　⇒　　　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　垳垳垳垳　　剱＝　　⇒　　　　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　垳垳　　　　　　　　剱　＝　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　垳垳　　　��　　　　　＝　　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　��　　　　＝＝＝＝　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　霙　　　　　　++++++++　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　　　剱『　��������　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　＝　　　　�后　　　　　　　　　　　瓠　　　�")
	_Map_SetRow("　　　　＝　　　＝　　　�鵠鵠后　瓠　＄帖　　　　　瓠　　　�")
	_Map_SetRow("　　　　＝　　　　＝　　　�鵠后　瓠　　　　　　　　瓠　　　�")
	_Map_SetRow("　　　　＝　　　　＝＝　　�后　　　　　　　　　　　瓠　　　�")
	_Map_SetRow("　　　　＝　　　　　　　��　　ΓΘΘΔ　／／＼／／＝　　　　")
	_Map_SetRow("　　　　＝　　　　��　　　　　ΗΛΛΙ　／／　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　　　ΗΛΛΙ　／＼　　　＝　　　　")
	_Map_SetRow("　　　　＝　　　　　　　　　　ΖΚΚΕ　／／　　　＝　　　　")
	_Map_SetRow("　　　　＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　　　　")
	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")
	_Map_SetRow("　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　")

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_v(sample, Unload)

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_ii(sample, Talk)

	IF ON(15,16) THEN
		_Talk(L"醤五稽 戚訓 塾崇精 乞 醤五月陥")
		HALT
	END

	IF ON(18,12) THEN
		_Talk("Talk script 2-1")
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

			_Talk("食奄澗 Necromancer研 幻劾 呪 赤澗 戚坤闘陥")
			_PressAnyKey()

			_Battle_Start(true)

			DECLARE(temp)
			ASSIGN(temp, _Battle_Result())

			IF EQUAL(temp, 1) THEN
				_Talk("舛雌旋昔 穿燈 曽戟")
			ELSE
				_Talk("亀諺")
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
		_Select_Add("歯稽錘 什滴験闘研 稽球馬獣畏柔艦猿?")
		_Select_Add("森")
		_Select_Add("焼艦神")
		_Select_Run()

		DECLARE(temp)
		ASSIGN(temp, _Select_Result())

		IF EQUAL(temp, 1) THEN
			_LoadScript("town2.cm2")
		END
	END
*/
ENDDEF
