#include "TizenYozoraForm.h"
#include "AppResourceId.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

////////////////////////////////////////////////////////////////////////////////

namespace yozora
{
	struct BufferDesc
	{
		void* p_start_address;
		int   width;
		int   height;
		int   bytes_per_line;
		int   bits_per_pixel;
	};

	void init(const char* sz_id);
	void done();
	bool loop(const BufferDesc& buffer_desc);
}

extern bool g_is_terminating;

namespace
{
	Bitmap* s_p_frame_buffer = 0;
	Canvas* s_p_current_canvas = 0;
}

void UpdateScreen(void);

////////////////////////////////////////////////////////////////////////////////

TizenYozoraForm::TizenYozoraForm(void)
{
}

TizenYozoraForm::~TizenYozoraForm(void)
{
	delete s_p_frame_buffer;
	s_p_frame_buffer = 0;
}

bool
TizenYozoraForm::Initialize(void)
{
	Construct(FORM_STYLE_NORMAL);

	return true;
}
result
TizenYozoraForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	// Setup back event listener
	SetFormBackEventListener(this);

	s_p_frame_buffer = new Bitmap;
	s_p_frame_buffer->Construct(Dimension(CoordinateSystem::ConvertToLogicalX(1280), CoordinateSystem::ConvertToLogicalY(720)), BITMAP_PIXEL_FORMAT_ARGB8888);

	yozora::init("lore20th");

	return r;
}

result
TizenYozoraForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	yozora::done();

	return r;
}

void
TizenYozoraForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}

result
TizenYozoraForm::OnDraw(void)
{
	if (g_is_terminating)
		return E_SUCCESS;

	Canvas* p_canvas = GetCanvasN();

	if (p_canvas)
	{
		s_p_current_canvas = p_canvas;

		BufferInfo buffer_info;

		Bitmap& bitmap = *s_p_frame_buffer;

		if (bitmap.Lock(buffer_info) == E_SUCCESS)
		{
			yozora::BufferDesc buffer_desc;

			buffer_desc.p_start_address = buffer_info.pPixels;
			buffer_desc.width           = buffer_info.width;
			buffer_desc.height          = buffer_info.height;
			buffer_desc.bytes_per_line  = buffer_info.pitch;
			buffer_desc.bits_per_pixel  = buffer_info.bitsPerPixel;

			g_is_terminating = !yozora::loop(buffer_desc);
		}
		bitmap.Unlock();

		p_canvas->SetBackgroundColor(Color(0xFF000000));
		p_canvas->Clear();
		p_canvas->DrawBitmap(p_canvas->GetBounds(), bitmap);
	}

	s_p_current_canvas = null;


	delete p_canvas;

	return E_SUCCESS;
}

void UpdateScreen(void)
{
	s_p_current_canvas->SetBackgroundColor(Color(0xFF000000));
	s_p_current_canvas->Clear();
	s_p_current_canvas->DrawBitmap(s_p_current_canvas->GetBounds(), *s_p_frame_buffer);

	s_p_current_canvas->Show();
}
