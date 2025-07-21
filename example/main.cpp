#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "Core/Engine.h"
#include "Core/geometry/GeometryUtils.h"
#include "Core/material/StandardPhysicalMaterial.h"
#include "Core/material/BasicLitMaterial.h"
#include "Core/scene/Object3D.h"
#include "Core/scene/Scene.h"
#include "Core/render/Camera.h"
#include "Core/light/AmbientLight.h"

int main(int argc, char** argv) {
    const int width = 1280;
    const int height = 720;

    // SDL Init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Core + SDL2 + Shadows",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewInit();

    // Core Init
    auto engine = Core::Engine::instance();
    engine->setDefaultRenderTargetToCurrent();
    engine->setRenderSize(width, height, true);

    auto scene = engine->createScene();
    engine->setActiveScene(scene);
    auto root = scene->getRoot();

    // Camera
    auto camObj = engine->createObject3D();
    camObj->getTransform().setLocalPosition(0.0f, 3.0f, 8.0f);
    auto camera = engine->createPerspectiveCamera(camObj, 70.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
    camObj->getTransform().lookAt(Core::Point3r(0.0f, 0.5f, 0.0f), Core::Vector3r(0.0f, 1.0f, 0.0f));
    root->addChild(camObj);

    camera->setHDREnabled(true);
    camera->setHDRToneMapTypeExposure(2.0f);
    camera->setHDRGamma(2.0f);

    // Ambient light
    auto ambientLightObject = engine->createObject3D();
    if (!ambientLightObject.isValid()) {
        printf("Failed to create ambient light object, exiting.\n");
        return -1;
    }
    ambientLightObject->setName("Ambient light");
    auto ambientLight = engine->createLight<Core::AmbientLight>(ambientLightObject);
    if (!ambientLight.isValid()) {
        printf("Failed to create ambient light, exiting.\n");
        return -1;
    }
    ambientLight->setIntensity(4.0f);
    ambientLight->setColor(0.75f, 0.75f, 0.75f, 1.0f);
    root->addChild(ambientLightObject);  // Add object to scene graph

    // Directional light
    auto directionalLightObject = engine->createObject3D();
    if (!directionalLightObject.isValid()) {
        printf("Failed to create directional light object, exiting.\n");
        return -1;
    }
    directionalLightObject->setName("Directional light");
    auto directionalLight = engine->createDirectionalLight<Core::DirectionalLight>(
        directionalLightObject,
        3,      // cascade count
        true,   // enable shadows
        4096,   // shadow map size
        0.0001, // constant shadow bias
        0.0005  // angular shadow bias
    );
    if (!directionalLight.isValid()) {
        printf("Failed to create directional light, exiting.\n");
        return -1;
    }
    directionalLight->setIntensity(4.0f);
    directionalLight->setColor(1.0f, 0.878f, 0.878f, 1.0f);
    directionalLight->setShadowSoftness(Core::ShadowLight::Softness::VerySoft);
    directionalLight->setFaceCullingEnabled(false);
    root->addChild(directionalLightObject);

    directionalLight->setShadowSoftness(Core::ShadowLight::Softness::VerySoft);
    directionalLight->setFaceCullingEnabled(false);

    // Point light
    auto pointLightObject = engine->createObject3D();
    if (!pointLightObject.isValid()) {
        printf("Failed to create point light object, exiting.\n");
        return -1;
    }
    auto pointLightMeshContainer = engine->createRenderableContainer<Core::MeshContainer, Core::Mesh>(pointLightObject);
    if (!pointLightMeshContainer.isValid()) {
        printf("Failed to create point light object container, exiting.\n");
        return -1;
    }
    pointLightObject->setName("Point light");
    root->addChild(pointLightObject);
    auto pointLight = engine->createPointLight<Core::PointLight>(pointLightObject, true, 2048, 0.0115, 0.35);
    if (!pointLight.isValid()) {
        printf("Failed to create point light, exiting.\n");
        return -1;
    }
    pointLight->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    pointLight->setShadowSoftness(Core::ShadowLight::Softness::VerySoft);
    pointLight->setRadius(30.0f);
    pointLightObject->getTransform().translate(Core::Vector3r(5.0f, 10.0f, 5.0f));

    //~ // Enable shadows on directional light
    //~ directionalLight->setShadowsEnabled(true);

    //~ // Enable shadows on point light
    //~ pointLight->setShadowsEnabled(true);

    // Materials
    auto redMat = engine->createMaterial<Core::StandardPhysicalMaterial>();
    redMat->setAlbedo(Core::Color(0.8f, 0.3f, 0.4f, 1.0f));
    redMat->setAmbientOcclusion(1.0f);

    auto grayMat = engine->createMaterial<Core::StandardPhysicalMaterial>();
    grayMat->setAlbedo(Core::Color(0.4f, 0.5f, 0.5f, 1.0f));
    grayMat->setAmbientOcclusion(1.0f);

    // Rotating Box
    auto boxMesh = Core::GeometryUtils::buildBoxMesh(1.0f, 1.0f, 1.0f, Core::Color(1, 1, 1, 1));
    auto boxObj = Core::GeometryUtils::buildMeshContainerObject(boxMesh, redMat, "RotatingBox");
    boxObj->getTransform().setLocalPosition(0.0f, 0.5f, 0.0f);
    root->addChild(boxObj);

    // Floor
    auto floorMesh = Core::GeometryUtils::buildBoxMesh(10.0f, 0.1f, 10.0f, Core::Color(1, 1, 1, 1));
    auto floorObj = Core::GeometryUtils::buildMeshContainerObject(floorMesh, grayMat, "Floor");
    floorObj->getTransform().setLocalPosition(0.0f, -1.0f, 0.0f);
    floorObj->getTransform().rotate(Core::Vector3r(1.0f, 0.3f, 0.0f), 0.5f);
    root->addChild(floorObj);

    // Animation loop
    float angle = 0.0f;
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        angle += 0.01f;
        boxObj->getTransform().rotate(Core::Vector3r(0.0f, 1.0f, 0.0f), 0.01f);
        floorObj->getTransform().rotate(Core::Vector3r(0.0f, 1.0f, 0.0f), -0.001f);

        engine->update();
        engine->render();
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
