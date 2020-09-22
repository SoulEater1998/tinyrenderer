#include"drawfunc.h"
#include"model.h"
#include<string>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);
	Model af_face("african_head.obj");
	Vec3f light_dir(0,0,-1);
	float zbuffer[width*height];
	memset(zbuffer,0,sizeof(zbuffer));
	for (int i=0; i<af_face.nfaces(); i++) { 
    	std::vector<int> face = af_face.face(i); 
    	Vec3f screen_coords[3];
		Vec3f world_coords[3]; 
    	for (int j=0; j<3; j++) { 
        	Vec3f v = af_face.vert(face[j]); 
			//垂直投影
        	screen_coords[j] = Vec3f((v.x+1.f)*height/2., (v.y+1.f)*height/2.,v.z); 
			//保存世界坐标计算法向量
			world_coords[j] = v;
    	}
		Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
		//材质默认为漫反射材质
		float intensity = n.normalize()*light_dir; 
    	if(intensity>0) triangle(screen_coords, zbuffer, image, TGAColor(intensity*255, intensity*255, intensity*255, 255)); 
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

