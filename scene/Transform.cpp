#include "../util/WeakPointer.h"
#include "Object3D.h"

namespace Core {

    Transform::Transform(const Object3D& target) : target(target) {
        this->localMatrix.setIdentity();
        this->worldMatrix.setIdentity();
    }

    Transform::Transform(const Object3D& target, const Matrix4x4& matrix) : target(target) {
        this->localMatrix.copy(matrix);
    }

    Transform::~Transform() {
    }

    Matrix4x4& Transform::getLocalMatrix() {
        return this->localMatrix;
    }

    const Matrix4x4& Transform::getConstLocalMatrix() const {
        return this->localMatrix;
    }

    Matrix4x4& Transform::getWorldMatrix() {
        return this->worldMatrix;
    }

    const Matrix4x4& Transform::getConstWorldMatrix() const {
        return this->worldMatrix;
    }

    /*
     * Copy this Transform object's local matrix into [dest].
     */
    void Transform::copyLocalMatrix(Matrix4x4& dest) const {
        dest.copy(this->localMatrix);
    }

    /*
     * Copy this Transform object's world matrix into [dest].
     */
    void Transform::copyWorldMatrix(Matrix4x4& dest) const {
        dest.copy(this->worldMatrix);
    }

    void Transform::setLocalMatrix(const Matrix4x4& mat) {
        this->localMatrix.copy(mat);
    }

    void Transform::applyTransformationTo(Vector4<Real>& vector) {
        this->updateWorldMatrix();
        this->worldMatrix.transform(vector);
    }

    void Transform::applyTransformationTo(Vector3Base<Real>& vector) {
        this->updateWorldMatrix();
        this->worldMatrix.transform(vector);
    }

    void Transform::calculateWorldMatrix(WeakPointer<Object3D> target, Matrix4x4& result) {
        result.setIdentity();
        if (!target.isValid()) return;
        result.copy(target->getTransform().getLocalMatrix());
        WeakPointer<Object3D> parent = target->getParent();
        while (parent.isValid()) {
            result.preMultiply(parent->getTransform().getLocalMatrix());
            parent = parent->getParent();
        }
    }

    void Transform::calculateWorldMatrix(Matrix4x4& result) {
        this->getAncestorWorldMatrix(result);
        result.multiply(this->localMatrix);
    }

    void Transform::getAncestorWorldMatrix(Matrix4x4& result) {
        Transform::calculateWorldMatrix(this->target.getParent(), result);
    }

    void Transform::updateWorldMatrix() {
        this->calculateWorldMatrix(this->worldMatrix);
    }

    /*
     * This method plays a critical part of performing transformations on scene objects in world space. In order to perform
     * these kinds of transformations, it is necessary to take into account each local transformation of each ancestor of
     * the scene object. If we wanted to apply a world transformation to a single matrix, we would simply pre-multiply that
     * matrix with the desired transformation. With scene objects that are part of a scene hierarchy, we can't do that since
     * the pre-multiplication would have to occur at the top of the hierarchy, and therefore quite likely to a different scene
     * object than the one in question (we only want to modify the transform of the target scene object).
     *
     * We solve this problem by doing some arithmetic to find the equivalent transformation in the scene object's local space
     * that accomplishes the same effect as the world space transformation that would occur on the scene object at the top of
     * the hierarchy:
     *
     *   S = The target scene object.
     *   A = Aggregate/concatenation of all ancestors of S.
     *   L = The local transformation of S.
     *   nWorld = The world space transformation.
     *   nLocal = The transformation in the local space of S.
     *
     *   F = The concatenation of A & L -> A * L
     *   FI = The inverse of F.
     *
     *   We can easily derive a desired world-space transformation that is suited for pre-multiplication. To apply that transformation,
     *   we could simply do: nWorld * F. The problem there is that we'd have to apply that transformation to the top of the hierarchy,
     *   which we cannot do as it would likely affect other scene objects. We find the equivalent transformation in the local space of S (nLocal) by:
     *
     *   	  nWorld * F = F * nLocal
     *   FI * nWorld * F = FI * F * nLocal
     *   			     = nLocal
     *
     *  Therefore the equivalent transformation in the local space of S is: FI * nWorld * F. This method takes in nWorld [worldTransformation]
     *  and produces (FI * nWorld * F) in [localTransformation].
     */
    void Transform::getLocalTransformationFromWorldTransformation(const Matrix4x4& newWorldTransformation, Matrix4x4& localTransformation) {
        Matrix4x4 currentFullTransformation;
        this->calculateWorldMatrix(currentFullTransformation);
        this->getLocalTransformationFromWorldTransformation(newWorldTransformation, currentFullTransformation, localTransformation);
    }

    void Transform::getLocalTransformationFromWorldTransformation(const Matrix4x4& newWorldTransformation, const Matrix4x4& currentFullTransformation, Matrix4x4& localTransformation) {
        Matrix4x4 fullInverse;
        localTransformation = currentFullTransformation;
        fullInverse.copy(currentFullTransformation);
        fullInverse.invert();
        localTransformation.preMultiply(newWorldTransformation);
        localTransformation.preMultiply(fullInverse);
    }

    void Transform::lookAt(const Point3r& target) {
        this->lookAt(target, Vector3r(0.0f, 1.0f, 0.0f));
    }

    void Transform::lookAt(const Point3r& target, const Vector3r& up) {

        Point3r src;
        this->updateWorldMatrix();
        this->worldMatrix.transform(src);

        Matrix4x4 temp;
        temp.lookAt(src, target, up);

        WeakPointer<Object3D> parent = this->target.getParent();

        if (parent.isValid()) {
            parent->getTransform().updateWorldMatrix();
            Matrix4x4 parentMat = parent->getTransform().getWorldMatrix();
            parentMat.invert();
            temp.preMultiply(parentMat);
        }

        this->getLocalMatrix().copy(temp);
    }

    void Transform::transformBy(const Matrix4x4& mat, TransformationSpace transformationSpace) {
        if (transformationSpace == TransformationSpace::Local) {
            this->localMatrix.multiply(mat);
        }
        else if (transformationSpace == TransformationSpace::PreLocal) {
            this->localMatrix.preMultiply(mat);
        }
        else {
            Matrix4x4 localTransformation;
            this->getLocalTransformationFromWorldTransformation(mat, localTransformation);
            this->localMatrix.multiply(localTransformation); 
        }
    }

    void Transform::translate(const Vector3<Real>& dir, TransformationSpace transformationSpace) {
        this->translate(dir.x, dir.y, dir.z, transformationSpace);
    }

    void Transform::translate(Real x, Real y, Real z, TransformationSpace transformationSpace) {
        if (transformationSpace == TransformationSpace::Local) {
            this->localMatrix.translate(x, y, z);
        }
        else if (transformationSpace == TransformationSpace::PreLocal) {
            this->localMatrix.preTranslate(x, y, z);
        }
        else {
            Matrix4x4 localTransformation;
            Matrix4x4 worldTransformation;
            worldTransformation.translate(x, y, z);
            this->getLocalTransformationFromWorldTransformation(worldTransformation, localTransformation);
            this->localMatrix.multiply(localTransformation);
            
        }
    }

    void Transform::setLocalPosition(Real x, Real y, Real z) {
        this->localMatrix.setTranslation(x, y, z);
    }

    void Transform::setLocalPosition(const Vector3<Real>& position) {
        this->setLocalPosition(position.x, position.y, position.z);
    }

    Point3r Transform::getLocalPosition() {
        Point3r position;
        this->localMatrix.transform(position);
        return position;
    }

    void Transform::rotate(const Vector3<Real>& axis, Real angle, TransformationSpace transformationSpace) {
        this->rotate(axis.x, axis.y, axis.z, angle, transformationSpace);
    }

    void Transform::rotate(Real x, Real y, Real z, Real angle, TransformationSpace transformationSpace) {
        if (transformationSpace == TransformationSpace::Local) {
            this->localMatrix.rotate(x, y, z, angle);
        }
        else if (transformationSpace == TransformationSpace::PreLocal) {
            this->localMatrix.preRotate(x, y, z, angle);
        }
        else {
            Matrix4x4 localTransformation;
            Matrix4x4 worldTransformation;
            worldTransformation.rotate(x, y, z, angle);
            this->getLocalTransformationFromWorldTransformation(worldTransformation, localTransformation);
            this->localMatrix.multiply(localTransformation);
        }
    }

    void Transform::rotateAround(const Vector3<Real>& axis, const Point3<Real>& pos, Real angle) {
        this->rotateAround(axis.x, axis.y, axis.z, pos.x, pos.y, pos.z, angle);
    }

    void Transform::rotateAround(Real ax, Real ay, Real az, Real px, Real py, Real pz, Real angle) {
        Matrix4x4 localTransformation;
        Matrix4x4 worldTransformation;
        worldTransformation.translate(-px, -py, -pz);
        worldTransformation.preRotate(ax, ay, az, angle);
        worldTransformation.preTranslate(px, py, pz);
        this->getLocalTransformationFromWorldTransformation(worldTransformation, localTransformation);
        this->localMatrix.multiply(localTransformation);
    }

    void Transform::scale(Real x, Real y, Real z) {

        // TODO: implement!
    }

    void Transform::setWorldPosition(const Vector3Base<Real>& position) {
        this->setWorldPosition(position.x, position.y, position.z);
    }

    void Transform::setWorldPosition(const Vector3<Real>& position) {
        this->setWorldPosition(position.x, position.y, position.z);
    }

    void Transform::setWorldPosition(Real x, Real y, Real z) {
        Point3r oldPosition;
        Matrix4x4 ancestorMatrix;
        Matrix4x4 fullMatrix;
        this->getAncestorWorldMatrix(ancestorMatrix);
        fullMatrix = ancestorMatrix;
        fullMatrix.multiply(this->localMatrix);
        fullMatrix.transform(oldPosition);
        Vector3r toNewPosition(x - oldPosition.x, y - oldPosition.y, z - oldPosition.z);
        Matrix4x4 worldTranslateMatrix;
        worldTranslateMatrix.preTranslate(toNewPosition);
        Matrix4x4 localTranslateMatrix;
        this->getLocalTransformationFromWorldTransformation(worldTranslateMatrix, fullMatrix, localTranslateMatrix);
        this->localMatrix.multiply(localTranslateMatrix);
        this->worldMatrix = ancestorMatrix;
        this->worldMatrix.multiply(this->localMatrix);
    }

    Point3r Transform::getWorldPosition() {
        Point3r position;
        this->updateWorldMatrix();
        this->worldMatrix.transform(position);
        return position;
    }
}
