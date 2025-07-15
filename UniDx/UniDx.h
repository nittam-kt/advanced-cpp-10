#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#include "UniDxDefine.h"
#include "Debug.h"

#include "GameObject.h"
#include "Transform.h"
#include "GameObject_impl.h"

namespace UniDx
{

using Microsoft::WRL::ComPtr;

std::string ToUtf8(const std::wstring& wstr);

}
