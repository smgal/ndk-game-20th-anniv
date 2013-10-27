
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

	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡áå÷¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡ðÄ¡¡¡¡¡¡¡¡¡¡¡¡¡¡¢Í¢Í¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¢Í¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡Ø¡¡¡á¡¡¡¡¡¡¢Í¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡Ô¸Ô¸Ô¸Ô¸¡¡¡¡Ñû¡á¡¡¡¡¢Í¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡Ô¸Ô¸¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡Ñû¡¡¡á¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡Ô¸Ô¸¡¡¡¡¡¡£®¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡£®¡¡¡¡¡¡¡¡¡á¡á¡á¡á¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡ðÄ¡¡¡¡¡¡¡¡¡¡¡¡++++++++¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡Ñû¡Ø¡¡£«£«£«£«¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡ô¹¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡á¡¡¡¡¡¡ô¹ô¹ô¹¡¡¡á¡¡¡¡ðÄ¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡ô¹ô¹¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡á¡á¡¡¡¡ô¹¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡£®¡¡¡¡¦£¦¨¦¨¦¤¡¡¡¿¡¿¡À¡¿¡¿¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡£®¡¡¡¡¡¡¡¡¡¡¦§¦«¦«¦©¡¡¡¿¡¿¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¦§¦«¦«¦©¡¡¡¿¡À¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¦¦¦ª¦ª¦¥¡¡¡¿¡¿¡¡¡¡¡¡¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")
	_Map_SetRow("¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡")

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_v(sample, Unload)

ENDDEF

////////////////////////////////////////////////////////////////////////////////

DEF_ii(sample, Talk)

	IF ON(15,16) THEN
		_Talk(L"¾ß¸Þ·Î ÀÌ·± ½Î¿òÀº ¸ð ¾ß¸Þ·î´Ù")
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

			_Talk("¿©±â´Â Necromancer¸¦ ¸¸³¯ ¼ö ÀÖ´Â ÀÌº¥Æ®´Ù")
			_PressAnyKey()

			_Battle_Start(true)

			DECLARE(temp)
			ASSIGN(temp, _Battle_Result())

			IF EQUAL(temp, 1) THEN
				_Talk("Á¤»óÀûÀÎ ÀüÅõ Á¾·á")
			ELSE
				_Talk("µµ¸Á")
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
		_Select_Add("»õ·Î¿î ½ºÅ©¸³Æ®¸¦ ·ÎµåÇÏ½Ã°Ú½À´Ï±î?")
		_Select_Add("¿¹")
		_Select_Add("¾Æ´Ï¿À")
		_Select_Run()

		DECLARE(temp)
		ASSIGN(temp, _Select_Result())

		IF EQUAL(temp, 1) THEN
			_LoadScript("town2.cm2")
		END
	END
*/
ENDDEF
