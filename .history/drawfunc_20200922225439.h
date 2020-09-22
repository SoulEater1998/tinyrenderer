#include "tgaimage.h"
#include "geometry.h"
#include <stdexcept>
#include <limits>

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

const int width  = 1920; 
const int height = 1080; 
 
Vec3f barycentric(Vec3f* pts, Vec3f P) { 
    Vec3f u = Vec3f(pts[2].x-pts[0].x, pts[1].x-pts[0].x, pts[0].x-P.x)^ Vec3f(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y-P.y);
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u.z)<1) return Vec3f(-1,1,1);
    return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
} 
 
void triangle(Vec3f *pts, float* zbuffer,TGAImage &image, TGAColor color) { 
    //这里的pts使用的世界坐标，或者说是垂直投影后的x，y然后加上世界坐标的z
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width()-1, image.get_height()-1); 
    for (int i=0; i<3; i++) { 
        bboxmin.x = std::max(0.f,     std::min(bboxmin.x, pts[i].x)); 
        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x)); 
        bboxmin.y = std::max(0.f,     std::min(bboxmin.y, pts[i].y)); 
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y)); 
        //TODO:optimize
    } 
    Vec3f P; 
    for (P.x=(int)bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=(int)bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            Vec3f bc_screen  = barycentric(pts, P); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 
            P.z=pts[0].z*bc_screen.x+pts[1].z*bc_screen.y+pts[2].z*bc_screen.z;
            if(zbuffer[int(P.x+P.y*width)]<P.z){
                zbuffer[int(P.x+P.y*width)]=P.z;
                image.set(P.x, P.y, color); 
            }
        } 
    } 
} 

void triangle(Vec3f *pts, Vec2i *vts, float* zbuffer, TGAImage &image, TGAImage &tecture, float intensity){
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width()-1, image.get_height()-1); 
    for (int i=0; i<3; i++) { 
        bboxmin.x = std::max(0.f,     std::min(bboxmin.x, pts[i].x)); 
        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x)); 
        bboxmin.y = std::max(0.f,     std::min(bboxmin.y, pts[i].y)); 
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y)); 
        //TODO:optimize
    } 
    Vec3f P; 
    Vec2i Pt;
    for (P.x=(int)bboxmin.x; P.x<=bboxmax.x; P.x++) { 
        for (P.y=(int)bboxmin.y; P.y<=bboxmax.y; P.y++) { 
            Vec3f bc_screen  = barycentric(pts, P); 
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 
            P.z=pts[0].z*bc_screen.x+pts[1].z*bc_screen.y+pts[2].z*bc_screen.z;
            if(zbuffer[int(P.x+P.y*width)]<P.z){
                zbuffer[int(P.x+P.y*width)]=P.z;
                Pt.x = vts[0].x*bc_screen.x+vts[1].x*bc_screen.y+vts[2].x*bc_screen.z;
                Pt.y = vts[0].y*bc_screen.x+vts[1].y*bc_screen.y+vts[2].y*bc_screen.z;
                TGAColor tempc = tecture.get(Pt.x,Pt.y);
                tempc.r *= intensity;
                tempc.g *= intensity;  
                tempc.b *= intensity; 
                image.set(P.x, P.y, tempc); 
            }
        } 
    }
}