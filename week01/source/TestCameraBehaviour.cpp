#pragma once

#include "TestCameraBehaviour.h"

#include <Transform.h>
#include <Input.h>
#include <UniDxTime.h>


using namespace DirectX;
using namespace UniDx;


void TestCameraBehaviour::OnEnable()
{
    transform->localPosition = Vector3(0, 6, -2);

    Quaternion rot = Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), 1);
    transform->localRotation = rot;
}


void TestCameraBehaviour::Update()
{
    const float speed = 1;

    Vector3 pos = transform->localPosition;
    if (Input::GetKey(Keyboard::A))
    {
        pos.x -= speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::D))
    {
        pos.x += speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::W))
    {
        pos.y += speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::S))
    {
        pos.y -= speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::Q))
    {
        pos.z += speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::E))
    {
        pos.z -= speed * Time::deltaTime;
    }
    transform->localPosition = pos;

    const float angleSpeed = 1.0f;
    Quaternion localRot = transform->localRotation;
    Quaternion rot;
    if (Input::GetKey(Keyboard::J))
    {
        rot = Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), -angleSpeed * Time::deltaTime);
    }
    if (Input::GetKey(Keyboard::K))
    {
        rot = Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), angleSpeed * Time::deltaTime);
    }
    transform->localRotation = localRot * rot;
}
