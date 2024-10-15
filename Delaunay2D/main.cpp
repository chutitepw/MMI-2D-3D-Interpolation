#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <array>


#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"

#include <wx/wx.h>
float RandomFloat(float a, float b) {
    const float random = ((float) rand()) / (float) RAND_MAX;
    const float diff = b - a;
    const float r = random * diff;
    return a + r;
}

int main(int argc, char * argv[])
{
	int numberPoints = 40;

	std::cout << "Generating " << numberPoints << " random points" << std::endl;

	std::vector<Vector2<float> > points;
	for(int i = 0; i < numberPoints; ++i) {
		points.push_back(Vector2<float>(RandomFloat(0, 800), RandomFloat(0, 600)));
	}

	Delaunay<float> triangulation;
	const std::vector<Triangle<float> > triangles = triangulation.triangulate(points);
	std::cout << triangles.size() << " triangles generated\n";
	const std::vector<Edge<float> > edges = triangulation.getEdges();

	std::cout << " ========= ";

	std::cout << "\nPoints : " << points.size() << std::endl;
	for(const auto &p : points)
		std::cout << p << std::endl;

	std::cout << "\nTriangles : " << triangles.size() << std::endl;
	for(const auto &t : triangles)
		std::cout << t << std::endl;

	std::cout << "\nEdges : " << edges.size() << std::endl;
	for(const auto &e : edges)
		std::cout << e << std::endl;

	wxInitAllImageHandlers();
	wxBitmap bmp(wxSize(800, 600));
	wxMemoryDC dc(bmp);
	dc.SetPen(*wxGREEN_PEN);
	for (unsigned int i = 0; i < edges.size(); ++i) {
		dc.DrawLine(wxCoord( edges[i].p1.x), wxCoord(edges[i].p1.y), wxCoord(edges[i].p2.x), wxCoord(edges[i].p2.y));
	}
	dc.SetPen(*wxRED_PEN);
	for (unsigned int i = 0; i < points.size(); ++i) {
		dc.DrawCircle(wxCoord(points[i].x), wxCoord(points[i].y),4);
	}
	wxImage  image(bmp.ConvertToImage());
	image.SaveFile("test.png");
	/*
	    img = wx.Image("./poster/Test2.png", wx.BITMAP_TYPE_ANY)
    bit = wx.EmptyBitmap(517,524)
    imgBit = wx.BitmapFromImage(img)
    dc = wx.MemoryDC(imgBit)
    dc.SetPen(wx.Pen(wx.RED, 1))
    dc.SelectObject(wx.NullBitmap)# I didn't know I had to deselect the DC
    self.imageCtrl.SetBitmap(imgBit)  
    imgBit.SaveFile("bit.bmp", wx.BITMAP_TYPE_BMP)
	*/

	return 0;
}
