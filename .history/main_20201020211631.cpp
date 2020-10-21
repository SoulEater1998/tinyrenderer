#include"drawfunc.h"
#include"model.h"
#include<cstring>
#include<map>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Vec3f origin_point(0,0,0);
Vec3f 		   eye(0,1,3);
Vec3f       center(0,0,0);
Vec3f           up(0,1,0);
Vec3f    light_dir(0,0,-1);

struct vertex_normal{
	Vec3f normal;
	float all;
	Vec3f av_normal(){
		return normal/all;
	}
};

struct World_point{
	int index;
	Vec3f coord;
};


int main(int argc, char** argv) {
	TGAImage image(1920, 1080, TGAImage::RGB);
	TGAImage texture;
	texture.read_tga_file("obj/diablo3_pose_diffuse.tga");
	Model af_face("obj/diablo3_pose.obj");
	//Vec3f light_dir(0,0,-1);
	printf("2 buffer\n");
	float zbuffer[width*height]={0.f};
	float shadowbuffer[width*height];
	//memset(zbuffer,0,sizeof(zbuffer));
	//memset(shadowbuffer,0,sizeof(shadowbuffer));
	printf("begin read\n");
	//normal average
	std::map<int,vertex_normal> vertices_normal;
	for (int i=0; i<af_face.nfaces(); i++){
		std::vector<int> face = af_face.face(i);
		World_point world_points[3];
		for (int j=0; j<3; j++){
			world_points[j].index = face[j];
			world_points[j].coord = af_face.vert(world_points[j].index);
		}
		Vec3f n = (world_points[2].coord-world_points[0].coord)^(world_points[1].coord-world_points[0].coord).normalize();
		for (int j=0; j<3; j++){
			std::map<int,vertex_normal>::iterator it = vertices_normal.find(world_points[j].index);
			if(it != vertices_normal.end()){
				it->second.normal=it->second.normal+n;
				it->second.all+=1;
			}
			else{
				vertex_normal tempv;
				tempv.normal = n;
				tempv.all = 1.f;
				vertices_normal[world_points[j].index] = tempv;
			}
		}
	}
	printf("shadow buffer begin\n");
	for (int i=0; i<af_face.nfaces(); i++) {
		std::vector<int> face = af_face.face(i);
		Vec3f screen_coords[3];
		Vec3f world_coords[3];
		int index[3]; 
		for (int j=0; j<3; j++) { 
			index[j] = face[j];
        	Vec3f v = af_face.vert(index[j]);
			Matrix pv = viewport(0,0,height,height) * my_perspective(2.4f, 4.f, -3.f) * (lookat(light_dir*(-1),origin_point,up) * Matrix(v));
			screen_coords[j] = Vec3f(pv[0][0]/pv[3][0],pv[1][0]/pv[3][0],pv[2][0]/pv[3][0]);
			world_coords[j] = v;
    	}
		Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
		//材质默认为漫反射材质
		float intensity = n.normalize()*light_dir; 
    	if(intensity>0){
			shadow_triangle(screen_coords, shadowbuffer, image);
		} 
	}//*/
	printf("shadow buffer over\n");
	for (int i=0; i<af_face.nfaces(); i++) { 
    	std::vector<int> face = af_face.face(i);
		std::vector<int> texture_face = af_face.texture_face(i); 
    	Vec3f screen_coords[3];
		Vec2i texture_coords[3];
		Vec3f world_coords[3];
		Vec3f shadow_coords[3];
		//Vec3f world_norms[3];
		int index[3]; 
    	for (int j=0; j<3; j++) { 
			index[j] = face[j];
        	Vec3f v = af_face.vert(index[j]);
			Vec2f tv = af_face.texture_vert(texture_face[j]);
			texture_coords[j] = Vec2i((int)(tv.x*texture.get_height()),(int)((1-tv.y)*texture.get_width()));
			//world_norms[j] = af_face.norm(index[j]);
			//垂直投影
        	//screen_coords[j] = Vec3f((v.x+1.f)*height/2., (v.y+1.f)*height/2., v.z+1.f); 
			//先透视再放大
			//变换
			Matrix pv = viewport(0,0,height,height) * my_perspective(2.4f, 4.f, -3.f) * (lookat(eye,center,up) * Matrix(v));
			Matrix sv = viewport(0,0,height,height) * my_perspective(2.4f, 4.f, -3.f) * (lookat(light_dir*(-1),origin_point,up) * Matrix(v));
			//Matrix pv = viewport(0,0,height,height) * my_perspective(2.4f, 4.f, -3.f) * Matrix(v);
			//直接投影到背后
			//Vec3f pv = my_perspective(2.4f, Vec3f(v.x, v.y, v.z-1.f));
			screen_coords[j] = Vec3f(pv[0][0]/pv[3][0],pv[1][0]/pv[3][0],pv[2][0]/pv[3][0]);
			shadow_coords[j] = Vec3f(sv[0][0]/sv[3][0],sv[1][0]/sv[3][0],sv[2][0]/sv[3][0]);
			//先放大再透视(失败，事实上所谓放大是将图像进行了平移和缩放)
			//Vec3f pv = my_perspective(1.f, 3.f, Vec3f((v.x+1.f)*height/2, (v.y+1.f)*height/2, v.z-2.f));
			//screen_coords[j] = pv;
			//保存世界坐标计算法向量
			world_coords[j] = v;
    	}
		Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
		//材质默认为漫反射材质
		float intensity = n.normalize()*light_dir; 
    	if(intensity>0){
			triangle(screen_coords, texture_coords, zbuffer, image, texture, light_dir, 
						vertices_normal[index[0]].av_normal(), 
						vertices_normal[index[1]].av_normal(), 
						vertices_normal[index[2]].av_normal(),
						eye/*,shadowbuffer,shadow_coords*/);
			//triangle(screen_coords, texture_coords, zbuffer, image, texture, intensity);
			//triangle(screen_coords, texture_coords, zbuffer, image, texture, light_dir, 
			//			world_norms[0], world_norms[1], world_norms[2]);
		} 
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

