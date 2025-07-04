#pragma once
#include <SimpleMath.h>

#include "Component.h"


namespace UniDx {

// --------------------
// Behaviour基底クラス
// --------------------
class Behaviour : public Component
{
public:
    virtual void FixedUpdate() {}
    virtual void Update() {}
    virtual void LateUpdate() {}

    virtual ~Behaviour() = default;
};


} // namespace UniDx
