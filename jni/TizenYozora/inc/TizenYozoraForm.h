#ifndef _TIZEN_YOZORA_FORM_H_
#define _TIZEN_YOZORA_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class TizenYozoraForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::Controls::IFormBackEventListener
{
public:
	TizenYozoraForm(void);
	virtual ~TizenYozoraForm(void);
	bool Initialize(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual result OnDraw(void);

};

#endif	//_TIZEN_YOZORA_FORM_H_
