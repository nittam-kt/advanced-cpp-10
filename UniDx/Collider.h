#pragma once
#include <SimpleMath.h>

#include "Component.h"
#include "Bounds.h"
#include "Physics.h"

namespace UniDx
{

class Rigidbody;
class SphereCollider;
class AABBCollider;

// --------------------
// Collider基底クラス
// --------------------
class Collider : public Component
{
public:
    Rigidbody* attachedRigidbody = nullptr;

    virtual void OnEnable() override
    {
        attachedRigidbody = findNearestRigidbody(transform);
        Physics::instance->register3d(this);
    }

    virtual void OnDisable() override
    {
        Physics::instance->unregister3d(this);
    }

    // ワールド空間における空間境界を取得
    virtual Bounds getBounds() const = 0;

    // 衝突チェック
    // 衝突していれば attachedRigidbody に addCorrectPosition(), addCorrectVelocity() で補正する
    virtual bool checkIntersect(Collider* other) = 0;
    virtual bool checkIntersect(SphereCollider* other) = 0;
    virtual bool checkIntersect(AABBCollider* other) = 0;

private:
    Rigidbody* findNearestRigidbody(Transform* t) const;
};


// --------------------
// AABBCollider
// --------------------
class AABBCollider : public Collider
{
public:
    Vector3 center;
    Vector3 size;

    AABBCollider() : center(Vector3::Zero), size(Vector3(0.5f,0.5f,0.5f)) {}

    // ワールド空間における空間境界を取得
    virtual Bounds getBounds() const override;

    // 衝突チェック
    // 衝突していれば attachedRigidbody に addCorrectPosition(), addCorrectVelocity() で補正する
    virtual bool checkIntersect(Collider* other) { return other->checkIntersect(this); }
    virtual bool checkIntersect(SphereCollider* other);
    virtual bool checkIntersect(AABBCollider* other);
};


// --------------------
// SphereCollider
// --------------------
class SphereCollider : public Collider
{
public:
    Vector3 center;
    float radius;

    SphereCollider() : center(Vector3::Zero), radius(0.5) {}

    // ワールド空間における空間境界を取得
    virtual Bounds getBounds() const override;

    // 衝突チェック
    // 衝突していれば attachedRigidbody に addCorrectPosition(), addCorrectVelocity() で補正する
    virtual bool checkIntersect(Collider* other) { return other->checkIntersect(this); }
    virtual bool checkIntersect(SphereCollider* other);
    virtual bool checkIntersect(AABBCollider* other);
};


} // namespace UniDx
