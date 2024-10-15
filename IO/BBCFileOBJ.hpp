#ifndef BBC_FILEOBJ_HPP
#define BBC_FILEOBJ_HPP

#include <string>
#include <vector>

struct ObjPt{
	double m_X;
	double m_Y;
	double m_Z;
};
struct ObjPtTex{
	double m_u;
	double m_v;
};
struct ObjFace{
	unsigned int m_a;
	unsigned int m_b;
	unsigned int m_c;
	unsigned int m_at;
	unsigned int m_bt;
	unsigned int m_ct;
	unsigned int m_an;
	unsigned int m_bn;
	unsigned int m_cn;
};

class BBCFileOBJ
{
public:
	BBCFileOBJ();
	~BBCFileOBJ();
	
	const bool Read(const std::string& filename);

	const unsigned int NbPts()const{ return (unsigned int)m_pts.size(); }
	const ObjPt& GetPtAt(const unsigned int& i)const{ return m_pts[i]; };

	const unsigned int NbFaces()const{ return (unsigned int)m_faces.size(); }
	const ObjFace& GetFaceAt(const unsigned int& i)const{ return m_faces[i]; };

	const unsigned int NbPtsText()const{ return (unsigned int)m_pts_text.size(); }
	const ObjPtTex& GetPtTextAt(const unsigned int& i)const{ return m_pts_text[i]; };

	const unsigned int NbPtsNorm()const{ return (unsigned int)m_pts_norm.size(); }
	const ObjPt& GetPtNormAt(const unsigned int& i)const{ return m_pts_norm[i]; };

	const bool CoordText()const{ return m_coord_text; }
	const bool CoordNorm()const{ return m_coord_norm; }

	const ObjPt& GetBarycenter()const { return m_barycenter; }
private:
	void ReadPts(const std::string& ligne);
	void ReadPtsNorm(const std::string& ligne);
	void ReadPtsText(const std::string& ligne);
	void ReadFace(const std::string& ligne);
	void Parse(const std::string& ligne);
	unsigned int Decompose(const std::string& ligne, unsigned int& a, unsigned int&b, unsigned int&c);

	std::vector<ObjPt> m_pts;
	std::vector<ObjPt> m_pts_norm;
	std::vector<ObjPtTex> m_pts_text;
	std::vector<ObjFace> m_faces;

	bool m_coord_text;
	bool m_coord_norm;

	std::string m_filename;
	ObjPt m_barycenter;
};


#endif