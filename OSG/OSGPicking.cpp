#include "OSGPicking.h"
#include <sstream>
#include <osg/io_utils>
bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
    switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::PUSH):
        {
            osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
            if (view) pick(view,ea);
            return false;
        }
        case(osgGA::GUIEventAdapter::KEYDOWN):
        {
            if (ea.getKey()=='c')
            {
                osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
                osg::ref_ptr<osgGA::GUIEventAdapter> event = new osgGA::GUIEventAdapter(ea);
                event->setX((ea.getXmin()+ea.getXmax())*0.5);
                event->setY((ea.getYmin()+ea.getYmax())*0.5);
                if (view) pick(view,*event);
            }
            return false;
        }
        default:
            return false;
    }
}

void PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea){
    osgUtil::LineSegmentIntersector::Intersections intersections;

    std::string gdlist="";

    if (view->computeIntersections(ea,intersections))
    {
        for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
            hitr != intersections.end();
            ++hitr)
        {
            std::ostringstream os;
            if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
            {
                // the geodes are identified by name.
                os<<"Object \""<<hitr->nodePath.back()->getName()<<"\""<<std::endl;
            }
            else if (hitr->drawable.valid())
            {
                os<<"Object \""<<hitr->drawable->className()<<"\""<<std::endl;
            }

            os<<"        local coords vertex("<< hitr->getLocalIntersectPoint()<<")"<<"  normal("<<hitr->getLocalIntersectNormal()<<")"<<std::endl;
            os<<"        world coords vertex("<< hitr->getWorldIntersectPoint()<<")"<<"  normal("<<hitr->getWorldIntersectNormal()<<")"<<std::endl;
            const osgUtil::LineSegmentIntersector::Intersection::IndexList& vil = hitr->indexList;
            for(unsigned int i=0;i<vil.size();++i)
            {
                os<<"        vertex indices ["<<i<<"] = "<<vil[i]<<std::endl;
            }

            gdlist += os.str();
        }
    }
    setLabel(gdlist);
}

bool  PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea, double *x, double *y, double *z){
	bool trouver = false;
	osgUtil::LineSegmentIntersector::Intersections intersections;
	if (view->computeIntersections(ea, intersections))	{
		for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			osg::Vec3d M = hitr->getWorldIntersectPoint();
			*x = M.x();
			*y = M.y();
			*z = M.z();
			trouver = true;
		}
	}
	return trouver;
}
///////////////////////////////////////////////////////////////////////////////
bool OSGPick::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea, double *x, double *y, double *z){
	bool trouver = false;
	osgUtil::LineSegmentIntersector::Intersections intersections;
	if (view->computeIntersections(ea, intersections))	{
		for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			osg::Vec3d M = hitr->getWorldIntersectPoint();
			*x = M.x();
			*y = M.y();
			*z = M.z();
			trouver = true;
		}
	}
	return trouver;
}
