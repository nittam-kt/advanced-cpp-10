#pragma once

#include <UniDx.h>
#include <Renderer.h>


class TestRenderer : public UniDx::MeshRenderer
{
protected:
    virtual void OnEnable();
};