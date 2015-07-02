// 湍流游戏

#include "ShadowPlane.h"
#include <vector>

USING_NS_CC;

bool ShadowPlane::init()
{
    assert(Node::init());

    initShader();
    initRenderData();
    initRenderTexture();

    return true;
}

void ShadowPlane::setShadowTarget(cocos2d::Node *target)
{
    this->_renderTextureTarget = target;
}

void ShadowPlane::initShader()
{
    _blendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
    auto fileUtils = FileUtils::getInstance();
    auto vertSource = fileUtils->getStringFromFile("3d/shadow.vsh");
    auto fragSource = fileUtils->getStringFromFile("3d/shadow.fsh");

    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    glprogram->bindAttribLocation("a_position", GLProgram::VERTEX_ATTRIB_POSITION);
    glprogram->bindAttribLocation("a_color", GLProgram::VERTEX_ATTRIB_COLOR);
    glprogram->bindAttribLocation("a_normal", GLProgram::VERTEX_ATTRIB_NORMAL);
    glprogram->link();
    glprogram->updateUniforms();

    _programState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    assert(_programState);
    _programState->retain();
    this->setGLProgramState(_programState);
}

void  ShadowPlane::initRenderData()
{
    // init the flat plane mesh
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> colors;
    std::vector<float> texs;

    float vertices[4*5] = { // pos, tex
        0.5, 0.0, 0.5, 1.0, 0.0,
        0.5, 0.0, -0.5, 1.0, 1.0,
        -0.5, 0.0, -0.5, 0.0, 1.0,
        -0.5, 0.0, 0.5, 0.0, 0.0
    };
    for (int i = 0; i < 4; i++) {
        positions.push_back(vertices[5*i]);
        positions.push_back(vertices[5*i+1]);
        positions.push_back(vertices[5*i+2]);

        texs.push_back(vertices[5*i+3]);
        texs.push_back(vertices[5*i+4]);
    }
    std::vector<unsigned short> triangleIndex = {
        0,1,2,
        2,3,0
    };

    _renderMesh = Mesh::create(positions, normals, texs, triangleIndex);
    assert(_renderMesh != nullptr);
    _renderMesh->retain();

    long offset = 0;
    auto attributeCount = _renderMesh->getMeshVertexAttribCount();
    CCLOG("attributeCount = %ld", attributeCount);
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = _renderMesh->getMeshVertexAttribute(k);
        _programState->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                              meshattribute.size,
                                              meshattribute.type,
                                              GL_FALSE,
                                              _renderMesh->getVertexSizeInBytes(),
                                              (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }

}


void ShadowPlane::initRenderTexture()
{
    auto  size  = Director::getInstance()->getWinSize();

    _renderTexture = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888,GL_DEPTH24_STENCIL8);

    _renderTexture->setClearColor(Color4F(0, 0, 0, 0));
    _renderTexture->setVisible(false);
    addChild(_renderTexture);
}


void ShadowPlane::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    if( _renderTextureTarget && _renderTexture)
    {
        CCLOG("%f %f", _renderTexture->getGlobalZOrder(), _renderTextureTarget->getGlobalZOrder());
        _renderTexture->beginWithClear(0, 0, 0.5, 0.5, 0, 0);
        _renderTextureTarget->visit();
        _renderTexture->end();
    }

    CC_SAFE_DELETE(_meshCommand);
    _meshCommand = new MeshCommand();
    //    _meshCommand->genMaterialID(0, getGLProgramState(), _renderMesh->getVertexBuffer(), _renderMesh->getIndexBuffer(), _blendFunc);
    _meshCommand->init(_globalZOrder, 0, getGLProgramState(), _blendFunc, _renderMesh->getVertexBuffer(), _renderMesh->getIndexBuffer(), (GLenum)_renderMesh->getPrimitiveType(), (GLenum)_renderMesh->getIndexFormat(), _renderMesh->getIndexCount(), transform, flags);

    _meshCommand->setCullFaceEnabled(true);
    _meshCommand->setDepthTestEnabled(true);
    _meshCommand->setTransparent(true);
    _meshCommand->setDepthWriteEnabled(true);

    getGLProgramState()->setUniformTexture("u_texture", _renderTexture->getSprite()->getTexture());
//    _meshCommand->setDisplayColor(Vec4{0,0,0,0});
    renderer->addCommand(_meshCommand);

    Node::draw(renderer, transform, flags);
}