#ifndef _DEFERREDRENDERER_H_
#define _DEFERREDRENDERER_H_

#include <boost/shared_ptr.hpp>
#include <Eigen/Geometry>
#include <Eigen/LU>

namespace Ezr
{
    class Fbo;
    class Camera;
    class Scene;
    class Texture;
    class DeferredDrawShader;

    class DeferredRenderer
    {
    public:
        DeferredRenderer(Scene* scene, 
                            Camera* camera, 
                            int fboWidth, 
                            int fboHeight,
                            float fov,
                            float nearPlane);
        ~DeferredRenderer();

        void render();

        inline void enableDirectionalLight(bool flag){_renderDirectionalLights = flag;}
        inline void enablePointlight(bool flag){_renderPointLights = flag;}
        inline void toggleDirectionalLight(){_renderDirectionalLights = !_renderDirectionalLights;}
        inline void togglePointLight(){_renderPointLights = !_renderPointLights;}
        inline void drawLightGeometry(bool flag){_drawLightGeometry = flag;}
        inline void toggleLightGeometry(){_drawLightGeometry = !_drawLightGeometry;}

    private:
        boost::shared_ptr<Fbo> _gBuffer;
        boost::shared_ptr<Fbo> _lightBuffer;
        Camera* _cam;
        Scene* _scene;
        int _fboWidth;
        int _fboHeight;
        boost::shared_ptr<DeferredDrawShader> _deferredShader;
        float _fov;
        float _nearPlane;
        boost::shared_ptr<Texture>  _colormap;
        bool _drawLightGeometry;
        bool _renderDirectionalLights;
        bool _renderPointLights;

        void drawPass(float nearPlane, float fov);
        void renderDirectionalLights();
        void renderPointLights();
    };
}

#endif
