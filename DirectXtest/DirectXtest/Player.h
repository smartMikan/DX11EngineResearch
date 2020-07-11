#pragma once
#include "Engine/GameObjectClass.h"
class Player : public GameObjectClass
{
public:
    bool InitialAnimations(ConstantBuffer<ConstantBuffer_Bones>& cb_Bones);


    void Frame(float frametime);

    bool HandleInput(InputClass* Input, Transform* orbitTrans);

    void Round(float speed);

    bool isCyclone = false;
};

