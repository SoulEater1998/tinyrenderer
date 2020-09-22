#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
	const int yinc=(y1>y0?1:-1);
	if(steep){
		for(int x=x0;x<x1;x++){
			image.set(y,x,color);
			error2+=derror2;
			if(error2>dx){
				y+=yinc;
				error2-=dx*2;
			}
		}
	}
	else{
		for(int x=x0;x<x1;x++){
			image.set(x,y,color);
			error2+=derror2;
			if(error2>dx){
				y+=yinc;
				error2-=dx*2;
			}
		}
	}
} 

int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);
	line(130, 200, 800, 400, image, white); 
	line(200, 130, 400, 800, image, red); 
	line(800, 400, 130, 200, image, red);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

