#include "tgaimage.h"
#include "geometry.h"

void line(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor color) { 
	int x0=t0.x;
	int y0=t0.y;
	int x1=t1.x;
	int y1=t1.y;
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

