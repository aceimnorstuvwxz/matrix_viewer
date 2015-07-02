//
//  PreviewScene.h
//  matrix_viewer
//
//  Created by chenbingfeng on 15/7/2.
//
//

#ifndef __matrix_viewer__PreviewScene__
#define __matrix_viewer__PreviewScene__

#include "cocos2d.h"


class PreviewScene: public cocos2d::Scene
{
public:
    virtual bool init() override;
    CREATE_FUNC(PreviewScene);


private:

    cocos2d::Layer* _3dLayer;
    cocos2d::Camera* _3dCamera;
    cocos2d::Sprite3D* _mainSprite;
    cocos2d::Sprite3D* _plane;

    int _cg = 0;
};

#endif /* defined(__matrix_viewer__PreviewScene__) */
