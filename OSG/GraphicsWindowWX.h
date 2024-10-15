#ifndef _GraphicsWindowWX_H_
#define _GraphicsWindowWX_H_

#include <osgViewer/GraphicsWindow>

class OSGCanvas;
class GraphicsWindowWX : public osgViewer::GraphicsWindow
{
 public:
   GraphicsWindowWX(OSGCanvas *canvas);
   ~GraphicsWindowWX();

   void init();

   void grabFocus();
   void grabFocusIfPointerInWindow();

   bool makeCurrentImplementation();
   void swapBuffersImplementation();

   // not implemented yet...just use dummy implementation to get working.
   virtual bool valid() const { return true; }
   virtual bool realizeImplementation() { return true; }
   virtual bool isRealizedImplementation() const  { return true; }
   virtual void closeImplementation() {}
   virtual bool releaseContextImplementation() { return true; }

 private:
	 OSGCanvas* m_OSGcanvas;
};

#endif // _GraphicsWindowWX_H_

