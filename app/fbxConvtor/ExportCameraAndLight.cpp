
#include "Export.h"

#include <iostream>
#include <list>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Components/Camera.h>
#include <eggEngine/Components/Light.h>


using namespace fbxsdk;



/*
*   输出camera相关信息
*/
egg::ComponentBase* exportCamera(FbxNode* pNode, FbxCamera* camera)
{
    egg::Camera* nb_cam = new egg::Camera();


    return nb_cam;
}


/*
*   输出light相关信息
*/
egg::ComponentBase* exportLight(FbxNode* pNode, FbxLight* ligbt)
{

    egg::Light* nb_light = new egg::Light();


    return nb_light;
}
