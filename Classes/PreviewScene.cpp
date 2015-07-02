//
//  PreviewScene.cpp
//  matrix_viewer
//
//  Created by chenbingfeng on 15/7/2.
//
//

#include "PreviewScene.h"

USING_NS_CC;

bool PreviewScene::init()
{
    assert(Scene::init());

    auto size = Director::getInstance()->getVisibleSize();

    _3dLayer = Layer::create();
    this->addChild(_3dLayer);

    _3dCamera = Camera::createPerspective(60, size.width/size.height, 1, 1000);
    _3dCamera->setPosition3D({10, 10, 10});
    _3dCamera->lookAt({0,0,0});
    _3dCamera->setCameraFlag(CameraFlag::USER1);
    _3dCamera->setCameraMask((unsigned short)CameraFlag::USER1);
    _3dLayer->addChild(_3dCamera);

    // Main Mesh
    _mainSprite = Sprite3D::create("3d/simple.c3b");
    _mainSprite->setPosition3D({0,0,0});
    _mainSprite->setCameraMask(_3dCamera->getCameraMask());
//    _mainSprite->setScale(0.3);
    _3dLayer->addChild(_mainSprite);

    // Bottom Plane
    _plane = Sprite3D::create("3d/pure_plane.c3b");
    _plane->setPosition3D({0, -5, 0});
    _plane->setScale(20);
    _plane->setCameraMask(_3dCamera->getCameraMask());
    _3dLayer->addChild(_plane);

    // Animation
    _mainSprite->runAction(RepeatForever::create(RotateBy::create(20.f, {0,360,0})));
    auto animation = Animation3D::create("3d/simple.c3b", "Take 001");
    auto animate = Animate3D::create(animation);
    _mainSprite->runAction(RepeatForever::create(animate));

    // Light
    auto light = DirectionLight::create(Vec3(-1.0f, -1.0f, 0.0f), Color3B::WHITE);
    addChild (light);

    auto light2 = AmbientLight::create (Color3B::WHITE);
    addChild (light2);

    _mainSprite->setTurnroCgEnable(true);
    auto cgfunc = [this](float dt){
        _mainSprite->setTurnroCgIndex(_cg++);
        if (_cg > 127) {
            _cg = 0;
        }
    };
    schedule(cgfunc, 0.033f, kRepeatForever, 0.f, "dsfsdf");

    return true;
}