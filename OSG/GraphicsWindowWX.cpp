#include "GraphicsWindowWX.h"

#include "OSGCanvas.h"


//#include <osgViewer/ViewerEventHandlers>
//#include <osgGA/TrackballManipulator>
//#include <osgDB/ReadFile>




GraphicsWindowWX::GraphicsWindowWX(OSGCanvas *canvas)
{
	m_OSGcanvas = canvas;

   _traits = new GraphicsContext::Traits;

   wxPoint pos = m_OSGcanvas->GetPosition();
   wxSize  size = m_OSGcanvas->GetSize();

   _traits->x = pos.x;
   _traits->y = pos.y;
   _traits->width = size.x;
   _traits->height = size.y;

   init();
}

GraphicsWindowWX::~GraphicsWindowWX()
{
}

void GraphicsWindowWX::init()
{
   if (valid())
      {
         setState( new osg::State );
         getState()->setGraphicsContext(this);

         if (_traits.valid() && _traits->sharedContext!=NULL)
            {
               getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
               incrementContextIDUsageCount( getState()->getContextID() );
            }
         else
            {
               getState()->setContextID( osg::GraphicsContext::createNewContextID() );
            }
      }
}

void GraphicsWindowWX::grabFocus()
{
   // focus the canvas
	m_OSGcanvas->SetFocus();
}

void GraphicsWindowWX::grabFocusIfPointerInWindow()
{
   // focus this window, if the pointer is in the window
   wxPoint pos = wxGetMousePosition();
   if (wxFindWindowAtPoint(pos) == m_OSGcanvas)
	   m_OSGcanvas->SetFocus();
}

bool GraphicsWindowWX::makeCurrentImplementation()
{
	m_OSGcanvas->SetCurrent(m_OSGcanvas->GetContext());
   return true;
}

void GraphicsWindowWX::swapBuffersImplementation()
{
	m_OSGcanvas->SwapBuffers();
}
