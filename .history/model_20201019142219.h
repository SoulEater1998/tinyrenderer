#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec2f> texture_verts_;
	std::vector<std::vector<int> > faces_;
	std::vector<std::vector<int> > texture_faces_;
	std::vector<Vec3f> norms_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int ntverts();
	int nfaces();
	Vec3f vert(int i);
	Vec2f texture_vert(int i);
	std::vector<int> face(int idx);
	std::vector<int> texture_face(int idx);
	Vec3f norm(int i);
};



#endif //__MODEL_H__