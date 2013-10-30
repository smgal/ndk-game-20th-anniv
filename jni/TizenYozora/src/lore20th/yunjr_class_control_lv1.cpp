﻿
#include "yunjr_base.h"
#include "yunjr_base_font.h"
#include "yunjr_base_gfx.h"

#include "yunjr_class.h"
#include "yunjr_class_chara.h"
#include "yunjr_class_control_lv1.h"
#include "yunjr_class_game_state.h"
#include "yunjr_util.h"

//#include "yunjr_res_string.h"

#include <algorithm>
#include <string.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// local

namespace yunjr
{
	// for playable updating
	template <>
	void Operator<Chara*, unsigned long>::operator()(Chara* p_obj)
	{
		(p_obj) ? p_obj->update(param) : (void)0;
	}

	// for playable rendering
	template <>
	void Operator<Chara*, FlatBoard32*>::operator()(Chara* p_obj)
	{
		(p_obj) ? p_obj->render(*param) : (void)0;
	}

}

////////////////////////////////////////////////////////////////////////////////
// class ControlWindow

yunjr::ControlWindow::ControlWindow()
{
}

yunjr::ControlWindow::~ControlWindow()
{
	deleteVectorForPair<ResId, Control>(child_list);
}

void yunjr::ControlWindow::addChild(ControlId control_id)
{
	child_list.push_back(control_id);
}

yunjr::Control* yunjr::ControlWindow::findControl(ResId res_id)
{
	assert((res_id.native_id & ResId::TAG_VERIFICATION_MASK) == ResId::TAG_VERIFICATION_STR);

	std::vector<ControlId>::iterator p_control = child_list.begin();

	for ( ; p_control != child_list.end(); ++p_control)
	{
		if (res_id == p_control->first)
			return p_control->second;
	}

	return 0;
}

void yunjr::ControlWindow::invalidateBounds(Pos pos, Size size)
{
	//?? should apply the specified rectangle
	std::vector<ControlId>::iterator p_control = child_list.begin();

	for ( ; p_control != child_list.end(); ++p_control)
		p_control->second->invalidate();
}

yunjr::ControlWindow* yunjr::ControlWindow::newInstance(int width, int height)
{
	struct AttributeWindow: public Visible::Attribute
	{
		struct { int width, height; } size;

		AttributeWindow(int width, int height)
		{
			size.width = width;
			size.height = height;
		}
	};

	struct ShapeWindow: public Visible::Shape
	{
		virtual void render(Visible* p_this, FlatBoard32& dest_board) const
		{
			ControlWindow* p_window = (ControlWindow*)p_this;

			std::for_each(p_window->child_list.begin(), p_window->child_list.end(), Operator<ControlId, FlatBoard32*>(&dest_board));
		}
	};

	struct UpdateWindow: public Visible::Update
	{
		virtual bool update(Visible* p_this, unsigned long tick)
		{
			ControlWindow* p_window = (ControlWindow*)p_this;

			std::for_each(p_window->child_list.rbegin(), p_window->child_list.rend(), Operator<ControlId, unsigned long>(tick));

			return true;
		}
	};

	ControlWindow* p_window = new ControlWindow();

	*p_window << new AttributeWindow(width, height) << new ShapeWindow() << new UpdateWindow();

	return p_window;
}

////////////////////////////////////////////////////////////////////////////////
// class ControlWaku

namespace
{
	static unsigned long* s_p_dest_buffer;
	static int s_dest_ppl;

	void baitFillRect(unsigned long* p_dest, int w, int h, int dest_pitch, unsigned long color, unsigned long alpha)
	{
		s_p_dest_buffer = p_dest;
		s_dest_ppl = dest_pitch;
	}
}

yunjr::ControlWaku::ControlWaku()
{
}

yunjr::ControlWaku* yunjr::ControlWaku::newInstance(void)
{
	struct AttributeWaku: public Visible::Attribute
	{
		struct { int x, y; } pos;
		const FlatBoard32& src_ui;

		AttributeWaku()
			: src_ui(resource::getResimage(ResId(ResId::TAG_TYPE_IMAGE, ResId::TAG_TYPE_IMAGE_UI, 0)))
		{
			pos.x = 0;
			pos.y = 0;
		}
	};

	struct ShapeWaku: public Visible::Shape
	{
		virtual void render(Visible* p_this, FlatBoard32& dest_board) const
		{
			AttributeWaku& attribute = *((AttributeWaku*)p_this->getAttribute());
			//dest_board.bitBlt(attribute.pos.x, attribute.pos.y, const_cast<FlatBoard32*>(&attribute.src_ui), 0, 0, attribute.src_ui.getBufferDesc().width, attribute.src_ui.getBufferDesc().height);

			const BufferDesc* p_buffer_desc = resource::getFrameBuffer();

			if (p_buffer_desc)
			{
				int src_w = attribute.src_ui.getBufferDesc().width;
				int src_h = attribute.src_ui.getBufferDesc().height;
				int src_p = src_w; // packed buffer

				int dst_w = p_buffer_desc->width;
				int dst_h = p_buffer_desc->height;
				int dst_p = (p_buffer_desc->bytes_per_line << 3) / p_buffer_desc->bits_per_pixel;

				// ura waza
				{
					*(const_cast<FlatBoard32*>(&attribute.src_ui)) << baitFillRect;
					(const_cast<FlatBoard32*>(&attribute.src_ui))->fillRect(0, 0, 1, 1, 0x0);
					*(const_cast<FlatBoard32*>(&attribute.src_ui)) << attribute.src_ui.getDefaultFillRect();
				}

				FlatBoard32::Pixel* p_src = s_p_dest_buffer;
				FlatBoard32::Pixel* p_dst = (FlatBoard32::Pixel*)p_buffer_desc->p_start_address;

				p_dst += attribute.pos.y * dst_p + attribute.pos.x;

				// assert(...);

				const int SCALE = 2;

				for (int y = 0; y < src_h; y++)
				{
					FlatBoard32::Pixel* p_s = p_src + y * src_p;
					FlatBoard32::Pixel* p_d = p_dst + SCALE * y * dst_p;
					FlatBoard32::Pixel* p_m = p_d;

					for (int x = 0; x < src_w; x++)
					{
						*p_d++ = *p_s;
						*p_d++ = *p_s++;
					}

					memcpy(p_m + dst_p, p_m, dst_p * sizeof(unsigned long) * (SCALE-1));
				}
			}
		}
	};

	struct UpdateWaku: public Visible::Update
	{
		virtual bool update(Visible* p_this, unsigned long tick)
		{
			return false;
		}
	};

	ControlWaku* p_waku = new ControlWaku();

	*p_waku << new AttributeWaku() << new ShapeWaku() << new UpdateWaku();

	return p_waku;
}

////////////////////////////////////////////////////////////////////////////////
// class ControlConsole

yunjr::ControlConsole::ControlConsole()
{
}

yunjr::ControlConsole::~ControlConsole()
{
}

yunjr::ControlConsole* yunjr::ControlConsole::newInstance(int x, int y, int width, int height, int margin_left, int margin_right, int margin_top, int margin_bottom)
{
	struct AttributeConsole: public Visible::Attribute
	{
		struct { int x, y; } pos;
		struct { int width, height; } size;
		struct { int left, right, top, bottom; } margin;

		AttributeConsole(int x, int y, int width, int height, int margin_left, int margin_right, int margin_top, int margin_bottom)
		{
			pos.x = x;
			pos.y = y;
			size.width = width;
			size.height = height;

			margin.left = margin_left;
			margin.right = margin_right;
			margin.top = margin_top;
			margin.bottom = margin_bottom;
		}
	};

	struct ShapeConsole: public Visible::Shape
	{
		virtual void render(Visible* p_this, FlatBoard32& dest_board) const
		{
			AttributeConsole& attribute = *((AttributeConsole*)p_this->getAttribute());

			dest_board.fillRect(attribute.pos.x, attribute.pos.y, attribute.size.width, attribute.size.height, 0xFF545454);

			const int DIALOG_WINDOW_X = attribute.pos.x;
			const int DIALOG_WINDOW_Y = attribute.pos.y;
			const int DIALOG_WINDOW_W = attribute.size.width;
			const int DIALOG_WINDOW_H = attribute.size.height;

			{
				const BufferDesc* p_buffer_desc = resource::getFrameBuffer();

				unsigned long* p = (unsigned long*)p_buffer_desc->p_start_address;
				int ppl = (p_buffer_desc->bytes_per_line << 3) / p_buffer_desc->bits_per_pixel;

				p += (ppl * DIALOG_WINDOW_Y);
				p += DIALOG_WINDOW_X;

				yunjr::gfx::TextBoard text_board(p, DIALOG_WINDOW_W, DIALOG_WINDOW_H, ppl);

				int text_x = attribute.margin.left;
				int text_y = attribute.margin.top + 26;
				int text_line_gap = 16 * 2;

				ControlConsole* _p_this = (ControlConsole*)p_this;

				std::vector<shared_ptr<Text> >::iterator p_text = _p_this->text_line.begin();

				for ( ; p_text != _p_this->text_line.end(); ++p_text)
				{
					text_board.renderTextFx(text_x, text_y, **p_text, 0xFFFFFFFF, 0xFFFFFFFF);

					text_y += text_line_gap;

				}
/*
				for (int i = 0; i < 3; i++)
				{
					if (_p_this->text[i])
						text_board.renderTextFx(text_x, text_y, *_p_this->text[i], 0xFFFFFFFF, 0xFFFFFFFF);

					text_y += text_line_gap;
				}
*/
			}
		}
	};

	struct UpdateConsole: public Visible::Update
	{
		virtual bool update(Visible* p_this, unsigned long tick)
		{
			AttributeConsole& attribute = *((AttributeConsole*)p_this->getAttribute());

			return false;
		}
	};

	ControlConsole* p_popup = new ControlConsole();

	*p_popup << new AttributeConsole(x, y, width, height, margin_left, margin_right, margin_top, margin_bottom) << new ShapeConsole() << new UpdateConsole();

	return p_popup;
}

////////////////////////////////////////////////////////////////////////////////
// class ControlStatus

yunjr::ControlStatus::ControlStatus()
{
}

yunjr::ControlStatus::~ControlStatus()
{
}

yunjr::ControlStatus* yunjr::ControlStatus::newInstance(int x, int y, int width, int height)
{
	struct AttributeConsole: public Visible::Attribute
	{
		struct { int x, y; } pos;
		struct { int width, height; } size;

		AttributeConsole(int x, int y, int width, int height)
		{
			pos.x = x;
			pos.y = y;
			size.width = width;
			size.height = height;
		}
	};

	struct ShapeConsole: public Visible::Shape
	{
		virtual void render(Visible* p_this, FlatBoard32& dest_board) const
		{
			AttributeConsole& attribute = *((AttributeConsole*)p_this->getAttribute());

			dest_board.fillRect(attribute.pos.x, attribute.pos.y, attribute.size.width, attribute.size.height, 0xFF545454);

			const int DIALOG_WINDOW_X = attribute.pos.x;
			const int DIALOG_WINDOW_Y = attribute.pos.y;
			const int DIALOG_WINDOW_W = attribute.size.width;
			const int DIALOG_WINDOW_H = attribute.size.height;

			{
				const BufferDesc* p_buffer_desc = resource::getFrameBuffer();

				unsigned long* p = (unsigned long*)p_buffer_desc->p_start_address;
				int ppl = (p_buffer_desc->bytes_per_line << 3) / p_buffer_desc->bits_per_pixel;

				p += (ppl * DIALOG_WINDOW_Y);
				p += DIALOG_WINDOW_X;

				yunjr::gfx::TextBoard text_board(p, DIALOG_WINDOW_W, DIALOG_WINDOW_H, ppl);

				int text_x = 35;
				int text_y = 22;
				int text_y_gap = 15 * 2;

				sena::vector<shared::PcPlayer>& player_list = game::object::getPlayerList();

				{
					//SET_FONT_SIZE(18);
					//shared_ptr<Font> font = GET_RESIZED_FONT;
					Typeface typeface;
					typeface.attribute.size = 18;
					typeface.attribute.is_italic = false;
					typeface.attribute.is_bold = false;

					for (int i = 0; i < 6; i++)
					{
						if (i < player_list.size())
						{
							Text text(typeface, player_list[i]->getName());

							text_board.renderTextFx(text_x, text_y, text, 0xFFFFFFFF, 0xFFFFFFFF);

							char s[256];
							sprintf(s, "%d,  %d,  %d,  %d,  %d,  "
								, player_list[i]->hp, player_list[i]->sp
								, player_list[i]->esp, player_list[i]->ac
								, player_list[i]->level[0]);

							Text text_aux1(typeface, s);
							text_board.renderTextFx(text_x+150, text_y, text_aux1, 0xFFFFFFFF, 0xFFFFFFFF);

							Text text_aux2(typeface, player_list[i]->getConditionString());
							text_board.renderTextFx(text_x+400, text_y, text_aux2, 0xFFFFFFFF, 0xFFFFFFFF);

							text_y += text_y_gap;
						}
						else
						{
							Text text(typeface, L"reserved");

							text_board.renderTextFx(text_x, text_y, text, 0xFF804020, 0xFF804020);

							text_y += text_y_gap;
						}
					}
				}
/* ?? test case
				{
					Typeface typeface(30);

					Text text1(typeface, L"April is the cruelest month, breeding Lilacs out of the dead land, mixing Memory and desire, sirring");
					Text text2(typeface, L"");
					Text text3(typeface, L"");
					Text text4(typeface, L"");
					Text text5(typeface, L"");

					int guide = 250;
					text1.split(guide, text2);
					if (!text2.isEmpty())
					{
						text2.split(guide, text3);
						if (!text3.isEmpty())
						{
							text3.split(guide, text4);
							if (!text4.isEmpty())
							{
								text4.split(guide, text5);
							}
						}
					}

					text_board.renderTextFx(300,  25, text1, 0xFFFF0000, 0xFFFFFFFF);
					text_board.renderTextFx(300,  50, text2, 0xFFFF0000, 0xFFFFFFFF);
					text_board.renderTextFx(300,  75, text3, 0xFFFF0000, 0xFFFFFFFF);
					text_board.renderTextFx(300, 100, text4, 0xFFFF0000, 0xFFFFFFFF);
					text_board.renderTextFx(300, 125, text5, 0xFFFF0000, 0xFFFFFFFF);
				}
*/
			}
		}
	};

	struct UpdateConsole: public Visible::Update
	{
		virtual bool update(Visible* p_this, unsigned long tick)
		{
			AttributeConsole& attribute = *((AttributeConsole*)p_this->getAttribute());

			return false;
		}
	};

	ControlStatus* p_status = new ControlStatus();

	*p_status << new AttributeConsole(x, y, width, height) << new ShapeConsole() << new UpdateConsole();

	return p_status;
}

////////////////////////////////////////////////////////////////////////////////
// class ControlPanel

yunjr::ControlPanel::ControlPanel()
{
}

yunjr::ControlPanel* yunjr::ControlPanel::newInstance(void)
{
	struct AttributePanel: public Visible::Attribute
	{
		struct { int x, y; } pos;
		const FlatBoard32& src_ui;
		const FlatBoard32& src_ui_mask;

		AttributePanel()
			: src_ui(resource::getResimage(ResId(ResId::TAG_TYPE_IMAGE, ResId::TAG_TYPE_IMAGE_UI, 0)))
			, src_ui_mask(resource::getResimage(ResId(ResId::TAG_TYPE_IMAGE, ResId::TAG_TYPE_IMAGE_UI, 1)))
		{
			pos.x = 0;
			pos.y = 0;
		}
	};

	struct ShapePanel: public Visible::Shape
	{
		virtual void render(Visible* p_this, FlatBoard32& dest_board) const
		{
		}
	};

	struct UpdatePanel: public Visible::Update
	{
		virtual bool update(Visible* p_this, unsigned long tick)
		{
			AttributePanel& attribute = *((AttributePanel*)p_this->getAttribute());

			GameState& game_state = GameState::getMutableInstance();
			target::InputUpdateInfo& touch_info = game_state.current_input_info;

			if (touch_info.is_touched)
			{
				int x_rel = touch_info.touch_pos.x - attribute.pos.x;
				int y_rel = touch_info.touch_pos.y - attribute.pos.y;

				if ((x_rel >= 0 && x_rel < attribute.src_ui_mask.getBufferDesc().width)
					&& (x_rel >= 0 && x_rel < attribute.src_ui_mask.getBufferDesc().width))
				{
					FlatBoard32::Pixel detect_buffer = 0xFFFFFFFF;
					FlatBoard32 dest_board((FlatBoard32::Pixel*)&detect_buffer, 1, 1, 1);

					dest_board.bitBlt(0, 0, const_cast<FlatBoard32*>(&attribute.src_ui_mask), x_rel, y_rel, 1, 1);

					if (detect_buffer != 0xFFFFFFFF)
					{
					#if !defined(PIXELFORMAT_ABGR)
						int r = (detect_buffer >> 16) & 0xFF;
						int g = (detect_buffer >> 8) & 0xFF;
						int b = (detect_buffer) & 0xFF;
					#else
						int b = (detect_buffer >> 16) & 0xFF;
						int g = (detect_buffer >> 8) & 0xFF;
						int r = (detect_buffer) & 0xFF;
					#endif

						int i = (r == 255) | (g == 255) | (b == 255);
						r     = (r > 0) ? 1 : 0;
						g     = (g > 0) ? 1 : 0;
						b     = (b > 0) ? 1 : 0;

						int index = i << 3 | r << 2 | g << 1 | b;

						switch (index)
						{
/*
						case 0: // CANCEL
							{
								const Resource& resource = Resource::getInstance();

								ControlConsole* p_console = (ControlConsole*)resource.getMainWindow()->findControl("CONSOL");

								if (p_console)
								{
									p_console->setText(yunjr::res::TEST_STRING4, yunjr::res::TEST_STRING5, yunjr::res::TEST_STRING6);
								}
							}
							break;
						case 1: // OK
							{
								const Resource& resource = Resource::getInstance();

								ControlConsole* p_console = (ControlConsole*)resource.getMainWindow()->findControl("CONSOL");

								if (p_console)
								{
									p_console->setText(yunjr::res::TEST_STRING1, yunjr::res::TEST_STRING2, yunjr::res::TEST_STRING3);
								}
							}
							break;
*/
						case 2: // MENU
							game_state.setKeyPressed(target::KEY_MENU);
							break;
						case 3: // RET
							break;
						case 4: // UP
							game_state.setKeyPressed(target::KEY_UP);
							break;
						case 5: // LEFT
							game_state.setKeyPressed(target::KEY_LEFT);
							break;
						case 6: // DOWN
							game_state.setKeyPressed(target::KEY_DOWN);
							break;
						case 7: // RIGHT
							game_state.setKeyPressed(target::KEY_RIGHT);
							break;
						}
					}
				}
			}

			if (game_state.checkKeyPressed(target::KEY_MENU))
				game_state.terminate();

			return false;
		}
	};

	ControlPanel* p_panel = new ControlPanel();

	*p_panel << new AttributePanel() << new ShapePanel() << new UpdatePanel();

	return p_panel;
}
