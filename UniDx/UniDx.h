#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#include "UniDxDefine.h"
#include "Debug.h"

namespace UniDx
{

std::string ToUtf8(const std::wstring& wstr);

}
