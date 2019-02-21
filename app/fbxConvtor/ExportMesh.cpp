
#include "Export.h"

#include <iostream>
#include <list>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Components/MeshRender.h>


using namespace fbxsdk;




//��¼һ�����Ƶ��Ȩ����Ϣ
struct ControlPointWeight
{
    ControlPointWeight() :index(0), weight(0) {}

    std::vector<int>   index;
    std::vector<float> weight;
};

void AssociateSkeletonWithCtrlPoint(FbxSkin* pSkin, egg::SkeletonActorControler* sac, std::vector<ControlPointWeight>& controlpointweights)
{
    FbxCluster::ELinkMode linkMode = FbxCluster::eNormalize;
    FbxCluster* pCluster = 0;
    FbxNode*    pLinkNode = 0;
    int         skeletonIndex = 0;
    //CSkeleton*   pSkeleton;
    FbxAMatrix  transformMatrix, transformLinkMatrix;
    int         clusterCount = pSkin->GetClusterCount();

    // ������ǰSkin�е�ÿ��Cluster����Ӧ��Skeleton��  
    for (int i = 0; i < clusterCount; ++i)
    {
        //��ȡ��
        pCluster = pSkin->GetCluster(i);
        if (!pCluster) { continue; }

        //��ȡ�ض�Ӧ��node
        pLinkNode = pCluster->GetLink();
        if (!pLinkNode) { continue; }

        //��ǰnode��������
        int skeletonIndex = egg::SkeletonActorControler::getSkeletonIndex(pLinkNode->GetName(), sac->mSkeletonMapping);

        // �õ�ÿ��Cluster��Skeleton������Ӧ��Transform��TransformLink���󣬺������˵��  
        pCluster->GetTransformMatrix(transformMatrix);
        pCluster->GetTransformLinkMatrix(transformLinkMatrix);

        // �������˵Ĳ�������Transform��TransformLinkת��Ϊӳ����󲢴洢����Ӧ��Skeleton��  
        // ...  

        int     associatedCtrlPointCount = pCluster->GetControlPointIndicesCount();
        int*    pCtrlPointIndices = pCluster->GetControlPointIndices();
        double* pCtrlPointWeights = pCluster->GetControlPointWeights();
        int     ctrlPointIndex = 0;
        double  ctrlPointWeight = 0;

        // ������ǰCluster��Ӱ�쵽��ÿ��Vertex����������Ӧ����Ϣ����¼�Ա�Skinningʱʹ��  
        for (int j = 0; j < associatedCtrlPointCount; ++j)
        {
            ctrlPointIndex = pCtrlPointIndices[j];  //����index
            ctrlPointWeight = pCtrlPointWeights[j];  //��Ӧ��ǰNode��Ȩ��
            //ctrlPointSkeletonList[ctrlPointIndex].AddSkeleton(skeletonIndex , pCtrlPointWeights[j]);  

            //�õ����Ƶ�����
            //printf("joint:%s,id:%d,vertex_index=%d,weight=%f\n" , pLinkNode->GetName(),skeletonIndex , ctrlPointIndex, ctrlPointWeight);
            controlpointweights[ctrlPointIndex].index.push_back(skeletonIndex);
            controlpointweights[ctrlPointIndex].weight.push_back((float)ctrlPointWeight);
        }
    }

}

//��ȡ����index ��Ӧ�Ĺ�����Ȩ����Ϣ
void AssociateSkeletonWithCtrlPoint(FbxMesh* pMesh, egg::SkeletonActorControler* sac, std::vector<ControlPointWeight>& controlpointweights)
{
    int ctrlPointCount = pMesh->GetControlPointsCount();
    int deformerCount = pMesh->GetDeformerCount();

    //��ʼ���������
    //ctrlPointSkeletonList.SetCapacity(ctrlPointCount);
    //ctrlPointSkeletonList.setListSize(ctrlPointCount);
    //controlpointweights.resize(ctrlPointCount);

    FbxDeformer* pFBXDeformer;
    FbxSkin*     pFBXSkin;

    for (int i = 0; i < deformerCount; ++i)
    {
        //��ȡ������
        pFBXDeformer = pMesh->GetDeformer(i);
        if (pFBXDeformer == NULL) { continue; }

        //��ȡskin����
        if (pFBXDeformer->GetDeformerType() != FbxDeformer::eSkin) { continue; }

        //��ȡskin����
        pFBXSkin = (FbxSkin*)(pFBXDeformer);
        if (pFBXSkin == NULL) { continue; }

        //��ʼ���������
        controlpointweights.resize(ctrlPointCount);

        //����skin
        AssociateSkeletonWithCtrlPoint(pFBXSkin, sac, controlpointweights);
    }
}



//��ȡ��Ӧ������Ĳ�������
void ConnectMaterialToMesh(FbxMesh* pMesh, int triangleCount, int* pTriangleMtlIndex)
{
    // Get the material index list of current mesh  
    FbxLayerElementArrayTemplate<int>* pMaterialIndices;
    FbxGeometryElement::EMappingMode   materialMappingMode = FbxGeometryElement::eNone;

    if (pMesh->GetElementMaterial())
    {
        pMaterialIndices = &pMesh->GetElementMaterial()->GetIndexArray();
        materialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
        if (pMaterialIndices)
        {
            switch (materialMappingMode)
            {
            case FbxGeometryElement::eByPolygon:
            {
                if (pMaterialIndices->GetCount() == triangleCount)
                {
                    for (int triangleIndex = 0; triangleIndex < triangleCount; ++triangleIndex)
                    {
                        int materialIndex = pMaterialIndices->GetAt(triangleIndex);

                        pTriangleMtlIndex[triangleIndex] = materialIndex;
                    }
                }
            }
            break;

            case FbxGeometryElement::eAllSame:
            {
                int lMaterialIndex = pMaterialIndices->GetAt(0);

                for (int triangleIndex = 0; triangleIndex < triangleCount; ++triangleIndex)
                {
                    int materialIndex = pMaterialIndices->GetAt(triangleIndex);

                    pTriangleMtlIndex[triangleIndex] = materialIndex;
                }
            }
            }
        }
    }
}

//����������
void ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, float* pVertex)
{
    FbxVector4* pCtrlPoint = pMesh->GetControlPoints();

    pVertex[0] = (float)pCtrlPoint[ctrlPointIndex][0];
    pVertex[1] = (float)pCtrlPoint[ctrlPointIndex][1];
    pVertex[2] = (float)pCtrlPoint[ctrlPointIndex][2];
    //pVertex[3] = (float)pCtrlPoint[ctrlPointIndex][3];
}

//������ɫ��
void ReadColor(FbxMesh* pMesh, int uvLayer, int ctrlPointIndex, int vertexCounter, float* pColor)
{
    //if(pMesh->GetElementVertexColorCount() < 1)  
    //{  
    //    return;  
    //}  

    FbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);
    switch (pVertexColor->GetMappingMode())
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (pVertexColor->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            pColor[0] = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
            pColor[1] = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
            pColor[2] = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
            pColor[3] = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
            pColor[0] = (float)pVertexColor->GetDirectArray().GetAt(id).mRed;
            pColor[1] = (float)pVertexColor->GetDirectArray().GetAt(id).mGreen;
            pColor[2] = (float)pVertexColor->GetDirectArray().GetAt(id).mBlue;
            pColor[3] = (float)pVertexColor->GetDirectArray().GetAt(id).mAlpha;
        }
        break;

        default:
            break;
        }
    }
    break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (pVertexColor->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            pColor[0] = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;
            pColor[1] = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;
            pColor[2] = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;
            pColor[3] = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;
        }
        break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);
            pColor[0] = (float)pVertexColor->GetDirectArray().GetAt(id).mRed;
            pColor[1] = (float)pVertexColor->GetDirectArray().GetAt(id).mGreen;
            pColor[2] = (float)pVertexColor->GetDirectArray().GetAt(id).mBlue;
            pColor[3] = (float)pVertexColor->GetDirectArray().GetAt(id).mAlpha;
        }
        break;
        default:
            break;
        }
    }
    break;
    }
}

//��UV����
void ReadUV(FbxMesh* pMesh, int uvLayer, int ctrlPointIndex, int textureUVIndex, float* pUV)
{
    //if(uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)  
    //{  
    //    return;
    //}

    FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);

    switch (pVertexUV->GetMappingMode())
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (pVertexUV->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            pUV[0] = (float)pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[0];
            pUV[1] = (float)pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[1];
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);
            pUV[0] = (float)pVertexUV->GetDirectArray().GetAt(id)[0];
            pUV[1] = (float)pVertexUV->GetDirectArray().GetAt(id)[1];
        }
        break;

        default:
            break;
        }
    }
    break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (pVertexUV->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        case FbxGeometryElement::eIndexToDirect:
        {
            pUV[0] = (float)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
            pUV[1] = (float)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
        }
        break;

        default:
            break;
        }
    }
    break;
    }
}

//����������
void ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, float* pNormal)
{
    //if(pMesh->GetElementNormalCount() < 1)  
    //{
    //    return;  
    //}

    FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);
    switch (leNormal->GetMappingMode())
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (leNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            pNormal[0] = (float)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[0];
            pNormal[1] = (float)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[1];
            pNormal[2] = (float)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[2];
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex);
            pNormal[0] = (float)leNormal->GetDirectArray().GetAt(id)[0];
            pNormal[1] = (float)leNormal->GetDirectArray().GetAt(id)[1];
            pNormal[3] = (float)leNormal->GetDirectArray().GetAt(id)[2];
        }
        break;

        default:
            break;
        }
    }
    break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (leNormal->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            pNormal[0] = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[0];
            pNormal[1] = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[1];
            pNormal[2] = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[2];
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int id = leNormal->GetIndexArray().GetAt(vertexCounter);
            pNormal[0] = (float)leNormal->GetDirectArray().GetAt(id)[0];
            pNormal[1] = (float)leNormal->GetDirectArray().GetAt(id)[1];
            pNormal[2] = (float)leNormal->GetDirectArray().GetAt(id)[2];
        }
        break;

        default:
            break;
        }
    }
    break;
    }
}

//����������
void ReadTangent(FbxMesh* pMesh, int uvLayer, int ctrlPointIndex, int vertecCounter, float* pTangent)
{
    //if(pMesh->GetElementTangentCount() < 1)  
    //{  
    //    return;  
    //}  

    FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);

    switch (leTangent->GetMappingMode())
    {
    case FbxGeometryElement::eByControlPoint:
    {
        switch (leTangent->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            pTangent[0] = (float)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[0];
            pTangent[1] = (float)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[1];
            pTangent[2] = (float)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[2];
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex);
            pTangent[0] = (float)leTangent->GetDirectArray().GetAt(id)[0];
            pTangent[1] = (float)leTangent->GetDirectArray().GetAt(id)[1];
            pTangent[2] = (float)leTangent->GetDirectArray().GetAt(id)[2];
        }
        break;

        default:
            break;
        }
    }
    break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (leTangent->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        {
            pTangent[0] = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[0];
            pTangent[1] = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[1];
            pTangent[2] = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[2];
        }
        break;

        case FbxGeometryElement::eIndexToDirect:
        {
            int id = leTangent->GetIndexArray().GetAt(vertecCounter);
            pTangent[0] = (float)leTangent->GetDirectArray().GetAt(id)[0];
            pTangent[1] = (float)leTangent->GetDirectArray().GetAt(id)[1];
            pTangent[2] = (float)leTangent->GetDirectArray().GetAt(id)[2];
        }
        break;

        default:
            break;
        }
    }
    break;
    }
}


/*
*   ���mesh�����Ϣ
*       geometry material
*
*   geometry������� ���RAW��ʽ������
*       vec4 ����λ�� (FBXò��ֻ֧��vec4)
*       vec3 ����     (�����)
*       vec3 ����     (�����)
*       vec2 UV��Ϣ   (�����)
*       vec4 ����ɫ�� (�����)
*
*   world_position �Ƿ������������ݵ���������
*/
egg::ComponentBase* exportMesh(FbxNode* pNode, FbxMesh* mesh, egg::SkeletonActorControler* sac, std::map<std::string, std::string>& copy_image_list, bool world_position)
{

    //��Ϊǿ�����ǻ��� ������������polygon����������
    int triangleCount = mesh->GetPolygonCount();
    if (triangleCount <= 0) return nullptr;

    //��ѯ����������(����������ֵ��û���κ�˳��)
    int material_count = pNode->GetMaterialCount();
    std::vector< int > material_index(triangleCount);//����Ĭ�ϳ�ʼ������ȫ��Ϊ0 ������ʲ����ھ���ȫ0
    ConnectMaterialToMesh(mesh, triangleCount, &material_index[0]);


    //����Ȩ����Ϣ ��ȡ���Ƶ��Ӧ��������Ȩ�ر�
    std::vector<ControlPointWeight> controlpointweights;
    AssociateSkeletonWithCtrlPoint(mesh, sac, controlpointweights);

    //����Ȩ����Ϣ ֻȡ4��Ȩ�� ���ҹ�һ��Ȩ�غ�
    {
    }

    //��ȡ��ǰmesh���������
    vmml::mat4f current_mesh_matrix = getWorldMatrix(pNode);
    //FbxAMatrix current_mesh_matrix; //Ĭ�ϵ�λ����
    if (world_position)
    {
        //FbxTime keyTimer;
        //keyTimer.SetSecondDouble( 0.0 );

        ////��������
        //FbxAMatrix curveKeyLocalMatrix  = pNode->EvaluateLocalTransform(/*keyTimer*/);
        //FbxAMatrix curveKeyGlobalMatrix = pNode->EvaluateGlobalTransform(/*keyTimer*/);

        //current_mesh_matrix = curveKeyGlobalMatrix;
    }

    //��ѯ��ǰ����ͨ���Ƿ����
    bool has_vertex = true;     //Ĭ��һ������
    bool has_color[4] = { false };  //0-3��ɫ��ͨ���Ƿ����
    bool has_uv[4] = { false };  //0-3��uvͨ���Ƿ����
    bool has_normal = false;    //λ�Ƶķ���ͨ���Ƿ����
    bool has_tangent[4] = { false };  //��Ӧ��ͬ��uvͨ�� Ӧ�ô��ڶ�Ӧ������ͨ��
    bool has_weight = controlpointweights.size() > 0;    //Ȩ����Ϣ
    bool has_boneindex = controlpointweights.size() > 0;    //Ȩ�ض�Ӧ����
    has_normal = mesh->GetElementNormalCount() > 0;  //ʵ���Ϸ�����Ϣû�г���1һ��ͨ��������
    for (int k = 0; k < mesh->GetElementVertexColorCount() && k < 4; ++k) { has_color[k] = true; }
    for (int k = 0; k < mesh->GetElementUVCount() && k < 4; ++k) { has_uv[k] = true; }
    for (int k = 0; k < mesh->GetElementTangentCount() && k < 4; ++k) { has_tangent[k] = true; }

    //����mesh���
    egg::MeshRender* meshrender = new egg::MeshRender();
    egg::ref_ptr<egg::Geometry> geometry = new egg::Geometry();
    //meshrender->setGeometry( geometry.get() );

    //����geometry������ 
    //      ��Ϊ��Ҫgeometry�����������ظ� 
    //      ���Լ򵥷�������ʹ��fbx��UID 
    //      FBX��UID������Ϊ��ȡʱ��ʹ������ı� ��������ļ������̶���
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(sizeof(FbxUInt64) * 2) << mesh->GetUniqueID();
    geometry->setUniformPath(oss.str());


    //������ص�material��Ϣ
    for (int materialIndex = 0; materialIndex < material_count; materialIndex++)
    {
        FbxSurfaceMaterial* pSurfaceMaterial = pNode->GetMaterial(materialIndex);
        egg::Material* mat = LoadMaterialAttribute(pSurfaceMaterial, copy_image_list);
        //LoadMaterialTexture  ( pSurfaceMaterial );
        if (mat)
        {
            meshrender->addMaterial(mat);
        }
    }


    //����ͨ�����ڹ�����buffer����
    geometry->mBuffers[egg_va_position] = new egg::ArrayT<float>(triangleCount * 3 * 3);
    if (has_normal) { geometry->mBuffers[egg_va_normal] = new egg::ArrayT<float>(triangleCount * 3 * 3); }
    if (has_color[0]) { geometry->mBuffers[egg_va_color0] = new egg::ArrayT<float>(triangleCount * 3 * 4); }
    if (has_color[1]) { geometry->mBuffers[egg_va_color1] = new egg::ArrayT<float>(triangleCount * 3 * 4); }
    if (has_color[2]) { geometry->mBuffers[egg_va_color2] = new egg::ArrayT<float>(triangleCount * 3 * 4); }
    if (has_color[3]) { geometry->mBuffers[egg_va_color3] = new egg::ArrayT<float>(triangleCount * 3 * 4); }
    if (has_uv[0]) { geometry->mBuffers[egg_va_uv0] = new egg::ArrayT<float>(triangleCount * 3 * 2); }
    if (has_uv[1]) { geometry->mBuffers[egg_va_uv1] = new egg::ArrayT<float>(triangleCount * 3 * 2); }
    if (has_uv[2]) { geometry->mBuffers[egg_va_uv2] = new egg::ArrayT<float>(triangleCount * 3 * 2); }
    if (has_uv[3]) { geometry->mBuffers[egg_va_uv3] = new egg::ArrayT<float>(triangleCount * 3 * 2); }
    if (has_tangent[0]) { geometry->mBuffers[egg_va_tangent0] = new egg::ArrayT<float>(triangleCount * 3 * 3); }
    if (has_tangent[1]) { geometry->mBuffers[egg_va_tangent1] = new egg::ArrayT<float>(triangleCount * 3 * 3); }
    if (has_tangent[2]) { geometry->mBuffers[egg_va_tangent2] = new egg::ArrayT<float>(triangleCount * 3 * 3); }
    if (has_tangent[3]) { geometry->mBuffers[egg_va_tangent3] = new egg::ArrayT<float>(triangleCount * 3 * 3); }
    if (has_boneindex) { geometry->mBuffers[egg_va_bone_indices] = new egg::ArrayT<int>(triangleCount * 3 * 4); }
    if (has_weight) { geometry->mBuffers[egg_va_blend_weights] = new egg::ArrayT<float>(triangleCount * 3 * 4); }

    //ÿ�δ洢�������������
    //����          ��ͨ��  ������  float����
    float vertex[3][3] = { 0 };
    float color[4][3][4] = { 0 };
    float uv[4][3][2] = { 0 };
    float normal[3][3] = { 0 };
    float tangent[4][3][3] = { 0 };
    int   boneindex[3][4] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
    float weight[3][4] = { 0 };

    //����������κβ�����Ϣ �ͼ�����һ��Ϊ0�Ĳ���id 
    //��Ȼ�����ֲ����ʵ�ȱʧ ���Ǳ�֤mesh���������
    //��Ϊmax�����崴��ʱ��û�в��ʵ�  maya����ʱһ����Ĭ�ϲ��� ���ǲ��ų���������
    if (material_count == 0)
    {
        material_count = 1;
    }

    //��¼�������������� ÿ����̬��¼�Լ��ĳ��ֵĴ���
    std::vector<int> material_histogram(material_count);
    for (int i = 0; i < material_count; i++)
    {
        material_histogram[i] = 0;
    }

    //���ݲ���id��˳�����
    unsigned int vertexCounter = 0;
    for (int mtl_index = 0; mtl_index < material_count; ++mtl_index)
    {
        //ѭ������������
        for (int tri_index = 0; tri_index < triangleCount; ++tri_index)
        {
            //�������id�����ڵ�ǰid ��ִ����һ��
            if (mtl_index != material_index[tri_index]) continue;

            //��¼ֱ��ͼ
            material_histogram[mtl_index] ++;


            //ѭ����ǰ���ǵ�������
            for (int point_index = 0; point_index < 3; point_index++)
            {
                //���㵱ǰ��
                int vertexCounter = tri_index * 3 + point_index;

                //��ǰ�������������������
                int ctrlPointIndex = mesh->GetPolygonVertex(tri_index, point_index);

                // Read the vertex
                ReadVertex(mesh, ctrlPointIndex, &vertex[point_index][0]);

                if (mesh->GetElementNormalCount() > 0)
                {
                    // Read the normal of each vertex
                    ReadNormal(mesh, ctrlPointIndex, vertexCounter, &normal[point_index][0]);
                }

                // Read the color of each vertex
                for (int channal_index = 0; channal_index < mesh->GetElementVertexColorCount() && channal_index < 4; ++channal_index)
                {
                    ReadColor(mesh, channal_index, ctrlPointIndex, vertexCounter, &color[channal_index][point_index][0]);
                }

                // Read the UV of each vertex
                for (int channal_index = 0; channal_index < mesh->GetElementUVCount() && channal_index < 4; ++channal_index)
                {
                    ReadUV(mesh, channal_index, ctrlPointIndex, mesh->GetTextureUVIndex(tri_index, point_index), &(uv[channal_index][point_index][0]));
                }

                // Read the tangent of each vertex
                for (int channal_index = 0; channal_index < mesh->GetElementTangentCount() && channal_index < 4; ++channal_index)
                {
                    ReadTangent(mesh, channal_index, ctrlPointIndex, vertexCounter, &tangent[channal_index][point_index][0]);
                }

                if (has_boneindex
                    && has_weight
                    && (ctrlPointIndex < (int)controlpointweights.size())
                    && (ctrlPointIndex >= 0)
                    )
                {
                    ControlPointWeight cpw = controlpointweights[ctrlPointIndex];
                    unsigned int index_size = cpw.index.size();
                    unsigned int weight_size = cpw.weight.size();

                    boneindex[point_index][0] = index_size > 0 ? cpw.index[0] : -1;
                    boneindex[point_index][1] = index_size > 1 ? cpw.index[1] : -1;
                    boneindex[point_index][2] = index_size > 2 ? cpw.index[2] : -1;
                    boneindex[point_index][3] = index_size > 3 ? cpw.index[3] : -1;

                    weight[point_index][0] = weight_size > 0 ? cpw.weight[0] : 0.0f;
                    weight[point_index][1] = weight_size > 1 ? cpw.weight[1] : 0.0f;
                    weight[point_index][2] = weight_size > 2 ? cpw.weight[2] : 0.0f;
                    weight[point_index][3] = weight_size > 3 ? cpw.weight[3] : 0.0f;
                }

                //vertexCounter++;
            }//for(int point_index = 0 ; point_index < 3 ; point_index++)

            //�ɹ���ȡһ����������Ϣ ׷�Ӹ�geometry��buffer
            //��Щ������ͨ�������˳���ǲ���id���ӵ�˳�� ���Ա�֤�������
            //printf("t%d",mtl_index);

            //��������ݵ����� ��ת��vertex normal tangent
            if (world_position
                || controlpointweights.size() > 0  //���û��Ȩ�ؾͲ����� ��Ϊ��Щ����ֻ�ǲ㼶��ϵ
                )
            {
                for (int w = 0; w < 3; w++) //��������
                {
                    //FbxVector4 v = FbxVector4(vertex[w][0],vertex[w][1],vertex[w][2]);
                    //v = current_mesh_matrix.MultT(v);
                    //vertex[w][0] = v[0];
                    //vertex[w][1] = v[1];
                    //vertex[w][2] = v[2];

                    //�����������
                    float* m = current_mesh_matrix;
                    vmml::mat4f vm;
                    vm.set(&m[0], &m[15], false);

                    //���㶥��
                    vmml::vec3f v(vertex[w][0], vertex[w][1], vertex[w][2]);
                    v = vm * v;
                    vertex[w][0] = (float)v[0];
                    vertex[w][1] = (float)v[1];
                    vertex[w][2] = (float)v[2];


                    //�������������Ҫλ����Ϣ
                    vm.set_translation(0, 0, 0);

                    if (has_normal)
                    {
                        v = vmml::vec3f(normal[w][0], normal[w][1], normal[w][2]);
                        v = vm * v;
                        v.normalize();
                        normal[w][0] = (float)v[0];
                        normal[w][1] = (float)v[1];
                        normal[w][2] = (float)v[2];
                    }

                    if (has_tangent[0])
                    {
                        v = vmml::vec3f(tangent[0][w][0], tangent[0][w][1], tangent[0][w][2]);
                        v = vm * v;
                        v.normalize();
                        tangent[0][w][0] = (float)v[0];
                        tangent[0][w][1] = (float)v[1];
                        tangent[0][w][2] = (float)v[2];
                    }
                }
            }

            //�������ݵ�buffer
            memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_position].get())[vertexCounter * 3 * 3], &vertex[0][0], sizeof(float) * 3 * 3);
            if (has_normal) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_normal].get())[vertexCounter * 3 * 3], &normal[0][0], sizeof(float) * 3 * 3); }
            if (has_color[0]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_color0].get())[vertexCounter * 3 * 4], &color[0][0][0], sizeof(float) * 3 * 4); }
            if (has_color[1]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_color1].get())[vertexCounter * 3 * 4], &color[1][0][0], sizeof(float) * 3 * 4); }
            if (has_color[2]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_color2].get())[vertexCounter * 3 * 4], &color[2][0][0], sizeof(float) * 3 * 4); }
            if (has_color[3]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_color3].get())[vertexCounter * 3 * 4], &color[3][0][0], sizeof(float) * 3 * 4); }
            if (has_uv[0]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_uv0].get())[vertexCounter * 3 * 2], &uv[0][0][0], sizeof(float) * 3 * 2); }
            if (has_uv[1]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_uv1].get())[vertexCounter * 3 * 2], &uv[1][0][0], sizeof(float) * 3 * 2); }
            if (has_uv[2]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_uv2].get())[vertexCounter * 3 * 2], &uv[2][0][0], sizeof(float) * 3 * 2); }
            if (has_uv[3]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_uv3].get())[vertexCounter * 3 * 2], &uv[3][0][0], sizeof(float) * 3 * 2); }
            if (has_tangent[0]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_tangent0].get())[vertexCounter * 3 * 3], &tangent[0][0][0], sizeof(float) * 3 * 3); }
            if (has_tangent[1]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_tangent1].get())[vertexCounter * 3 * 3], &tangent[1][0][0], sizeof(float) * 3 * 3); }
            if (has_tangent[2]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_tangent2].get())[vertexCounter * 3 * 3], &tangent[2][0][0], sizeof(float) * 3 * 3); }
            if (has_tangent[3]) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_tangent3].get())[vertexCounter * 3 * 3], &tangent[3][0][0], sizeof(float) * 3 * 3); }
            if (has_boneindex) { memcpy(&(*(egg::ArrayT<int>*)geometry->mBuffers[egg_va_bone_indices].get())[vertexCounter * 3 * 4], &boneindex[0][0], sizeof(int) * 3 * 4); }
            if (has_weight) { memcpy(&(*(egg::ArrayT<float>*)geometry->mBuffers[egg_va_blend_weights].get())[vertexCounter * 3 * 4], &weight[0][0], sizeof(float) * 3 * 4); }

            //���ö�������˵��
            geometry->mVectexAttribute[egg_va_position] = new egg::VectexAttribute(egg_va_position, 3, egg::DT_FLOAT, false, 0, egg_va_position);
            if (has_normal) { geometry->mVectexAttribute[egg_va_normal] = new egg::VectexAttribute(egg_va_normal, 3, egg::DT_FLOAT, false, 0, egg_va_normal); }
            if (has_color[0]) { geometry->mVectexAttribute[egg_va_color0] = new egg::VectexAttribute(egg_va_color0, 4, egg::DT_FLOAT, false, 0, egg_va_color0); }
            if (has_color[1]) { geometry->mVectexAttribute[egg_va_color1] = new egg::VectexAttribute(egg_va_color1, 4, egg::DT_FLOAT, false, 0, egg_va_color1); }
            if (has_color[2]) { geometry->mVectexAttribute[egg_va_color2] = new egg::VectexAttribute(egg_va_color2, 4, egg::DT_FLOAT, false, 0, egg_va_color2); }
            if (has_color[3]) { geometry->mVectexAttribute[egg_va_color3] = new egg::VectexAttribute(egg_va_color3, 4, egg::DT_FLOAT, false, 0, egg_va_color3); }
            if (has_uv[0]) { geometry->mVectexAttribute[egg_va_uv0] = new egg::VectexAttribute(egg_va_uv0, 2, egg::DT_FLOAT, false, 0, egg_va_uv0); }
            if (has_uv[1]) { geometry->mVectexAttribute[egg_va_uv1] = new egg::VectexAttribute(egg_va_uv1, 2, egg::DT_FLOAT, false, 0, egg_va_uv1); }
            if (has_uv[2]) { geometry->mVectexAttribute[egg_va_uv2] = new egg::VectexAttribute(egg_va_uv2, 2, egg::DT_FLOAT, false, 0, egg_va_uv2); }
            if (has_uv[3]) { geometry->mVectexAttribute[egg_va_uv3] = new egg::VectexAttribute(egg_va_uv3, 2, egg::DT_FLOAT, false, 0, egg_va_uv3); }
            if (has_tangent[0]) { geometry->mVectexAttribute[egg_va_tangent0] = new egg::VectexAttribute(egg_va_tangent0, 3, egg::DT_FLOAT, false, 0, egg_va_tangent0); }
            if (has_tangent[1]) { geometry->mVectexAttribute[egg_va_tangent1] = new egg::VectexAttribute(egg_va_tangent1, 3, egg::DT_FLOAT, false, 0, egg_va_tangent1); }
            if (has_tangent[2]) { geometry->mVectexAttribute[egg_va_tangent2] = new egg::VectexAttribute(egg_va_tangent2, 3, egg::DT_FLOAT, false, 0, egg_va_tangent2); }
            if (has_tangent[3]) { geometry->mVectexAttribute[egg_va_tangent3] = new egg::VectexAttribute(egg_va_tangent3, 3, egg::DT_FLOAT, false, 0, egg_va_tangent3); }
            if (has_boneindex) { geometry->mVectexAttribute[egg_va_bone_indices] = new egg::VectexAttribute(egg_va_bone_indices, 4, egg::DT_INT, false, 0, egg_va_bone_indices); }
            if (has_weight) { geometry->mVectexAttribute[egg_va_blend_weights] = new egg::VectexAttribute(egg_va_blend_weights, 4, egg::DT_FLOAT, false, 0, egg_va_blend_weights); }



            //����¼�������� �����Ե�������ʹ��
            vertexCounter++;
        }//for(int tri_index = 0 ; tri_index < triangleCount ; ++tri_index)
    }//for(int mtl_index = 0 ; mtl_index < material_count ; ++mtl_index)


    //���ݲ��������Ľ������subgeometry
    int current_vertex_index = 0;
    for (unsigned int i = 0; i < material_histogram.size(); i++)
    {
        geometry->mSubGeometry.push_back(new egg::SubGeometry(
            egg::PrimitiveSet::Mode::TRIANGLES
            , current_vertex_index * 3
            , material_histogram[i] * 3
        ));
        current_vertex_index += material_histogram[i];
    }

    //ѹ������
    if (false)
    {
        //������Ŀ
        unsigned int vertex_count = 0;//geometry->get;

        //����������������

        //����ȫ�����ܴ��ڵ�VA���� ���ڴ洢�����������

        for (unsigned int vi = 0; vi < vertex_count; vi++)
        {
            //ѭ���Ա��Ѿ����ɵĶ������� ���һ��������ȫ��� ����Ϊ�ظ���

        }
    }

    //����geometry
    meshrender->setGeometry(geometry.get());

    return meshrender;
}


