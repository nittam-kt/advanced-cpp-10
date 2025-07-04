#pragma once

#include <UniDx.h>
#include <Behaviour.h>


class TestCameraBehaviour : public UniDx::Behaviour
{
public:
    virtual void OnEnable() override;
    virtual void Update() override;
};