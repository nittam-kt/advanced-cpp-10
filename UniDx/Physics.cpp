#include "Physics.h"
#include "Collider.h"
#include "Rigidbody.h"


namespace UniDx
{

using namespace std;

// 初期化
void PhysicsShape::initialize(Collider* collider)
{
    collider_ = collider;
    // moveBounds
}


// 初期化
void PhysicsActor::initialize(Rigidbody* rigidbody)
{
    rigidbody_ = rigidbody;
}


// Rigidbodyを登録
void Physics::registerRigidbody(Rigidbody* rigidbody)
{
    for (size_t i = 0; i < physicsActors.size(); ++i)
    {
        if (!physicsActors[i].isValid())
        {
            physicsActors[i].initialize(rigidbody);
            return;
        }
        if (physicsActors[i].getRigidbody() == rigidbody)
        {
            return; // 登録済み
        }
    }

    // 無効化されたものがなければ追加
    physicsActors.push_back(PhysicsActor());
    physicsActors.back().initialize(rigidbody);
}


// 3D形状を持ったコライダーの登録を解除
void Physics::unregisterRigidbody(Rigidbody* rigidbody)
{
    for (size_t i = 0; i < physicsActors.size(); ++i)
    {
        if (physicsActors[i].getRigidbody() == rigidbody)
        {
            physicsActors[i].setInvalid();
            return;
        }
    }
}


// 3D形状を持ったコライダーを登録
void Physics::register3d(Collider* collider)
{
    for(size_t i  = 0; i < physicsShapes.size(); ++i)
    {
        if(!physicsShapes[i].isValid())
        {
            physicsShapes[i].initialize(collider);
            return;
        }
        if (physicsShapes[i].getCollider() == collider)
        {
            return; // 登録済み
        }
    }

    // 無効化されたものがなければ追加
    physicsShapes.push_back(PhysicsShape());
    physicsShapes.back().initialize(collider);
}


// 3D形状を持ったコライダーの登録を解除
void Physics::unregister3d(Collider* collider)
{
    for (size_t i = 0; i < physicsShapes.size(); ++i)
    {
        if (physicsShapes[i].getCollider() == collider)
        {
            physicsShapes[i].setInvalid();
            return;
        }
    }
}


// 物理計算準備
void Physics::initializeSimulate(float step)
{
    // 無効になっているものをvectorから削除
    for (vector<PhysicsActor>::iterator it = physicsActors.begin(); it != physicsActors.end();)
    {
        if (!it->isValid())
        {
            it = physicsActors.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (vector<PhysicsShape>::iterator it = physicsShapes.begin(); it != physicsShapes.end();)
    {
        if (!it->isValid())
        {
            it = physicsShapes.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Rigidbodyの更新
    for (auto& act : physicsActors)
    {
        act.getRigidbody()->physicsUpdate();
    }

    // Shapeの移動Boundsを更新
    for (auto& shape : physicsShapes)
    {
        Bounds bounds = shape.getCollider()->getBounds();
        auto rb = shape.getCollider()->attachedRigidbody;
        if (rb != nullptr)
        {
            bounds.Encapsulate(bounds.min() + rb->getMoveVector(step));
            bounds.Encapsulate(bounds.max() + rb->getMoveVector(step));
        }
        shape.moveBounds = bounds;
    }
}


// 位置補正法（射影法）による物理計算のシミュレート
void Physics::simulatePositionCorrection(float step)
{
    initializeSimulate(step);

    // まずは当たりそうなペアをAABBで判定して抽出
    potentialPairs.clear();
    for (size_t i = 0; i < physicsShapes.size(); ++i)
    {
        for (size_t j = i + 1; j < physicsShapes.size(); ++j)
        {
            if (physicsShapes[i].moveBounds.Intersects(physicsShapes[j].moveBounds))
            {
                // 同じ Rigidbody に属しているコンパウンド同士は自己衝突なのでスキップ
                auto rbA = physicsShapes[i].getCollider()->attachedRigidbody;
                auto rbB = physicsShapes[j].getCollider()->attachedRigidbody;
                if (rbA && rbA == rbB) continue;

                // ペアを記憶
                potentialPairs.push_back({ &physicsShapes[i], &physicsShapes[j] });
            }
        }
    }

    // 先に位置を更新する
    for (auto& act : physicsActors)
    {
        act.getRigidbody()->applyMove(step);
    }

    // 衝突をチェックする
    for (auto& pair : potentialPairs)
    {
        pair.a->getCollider()->checkIntersect(pair.b->getCollider());
    }

    // 衝突で生じた補正を含めて位置と速度を解決する
    for (auto& act : physicsActors)
    {
        act.getRigidbody()->solveCorrection();
    }
}


// 物理計算のシミュレート
void Physics::simulate(float step)
{
    initializeSimulate(step);

    // まずは当たりそうなペアをAABBで判定して抽出
    potentialPairs.clear();
    for (size_t i = 0; i < physicsShapes.size(); ++i)
    {
        for (size_t j = i + 1; j < physicsShapes.size(); ++j)
        {
            if (physicsShapes[i].moveBounds.Intersects(physicsShapes[j].moveBounds))
            {
                // 同じ Rigidbody に属しているコンパウンド同士は自己衝突なのでスキップ
                auto rbA = physicsShapes[i].getCollider()->attachedRigidbody;
                auto rbB = physicsShapes[j].getCollider()->attachedRigidbody;
                if (rbA && rbA == rbB) continue;

                // ペアを記憶。ここでは詳細判定しない
                potentialPairs.push_back({ &physicsShapes[i], &physicsShapes[j] });
            }
        }
    }

    // 形状ごとに実衝突を確定する
    manifolds.clear();
    for (auto& pair : potentialPairs)
    {
        ContactManifold m;
//        if (intersectShapes(*pair.a, *pair.b, &m)) {  // ← ここが Narrow-phase
//            manifolds.push_back(m);
//        }
    }

    // ソルバ
    for (auto& m : manifolds)
    {
        Rigidbody* rbA = m.a->getCollider()->attachedRigidbody;
        Rigidbody* rbB = m.b->getCollider()->attachedRigidbody;

        // 速度レベルの反発インパルス (Impulses) をかける
        solveVelocityConstraint(rbA, rbB, m);

        // 位置のめり込みを少し戻す (Baumgarte / Position correction)
        solvePositionConstraint(rbA, rbB, m);
    }
}


void Physics::solveVelocityConstraint(Rigidbody* A, Rigidbody* B, const ContactManifold& m)
{
    const float restitution = 0.2f;   // 反発係数

    for (int i = 0; i < m.numContacts; ++i)
    {
    }
}


void Physics::solvePositionConstraint(Rigidbody* A, Rigidbody* B, const ContactManifold& m)
{

}


} // UniDx
