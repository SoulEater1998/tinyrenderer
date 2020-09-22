#include"drawfunc.h"
#include"model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);
	Model af_face("african_head.obj");
	for (int i=0; i<af_face.nfaces(); i++) { 
    	std::vector<int> face = af_face.face(i); 
    	Vec2i screen_coords[3]; 
    	for (int j=0; j<3; j++) { 
        	Vec3f world_coords = af_face.vert(face[j]); 
			//垂直投影
        	screen_coords[j] = Vec2i((world_coords.x+1.)*height/2., (world_coords.y+1.)*height/2.); 
    	} 
    	triangle(screen_coords, image, TGAColor(rand()%255, rand()%255, rand()%255, 255)); 
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

