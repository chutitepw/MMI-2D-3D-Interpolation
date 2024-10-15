#ifndef _OSGPICKING_H_
#define _OSGPICKING_H_

#include <osgGA/StateSetManipulator>
#include <osgText/Text>
#include <osgViewer/Viewer>

// class to handle events with a pick
class PickHandler : public osgGA::GUIEventHandler {
public:

    PickHandler(osgText::Text* updateText):
        _updateText(updateText) {}

    ~PickHandler() {}

    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

    virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	bool pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea, double *x, double *y, double *z);

    void setLabel(const std::string& name)
    {
        if (_updateText.get()) _updateText->setText(name);
    }

protected:

    osg::ref_ptr<osgText::Text>  _updateText;
};
///////////////////////////////////////////////////////////////////////////////
class OSGPick {
public:
	OSGPick() {}
	~OSGPick() {}
	bool pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea, double *x, double *y, double *z);
protected:
};

#endif // _OSGCANVAS_H_

