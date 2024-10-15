CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (Split)


find_package(wxWidgets COMPONENTS adv base core xml aui xrc html zlib png expat gl REQUIRED)
include( "${wxWidgets_USE_FILE}" )
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
message(STATUS "${wxWidgets_INCLUDE_DIRS}")
message(STATUS "${wxWidgets_LIBRARIES}")

#find_package(OpenSceneGraph  REQUIRED osgText osgSim osgDB osgParticle osgGA osgViewer osgUtil HINTS "C:/Program Files/OpenSceneGraph")
#find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer) 
#INCLUDE_DIRECTORIES(${OPENSCENEGRAPH_INCLUDE_DIRS})
#message(STATUS "${OPENSCENEGRAPH_INCLUDE_DIRS}")
#message(STATUS "${OPENSCENEGRAPH_LIBRARIES}")

#FIND_PACKAGE(OpenThreads)
#FIND_PACKAGE(osg)
#FIND_PACKAGE(osgText)
#FIND_PACKAGE(osgSim)
#FIND_PACKAGE(osgDB)
#FIND_PACKAGE(osgParticle)
#FIND_PACKAGE(osgUtil)
#FIND_PACKAGE(osgGA)
#FIND_PACKAGE(osgViewer)

INCLUDE_DIRECTORIES("C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/include")

ADD_EXECUTABLE( 
	Split
	WIN32 
	MainRotate.h
	mainRotate.cpp
	Constants.h
	MyFrame.h
	MyFrame.cpp
	ScrolledImageComponent.h
	ScrolledImageComponent.cpp
	DPoint.h
	./DrawShape/Shape.hpp
	./DrawShape/Rectangle.hpp
	./DrawShape/Rectangle.cpp
	./DrawShape/Circle.hpp
	./DrawShape/Circle.cpp
	./DrawShape/Point.hpp
	./DrawShape/Point.cpp
	./DrawShape/Line.hpp
	./DrawShape/Line.cpp
	./DrawShape/Polygon.hpp
	./DrawShape/Polygon.cpp
	./DrawShape/Reference.hpp
	./DrawShape/Reference.cpp
	
	./Draw3DShape/3DReference.cpp
	./Draw3DShape/3DReference.h
	./Draw3DShape/Sphere.cpp
	./Draw3DShape/Sphere.h
	./Draw3DShape/3DShape.h
	./Draw3DShape/3DPoint.h
	./Draw3DShape/3DPoint.cpp
	./Draw3DShape/3DLine.h
	./Draw3DShape/3DLine.cpp
	./Draw3DShape/3DPolygon.h
	./Draw3DShape/3DPolygon.cpp
	
	./OSG/OSGCanvas.h
	./OSG/OSGCanvas.cpp
	./OSG/OSGTools.h
	./OSG/OSGTools.cpp
	./OSG/OSGPicking.h
	./OSG/OSGPicking.cpp
	./OSG/GraphicsWindowWX.h
	./OSG/GraphicsWindowWX.cpp
	./OSG/OSGFileObj.h
	./OSG/OSGFileObj.cpp
	./IO/BBCFileOBJ.hpp
	./IO/BBCFileOBJ.cpp
	
	./Delaunay2D/delaunay.h
	./Delaunay2D/edge.h
	./Delaunay2D/numeric.h
	./Delaunay2D/triangle.h
	./Delaunay2D/vector2.h
)

ADD_DEFINITIONS(  -DOSG_LIBRARY_STATIC)

TARGET_LINK_LIBRARIES (
	Split
	${wxWidgets_LIBRARIES}
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/OpenThreads.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osg.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgAnimation.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgDB.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgFX.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgGA.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgManipulator.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgParticle.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgPresentation.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgShadow.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgSim.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgTerrain.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgText.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgUtil.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgUI.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgViewer.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgVolume.lib"
optimized "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgWidget.lib"

debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/OpenThreadsd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgAnimationd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgDBd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgFXd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgGAd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgManipulatord.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgParticled.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgPresentationd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgShadowd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgSimd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgTerraind.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgTextd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgUId.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgUtild.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgViewerd.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgVolumed.lib"
debug "C:/MMI/OpenSceneGraph-OpenSceneGraph-3.6.0/BUILDCmake/lib/osgWidgetd.lib"
)


