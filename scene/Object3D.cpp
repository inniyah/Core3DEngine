#include "Object3D.h"
#include "../Engine.h"
#include "Transform.h"
#include "Object3DComponent.h"
#include "../light/Light.h"
#include "../light/AmbientIBLLight.h"
#include "../render/Camera.h"
#include "../render/ReflectionProbe.h"
#include "../render/BaseObject3DRenderer.h"
#include "../render/BaseRenderable.h"
#include "../render/MeshContainer.h"
#include "../render/MeshRenderer.h"
#include "../particles/ParticleSystem.h"
#include "../particles/renderer/ParticleSystemRenderer.h"

namespace Core {

    UInt64 Object3D::_nextID = 0;

    Object3D::Object3D() : transform(*this), active(true) {
        this->id = Object3D::getNextID();
        this->layer = (Int32) Object3D::ObjectLayer::Default;
    }

    Object3D::~Object3D() {
        for (UInt32 i = 0; i < this->children.size(); i ++) {
            WeakPointer<Object3D> child = this->children[i];
             Engine::safeReleaseObject(child);
        }
        for (UInt32 i = 0; i < this->components.size(); i ++) {
            WeakPointer<Object3DComponent> component = this->components[i];
            WeakPointer<Camera> cameraComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<Camera>(component);
            WeakPointer<Light> lightComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<Light>(component);
            WeakPointer<BaseRenderable> renderableComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<BaseRenderable>(component);
            WeakPointer<BaseRenderableContainer> renderableContainerComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<BaseRenderableContainer>(component);
            WeakPointer<BaseObject3DRenderer> rendererComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<BaseObject3DRenderer>(component);
            WeakPointer<ReflectionProbe> reflectionProbeComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<ReflectionProbe>(component);
            WeakPointer<ParticleSystem> particleSystemComponent = WeakPointer<Object3DComponent>::dynamicPointerCast<ParticleSystem>(component);
            if (lightComponent || cameraComponent || renderableComponent || rendererComponent || renderableContainerComponent ||
                reflectionProbeComponent || particleSystemComponent) {
                 Engine::safeReleaseObject(component);
            }
        }
    }

    UInt64 Object3D::getID() const {
        return this->id;
    }

    UInt64 Object3D::getNextID() {
        return _nextID++;
    }

    Transform& Object3D::getTransform() {
        return this->transform;
    }

    SceneObjectIterator<Object3D> Object3D::beginIterateChildren() {
        return  SceneObjectIterator<Object3D>(this->children.begin());
    }

    SceneObjectIterator<Object3D> Object3D::endIterateChildren() {
        return SceneObjectIterator<Object3D>(this->children.end());
    }

    SceneObjectIterator<Object3DComponent> Object3D::beginIterateComponents() {
        return SceneObjectIterator<Object3DComponent>(this->components.begin());
    }

    SceneObjectIterator<Object3DComponent> Object3D::endIterateComponents() {
        return SceneObjectIterator<Object3DComponent>(this->components.end());
    }

    UInt32 Object3D::childCount() const {
        return this->children.size();
    }

    void Object3D::addChild(WeakPointer<Object3D> object) {

        if (object->parent.isValid() && object->parent.get() == this) return;
        
        if (object->parent.isValid()) {
            object->parent->removeChild(object);
        }

        Transform& worldTransform = this->getTransform();
        worldTransform.updateWorldMatrix();
        Matrix4x4 worldInverse = worldTransform.getWorldMatrix();
        worldInverse.invert();

        object->getTransform().getLocalMatrix().preMultiply(worldInverse);

        this->children.push_back(object);
        object->parent = this->_self;
    }

    void Object3D::removeChild(WeakPointer<Object3D> object) {
        auto end = this->endIterateChildren();
        auto result = end;
        for (SceneObjectIterator<Object3D> itr = this->beginIterateChildren(); itr != end; ++itr) {
            if (*itr == object) {
                result = itr;
                break;
            }
        }
        if (result != end) {
            Transform& transform = object->getTransform();
            transform.updateWorldMatrix();
            transform.getLocalMatrix().copy(transform.getWorldMatrix());
            this->children.erase(result.getSrc());
            object->parent = PersistentWeakPointer<Object3D>::nullPtr();
        }
    }

    WeakPointer<Object3D> Object3D::getParent() const {
        return this->parent;
    }

    Bool Object3D::addComponent(WeakPointer<Object3DComponent> component) {
        SceneObjectIterator<Object3DComponent> end = this->endIterateComponents();
        SceneObjectIterator<Object3DComponent> result = end;
        UNUSED(result);
        for(SceneObjectIterator<Object3DComponent> itr = this->beginIterateComponents(); itr != end; ++itr) {
            // don't add component if it already is present in list
            if (component == *itr) {
                return false;
            }
        }
        this->components.push_back(component);

        Bool addedBaseObject3DRenderer = this->testAndSetComponentMemberVar<BaseObject3DRenderer>(component, this->baseRenderer, std::string("base renderer"));
        Bool addedBaseRenderableContainer = this->testAndSetComponentMemberVar<BaseRenderableContainer>(component, this->baseRenderableContainer, std::string("base renderable container"));
        Bool addedLight = this->testAndSetComponentMemberVar<Light>(component, this->light, std::string("light"));
        this->testAndSetComponentMemberVar<ReflectionProbe>(component, this->reflectionProbe, std::string("reflection probe"));
        this->testAndSetComponentMemberVar<Camera>(component, this->camera, std::string("camera"));
        this->testAndSetComponentMemberVar<ParticleSystem>(component, this->particleSystem, std::string("particle system"));

        if (addedBaseObject3DRenderer) {
            WeakPointer<MeshRenderer> meshRenderer = WeakPointer<BaseObject3DRenderer>::dynamicPointerCast<MeshRenderer>(this->baseRenderer);
            if (meshRenderer.isValid()) {
                this->meshRenderer = meshRenderer;
            }
            WeakPointer<ParticleSystemRenderer> particleSystemRenderer = WeakPointer<BaseObject3DRenderer>::dynamicPointerCast<ParticleSystemRenderer>(this->baseRenderer);
            if (particleSystemRenderer.isValid()) {
                this->particleSystemRenderer = particleSystemRenderer;
            }
        }

        if (addedBaseRenderableContainer) {
            WeakPointer<MeshContainer> meshContainer = WeakPointer<BaseRenderableContainer>::dynamicPointerCast<MeshContainer>(this->baseRenderableContainer);
            if (meshContainer.isValid()) {
                this->meshContainer = meshContainer;
            }
        }

        if (addedLight) {
            WeakPointer<DirectionalLight> directionalLight = WeakPointer<Light>::dynamicPointerCast<DirectionalLight>(this->light);
            if (directionalLight.isValid()) {
                this->directionalLight = directionalLight;
            }
            WeakPointer<PointLight> pointLight = WeakPointer<Light>::dynamicPointerCast<PointLight>(this->light);
            if (pointLight.isValid()) {
                this->pointLight = pointLight;
            }
            WeakPointer<AmbientLight> ambientLight = WeakPointer<Light>::dynamicPointerCast<AmbientLight>(this->light);
            if (ambientLight.isValid()) {
                this->ambientLight = ambientLight;
            }
            WeakPointer<AmbientIBLLight> ambientIBLLight = WeakPointer<Light>::dynamicPointerCast<AmbientIBLLight>(this->light);
            if (ambientIBLLight.isValid()) {
                this->ambientIBLLight = ambientIBLLight;
            }
        }
        
        return true;
    }

    Int32 Object3D::getLayer() {
        return this->layer;
    }

    void Object3D::setLayer(Int32 layer) {
        this->layer = layer;
    }

    void Object3D::setActive(Bool active) {
        this->active = active;
    }

    Bool Object3D::isActive() const {
        return this->active;
    }

    void Object3D::setStatic(Bool objStatic) {
        this->objStatic = objStatic;
    }

    Bool Object3D::isStatic() const {
        return this->objStatic;
    }

    void Object3D::setName(const std::string& name) {
        this->name = name;
    }
    
    const std::string& Object3D::getName() const {
        return this->name;
    }

    UInt32 Object3D::childCount() {
        return this->children.size();
    }

    WeakPointer<Object3D> Object3D::getChild(UInt32 index) {
        return this->children[index];
    }

    WeakPointer<BaseObject3DRenderer> Object3D::getBaseRenderer() {
        return this->baseRenderer;
    }

    WeakPointer<MeshRenderer> Object3D::getMeshRenderer() {
        return this->meshRenderer;
    }

    WeakPointer<ParticleSystemRenderer> Object3D::getParticleSystemRenderer() {
        return this->particleSystemRenderer;
    }

    WeakPointer<BaseRenderableContainer> Object3D::getBaseRenderableContainer() {
        return this->baseRenderableContainer;
    }

    WeakPointer<MeshContainer> Object3D::getMeshContainer() {
        return this->meshContainer;
    }

    WeakPointer<ParticleSystem> Object3D::getParticleSystem() {
        return this->particleSystem;
    }

    WeakPointer<Light> Object3D::getLight() {
        return this->light;
    }

    WeakPointer<DirectionalLight> Object3D::getDirectionalLight() {
        return this->directionalLight;
    }

    WeakPointer<PointLight> Object3D::getPointLight() {
        return this->pointLight;
    }

    WeakPointer<AmbientLight> Object3D::getAmbientLight() {
        return this->ambientLight;
    }

    WeakPointer<AmbientIBLLight> Object3D::getAmbientIBLLight() {
        return this->ambientIBLLight;
    }

    WeakPointer<ReflectionProbe> Object3D::getReflectionProbe() {
        return this->reflectionProbe;
    }

    WeakPointer<Camera> Object3D::getCamera() {
        return this->camera;
    }
}