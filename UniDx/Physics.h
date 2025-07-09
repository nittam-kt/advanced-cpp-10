#pragma once

#include <vector>
#include <array>

#include "Property.h"
#include "Singleton.h"
#include "Bounds.h"

namespace UniDx
{

class Collider;
class Rigidbody;
class PhysicsShape;


struct Contact
{
//    Vector3 point;
    Vector3 normal;     // from A to B
    float   penetration;
};

struct ContactManifold
{
    PhysicsShape* a;
    PhysicsShape* b;
    std::array<Contact, 4> contacts;  // 1〜4点
    int numContacts;
};

class AABBGeometory;
class SpheresGeometory;
class CapsulesGeometory;
class BoxGeometory;


// --------------------
// PhysicsActor
// --------------------
class  PhysicsActor
{
public:
    void initialize(Rigidbody* rigidbody);

    Rigidbody* getRigidbody() const { return rigidbody_; }
    bool isValid() const { return rigidbody_ != nullptr; }
    void setInvalid() { rigidbody_ = nullptr; }

private:
    Rigidbody* rigidbody_;
};


// --------------------
// PhysicsShape
// --------------------
class  PhysicsShape
{
public:
    void initialize(Collider* collider);

    Bounds moveBounds;  // コライダーの bounds に移動量を広げた範囲

    Collider* getCollider() const { return collider_; }
    bool isValid() const { return collider_ != nullptr; }
    void setInvalid() { collider_ = nullptr; }

private:
    Collider* collider_;
};


// --------------------
// Physics
// --------------------
class Physics : public Singleton<Physics>
{
public:
    static inline float gravity = -9.81f;

    void simulate(float setp);
    void simulatePositionCorrection(float step);

    void registerRigidbody(Rigidbody* rigidbody);
    void unregisterRigidbody(Rigidbody* rigidbody);
    void register3d(Collider* collider);
    void unregister3d(Collider* collider);

private:
    struct PotentialPair {
        PhysicsShape* a;
        PhysicsShape* b;
    };
    std::vector<PotentialPair> potentialPairs;

    std::vector<ContactManifold> manifolds;

    std::vector<PhysicsActor> physicsActors;
    std::vector<PhysicsShape> physicsShapes;

    void initializeSimulate(float step);
    bool intersectShapes(PhysicsShape& a, PhysicsShape& b, ContactManifold* m);
    void solveVelocityConstraint(Rigidbody* A, Rigidbody* B, const ContactManifold& m);
    void solvePositionConstraint(Rigidbody* A, Rigidbody* B, const ContactManifold& m);
};

}
