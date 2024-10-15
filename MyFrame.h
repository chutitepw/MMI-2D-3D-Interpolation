#ifndef MYFRAME
#define MYFRAME
#include "wx/wx.h"
#include <wx/log.h>
#include <wx/textctrl.h>
#include <wx/splitter.h>
#include <wx/sizer.h>
#include <wx/xml/xml.h>

#include "DPoint.h"

enum
{
	TOOLS_ZOOMIN,
	TOOLS_ZOOMOUT,
	TOOLS_DEFAULT,
	TOOLS_LINE,
	TOOLS_POINT,
	TOOLS_CIRCLE,
	TOOLS_RECTANGLE,
	TOOLS_POLYGON,
	TOOLS_REFERENCE,
	TOOLS_ROTATELEFT,
	TOOLS_ROTATERIGHT,
	TOOLS_OPENSHAPE,
	TOOLS_SAVESHAPE,
	TOOLS_SELECT,
	TOOLS_LINK,
	TOOLS_2DTO3D,
	TOOLS_3DTO2D,
	TOOLS_ONTRI,
	TOOLS_OFFTRI,
	TOOLS_OPEN3D

};

class OSGCanvas;
class ScrolledImageComponent;
class MyFrame : public wxFrame
{

	

	ScrolledImageComponent *m_ImagePanel;
	wxString m_path;
//	ScrolledImageComponent * drawPane;
	wxToolBar *m_toolbar;
	OSGCanvas * m_Panel3D;
	
	
	
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	//MyFrame(const wxString& title);
	~MyFrame();
	
	/*3D*/
	//@ 
	void SetInfoStatusBar(const std::string & s);

	void Interpolate_3D_2_2D(std::string shapetype, std::vector<double> area, std::vector<DPoint> triangle);
	void Interpolate_2D_2_3D(std::string shapetype, std::vector<double> area, std::vector<wxRealPoint> triangle);
	void OnEndShape(std::string shapetype);
	void OnEndDShape();


private:
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);//unuse
	void OnOpenShape(wxCommandEvent& event);
	void OnSaveShape(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnRotateR(wxCommandEvent& event);
	void OnRotateL(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);//unuse
	void OnCircle(wxCommandEvent& event);
	void OnRectangle(wxCommandEvent& event);
	void OnDot(wxCommandEvent& event);
	void OnDefault(wxCommandEvent& event);
	void OnLine(wxCommandEvent& event);
	void OnPolygon(wxCommandEvent& event);
	void OnZoomI(wxCommandEvent& event);
	void OnZoomO(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnSelect(wxCommandEvent& event);
	void OnReference(wxCommandEvent& event);
	void OnLink(wxCommandEvent& event);
	void SaveLink(wxXmlNode * node);
	bool OpenLink(wxXmlNode * node);
	double Area(const wxRealPoint pt);
	void OnIdle(wxIdleEvent &event);

	void OnInter2D(wxCommandEvent &event);
	void OnInter3D(wxCommandEvent &event);
	void OnTri(wxCommandEvent &event);
	void OnOpenModel(wxCommandEvent &event);
	

	std::vector <std::pair <wxRealPoint, DPoint>> RefLink;
	wxRealPoint refp;
	DPoint refdp;
	
	DECLARE_EVENT_TABLE()
};
#endif