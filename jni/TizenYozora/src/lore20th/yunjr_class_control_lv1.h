
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
		std::vector<shared_ptr<Text> > text_line;

	public:
		virtual ~ControlConsole();

		void clear(void)
		{
			text_line.clear();

			this->invalidate();
		}

		void add(Text& text)
		{
			Text remaining;

			//?? 700 is a temporary constant
			text.split(620, remaining);

			{
				shared_ptr<Text> p_text(new Text());
				text.split(0, *p_text);
				text_line.push_back(p_text);
			}

			if (!remaining.isEmpty())
				add(remaining);

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
