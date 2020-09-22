#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
//#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            float ftrash;
            Vec2f v;
            for (int i=0;i<2;i++) iss >> v.raw[i];
            iss >> trash;
            texture_verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            std::vector<int> tex_f;
            int tex_idx, idx, itrash;
            iss >> trash;
            while (iss >> idx >> trash >> tex_idx >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                tex_idx--;
                f.push_back(idx);
                tex_f.push_back(tex_idx);
            }
            faces_.push_back(f);
            texture_faces_.push_back(tex_f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::ntverts() {
    return (int)texture_verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

std::vector<int> Model::texture_face(int idx) {
    return texture_faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2f Model::texture_vert(int i){
    return texture_verts_[i];
}