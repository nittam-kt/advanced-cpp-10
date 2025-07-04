// week01.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "Engine.h"

#include <string>
#include <chrono>

#include <Keyboard.h>          // DirectXTK
#include <SimpleMath.h>        // DirectXTK �֗����w���[�e�B���e�B
using namespace DirectX;
using namespace DirectX::SimpleMath;

// DirectX�e�N�X�`�����C�u�������g�p�ł���悤�ɂ���
#include <DirectXTex.h>

// DirextX�t�H���g���C�u�������g�p�ł���悤�ɂ���
#include <SpriteFont.h>

#include "D3DManager.h"
#include "UniDxTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Behaviour.h"
#include "Camera.h"
#include "Renderer.h"
#include "Transform.h"
#include "Input.h"
#include "Debug.h"

// �t�H���g�`��p
std::unique_ptr<SpriteBatch> g_spriteBatch;
std::unique_ptr<SpriteFont>  g_spriteFont;
std::wstring text[4];

using namespace std;
using namespace UniDx;

namespace UniDx
{

//
//   �֐�: Initialize(HWND hWnd)
//
void Engine::Initialize(HWND hWnd)
{
    // Direct3D�C���X�^���X�쐬
    D3DManager::create();

    // Direct3D������
    D3DManager::instance->Initialize(hWnd, 1280, 720);

    // �V�[���}�l�[�W���̃C���X�^���X�쐬
    SceneManager::create();

    // ���͂̏�����
    Input::initialize();

    // �t�H���g������
    g_spriteBatch = std::make_unique<SpriteBatch>(D3DManager::instance->GetContext().Get());
    g_spriteFont = std::make_unique<SpriteFont>(D3DManager::instance->GetDevice().Get(), L"Resource/M PLUS 1.spritefont");
}


// �����V�[���쐬
void Engine::createScene()
{
    SceneManager::instance->createScene();

    // Awake
    for (auto& it : SceneManager::instance->GetActiveScene()->GetRootGameObjects())
    {
        awake(&*it);
    }
}


int Engine::PlayerLoop()
{
    MSG msg;

    Time::Start();
    double restFixedUpdateTime = 0.0f;

    // �f�t�H���g�̃V�[���쐬
    createScene();

    // ���C�� ���b�Z�[�W ���[�v:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //============================================
            // �E�B���h�E���b�Z�[�W����
            //============================================
            // �I�����b�Z�[�W������
            if (msg.message == WM_QUIT) {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        using clock = std::chrono::steady_clock;          // ���m�g�j�b�N�Ȃ̂Ōo�ߎ��Ԍv������
        auto start = clock::now();

        //============================================
        // �Q�[���̏���������
        //============================================
        // ��ʂ�h��Ԃ�
        D3DManager::instance->Clear(0.3f, 0.5f, 0.9f, 1.0f);

        Time::SetDeltaTimeFixed();

        while (restFixedUpdateTime > Time::fixedDeltaTime)
        {
            // �Œ莞�ԍX�V�X�V
            fixedUpdate();

            // �����v�Z
            physics();

            restFixedUpdateTime -= Time::fixedDeltaTime;
        }

        Time::SetDeltaTimeFrame();

        // ���͍X�V
        input();

        // �X�V����
        update();

        // ��X�V����
        lateUpdate();

        // �`�揈��
        render();

        // �o�b�N�o�b�t�@�̓��e����ʂɕ\��
        D3DManager::instance->Present();

        // ���Ԍv�Z
        double deltaTime = std::chrono::duration<double>(clock::now() - start).count();
        restFixedUpdateTime += deltaTime;

        Time::UpdateFrame(deltaTime);
    }

    // �I������
    finalize();

    return (int)msg.wParam;
}


// �Œ莞�ԍX�V�X�V
void Engine::fixedUpdate()
{
    for (auto& it : SceneManager::instance->GetActiveScene()->GetRootGameObjects())
    {
        fixedUpdate(&*it);
    }
}


// �����v�Z
void Engine::physics()
{

}


// ���͍X�V
void Engine::input()
{
    Input::update();
}


//
//  �֐�: Update()
//
//  �ړI: �Q�[���̍X�V�������s���܂��B
//
void Engine::update()
{
    // �e�R���|�[�l���g�� Start()
    for (auto& it : SceneManager::instance->GetActiveScene()->GetRootGameObjects())
    {
        checkStart(&*it);
    }

    // �e�R���|�[�l���g�� Update()
    for (auto& it : SceneManager::instance->GetActiveScene()->GetRootGameObjects())
    {
        update(&*it);
    }

    text[0] = std::to_wstring(0);
    text[1] = std::to_wstring(0);
    text[2] = std::to_wstring(0);
    text[3] = std::to_wstring(0);
}


// ��X�V����
void Engine::lateUpdate()
{
    // �e�R���|�[�l���g�� LateUpdate()
    for (auto& it : SceneManager::instance->GetActiveScene()->GetRootGameObjects())
    {
        lateUpdate(&*it);
    }
}


//
//  �֐�: Render()
//
//  �ړI: ��ʂ̕`�揈�����s���܂��B
//
void Engine::render()
{
    g_spriteBatch->Begin();

    Vector2 drawPos(100, 100);
    for (auto& str : text)
    {
        g_spriteFont->DrawString(g_spriteBatch.get(),
            str.c_str(),
            drawPos);
        drawPos.y += 50;
    }

    g_spriteBatch->End();

    // �e�R���|�[�l���g�� Render()
    Camera* camera = Camera::main;
    if (camera != nullptr)
    {
        for (auto& it : SceneManager::instance->GetActiveScene()->GetRootGameObjects())
        {
            render(&*it, *camera);
        }
    }
}


// �I������
void Engine::finalize()
{

}


void Engine::awake(GameObject* object)
{
    // ���g�̃R���|�[�l���g�̒���Awake���Ăяo���Ă��Ȃ����̂��Ă�
    for (auto& it : object->GetComponents())
    {
        it->checkAwake();
    }

    // �q���̃I�u�W�F�N�g�ɂ��čċA
    for (auto& it : object->transform->getChildGameObjects())
    {
        awake(&*it);
    }
}


void Engine::fixedUpdate(GameObject* object)
{
    // FixedUpdate���Ă�
    for (auto& it : object->GetComponents())
    {
        auto behaviour = dynamic_cast<Behaviour*>(it.get());
        if (behaviour != nullptr && behaviour->enabled)
        {
            behaviour->FixedUpdate();
        }
    }

    // �q���̃I�u�W�F�N�g�ɂ��čċA
    for (auto& it : object->transform->getChildGameObjects())
    {
        fixedUpdate(&*it);
    }
}


void Engine::checkStart(GameObject* object)
{
    // ���g�̃R���|�[�l���g�̒���Start���Ăяo���Ă��Ȃ����̂��Ă�
    for (auto& it : object->GetComponents())
    {
        auto behaviour = dynamic_cast<Behaviour*>(it.get());
        if (behaviour != nullptr)
        {
            behaviour->checkStart();
        }
    }

    // �q���̃I�u�W�F�N�g�ɂ��čċA
    for (auto& it : object->transform->getChildGameObjects())
    {
        checkStart(&*it);
    }
}


void Engine::update(GameObject* object)
{
    // Update���Ă�
    for (auto& it : object->GetComponents())
    {
        auto behaviour = dynamic_cast<Behaviour*>(it.get());
        if (behaviour != nullptr && behaviour->enabled)
        {
            behaviour->Update();
        }
    }

    // �q���̃I�u�W�F�N�g�ɂ��čċA
    for (auto& it : object->transform->getChildGameObjects())
    {
        update(&*it);
    }
}


void Engine::lateUpdate(GameObject* object)
{
    // ���g�̃R���|�[�l���g�̒���FixedUpdate���Ăяo���Ă��Ȃ����̂��Ă�
    for (auto& it : object->GetComponents())
    {
        auto behaviour = dynamic_cast<Behaviour*>(it.get());
        if (behaviour != nullptr && behaviour->enabled)
        {
            behaviour->LateUpdate();
        }
    }

    // �q���̃I�u�W�F�N�g�ɂ��čċA
    for (auto& it : object->transform->getChildGameObjects())
    {
        lateUpdate(&*it);
    }
}


void Engine::render(GameObject* object, const Camera& camera)
{
    // ���g�̃R���|�[�l���g�̒���FixedUpdate���Ăяo���Ă��Ȃ����̂��Ă�
    for (auto& it : object->GetComponents())
    {
        auto renderer = dynamic_cast<Renderer*>(it.get());
        if (renderer != nullptr && renderer->enabled)
        {
            renderer->Render(camera);
        }
    }

    // �q���̃I�u�W�F�N�g�ɂ��čċA
    for (auto& it : object->transform->getChildGameObjects())
    {
        render(&*it, camera);
    }
}

}