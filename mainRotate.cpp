#include "MyFrame.h"
#include "MainRotate.h"

bool MyApp::OnInit()
{

	wxInitAllImageHandlers();
	//wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(1200, 800));

	//frame->SetSizer(sizer);

	frame->Show();
	return true;
}