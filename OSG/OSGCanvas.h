#ifndef _OSGCANVAS_H_
#define _OSGCANVAS_H_

#include "wx/glcanvas.h"

#include <osgViewer/Viewer>
#include <osgText/Text>
#include <osgUtil/DelaunayTriangulator>

#include <wx/xml/xml.h>
#include <string>

#include "../DPoint.h"

class GraphicsWindowWX;
class DShape;
enum DDDrawingTools {
	dt_Default,
	dt_Reference,
	dt_Select,
	dt_Line,
	dt_Point,
	dt_Sphere,
	dt_Polygon
};

class OSGCanvas : public wxGLCanvas
{
public:
	OSGCanvas();
	OSGCanvas(wxWindow *parent, bool is_alone = false,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0,
		const wxString& name = wxT("TestGLCanvas"),
		int *attributes = 0);
	virtual ~OSGCanvas();
	void OnCharM(int key);
	void Update();
	wxGLContext& GetContext();

	void SetPosCam(double x, double y, double z, double vx, double vy, double vz, double fov);
	bool Get3DPt(const double& x_pix, const double& y_pix, double* x, double* y, double* z);

	/**** test *****/
	DDDrawingTools d_tools;

	void Reference(double x, double y, double z);
	void Sphere(double x, double y, double z);
	void Selection(double x, double y, double z);
	void Point(double x, double y, double z);
	void DLine(double x, double y, double z);
	void Polygon(double x, double y, double z);
	void tPolygon() { d_tools = dt_Polygon; }
	void tReference() { d_tools = dt_Reference; }
	void tSelect() { d_tools = dt_Select; }
	void tDefault() { d_tools = dt_Default; }
	void tSphere() { d_tools = dt_Sphere; }
	void tDot() { d_tools = dt_Point; }
	void tLine() { d_tools = dt_Line; resetline = false;}
	void Remove();
	bool LoadModel(std::string fileName);
	DPoint RSelect();

	// here is the data !
	osg::ref_ptr<osg::Group> m_pGroup;
	osg::ref_ptr<osg::Group> m_pShape;
	osg::ref_ptr<osg::Group> m_pGroupRefPts;
	osg::ref_ptr<osg::Group> m_DelTri;
	/*reference*/
	long refcount = 0;
	void refText(wxRealPoint pt, DPoint dpt);

	/*Save XML*/
	void SaveShape(wxXmlNode* node);
	void OpenShape(wxXmlNode* child);

	/*Interpol*/
	void Link3D(std::vector <std::pair <wxRealPoint, DPoint>> Link3D);
	void SetLink(std::vector <std::pair <wxRealPoint, DPoint>> Link3D);
	double Area(std::vector<DPoint> &refpt);
	std::vector <std::pair <wxRealPoint, DPoint>> RefLink;
	//wxRealPoint interpol;

	
	std::vector<DPoint> Dtriangle;

	void ToggleDelaunay();
	bool T_Delaunay = false;
	void Delaunay();
	void Enter();
	void ESC();
	void EndShape(std::string shapetype);
protected:

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

	//void OnChar(wxKeyEvent &event);
	
	void OnKeyUp(wxKeyEvent &event);

	void OnMouseEnter(wxMouseEvent &event);
	void OnMouseLeftDown(wxMouseEvent &event);
	void OnMouseLeftUp(wxMouseEvent &event);
	void OnMouseDown(wxMouseEvent &event);
	void OnMouseUp(wxMouseEvent &event);
	void OnMouseMotion(wxMouseEvent &event);
	void OnMouseWheel(wxMouseEvent &event);
	
	void InitializeObject();
	
private:
	bool m_leftClickDown;
	wxPoint m_leftClickDownPos;

	DECLARE_EVENT_TABLE()
	osgViewer::GraphicsWindow* m_graphics_window;
	osg::ref_ptr<osgViewer::Viewer> m_viewer;
	wxGLContext* m_context;

	/* Must have light source in order to illuminate the model */
	void InitLight();
	osg::ref_ptr<osg::LightSource> Light(unsigned int id, double x, double y, double z);
	osg::Node* createHUD(osgText::Text* updateText);

	std::vector<DShape*> m_dshapes;
	DShape * m_dshape_line = NULL;
	DShape * m_dshape_sphere = NULL;
	DShape * m_dshape_polygon = NULL;
	DShape * m_dshape_select = NULL;
	DShape * m_dshape_select_temp = NULL;
	int m_dselect = 0;
	int m_dselected = 999;
	int sizex, sizey;
	bool resetline = true;
};

#endif // _OSGCANVAS_H_

