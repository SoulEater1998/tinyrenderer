#include"drawfunc.h"
#include"model.h"
#include<cstring>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);
	TGAImage texture;
	texture.read_tga_file("african_head_diffuse.tga");
	Model af_face("african_head.obj");
	Vec3f light_dir(0,0,-1);
	float zbuffer[width*height];
	memset(zbuffer,0,sizeof(zbuffer));
	for (int i=0; i<af_face.nfaces(); i++) { 
    	std::vector<int> face = af_face.face(i);
		std::vector<int> texture_face = af_face.texture_face(i); 
    	Vec3f screen_coords[3];
		Vec2i texture_coords[3];
		Vec3f world_coords[3]; 
    	for (int j=0; j<3; j++) { 
        	Vec3f v = af_face.vert(face[j]);
			Vec2f tv = af_face.texture_vert(texture_face[j]);
			texture_coords[j] = Vec2i((int)((1-tv.x)*texture.get_height()),(int)((1-tv.y)*texture.get_width()));
			//垂直投影
        	//screen_coords[j] = Vec3f((v.x+1.f)*height/2., (v.y+1.f)*height/2., v.z+1.f); 
			//先透视再放大
			Vec3f pv = my_perspective(1.4f, 100.f, Vec3f(v.x, v.y, v.z-2.f));
			screen_coords[j] = Vec3f((pv.x+1.f)*height/2, (pv.y+1.f)*height/2, pv.z);
			//先放大再透视(失败，事实上所谓放大是将图像进行了平移和缩放)
			//Vec3f pv = my_perspective(1.f, 3.f, Vec3f((v.x+1.f)*height/2, (v.y+1.f)*height/2, v.z-2.f));
			//screen_coords[j] = pv;
			//保存世界坐标计算法向量
			world_coords[j] = v;
    	}
		Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
		//材质默认为漫反射材质
		float intensity = n.normalize()*light_dir; 
    	if(intensity>0) triangle(screen_coords, texture_coords, zbuffer, image, texture, intensity);
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

