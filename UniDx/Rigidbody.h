#pragma once

#include "Component.h"
#include "Transform.h"
#include "UniDxTime.h"
#include "Physics.h"
#include "Bounds.h"
#include "Debug.h"


namespace UniDx {


// --------------------
// Rigidbodyクラス
// --------------------
class Rigidbody : public Component
{
public:
    // 位置。値を直接設定するとテレポートする。
    Property<Vector3> position;

    // 速度
    Vector3 velocity{ 0, 0, 0 };

    // 重力スケール（1.0fで標準重力、0で無重力、負値で逆重力）
    float gravityScale = 1.0f;

    // 質量（0以下は1.0fとして扱う）
    float mass = 1.0f;

    bool isKinematic = false;

    Rigidbody() :
        position(
            [this]() { return position_; },
            [this](const Vector3& v) { position_ = v; move_ = Vector3::Zero; hasMovePos_ = true; }
        )
    {
    }

    // 初期化
    virtual void Awake() override
    {
        position_ = transform->position;
    }

    virtual void OnEnable() override
    {
        Physics::instance->registerRigidbody(this);
    }

    virtual void OnDisable() override
    {
        Physics::instance->unregisterRigidbody(this);
    }

    // 指定位置に移動。補間が有効な場合は間の衝突判定を行う。
    void MovePosition(const Vector3& pos)
    {
        move_ = pos - position_;
        hasMovePos_ = true;
    }

    // ステップ時間を指定して移動ベクトルを取得
    Vector3 getMoveVector(float step) { return move_ * (Time::fixedDeltaTime > 0 ? step / Time::fixedDeltaTime : 1); }

    // 衝突前の物理更新
    // ここで移動量などを設定しておくが、位置や速度の更新はコリジョン処理の後
    virtual void physicsUpdate()
    {
        if (!enabled) return;

        // 重力適用
        if (gravityScale != 0.0f)
        {
            velocity.y += Physics::gravity * gravityScale * Time::fixedDeltaTime;
        }

        // 位置の直接指定がなければ、移動ベクトルに速度を入れる
        if (!hasMovePos_)
        {
            move_ = velocity * Time::fixedDeltaTime;
        }
    }

    // 移動ベクトルを位置に適用
    virtual void applyMove(float step)
    {
        position_ += getMoveVector(step);

        move_ = Vector3::Zero;
        hasMovePos_ = false;
    }

    // 位置と速度の補正を適用してTransformに反映
    virtual void solveCorrection()
    {
        // 位置と速度の補正
        position_ += correctPositionBounds.min();
        position_ += correctPositionBounds.max();
        correctPositionBounds.Center = Vector3::Zero;
        correctPositionBounds.Extents = Vector3::Zero;

        velocity += correctVelocityBounds.min();
        velocity += correctVelocityBounds.max();
        correctVelocityBounds.Center = Vector3::Zero;
        correctVelocityBounds.Extents = Vector3::Zero;

        // Transformに位置を反映
        transform->position = position_;
    }

    // 位置を補正する差分ベクトルを登録
    void addCorrectPosition(const Vector3& vec)
    {
        correctPositionBounds.Encapsulate(vec);
    }

    // 速度を補正する差分ベクトルを登録
    void addCorrectVelocity(const Vector3& vec)
    {
        correctVelocityBounds.Encapsulate(vec);
    }

private:
    Vector3 position_;
    Vector3 move_{ 0, 0, 0 };
    Bounds correctPositionBounds;
    Bounds correctVelocityBounds;

    bool hasMovePos_ = false;
};


} // namespace UniDx
