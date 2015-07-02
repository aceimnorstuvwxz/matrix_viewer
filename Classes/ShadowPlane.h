// (C) 2015 湍流游戏

#ifndef __matrix_viewer__ShadowPlane__
#define __matrix_viewer__ShadowPlane__

#include "cocos2d.h"

class ShadowPlane: public cocos2d::Node
{
public:
    virtual bool init() override;
    CREATE_FUNC(ShadowPlane);

    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

    void setShadowTarget(cocos2d::Node* target);

private:
    // renderer data
    cocos2d::GLProgramState* _programState = nullptr;
    cocos2d::Mesh* _renderMesh = nullptr;
    cocos2d::Texture2D* _texture = nullptr;
    cocos2d::MeshCommand* _meshCommand = nullptr;
    cocos2d::BlendFunc _blendFunc;
    void initShader();
    void initRenderData();

    // target
    cocos2d::Node* _renderTextureTarget = nullptr;
    cocos2d::RenderTexture* _renderTexture = nullptr;
    void initRenderTexture();
};

#endif /* defined(__matrix_viewer__ShadowPlane__) */
