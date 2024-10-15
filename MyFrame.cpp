#include "MyFrame.h"

#include "ScrolledImageComponent.h"

#include "./OSG/OSGCanvas.h"
#include "./OSG/GraphicsWindowWX.h"

std::string m_folder_ressource="C:/MMI/Split/images";
///////////////////////////////////////////////////////////////////////////////
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)

{
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
	m_toolbar = this->CreateToolBar();

	wxBitmap tzoomin(m_folder_ressource + wxT("/ico/Zoom in.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tzoomout(m_folder_ressource + wxT("/ico/Zoom out.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tdefault(m_folder_ressource + wxT("/ico/Mouse pointer.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tline(m_folder_ressource + wxT( "/ico/Line.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tpoint(m_folder_ressource + wxT("/ico/Pen.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tcircle(m_folder_ressource + wxT("/ico/Circle.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap trectangle(m_folder_ressource + wxT("/ico/Restangle.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tpolygon(m_folder_ressource + wxT("/ico/Hexagon.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap trotateleft(m_folder_ressource + wxT("/ico/Rotate CCW.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap trotateright(m_folder_ressource + wxT("/ico/Rotate CW.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap topenshape(m_folder_ressource + wxT("/ico/Download image.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tsaveshape(m_folder_ressource + wxT("/ico/Upload image.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tselectshape(m_folder_ressource + wxT("/ico/Selection.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap treferenceshape(m_folder_ressource + wxT("/ico/Curve points.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tlink(m_folder_ressource + wxT("/ico/Diagram.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap t2Dto3D(m_folder_ressource + wxT("/ico/Color layers.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap t3Dto2D(m_folder_ressource + wxT("/ico/Layers.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tontri(m_folder_ressource + wxT("/ico/Sharpness.ico"), wxBITMAP_TYPE_ICO);
	wxBitmap tofftri(m_folder_ressource + wxT("/ico/Triangle.ico"), wxBITMAP_TYPE_ICO);
	
	//
	m_toolbar->AddRadioTool(TOOLS_DEFAULT, wxT("Default"), tdefault);
	m_toolbar->AddRadioTool(TOOLS_REFERENCE, wxT("Reference"), treferenceshape);
	m_toolbar->AddRadioTool(TOOLS_SELECT, wxT("Select"), tselectshape);
	m_toolbar->AddRadioTool(TOOLS_LINE, wxT("Line"), tline);
	m_toolbar->AddRadioTool(TOOLS_POINT, wxT("Point"), tpoint);
	m_toolbar->AddRadioTool(TOOLS_CIRCLE, wxT("Circle"), tcircle);
	m_toolbar->AddRadioTool(TOOLS_RECTANGLE, wxT("Rectangle"), trectangle);
	m_toolbar->AddRadioTool(TOOLS_POLYGON, wxT("Polygon"), tpolygon);
	
	m_toolbar->AddSeparator();
	m_toolbar->AddTool(TOOLS_ZOOMIN, wxT("Zoom in"), tzoomin);
	m_toolbar->AddTool(TOOLS_ZOOMOUT, wxT("Zoom out"), tzoomout);
	m_toolbar->AddTool(TOOLS_ROTATELEFT, wxT("Rotate left"), trotateleft);
	m_toolbar->AddTool(TOOLS_ROTATERIGHT, wxT("Rotate right"), trotateright);
	m_toolbar->AddSeparator();
	m_toolbar->AddTool(TOOLS_LINK, wxT("Link"), tlink);
	m_toolbar->AddTool(TOOLS_2DTO3D, wxT("Link"), t2Dto3D);
	m_toolbar->AddTool(TOOLS_3DTO2D, wxT("Link"), t3Dto2D);
	m_toolbar->AddSeparator();
	m_toolbar->AddTool(TOOLS_OPENSHAPE, wxT("Load shape"), topenshape);
	m_toolbar->AddTool(TOOLS_SAVESHAPE, wxT("Save shape"), tsaveshape);

	m_toolbar->AddSeparator();

	m_toolbar->AddCheckTool(TOOLS_ONTRI ,"TEST BDC",tontri);

	
	m_toolbar->Realize();

	
//	vbox->Add(toolbar, 0, wxEXPAND);

	Connect(TOOLS_ZOOMIN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnZoomI));
	Connect(TOOLS_ZOOMOUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnZoomO));

	Connect(TOOLS_DEFAULT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnDefault));

	Connect(TOOLS_LINE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnLine));
	Connect(TOOLS_POINT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnDot));
	Connect(TOOLS_CIRCLE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnCircle));
	Connect(TOOLS_RECTANGLE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnRectangle));
	Connect(TOOLS_POLYGON, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnPolygon));
	Connect(TOOLS_ROTATELEFT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnRotateL));
	Connect(TOOLS_ROTATERIGHT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnRotateR));
	Connect(TOOLS_OPENSHAPE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnOpenShape));
	Connect(TOOLS_SAVESHAPE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnSaveShape));
	Connect(TOOLS_SELECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnSelect));
	Connect(TOOLS_REFERENCE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnReference));
	Connect(TOOLS_LINK, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnLink));
	Connect(TOOLS_REFERENCE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnReference));
	Connect(TOOLS_2DTO3D, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnInter2D));
	Connect(TOOLS_3DTO2D, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnInter3D));
	Connect(TOOLS_ONTRI, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MyFrame::OnTri));
	m_path = m_folder_ressource + "/Face_Cours_Perpective.jpg";
	

	/*******************************test**********************************/
	//main sizer
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	wxSplitterWindow *splittermain = new wxSplitterWindow(this, wxID_ANY);
	splittermain->SetSashGravity(0.5);
	splittermain->SetMinimumPaneSize(20); // Smalest size the

	vbox->Add(splittermain, 1, wxEXPAND, 5);

	m_ImagePanel = new ScrolledImageComponent(splittermain, wxID_ANY);
	m_ImagePanel->LoadImage(m_path);

	
	/*3D*/
	int attributes[7];
	attributes[0] = int(WX_GL_DOUBLEBUFFER);
	attributes[1] = WX_GL_RGBA;
	attributes[2] = WX_GL_DEPTH_SIZE;
	attributes[3] = 8;
	attributes[4] = WX_GL_STENCIL_SIZE;
	attributes[5] = 8;
	attributes[6] = 0;

	/* Create 3D panel */

	m_Panel3D = new OSGCanvas(splittermain, false, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER, wxT("osgviewerWX"), attributes);

	splittermain->SplitVertically(m_Panel3D, m_ImagePanel);
	

	this->SetSizer(vbox);

	/*menu*/
	wxMenuBar *menubar = new wxMenuBar;
	wxMenu *file = new wxMenu;
	file->Append(wxID_OPEN, wxT("&Open"));
	file->Append(wxID_SAVE, wxT("&Save"));
	file->Append(TOOLS_OPEN3D, wxT("&Open Model"));
	
	menubar->Append(file, wxT("&File"));
	SetMenuBar(menubar);

	Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnOpen));
	Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnSave));
	Connect(TOOLS_OPEN3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnOpenModel));
	this->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnKeyDown, this);
	Centre();
}
///////////////////////////////////////////////////////////////////////////////
MyFrame::~MyFrame() {

}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnCircle(wxCommandEvent& event)
{
	m_ImagePanel->Circle();
	m_Panel3D->tSphere();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnRectangle(wxCommandEvent& event)
{
	m_ImagePanel->Rectangle();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnDot(wxCommandEvent& event)
{
	m_ImagePanel->Dot();
	m_Panel3D->tDot();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnDefault(wxCommandEvent& event)
{
	m_ImagePanel->Default();
	m_Panel3D->tDefault();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnLine(wxCommandEvent& event)
{
	m_ImagePanel->Line();
	m_Panel3D->tLine();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnPolygon(wxCommandEvent& event)
{
	m_ImagePanel->Polygon();
	m_Panel3D->tPolygon();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnReference(wxCommandEvent& event)
{
	m_ImagePanel->Reference();
	m_Panel3D->tReference();
	
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnZoomI(wxCommandEvent& event)
{
	m_ImagePanel->ZoomI();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnZoomO(wxCommandEvent& event)
{
	m_ImagePanel->ZoomO();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnRotateR(wxCommandEvent& event)
{
	m_ImagePanel->RotateRight();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnRotateL(wxCommandEvent& event)
{
	m_ImagePanel->RotateLeft();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnOpenShape(wxCommandEvent& event)
{
	RefLink.clear();
	wxFileDialog * openFileDialog = new wxFileDialog(this);
	if (openFileDialog->ShowModal() == wxID_OK) {
		wxString fileShape = openFileDialog->GetPath();

		wxXmlDocument doc;
		if (!doc.Load(fileShape))
			return;
		// start processing the XML file
		wxString name = doc.GetRoot()->GetName();
		if (doc.GetRoot()->GetName() != "Shape")
			return;

		wxXmlNode *child = doc.GetRoot()->GetChildren();
		while (child) {
			
			if (child->GetName() == "DD") {
				wxXmlNode *child2 = child->GetChildren();
				m_ImagePanel->OpenShape(child2);
			}
			else if (child->GetName() == "DDD") {
				wxXmlNode *child2 = child->GetChildren();
				m_Panel3D->OpenShape(child2);
			}
			else if (child->GetName() == "Link") {
				wxXmlNode *child2 = child->GetChildren();
				while (child2) {
					if (child2->GetName() == "Pair") {
						wxXmlNode *child3 = child2->GetChildren();
						while (child3) {
							bool res = OpenLink(child3);
							if (res == true)
								RefLink.push_back(std::make_pair(refp, refdp));
							child3 = child3->GetNext();
						}	
					}
					child2 = child2->GetNext();
					m_ImagePanel->SetLink(RefLink);
					m_ImagePanel->Delaunay();
					m_Panel3D->SetLink(RefLink);
					m_Panel3D->Delaunay();
				}			
			}
			child = child->GetNext();
		}
	}
}	
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnSaveShape(wxCommandEvent& event)
{
	wxString fileShape = wxFileSelector(_T("Save XML"), _T(""), _T(""), _T("*.xml"), _T("XML files (*.xml)|*.xml|All files (*.*)|*.*"), wxFD_SAVE);
	if (!fileShape.empty()) {
		wxCharBuffer buffer = fileShape.ToUTF8();
		wxXmlDocument doc;
		wxXmlNode * XmlShape = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Shape"));
		doc.SetRoot(XmlShape);
		m_ImagePanel->SaveShape(XmlShape);
		m_Panel3D->SaveShape(XmlShape);
		SaveLink(XmlShape);
		
		doc.Save(buffer);
	}
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::SaveLink(wxXmlNode * node)
{
	wxXmlNode * XmlLink = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Link"));
	
	for (unsigned int i = 0; i < RefLink.size(); ++i) {
		wxXmlNode * XmlPair = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Pair"));
		wxXmlNode * XmlPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("pt"));
		wxXmlNode * XmlDPtN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Dpt"));

		wxString centerx;
		centerx << RefLink[i].first.x;
		wxXmlNode * XmlxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xorigin"));
		wxXmlNode * XmlxOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("xorigin"), centerx);
		XmlxOriginN->AddChild(XmlxOrigin);

		wxString centery;
		centery << RefLink[i].first.y;

		wxXmlNode * XmlyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("yorigin"));
		wxXmlNode * XmlyOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("yorigin"), centery);
		XmlyOriginN->AddChild(XmlyOrigin);


		wxString dcenterx;
		dcenterx << RefLink[i].second.x;

		wxXmlNode * XmlDxOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("xorigin"));
		wxXmlNode * XmlDxOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("xorigin"), dcenterx);
		XmlDxOriginN->AddChild(XmlDxOrigin);

		wxString dcentery;
		dcentery << RefLink[i].second.y;

		wxXmlNode * XmlDyOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("yorigin"));
		wxXmlNode * XmlDyOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("yorigin"), dcentery);
		XmlDyOriginN->AddChild(XmlDyOrigin);

		wxString dcenterz;
		dcenterz << RefLink[i].second.z;

		wxXmlNode * XmlDzOriginN = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("zorigin"));
		wxXmlNode * XmlDzOrigin = new wxXmlNode(wxXML_TEXT_NODE, wxT("zorigin"), dcenterz);
		XmlDzOriginN->AddChild(XmlDzOrigin);

		XmlPtN->AddChild(XmlxOriginN);
		XmlPtN->AddChild(XmlyOriginN);

		XmlDPtN->AddChild(XmlDxOriginN);
		XmlDPtN->AddChild(XmlDyOriginN);
		XmlDPtN->AddChild(XmlDzOriginN);
		XmlPair->AddChild(XmlPtN);
		XmlPair->AddChild(XmlDPtN);
		XmlLink->AddChild(XmlPair);

	}

	node->AddChild(XmlLink);
}
///////////////////////////////////////////////////////////////////////////////
bool MyFrame::OpenLink(wxXmlNode * node)
{
	bool pass_x, pass_y, pass_dx, pass_dy, pass_dz;
	pass_x = pass_y = pass_dx = pass_dy = pass_dz = false;
	while (node) {
		if (node->GetName() == "pt") {
			wxXmlNode *child = node->GetChildren();
			while (child) {
				if (child->GetName() == "xorigin") {
					wxString s = child->GetNodeContent();
					if (s.ToCDouble(&refp.x) == false)
						return false;
					pass_x = true;
				}
				if (child->GetName() == "yorigin") {
					wxString s = child->GetNodeContent();
					if (s.ToCDouble(&refp.y) == false)
						return false;
					pass_y = true;
				}
				child = child->GetNext();
			}
		}
		if (node->GetName() == "Dpt") {
			wxXmlNode *child = node->GetChildren();
			while (child) {
				if (child->GetName() == "xorigin") {
					wxString s = child->GetNodeContent();
					if (s.ToCDouble(&refdp.x) == false)
						return false;
					pass_dx = true;
				}
				if (child->GetName() == "yorigin") {
					wxString s = child->GetNodeContent();
					if (s.ToCDouble(&refdp.y) == false)
						return false;
					pass_dy = true;
				}
				if (child->GetName() == "zorigin") {
					wxString s = child->GetNodeContent();
					if (s.ToCDouble(&refdp.z) == false)
						return false;
					pass_dz = true;
				}
				child = child->GetNext();
			}
		}
		node = node->GetNext();
	}//while

	return  pass_x && pass_y && pass_dx && pass_dy && pass_dz;
}

///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnSelect(wxCommandEvent& event)
{
	m_ImagePanel->Select();
	m_Panel3D->tSelect();

}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnClear(wxCommandEvent& event){

}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog * openFileDialog = new wxFileDialog(this);
	if (openFileDialog->ShowModal() == wxID_OK) {
		wxString fileName = openFileDialog->GetPath();
		m_ImagePanel->LoadImage(fileName);
	}
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnSave(wxCommandEvent& event)
{

	/*wxString filename = wxFileSelector(_T("Save image as"), _T(""), _T(""), _T("*.bmp"), _T("BMP files (*.bmp)|*.bmp|GIF files (*gif)|*.gif|JPEG files (*jpg)|*.jpg|PNG files (*png)|*.png|TIFF files (*tif)|*.tif|XPM files (*xpm)|*.xpm|All files (*.*)|*.*"), wxFD_SAVE);
	if (!filename.empty())*/
	m_ImagePanel->SaveImage();


}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxString::Format
	(
		"Welcome to %s!\n"
		"\n"
		"This is the minimal wxWidgets sample\n"
		"running under %s.",
		wxVERSION_STRING,
		wxGetOsDescription()
	),
		"About wxWidgets minimal sample",
		wxOK | wxICON_INFORMATION,
		this);

}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnKeyDown(wxKeyEvent& event) {
	if (event.GetKeyCode() == WXK_ESCAPE)
	{
		m_Panel3D->ESC();
		m_ImagePanel->ESC();
		return;
	}
	else if (event.GetKeyCode() == WXK_RETURN)
	{
		m_Panel3D->Enter();
		m_ImagePanel->Enter();
		return;
	}
	else if (event.GetKeyCode() == WXK_DELETE)
	{
		

		refp = m_ImagePanel->RSelect();
		refdp = m_Panel3D->RSelect();
		if (refp.x != NULL && refp.y != NULL) {
			for (int i = 0; i < RefLink.size(); i++) {
				if (refp == RefLink[i].first) {
					RefLink.erase(RefLink.begin() + i);
				}
			}
		}
		else if(refdp.x != NULL && refdp.y != NULL && refdp.z != NULL) {
			for (int i = 0; i < RefLink.size(); i++) {
				if(refdp.x == RefLink[i].second.x && refdp.y == RefLink[i].second.y &&refdp.z == RefLink[i].second.z)
					RefLink.erase(RefLink.begin() + i);
			}
			
		}
		m_ImagePanel->Remove();
		m_Panel3D->Remove();

		return;
	}
	else {
		int key = event.GetKeyCode();
		m_Panel3D->OnCharM(key);
	}

	event.Skip();
}
///////////////////////////////////////////////////////////////////////////////
/*3D*/
void MyFrame::SetInfoStatusBar(const std::string & s) {
	SetStatusText(s);
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnIdle(wxIdleEvent &event)
{
	m_Panel3D->Update();
	event.RequestMore();
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnLink(wxCommandEvent& WXUNUSED(event))
{
	double x, y;
	refp = m_ImagePanel->RSelect();
	refdp = m_Panel3D->RSelect();
	if (refp.x != NULL && refp.y != NULL && refdp.x != NULL && refdp.y != NULL && refdp.z != NULL) {
		RefLink.push_back(std::make_pair(refp, refdp));
		m_Panel3D->refText(refp, refdp);
		m_ImagePanel->refText(refp, refdp);
	}
	
	
	wxString ref;
	ref << "Link 2D x: " << refp.x << " y: " << refp.y << " 3D X: " << refdp.x << " Y: " << refdp.y << " Z: " << refdp.z;
	//ref << RefLink.first.x << RefLink.second.x;
	SetStatusText(ref);
	
	
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::Interpolate_2D_2_3D(std::string shapetype, std::vector<double> area, std::vector<wxRealPoint> triangle)
{
	
	
	/*Total area*/
	double TotalA;
	std::vector<std::pair <wxRealPoint, DPoint>> RefPoint;
	TotalA = 0;
	RefPoint.clear();
	//TotalA = fabs((((RefLink[1].first.x - RefLink[0].first.x)*(RefLink[2].first.y - RefLink[0].first.y)) - ((RefLink[1].first.y - RefLink[0].first.y)*(RefLink[2].first.x - RefLink[0].first.x))) / 2);
	
	/*find coordinate*/
	double interx, intery, interz;
	interx = intery = interz = 0;
	for (int n = 0; n < triangle.size(); n++) {
		for (int i = 0; i < RefLink.size(); i++) {
		
			if (triangle[n].x == RefLink[i].first.x &&triangle[n].y == RefLink[i].first.y) {
				RefPoint.push_back(RefLink[i]);
			}
			
		}
		
	}
	
	if (RefPoint.size() == area.size()) {
		for (int i = 0; i < area.size(); i++) {
			interx = interx + area[i] * RefPoint[i].second.x;
			intery = intery + area[i] * RefPoint[i].second.y;
			interz = interz + area[i] * RefPoint[i].second.z;
			TotalA = TotalA + area[i];
		}
	}
	
	
	interx = interx / TotalA;
	intery = intery / TotalA;
	interz = interz / TotalA;

	if(shapetype == "Point")
		m_Panel3D->Point(interx, intery, interz);
	if (shapetype == "Circle")
		m_Panel3D->Sphere(interx, intery, interz);
	if (shapetype == "Line")
		m_Panel3D->DLine(interx, intery, interz);
	if (shapetype == "Polygon"|| shapetype == "Rectangle")
		m_Panel3D->Polygon(interx, intery, interz);

}
///////////////////////////////////////////////////////////////////////////////
double MyFrame::Area(const wxRealPoint pt)
{
	double area;
	/*for (int i = 0; i < RefLink.size(); i++) {

	}
	wxString ref;
	ref << "Link 2D x: " << refp.x << " y: " << refp.y << " 3D X: " << refdp.x << " Y: " << refdp.y << " Z: " << refdp.z;
	//ref << RefLink.first.x << RefLink.second.x;
	SetStatusText(ref);*/
	return area;
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnEndShape(std::string shapetype) {
	m_Panel3D->EndShape(shapetype);
}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnEndDShape() {
	m_ImagePanel->EndShape();
}
///////////////////////////////////////////////////////////////////////////////

void MyFrame::Interpolate_3D_2_2D(std::string shapetype, std::vector<double> area, std::vector<DPoint> triangle)
{


	/*Total area*/
	double TotalA;
	std::vector<std::pair <wxRealPoint, DPoint>> RefPoint;
	TotalA = 0;
	RefPoint.clear();

	/*find coordinate*/
	double interx, intery;
	interx = intery = 0;
	for (int n = 0; n < triangle.size(); n++) {
		for (int i = 0; i < RefLink.size(); i++) {

			if (triangle[n].x == RefLink[i].second.x && triangle[n].y == RefLink[i].second.y && triangle[n].z == RefLink[i].second.z) {
				RefPoint.push_back(RefLink[i]);
			}

		}

	}

	for (int i = 0; i < area.size(); i++) {
		interx = interx + area[i] * RefPoint[i].first.x;
		intery = intery + area[i] * RefPoint[i].first.y;
		TotalA = TotalA + area[i];
	}

	interx = interx / TotalA;
	intery = intery / TotalA;
	wxRealPoint inter;
	inter.x = interx;
	inter.y = intery;

	m_ImagePanel->GetInter(inter, shapetype);


}

///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnInter2D(wxCommandEvent &event)
{
	//pair
	if (RefLink.size() > 2) {
		m_ImagePanel->Link2D(RefLink);
		//m_Panel3D->Link3D(RefLink);
		//Interpolate_2D_2_3D(RefLink);

	}

}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnInter3D(wxCommandEvent &event)
{
	
	if (RefLink.size() > 2) {
		//m_ImagePanel->Link2D(RefLink);
		m_Panel3D->Link3D(RefLink);
		//Interpolate_2D_2_3D(RefLink);

	}

}
///////////////////////////////////////////////////////////////////////////////
void MyFrame::OnTri(wxCommandEvent &event)
{
	if (RefLink.size() > 2) {
		m_ImagePanel->SetLink(RefLink);
		m_Panel3D->SetLink(RefLink);
		m_ImagePanel->ToggleDelaunay();
		m_Panel3D->ToggleDelaunay();
	}
}
///////////////////////////////////////////////////////////////////////////////

void MyFrame::OnOpenModel(wxCommandEvent &event)
{
	wxFileDialog * openFileDialog = new wxFileDialog(this);
	if (openFileDialog->ShowModal() == wxID_OK) {
		std::string fileName = openFileDialog->GetPath();
		m_Panel3D->LoadModel(fileName);
	}

}
///////////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
EVT_MENU(wxID_SAVE, MyFrame::OnSave)
EVT_MENU(TOOLS_OPEN3D, MyFrame::OnOpenModel)
EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

