#pragma once

#include <vector>
#include <memory>

#include <d3d11.h>
#include <wrl/client.h>
#include <SimpleMath.h>

namespace UniDx
{
using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Color;

class Object;
class GameObject;
class Component;
class Transform;

typedef std::vector<std::unique_ptr<GameObject>> GameObjectContainer;

}
