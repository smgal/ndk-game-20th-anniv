
#ifndef __YUNJR_CLASS_CHARA_H__
#define __YUNJR_CLASS_CHARA_H__

#include "yunjr_base.h"
#include "yunjr_base_gfx.h"

#include "yunjr_class.h"
#include "yunjr_class_game_state.h"

#include "yunjr_res.h"

#include <vector>

namespace yunjr
{
	struct Chara: public Visible
	{
	};

	struct Playable: public Chara
	{
		static Playable* newInstance(void)
		{
			struct AttributeChara: public Visible::Attribute
			{
				struct { int x, y; }   pos;
				struct { int x1, y1; } dir;

				void move(int x_vec, int y_vec)
				{
					if (x_vec != 0 || y_vec != 0)
					{
						pos.x += x_vec;
						pos.y += y_vec;

						_convertVecToDir(float(x_vec), float(y_vec), dir.x1, dir.y1);
					}
				}

				void _convertVecToDir(float x_vec, float y_vec, int& x_dir, int& y_dir)
				{
					x_dir = (x_vec == 0.0f) ? 0 : ((x_vec > 0.0f) ? 1 : -1);
					y_dir = (y_vec == 0.0f) ? 0 : ((y_vec > 0.0f) ? 1 : -1);
				/*
					if (x_dir != 0 || y_dir != 0)
					{
						x_vec = (x_vec >= 0.0f) ? x_vec : -x_vec;
						y_vec = (y_vec >= 0.0f) ? y_vec : -y_vec;

						const float TAN_67d5 = 2.41421356237f;
						const float TAN_22d5 = 0.41421356237f;

						float tangent = y_vec / x_vec;

						if (tangent > TAN_67d5)
							x_dir = 0;
						if (tangent < TAN_22d5)
							y_dir = 0;
					}
				*/
				}
			};

			struct ShapeChara: public Visible::Shape
			{
				TileId tile_id;
				std::vector<int> dir_offset[3][3];

				virtual void render(Visible* p_this, FlatBoard32& dest_board) const
				{
					AttributeChara& attribute = *((AttributeChara*)p_this->getAttribute());

					int current_offset = this->dir_offset[attribute.dir.x1+1][attribute.dir.y1+1][0];
					gfx::drawTile(dest_board, attribute.pos.x, attribute.pos.y, this->tile_id, current_offset);
				}
			};

			struct UpdateChara: public Visible::Update
			{
				virtual bool update(Visible* p_this, unsigned long tick)
				{
					int x1 = 0;
					int y1 = 0;

					const GameState& game_state = GameState::getInstance();

					if (game_state.checkKeyPressed(target::KEY_LEFT))
						x1 = -1;
					if (game_state.checkKeyPressed(target::KEY_RIGHT))
						x1 = 1;
					if (game_state.checkKeyPressed(target::KEY_UP))
						y1 = -1;
					if (game_state.checkKeyPressed(target::KEY_DOWN))
						y1 = 1;

					{
						AttributeChara& attribute = *((AttributeChara*)p_this->getAttribute());

						attribute.move(x1 * 4, y1 * 4);
					}

					return true;
				}
			};

			Playable* p_playable = new Playable;

			{
				AttributeChara* p_attribute = new AttributeChara();

				p_attribute->pos.x  = 4 * yunjr::TILE_W;
				p_attribute->pos.y  = 4 * yunjr::TILE_H;
				p_attribute->dir.x1 = 0;
				p_attribute->dir.y1 = 1;

				*p_playable << p_attribute;
			}

			{
				ShapeChara* p_shape = new ShapeChara();

				p_shape->tile_id = TILE_ID_PLAYER;

				const int X__ = 0;
				const int _X_ = 1;
				const int __X = 2;
				const int Y__ = 0;
				const int _Y_ = 1;
				const int __Y = 2;

				p_shape->dir_offset[X__][Y__].push_back(5);
				p_shape->dir_offset[_X_][Y__].push_back(5);
				p_shape->dir_offset[__X][Y__].push_back(5);
				p_shape->dir_offset[X__][_Y_].push_back(7);
				p_shape->dir_offset[_X_][_Y_].push_back(4);
				p_shape->dir_offset[__X][_Y_].push_back(6);
				p_shape->dir_offset[X__][__Y].push_back(4);
				p_shape->dir_offset[_X_][__Y].push_back(4);
				p_shape->dir_offset[__X][__Y].push_back(4);

				*p_playable << p_shape;
			}

			{
				*p_playable << new UpdateChara();
			}

			return p_playable;
		}
	};
}

#endif // #ifndef __YUNJR_CLASS_CHARA_H__
