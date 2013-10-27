
#ifndef __YUNJR_CLASS_CONTROL_LV1_H__
#define __YUNJR_CLASS_CONTROL_LV1_H__

#include "yunjr_base.h"

#include "yunjr_class.h"
#include "yunjr_class_control.h"

namespace yunjr
{
	struct ControlWindow: public Control
	{
		std::vector<ControlId> child_list;

	public:
		virtual ~ControlWindow();

		void     addChild(ControlId control_id);
		Control* findControl(ResId res_id);

		void     invalidateBounds(Pos pos, Size size);

		static   ControlWindow* newInstance(int width, int height);

	private:
		ControlWindow();
	};

	struct ControlWaku: public Control
	{
	public:
		static   ControlWaku* newInstance(void);

	private:
		ControlWaku();

	};

	struct ControlConsole: public Control
	{
		Text* title;
		Text* text[3];

	public:
		virtual ~ControlConsole();

		template<typename CharCode>
		void setTitle(const CharCode* sz_text)
		{
			delete title;
			title = new Text(sz_text);
		}

		template<typename CharCode>
		void setText(const CharCode* sz_text1, const CharCode* sz_text2, const CharCode* sz_text3)
		{
			delete text[0];
			delete text[1];
			delete text[2];

			text[0] = new Text(sz_text1);
			text[1] = new Text(sz_text2);
			text[2] = new Text(sz_text3);

			this->invalidate();
		}

		static ControlConsole* newInstance(int x, int y, int width, int height, int margin_left, int margin_right, int margin_top, int margin_bottom);

	private:
		ControlConsole();

	};

	struct ControlStatus: public Control
	{
	public:
		virtual ~ControlStatus();

		static   ControlStatus* newInstance(int x, int y, int width, int height);

	private:
		ControlStatus();

	};

	struct ControlPanel: public Control
	{
	public:
		static   ControlPanel* newInstance(void);

	private:
		ControlPanel();

	};

}

#endif // #ifndef __YUNJR_CLASS_CONTROL_LV1_H__
