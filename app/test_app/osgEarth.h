#ifndef _EGG_osgEarth_H_
#define _EGG_osgEarth_H_

//#define EGG_USE_OSGEARTH 1

#include <eggEngine/Component.h>

#if EGG_USE_OSGEARTH

#include <osgEarth/MapNode>


#include <osgEarthUtil/ExampleResources>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/LogarithmicDepthBuffer>

#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/sky_simple/SimpleSkyOptions>
#include <osgEarthDrivers/sky_gl/GLSkyOptions>
//#include <osgEarthDrivers/sky_silverlining/SilverLiningOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
//#include <osgEarthDrivers/sky_gl/>

//#include <osgEarthSymbology/AGG.h>

#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/AnnotationRegistry>

#include <osgEarth/Capabilities>
#include <osgEarth/Registry>

#include "osgViewer.h"


class oeMapNode
    :public egg::ComponentBase
{
public:
    oeMapNode(unsigned int context_id = 0) 
    :mContextID(context_id)
    {}
    virtual ~oeMapNode() {}
    virtual std::string ObjectClassName() override { return "oeMapNode"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override
    {
        //这里必须在第一帧之后才能初始化
        // 以为mapnode的init过程需要正确的context
        if (inited == 0)
        {
            //初始化GL环境设置
            osgEarth::Capabilities* cap = new osgEarth::Capabilities(mContextID);
            osgEarth::Registry::instance()->setCapabilities(cap);

            //设置
            osgEarth::Drivers::FileSystemCacheOptions cacheOptions;
            cacheOptions.rootPath() = "d:/aaaaa";
            osgEarth::Drivers::MapOptions mapOptions;
            mapOptions.cache() = cacheOptions;

            //建立map
            osgEarth::Map* map = new osgEarth::Map(mapOptions);
            //map->getProfile();

            //建立mapnode
            mMapNode = new osgEarth::MapNode(map);
            //mRoot->addChild(mapNode);

            egg::SceneNode* parent = dynamic_cast<egg::SceneNode*>(this->getNode()->getParent(0));
            if (parent)
            {
                Component_osgViewer* com = parent->getComponent<Component_osgViewer>();
                if (com)
                {
                    com->mRoot->addChild(mMapNode);
                    com->mViewer->setCameraManipulator(new osgEarth::Util::EarthManipulator());
                    com->mViewer->home();

                    //强制设置正确的IO 否则OE无法正确识别buffer
                    com->mViewer->getCamera()->setDrawBuffer(GL_BACK);
                    com->mViewer->getCamera()->setReadBuffer(GL_BACK);
                }
            }

            inited++;
        }
        else
        {
            inited++;
        }
    }

    osg::ref_ptr<osgEarth::MapNode> mMapNode;

    int inited = 0;
    unsigned int mContextID = 0;
};


void testOSGEarth(egg::SceneNode* parent, unsigned int context_id)
{
    //osgViewer
    egg::SceneNode* sn_viewer = new egg::SceneNode("sn_viewer");
    Component_osgViewer* viewer = new Component_osgViewer();
    sn_viewer->addComponent(viewer);

    egg::SceneNode* sn_mapnode = new egg::SceneNode("sn_mapnode");
    oeMapNode* mapnode = new oeMapNode(context_id);
    sn_mapnode->addComponent(mapnode);
    sn_viewer->addChild(sn_mapnode);

    //
    parent->addChild(sn_viewer);
}


#endif //EGG_USE_OSGEARTH
#endif //_EGG_osgEarth_H_

