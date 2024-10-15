#include "OSGCanvas.h"

#include "../MyFrame.h"

#include "OSGTools.h"
#include "GraphicsWindowWX.h"

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/UFOManipulator>


#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/PositionAttitudeTransform>


#include <wx/image.h>
#include <wx/menu.h>
#include <wx/menu.h>
#include <wx/dcclient.h>

#include <iostream>

#include "OSGFileObj.h"

#include "../Draw3DShape/Sphere.h"
#include "../Draw3DShape/3DReference.h"
#include "../Draw3DShape/3DPoint.h"
#include "../Draw3DShape/3DLine.h"
#include "../Draw3DShape/3DPolygon.h"

BEGIN_EVENT_TABLE(OSGCanvas, wxGLCanvas)
EVT_SIZE(OSGCanvas::OnSize)
EVT_PAINT(OSGCanvas::OnPaint)
EVT_ERASE_BACKGROUND(OSGCanvas::OnEraseBackground)

//EVT_CHAR(OSGCanvas::OnChar)
EVT_KEY_UP(OSGCanvas::OnKeyUp)

EVT_ENTER_WINDOW(OSGCanvas::OnMouseEnter)
EVT_LEFT_DOWN(OSGCanvas::OnMouseLeftDown)
EVT_MIDDLE_DOWN(OSGCanvas::OnMouseDown)
EVT_RIGHT_DOWN(OSGCanvas::OnMouseDown)
EVT_LEFT_UP(OSGCanvas::OnMouseLeftUp)
EVT_MIDDLE_UP(OSGCanvas::OnMouseUp)
EVT_RIGHT_UP(OSGCanvas::OnMouseUp)
EVT_MOTION(OSGCanvas::OnMouseMotion)
EVT_MOUSEWHEEL(OSGCanvas::OnMouseWheel)
END_EVENT_TABLE()

#include "OSGPicking.h"



// changed constructor to new wxGLCanvas constructor in 2.9.6
OSGCanvas::OSGCanvas(wxWindow *parent, bool is_alone, wxWindowID id,
	const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *attributes)
	: wxGLCanvas(parent, id, attributes, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name)
{
	m_context = new wxGLContext(this);
	m_viewer = new osgViewer::Viewer;
	m_graphics_window = new GraphicsWindowWX(this);
	m_viewer->getCamera()->setGraphicsContext(m_graphics_window);
	sizex = size.x;
	sizey = size.y;
	/* Camera mode */
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
	keyswitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator());
	keyswitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator());
	keyswitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator());
	keyswitchManipulator->addMatrixManipulator('4', "Terrain", new osgGA::TerrainManipulator());
	keyswitchManipulator->addMatrixManipulator('5', "FPM", new osgGA::FirstPersonManipulator());
	keyswitchManipulator->addMatrixManipulator('6', "UFO", new osgGA::UFOManipulator());

	m_viewer->setCameraManipulator(keyswitchManipulator.get());
	m_viewer->getCamera()->setViewport(0, 0, size.x, size.y);
	m_viewer->getCamera()->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 1.0f));
	m_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);


	m_pGroup = new osg::Group;
	m_pGroupRefPts = new osg::Group;
	m_DelTri = new osg::Group;
	//m_pShape = new osg::Group;
	m_pGroup->addChild(m_pGroupRefPts);
	m_pGroup->addChild(m_DelTri);
	//m_pGroup->addChild(m_pShape);

	osg::ref_ptr<osgText::Text> updateText = new osgText::Text;
	m_viewer->addEventHandler(new PickHandler(updateText.get()));

	InitLight();

	//sample exemple to load "pyramid"
	InitializeObject();

	
	//sample exemple to load file obj
	OSGFileOBJ file_obj("C:/MMI/Split/images/Modele_HEIG.obj", 0.5, 0.2, 0.2, 0.5);
	if (file_obj.Init()) {
		osg::ref_ptr<osg::Node> node=file_obj.createNodeFileObj("C:/MMI/Split/images/Modele_HEIG.obj", 532, 1209, 457, 0.5, 0.2, 0.2, 0.5);
		m_pGroup->addChild(node);
	}

	m_viewer->setSceneData(m_pGroup);


	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool OSGCanvas::LoadModel(std::string filename) {

	//remove last model
	m_pGroup->removeChild((m_pGroup->getNumChildren())-1,1);
	OSGFileOBJ file_obj(filename, 0.5, 0.2, 0.2, 0.5);
	if (file_obj.Init()) {
		osg::ref_ptr<osg::Node> node = file_obj.createNodeFileObj(filename, 532, 1209, 457, 0.5, 0.2, 0.2, 0.5);
		m_pGroup->addChild(node);
	}

	m_viewer->setSceneData(m_pGroup);
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OSGCanvas::InitializeObject() {

	
	/* Draw pyramid */
	/*osg::Geode* pyramidGeode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> pyramidGeometry = new osg::Geometry();

	// Associate the pyramid geometry with the pyramid geode
	// Add the pyramid geode to the root node of the scene graph.
	pyramidGeode->addDrawable(pyramidGeometry.get());
	m_pGroup->addChild(pyramidGeode);

	
	osg::Vec3 sp(10, 0, 0);
	osg::Vec3 ep(10, 10, 0);
	osg::ref_ptr<osg::Geometry> beam(new osg::Geometry);
	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array;
	points->push_back(sp);
	points->push_back(ep);
	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
	color->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	beam->setVertexArray(points.get());
	beam->setColorArray(color.get());
	beam->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
	beam->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 2));

	osg::Geode *geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> g(new osg::Geometry);
	osg::Vec3Array *v = new osg::Vec3Array;
	pyramidGeode->addDrawable(beam.get());
	
	osg::Vec3 sp1(10, 0, 0);
	osg::Vec3 sp2(0, 0, 0);
	osg::Vec3 sp3(10, 0, 10);
	osg::Vec3 sp4(0, 0, 10);
	v->push_back(sp1);
	v->push_back(sp2);
	v->push_back(sp3);
	
	v->push_back(sp4);

	osg::DrawArrays *da = new
		osg::DrawArrays(osg::PrimitiveSet::LINES, 0, v->size());

	g->setVertexArray(v);
	g->addPrimitiveSet(da);


	geode->addDrawable(g);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pyramidGeode->addDrawable(g.get());
	
	
	

	// Declare an array of vertices.
	osg::ref_ptr<osg::Vec3Array> pyramidVertices = new osg::Vec3Array;
	pyramidVertices->push_back(osg::Vec3(0, 0, 0)); // front left
	pyramidVertices->push_back(osg::Vec3(10, 0, 0)); // front right
	pyramidVertices->push_back(osg::Vec3(10, 10, 0)); // back right
	pyramidVertices->push_back(osg::Vec3(0, 10, 0)); // back left
	pyramidVertices->push_back(osg::Vec3(5, 5, 10)); // peak

													 // Associate this set of vertices with the geometry associated with the
													 // geode we added to the scene.
	pyramidGeometry->setVertexArray(pyramidVertices.get());

	// Create a primitive set and add it to the pyramid geometry.
	osg::ref_ptr<osg::DrawElementsUInt> pyramidBase = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	pyramidBase->push_back(3);
	pyramidBase->push_back(2);
	pyramidBase->push_back(1);
	pyramidBase->push_back(0);
	pyramidGeometry->addPrimitiveSet(pyramidBase.get());

	// Repeat the same for each of the four sides. Again, vertices are
	// specified in counter-clockwise order.
	osg::ref_ptr<osg::DrawElementsUInt> pyramidFaceOne = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceOne->push_back(0);
	pyramidFaceOne->push_back(1);
	pyramidFaceOne->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceOne.get());

	osg::ref_ptr<osg::DrawElementsUInt> pyramidFaceTwo = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceTwo->push_back(1);
	pyramidFaceTwo->push_back(2);
	pyramidFaceTwo->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceTwo.get());

	osg::ref_ptr<osg::DrawElementsUInt> pyramidFaceThree = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceThree->push_back(2);
	pyramidFaceThree->push_back(3);
	pyramidFaceThree->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceThree.get());

	osg::ref_ptr<osg::DrawElementsUInt> pyramidFaceFour = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceFour->push_back(3);
	pyramidFaceFour->push_back(0);
	pyramidFaceFour->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceFour.get());

	//Declare and load an array of Vec4 elements to store colors.
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); //index 0 red
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); //index 1 green
	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); //index 2 blue
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //index 3 white

														  // Declare the variable that will match vertex array elements to color
														  // array elements. 
														  // Needed incase the number of vertices and the number of colors are different.
	osg::ref_ptr<osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>> colorIndexArray;
	colorIndexArray = new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>;
	colorIndexArray->push_back(0); // vertex 0 assigned color array element 0
	colorIndexArray->push_back(1); // vertex 1 assigned color array element 1
	colorIndexArray->push_back(2); // vertex 2 assigned color array element 2
	colorIndexArray->push_back(3); // vertex 3 assigned color array element 3
	colorIndexArray->push_back(0); // vertex 4 assigned color array element 0

								   //The next step is to associate the array of colors with the geometry,
								   //assign the color indices created above to the geometry and set the
								   //binding mode to BIND_PER_VERTEX.
	pyramidGeometry->setColorArray(colors.get());
	//	pyramidGeometry->setColorIndices(colorIndexArray.get());
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	// Declare and initialize a transform node.
	osg::PositionAttitudeTransform* pyramidTwoXForm = new osg::PositionAttitudeTransform();

	// Use the 'addChild' method of the osg::Group class to
	// add the transform as a child of the root node and the
	// pyramid node as a child of the transform.
	m_pGroup->addChild(pyramidTwoXForm);
	pyramidTwoXForm->addChild(pyramidGeode);

	// Declare and initialize a Vec3 instance to change the
	// position of the model in the scene
	osg::Vec3 pyramidTwoPosition(15, 0, 0);
	pyramidTwoXForm->setPosition(pyramidTwoPosition);*/

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::LightSource> OSGCanvas::Light(unsigned int id, double x, double y, double z) {

	osg::ref_ptr<osg::Light> pLight = new osg::Light;
	pLight->setLightNum(id);
	pLight->setAmbient(osg::Vec4d(0.1, 0.1, 0.1, 1.0));
	if (id == 1 || id == 2) {
		pLight->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pLight->setSpecular(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else {
		pLight->setDiffuse(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
		pLight->setSpecular(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
	}
	pLight->setPosition(osg::Vec4(x, y, z, 1.0f));

	osg::Vec3 dir(-x, -y, -z);
	dir.normalize();

	pLight->setDirection(dir);

	osg::ref_ptr<osg::LightSource> pLightSource = new osg::LightSource;
	pLightSource->setLight(pLight);
	pLightSource->setLocalStateSetModes(osg::StateAttribute::ON);
	return pLightSource;
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::InitLight() {
	/*
	osg::BoundingSphere bs = g->getBound();
	if (bs.radius() < 0)
		return;
	bs.radius() *= 1.5f;
	osg::BoundingBox bb;
	bb.expandBy(bs);
	wxString s;
	s <<" X : "<< bb.xMin() << " " << bb.xMax() ;
	s << " Y : " << bb.yMin() << " " << bb.yMax();
	s << " Z : " << bb.zMin() << " " << bb.zMax();
	wxLogMessage(s);
	*/
	osg::StateSet* state = m_pGroup->getOrCreateStateSet();

	state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	state->setMode(GL_LIGHT0, osg::StateAttribute::OFF);
	//	m_pGroup->addChild(Light(1, bb.xMax(), bb.yMin(), bb.zMax()));
	//	m_pGroup->addChild(Light(2, bb.xMin(), bb.yMax(), bb.zMax()));
	m_pGroup->addChild(Light(1, 100.0, -100.0, 100));
	m_pGroup->addChild(Light(2, -100.0, 100.0, 100));
	m_pGroup->addChild(Light(3, 100.0, 100.0, 100));
	m_pGroup->addChild(Light(4, -100.0, -100.0, 100));
	state->setMode(GL_LIGHT1, osg::StateAttribute::ON);
	state->setMode(GL_LIGHT2, osg::StateAttribute::ON);
	state->setMode(GL_LIGHT3, osg::StateAttribute::ON);
	state->setMode(GL_LIGHT4, osg::StateAttribute::ON);
}

///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::Update() {
	m_viewer->frame();
}
///////////////////////////////////////////////////////////////////////////////
OSGCanvas::~OSGCanvas()
{
}
///////////////////////////////////////////////////////////////////////////////
wxGLContext& OSGCanvas::GetContext()
{
	return *m_context;
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	/* must always be here */
	wxPaintDC dc(this);
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	// method not available in wxWidgets 2.9.6
	//    wxGLCanvas::OnSize(event);

	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int width, height;
	GetClientSize(&width, &height);

	if (m_graphics_window->valid())
	{
		// update the window dimensions, in case the window has been resized.
		m_graphics_window->getEventQueue()->windowResize(0, 0, width, height);
		m_graphics_window->resized(0, 0, width, height);
		m_viewer->getCamera()->setViewport(0, 0, width, height);
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	/* Do nothing, to avoid flashing on MSW */
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnCharM(int key)
{

	if (m_graphics_window->valid())
		m_graphics_window->getEventQueue()->keyPress(key);

	// If this key event is not processed here, we should call
	// event.Skip() to allow processing to continue.
}
///////////////////////////////////////////////////////////////////////////////
/*void OSGCanvas::OnChar(wxKeyEvent &event)
{
#if wxUSE_UNICODE
	int key = event.GetUnicodeKey();
#else
	int key = event.GetKeyCode();
#endif

	if (m_graphics_window->valid())
		m_graphics_window->getEventQueue()->keyPress(key);

	// If this key event is not processed here, we should call
	// event.Skip() to allow processing to continue.
}*/
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnKeyUp(wxKeyEvent &event)
{
#if wxUSE_UNICODE
	int key = event.GetUnicodeKey();
#else
	int key = event.GetKeyCode();
#endif

	if (m_graphics_window->valid())
		m_graphics_window->getEventQueue()->keyRelease(key);

	// If this key event is not processed here, we should call
	// event.Skip() to allow processing to continue.
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnMouseEnter(wxMouseEvent &event)
{
	// Set focus to ourselves, so keyboard events get directed to us
	if (m_parent->HasFocus())
		SetFocus();
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnMouseDown(wxMouseEvent &event)
{
	if (m_graphics_window->valid())
	{
		m_graphics_window->getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(),
			event.GetButton());

		/* Get 3D postition */
		double x, y, z;
		bool res = Get3DPt(event.GetX(), event.GetY(), &x, &y, &z);
		if (res) {
			std::stringstream ss;
			ss << "x : " << x << " y : " << y << " z : " << z;
			((MyFrame*)(m_parent->GetParent()))->SetInfoStatusBar(ss.str());
		}
		else {
			((MyFrame*)(m_parent->GetParent()))->SetInfoStatusBar("UNDEFINED");

		}

	}
}
///////////////////////////////////////////////////////////////////////////////
void  OSGCanvas::OnMouseLeftDown(wxMouseEvent &event) {
	m_leftClickDown = true;
	m_leftClickDownPos = event.GetPosition();
	if (m_graphics_window->valid())
		m_graphics_window->getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(),event.GetButton());
}
///////////////////////////////////////////////////////////////////////////////
void  OSGCanvas::OnMouseLeftUp(wxMouseEvent &event) {
	m_leftClickDown = false;
	if (m_graphics_window->valid())
		m_graphics_window->getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(), event.GetButton());
	//return;
	if (m_leftClickDownPos == event.GetPosition()) {
		double x, y, z;
		bool res = Get3DPt(event.GetX(), event.GetY(), &x, &y, &z);
		if (res) {
			
			if (d_tools == dt_Reference)
				Reference(x, y, z);
			if (d_tools == dt_Select)
				Selection(x, y, z);
			if (d_tools == dt_Point) 
				Point(x, y, z);
			if (d_tools == dt_Line)
				DLine(x, y, z);
			if (d_tools == dt_Sphere)
				Sphere(x, y, z);
			if (d_tools == dt_Polygon)
				Polygon(x, y, z);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnMouseUp(wxMouseEvent &event)
{
	if (m_graphics_window->valid())
	{
		m_graphics_window->getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(),
			event.GetButton());
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnMouseMotion(wxMouseEvent &event)
{
	if (m_graphics_window->valid())
		m_graphics_window->getEventQueue()->mouseMotion(event.GetX(), event.GetY());
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OnMouseWheel(wxMouseEvent &event)
{
	int delta = event.GetWheelRotation() / event.GetWheelDelta() * event.GetLinesPerAction();

	if (m_graphics_window->valid()) {
		m_graphics_window->getEventQueue()->mouseScroll(
			delta > 0 ?
			osgGA::GUIEventAdapter::SCROLL_UP :
			osgGA::GUIEventAdapter::SCROLL_DOWN);
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::SetPosCam(double x, double y, double z, double vx, double vy, double vz, double fov) {
	osg::Vec3d eye(x, y, z);
	osg::Vec3d center(vx, vy, vz);

	osg::Vec3d v = (center - eye);
	v.normalize();
	osg::Vec3d up(0.0, 0.0, 1.0);
	osg::Vec3d left = up ^ v;
	up = (v^left);
	up.normalize();
	//	m_viewer->getCamera()->setViewMatrixAsLookAt(eye, center, up);


	m_viewer->getCamera()->setProjectionMatrixAsPerspective(fov, 1.0, 0.5, 5000);
	m_viewer->getCamera()->setViewMatrix(osg::Matrix::lookAt(eye, center, up));

	Update();
}

#include <osg/ShapeDrawable>
#include <osg/Material>
///////////////////////////////////////////////////////////////////////////////
bool OSGCanvas::Get3DPt(const double& x_pix, const double& y_pix, double* x, double* y, double* z) {
	int width, height;
	GetClientSize(&width, &height);
	osgGA::GUIEventAdapter* ga = new osgGA::GUIEventAdapter;
	ga->setWindowRectangle(0, 0, width, height);
	ga->setX(x_pix);
	ga->setY(height - y_pix);
	osgText::Text* t = new  osgText::Text;
	PickHandler test(t);
	return test.pick(m_viewer, *ga, x, y, z);
}
/******** test ***********/
void OSGCanvas::Reference(double x, double y, double z) {

	refcount++;
	DShapeReference * dsp = new DShapeReference();
	dsp->SetData(x, y, z);
	dsp->SetRefpoint(refcount);
	dsp->Draw(this);
	m_dshapes.push_back(dsp);
	/*for (unsigned int i = 0; i < m_dshapes.size(); ++i)
	m_dshapes[i]->Draw(this);*/
}
void OSGCanvas::Sphere(double x, double y, double z) {
	
	if (m_dshape_sphere != NULL) {
		m_dshape_sphere->SetDatas(x, y, z);

		m_dshape_sphere->Draw(this);
		m_dshapes.push_back(m_dshape_sphere);
		m_dshape_sphere = NULL;
		
		return;
	}
	if (m_dshape_sphere == NULL) {

		m_dshape_sphere = new DShapeSphere();
		m_dshape_sphere->SetDatas(x, y, z);

	}
}
void OSGCanvas::Point(double x, double y, double z) {

	DShapePoint * dsp = new DShapePoint();
	dsp->SetData(x, y, z);
	dsp->Draw(this);
	m_dshapes.push_back(dsp);

	
	
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::DLine(double x, double y, double z) {

	/*DShapeLine * dsp = new DShapeLine();
	dsp->SetData(x, y, z);
	dsp->Draw(this);
	m_dshapes.push_back(dsp);*/
	
	if (m_dshape_line != NULL) {
		m_dshape_line->SetDatas(x, y, z);
		
		m_dshape_line->Draw(this);
		

		return;
	}
	if (m_dshape_line == NULL) {
		
		m_dshape_line = new DShapeLine();
		m_dshape_line->SetDatas(x, y, z);
		//resetline == false;
		//m_dshapes.push_back(m_dshape_line);
	}
	
	
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::Polygon(double x, double y, double z) {

	if (m_dshape_polygon != NULL) {
		m_dshape_polygon->SetDatas(x, y, z);

		m_dshape_polygon->Draw(this);


		return;
	}
	if (m_dshape_polygon == NULL) {

		m_dshape_polygon = new DShapePolygon();
		m_dshape_polygon->SetDatas(x, y, z);
		
	}

}
///////////////////////////////////////////////////////////////////////////////
osg::Node* OSGCanvas::createHUD(osgText::Text* updateText)
{

	// create the hud. derived from osgHud.cpp
	// adds a set of quads, each in a separate Geode - which can be picked individually
	// eg to be used as a menuing/help system!
	// Can pick texts too!

	osg::Camera* hudCamera = new osg::Camera;
	hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	hudCamera->setProjectionMatrixAsOrtho2D(0, 1280, 0, 1024);
	hudCamera->setViewMatrix(osg::Matrix::identity());
	hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	std::string timesFont("fonts/times.ttf");

	// turn lighting off for the text and disable depth test to ensure its always ontop.
	osg::Vec3 position(150.0f, 800.0f, 0.0f);
	osg::Vec3 delta(0.0f, -60.0f, 0.0f);

	{
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		geode->setName("simple");
		hudCamera->addChild(geode);

		osgText::Text* text = new  osgText::Text;
		geode->addDrawable(text);

		text->setFont(timesFont);
		text->setText("Picking in Head Up Displays is simple!");
		text->setPosition(position);

		position += delta;
	}


	for (int i = 0; i < 5; i++) {
		osg::Vec3 dy(0.0f, -30.0f, 0.0f);
		osg::Vec3 dx(120.0f, 0.0f, 0.0f);
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		const char *opts[] = { "One", "Two", "Three", "January", "Feb", "2003" };
		osg::Geometry *quad = new osg::Geometry;
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		std::string name = "subOption";
		name += " ";
		name += std::string(opts[i]);
		geode->setName(name);
		osg::Vec3Array* vertices = new osg::Vec3Array(4); // 1 quad
		osg::Vec4Array* colors = new osg::Vec4Array;
		colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(0.8 - 0.1*i, 0.1*i, 0.2*i, 1.0));
		quad->setColorArray(colors, osg::Array::BIND_OVERALL);
		(*vertices)[0] = position;
		(*vertices)[1] = position + dx;
		(*vertices)[2] = position + dx + dy;
		(*vertices)[3] = position + dy;
		quad->setVertexArray(vertices);
		quad->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
		geode->addDrawable(quad);
		hudCamera->addChild(geode);

		position += delta;
	}



	{ // this displays what has been selected
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		geode->setName("The text label");
		geode->addDrawable(updateText);
		hudCamera->addChild(geode);

		updateText->setCharacterSize(20.0f);
		updateText->setFont(timesFont);
		updateText->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		updateText->setText("");
		updateText->setPosition(position);
		updateText->setDataVariance(osg::Object::DYNAMIC);

		position += delta;
	}

	return hudCamera;

}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::SaveShape(wxXmlNode* node){
	wxXmlNode * Xml3D = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("DDD"));
	node->AddChild(Xml3D);
	for (unsigned int i = 0; i < m_dshapes.size(); ++i) {
		//m_shapes[i]->WriteXML(XmlShape);
		m_dshapes[i]->WriteXML(Xml3D);
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::OpenShape(wxXmlNode* child)
{
	/*for (unsigned int i = 0; i < m_dshapes.size(); ++i) {
		
		m_dshapes.erase(m_dshapes.begin() + i);
		
	}*/
	m_pGroupRefPts->removeChildren(0, m_pGroupRefPts->getNumChildren());
	m_DelTri->removeChildren(0, m_DelTri->getNumChildren());
	m_dshapes.clear();
	while (child) {
		DShape* dshape = NULL;
		if (child->GetName() == "dreference") {
			dshape = new DShapeReference();
			/*Get ref number*/
			wxXmlNode *child2 = child->GetChildren();
			while (child2) {
				if (child2->GetName() == "pt") {
					wxXmlNode *child3 = child2->GetChildren();
					while (child3) {
						if (child3->GetName() == "Ref") {
							wxString s = child3->GetNodeContent();
							s.ToLong(&refcount);
						}
						child3 = child3->GetNext();
					}
				}
				child2 = child2->GetNext();
			}
		}
		if (child->GetName() == "dpoint") {
			dshape = new DShapePoint();
		}
		if (child->GetName() == "dline") {
			dshape = new DShapeLine();
		}
		if (child->GetName() == "dsphere") {
			dshape = new DShapeSphere();
		}
		if (child->GetName() == "dpolygon") {
			dshape = new DShapePolygon();
		}
		if (dshape != NULL) {
			bool res = dshape->LoadXML(child);
			if (res == true)
				m_dshapes.push_back(dshape);
			else
				delete dshape;
		}
		child = child->GetNext();
	}//while
	for (unsigned int i = 0; i < m_dshapes.size(); ++i)
		m_dshapes[i]->Draw(this);
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::Selection(double x, double y, double z) {
	if (!m_dshapes.empty()) {
		//dc.SetPen(*wxRED_PEN);
		double d_min = m_dshapes[0]->Distance(x, y, z);
		m_dselect = 0;
		unsigned int pos_min = 0;
		for (unsigned int i = 1; i < m_dshapes.size(); ++i) {
			double d = m_dshapes[i]->Distance(x, y, z);
			if (d < d_min) {
				d_min = d;
				m_dselect = i;
			}
		}
		if (m_dselect == m_dselected) {
			//make the standard the color here
			osg::Node* node = m_pGroupRefPts->getChild(m_dselected);
			osg::ref_ptr<osg::Material> mat = new osg::Material;
			mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1., 1., 1.0));
			node->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);

			m_dshape_select = NULL;
			m_dselected = 999;
		}
		else {
			//remove old colour 
			for(int i=0; i<m_dshapes.size();i++){
				osg::Node* nodede = m_pGroupRefPts->getChild(i);
				osg::ref_ptr<osg::Material> mat1 = new osg::Material;
				mat1->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1., 1., 1.0));
				nodede->getOrCreateStateSet()->setAttributeAndModes(mat1, osg::StateAttribute::OVERRIDE);
			}
			m_dselected = m_dselect;
			m_dshape_select = m_dshapes[m_dselect];
			//change the color to red
			osg::Node* node= m_pGroupRefPts->getChild(m_dselected);
			osg::ref_ptr<osg::Material> mat2 = new osg::Material;
			mat2->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 0., 0., 1.0));
			node->getOrCreateStateSet()->setAttributeAndModes(mat2,  osg::StateAttribute::OVERRIDE);
		}
		//m_dshape_select_temp = m_dshapes[i_min];
		//m_dshape_select_temp->Draw(&dc, this);
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::Remove()
{
	if (m_dshape_select != NULL) {
		m_dshape_select = NULL;
		m_dshapes.erase(m_dshapes.begin() + m_dselect);
		m_pGroupRefPts->removeChild(m_dselect);
		m_dselected = 999;
	}
	/*std::stringstream ss;
	ss << "delete";
	((MyFrame*)(m_parent->GetParent()))->SetInfoStatusBar(ss.str());*/
}
///////////////////////////////////////////////////////////////////////////////
DPoint OSGCanvas::RSelect()
{
	if (m_dshape_select != NULL) {
		if (typeid(*m_dshape_select) == typeid(DShapeReference)) {
			DPoint refp = m_dshape_select->RSelectShape();
			return refp;
		}
		else {
			DPoint refp;
			refp.x = NULL;
			refp.y = NULL;
			refp.z = NULL;
			return refp;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::refText(wxRealPoint pt, DPoint dpt)
{
	if(m_dshape_select!= NULL)
		m_dshape_select->SetRefT(pt, dpt);
}
///////////////////////////////////////////////////////////////////////////////
double OSGCanvas::Area(std::vector<DPoint> &refpt) {

	osg::Vec3d ab(refpt[1].x - refpt[0].x, refpt[1].y - refpt[0].y, refpt[1].z - refpt[0].z);
	osg::Vec3d ac(refpt[2].x - refpt[0].x, refpt[2].y - refpt[0].y, refpt[2].z - refpt[0].z);
	double area = ((ab ^ ac).length()) / 2;

	/*double x = ((refpt[1].x - refpt[0].x)*(refpt[2].y - refpt[0].y) - (refpt[1].y - refpt[0].y)*(refpt[2].x - refpt[0].x));
	double y = ((refpt[1].y - refpt[0].y)*(refpt[2].z - refpt[0].z) - (refpt[1].z - refpt[0].z)*(refpt[2].y - refpt[0].y));
	double z = ((refpt[1].z - refpt[0].z)*(refpt[2].x - refpt[0].x) - (refpt[1].x - refpt[0].x)*(refpt[2].z - refpt[0].z));
	double area = sqrt((x * x) + (y * y) + (z * z)) / 2;*/
	return area;
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::Link3D(std::vector <std::pair <wxRealPoint, DPoint>> Link3D) {
	RefLink = Link3D;

	double sumarea;
	double Tarea;
	std:: vector<DPoint> triangle;
	std::vector<double> area;

	if (!RefLink.empty() && RefLink.size() > 2) {
		Delaunay();
		for (int n = 0; n < m_dshapes.size(); n++) {
			if (typeid(*m_dshapes[n]) == typeid(DShapePoint)) {
				std::string shapetype = "Point";
				
				for (int j = 0; j < Dtriangle.size(); j += 3)
				{
					sumarea = 0;

					triangle.push_back(Dtriangle[j]);
					triangle.push_back(Dtriangle[j + 1]);
					triangle.push_back(Dtriangle[j + 2]);

					Tarea = Area(triangle);

					for (int i = 0; i < triangle.size(); i++) {
						DPoint temp_pt;
						temp_pt = triangle[i];
						triangle[i] = m_dshapes[n]->GetDatas();
						double calarea = Area(triangle);
						area.push_back(calarea);
						triangle[i] = temp_pt;
						sumarea = sumarea + calarea;
					}
					if (trunc(1000.*sumarea) == trunc(1000.*Tarea)) {
						((MyFrame*)(m_parent->GetParent()))->Interpolate_3D_2_2D(shapetype, area, triangle);
						area.clear();
						triangle.clear();
						//break;
					}
					else {
						area.clear();
						triangle.clear();
					}
				}
			}
			if (typeid(*m_dshapes[n]) == typeid(DShapeLine)) {
				std::string shapetype = "Line";
				std::vector<osg::Vec3> vecpts = m_dshapes[n]->GetMultiDatas();
				std::vector<DPoint> pts;
				for (int i = 0; i < vecpts.size(); i++) {
					DPoint temp_pts;
					temp_pts.x = vecpts[i].x();
					temp_pts.y = vecpts[i].y();
					temp_pts.z = vecpts[i].z();

					pts.push_back(temp_pts);
				}
				for (int l=0; l < pts.size(); l++) {
					for (int j = 0; j < Dtriangle.size(); j += 3)
					{
						sumarea = 0;

						triangle.push_back(Dtriangle[j]);
						triangle.push_back(Dtriangle[j + 1]);
						triangle.push_back(Dtriangle[j + 2]);

						Tarea = Area(triangle);

						for (int i = 0; i < triangle.size(); i++) {
							DPoint temp_pt;
							temp_pt = triangle[i];
							triangle[i] = pts[l];
							double calarea = Area(triangle);
							area.push_back(calarea);
							triangle[i] = temp_pt;
							sumarea = sumarea + calarea;
						}
						if (trunc(1000.*sumarea) == trunc(1000.*Tarea)) {
							((MyFrame*)(m_parent->GetParent()))->Interpolate_3D_2_2D(shapetype, area, triangle);
							area.clear();
							triangle.clear();
							//break;
						}
						else {
							area.clear();
							triangle.clear();
						}
					}
				}
				((MyFrame*)(m_parent->GetParent()))->OnEndDShape();
			}
			if (typeid(*m_dshapes[n]) == typeid(DShapePolygon)) {
				std::string shapetype = "Polygon";

				std::vector<osg::Vec3> vecpts = m_dshapes[n]->GetMultiDatas();
				std::vector<DPoint> pts;
				for (int i = 0; i < vecpts.size(); i++) {
					DPoint temp_pts;
					temp_pts.x = vecpts[i].x();
					temp_pts.y = vecpts[i].y();
					temp_pts.z = vecpts[i].z();

					pts.push_back(temp_pts);
				}
				for (int l = 0; l < pts.size(); l++) {
					for (int j = 0; j < Dtriangle.size(); j += 3)
					{
						sumarea = 0;

						triangle.push_back(Dtriangle[j]);
						triangle.push_back(Dtriangle[j + 1]);
						triangle.push_back(Dtriangle[j + 2]);

						Tarea = Area(triangle);

						for (int i = 0; i < triangle.size(); i++) {
							DPoint temp_pt;
							temp_pt = triangle[i];
							triangle[i] = pts[l];
							double calarea = Area(triangle);
							area.push_back(calarea);
							triangle[i] = temp_pt;
							sumarea = sumarea + calarea;
						}
						if (trunc(1000.*sumarea) == trunc(1000.*Tarea)) {
							((MyFrame*)(m_parent->GetParent()))->Interpolate_3D_2_2D(shapetype, area, triangle);
							area.clear();
							triangle.clear();
							//break;
						}
						else {
							area.clear();
							triangle.clear();
						}
					}
				}
				((MyFrame*)(m_parent->GetParent()))->OnEndDShape();
			}
			if (typeid(*m_dshapes[n]) == typeid(DShapeSphere)) {
				std::string shapetype = "Circle";

				std::vector<osg::Vec3> vecpts = m_dshapes[n]->GetMultiDatas();
				std::vector<DPoint> pts;
				for (int i = 0; i < vecpts.size(); i++) {
					DPoint temp_pts;
					temp_pts.x = vecpts[i].x();
					temp_pts.y = vecpts[i].y();
					temp_pts.z = vecpts[i].z();

					pts.push_back(temp_pts);
				}
				for (int l = 0; l < pts.size(); l++) {
					for (int j = 0; j < Dtriangle.size(); j += 3)
					{
						sumarea = 0;

						triangle.push_back(Dtriangle[j]);
						triangle.push_back(Dtriangle[j + 1]);
						triangle.push_back(Dtriangle[j + 2]);

						Tarea = Area(triangle);

						for (int i = 0; i < triangle.size(); i++) {
							DPoint temp_pt;
							temp_pt = triangle[i];
							triangle[i] = pts[l];
							double calarea = Area(triangle);
							area.push_back(calarea);
							triangle[i] = temp_pt;
							sumarea = sumarea + calarea;
						}
						if (trunc(1000.*sumarea) == trunc(1000.*Tarea)) {
							((MyFrame*)(m_parent->GetParent()))->Interpolate_3D_2_2D(shapetype, area, triangle);
							area.clear();
							triangle.clear();
							//break;
						}
						else {
							area.clear();
							triangle.clear();
						}
					}
				}
				((MyFrame*)(m_parent->GetParent()))->OnEndDShape();
			}
		}
		

	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::SetLink(std::vector <std::pair <wxRealPoint, DPoint>> Link3D) {
	RefLink = Link3D;
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::ToggleDelaunay() {
	if (T_Delaunay) {
		T_Delaunay = false;
		m_DelTri->removeChildren(0, m_DelTri->getNumChildren());
	}
		
	else {
		
		T_Delaunay = true;
		Delaunay();
		/*Interpol*/
	/*	if (!RefLink.empty() && RefLink.size() > 2) {

			
			osg::Geode* geometryGeode = new osg::Geode();
			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			osgUtil::DelaunayTriangulator* DTri = new osgUtil::DelaunayTriangulator();
			osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array();

			points->clear();

			m_DelTri->removeChildren(0, m_DelTri->getNumChildren());
			//osg::Vec3Array* points = new osg::Vec3Array();
			osg::Vec3Array *normals = 0;

			
			std::vector<DPoint> RefPoint;
			std::vector<double> area;

			for (int i = 0; i < RefLink.size(); i++) {
				RefPoint.push_back(RefLink[i].second);

				points->push_back(osg::Vec3(RefLink[i].second.x, RefLink[i].second.y, RefLink[i].second.z));

			}
			osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
			color->push_back(osg::Vec4(1.0, 1.0, 1.0, 0.5));
			geometry->setColorArray(color.get());
			geometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

			DTri->setInputPointArray(points);
			DTri->triangulate();
			m_DelTri->addChild(geometryGeode);

			geometry->setVertexArray(points.get());
			geometry->addPrimitiveSet(DTri->getTriangles());

			geometryGeode->addDrawable(geometry.get());

		}*/
	}
		
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::Delaunay() {
	
		m_DelTri->removeChildren(0, m_DelTri->getNumChildren());
		
		if (!RefLink.empty() && RefLink.size() > 2) {

			/*Delaunay Triangulator*/
			osg::Geode* geometryGeode = new osg::Geode();
			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			osgUtil::DelaunayTriangulator* DTri = new osgUtil::DelaunayTriangulator();
			osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array();

			points->clear();

			m_DelTri->removeChildren(0, m_DelTri->getNumChildren());
			//osg::Vec3Array* points = new osg::Vec3Array();
			osg::Vec3Array *normals = 0;

			/*Cal area*/
			std::vector<DPoint> RefPoint;
			std::vector<double> area;

			for (int i = 0; i < RefLink.size(); i++) {
				RefPoint.push_back(RefLink[i].second);

				points->push_back(osg::Vec3(RefLink[i].second.x, RefLink[i].second.y, RefLink[i].second.z));

			}
			osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
			color->push_back(osg::Vec4(0.0, 1.0, 0.0, 0.1));
			geometry->setColorArray(color.get());
			geometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

			DTri->setInputPointArray(points);
			DTri->triangulate();

			osg::DrawElements* datas = DTri->getTriangles();
			Dtriangle.clear();
			for (int k = 0; k < datas->getNumIndices(); k++)
			{
				Dtriangle.push_back(RefLink[datas->getElement(k)].second);
			}

			if (T_Delaunay) {
				m_DelTri->addChild(geometryGeode);

				geometry->setVertexArray(points.get());
				geometry->addPrimitiveSet(datas);
				DTri->getOutputNormalArray();
				geometryGeode->addDrawable(geometry.get());
			}

		}
	

}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::ESC()
{
	if (d_tools == dt_Line) {
		if (m_dshape_line != NULL) {
			//m_dshapes.push_back(m_dshape_line);
			m_pGroupRefPts->removeChild(m_pGroupRefPts->getNumChildren()-1);
			
			m_dshape_line = NULL;
		}
	}
	if (d_tools == dt_Sphere) {
		if (m_dshape_sphere != NULL) {
			m_dshape_sphere = NULL;
		}
	}
	if (d_tools == dt_Polygon) {
		if (m_dshape_polygon != NULL) {
			//m_dshapes.push_back(m_dshape_polygon);

			m_pGroupRefPts->removeChild(m_pGroupRefPts->getNumChildren()-1);
			m_dshape_polygon = NULL;
		}
	}
		
	
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::Enter()
{
	if (d_tools == dt_Line) {
		if (m_dshape_line != NULL) {

			
			m_dshapes.push_back(m_dshape_line);

			/*Change color after finish drawing*/
			osg::Node* node = m_pGroupRefPts->getChild(m_dshapes.size()-1);
			osg::ref_ptr<osg::Material> mat = new osg::Material;
			mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1., 1, 1.0));
			node->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);


			m_dshape_line = NULL;

		}
		
		
	}

	if (d_tools == dt_Polygon) {
		if (m_dshape_polygon != NULL) {
			m_dshapes.push_back(m_dshape_polygon);

			/*Change color after finish drawing*/
			osg::Node* node = m_pGroupRefPts->getChild(m_dshapes.size() - 1);
			osg::ref_ptr<osg::Material> mat = new osg::Material;
			mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1., 1, 1.0));
			node->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);

			m_dshape_polygon = NULL;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void OSGCanvas::EndShape(std::string shapetype)
{
	if (shapetype == "Circle") {
		m_dshape_sphere = NULL;
	}
	if (shapetype == "Line") {
		m_dshapes.push_back(m_dshape_line);
		m_dshape_line = NULL;
	}
	if (shapetype == "Polygon" || shapetype == "Rectangle") {
		m_dshapes.push_back(m_dshape_polygon);
		m_dshape_polygon = NULL;
	}
		
}
///////////////////////////////////////////////////////////////////////////////