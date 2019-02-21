#ifndef _FBX_CONVTOR_EXPORT_H
#define _FBX_CONVTOR_EXPORT_H

#include "Utils.h"

#include <fbxsdk.h>

#include <eggEngine/SceneNode.h>
#include <eggEngine/ResourceManager.h>
#include <eggEngine/Scene.h>
#include <eggEngine/Components/SkeletonActorControler.h>
#include <eggEngine/Components/Joint3D.h>
#include <eggEngine/Components/Transfrom3D.h>


using namespace fbxsdk;

extern FbxScene* importFBXScene(FbxManager* lSdkManager, const char* lFilename);

extern egg::SceneNode* exportNode(FbxNode* pNode, egg::SkeletonActorControler* sac
    , std::map<std::string, std::string>& copy_image_list
    , bool collapse_mesh
    , bool skinning_mode);

extern egg::ComponentBase* exportMesh(FbxNode* pNode, FbxMesh* mesh, egg::SkeletonActorControler* sac, std::map<std::string, std::string>& copy_image_list, bool world_position);

extern egg::SkeletonActorControler* LoadSkeletonActorControler(FbxScene* scene);

extern egg::Animation* LoadAnimation(FbxScene* scene, int _frame_begin = -1, int _frame_count = -1);

extern egg::ComponentBase* exportCamera(FbxNode* pNode, FbxCamera* mesh);

extern egg::ComponentBase* exportLight(FbxNode* pNode, FbxLight* mesh);

extern egg::ComponentBase* exportLine(FbxNode* pNode, FbxLine* line);

extern egg::Texture* LoadMaterialTexture(FbxProperty* pProperty, std::map<std::string, std::string>& copy_image_list);

extern egg::Material* LoadMaterialAttribute(FbxSurfaceMaterial* pSurfaceMaterial, std::map<std::string, std::string>& copy_image_list);

//extern nbBase::ref_ptr<nbSG::Program> maya_normalmap_program;
//
//extern nbBase::ref_ptr<nbSG::Program> maya_skinning_normalmap_program;

//extern void ExportDefaultProgramAndShader(nbSG::ResourceManager* rmg);

//extern void ExportDefaultTextureAndImage(nbSG::ResourceManager* rmg);


#endif//_FBX_CONVTOR_EXPORT_H

