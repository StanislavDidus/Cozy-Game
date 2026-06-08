#pragma once

class State
{
    virtual onEnter() = 0;
    virtual onUpdate() = 0;
    virtual onRender() = 0;
    virtual onRenderUI() = 0;
    virtual onExit() = 0;
};
