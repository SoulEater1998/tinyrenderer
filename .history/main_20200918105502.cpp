#include"drawfunc.h"
#include"model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);
	Model af_face("african_head.obj");
	Vec3f light_dir(0,0,-1);
	for (int i=0; i<af_face.nfaces(); i++) { 
    	std::vector<int> face = af_face.face(i); 
    	Vec2i screen_coords[3];
		Vec3f world_coords[3]; 
    	for (int j=0; j<3; j++) { 
        	Vec3f v = af_face.vert(face[j]); 
			//垂直投影
        	screen_coords[j] = Vec2i((v.x+1.f)*height/2., (v.y+1.f)*height/2.); 
			//保存世界坐标计算法向量
			world_coords[j] = v;
    	}
		Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
		float intensity = n.normalize()*light_dir; 
    	if(intensity>0) triangle(screen_coords, image, TGAColor(intensity*255, intensity*255, intensity*255, 255)); 
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

