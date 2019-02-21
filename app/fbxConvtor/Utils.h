#ifndef _FBX_UTILS_EXPORT_H
#define _FBX_UTILS_EXPORT_H

#include <vector>
#include <fstream>
//#include <vmmlib/matrix.hpp>
#include <Eigen/Eigen>
#include <fbxsdk.h>

std::vector<char> readfile(std::string filename);


//合并当前node的local矩阵
Eigen::Matrix4f getLocalMatrix(FbxNode* node);

Eigen::Matrix4f getWorldMatrix(FbxNode* node);

bool hasSkin(FbxMesh* pMesh);

void getCustomNodeProperty(FbxNode* pNode);

//遍历场景搜索joint的bindpose位置
void getPose(FbxScene* fbx_scene);


#endif//_FBX_UTILS_EXPORT_H

