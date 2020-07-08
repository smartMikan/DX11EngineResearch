#pragma once
#include "Engine/GameObjectClass.h"
class Player :
    public GameObjectClass
{
public:
    bool InitialAnimations(ConstantBuffer<ConstantBuffer_Bones>& cb_Bones);


    bool HandleInput(InputClass* Input, float frameTime, float fps, Transform* orbitTrans);

    void Round();

    bool isCyclone = false;
};

