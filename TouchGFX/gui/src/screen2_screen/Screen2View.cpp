#include <gui/screen2_screen/Screen2View.hpp>

Screen2View::Screen2View():    
    xAngle3D(0.0f),
    yAngle3D(0.0f),
    zAngle3D(0.0f),
    deltaXangle3D(0.032f),
    deltaYangle3D(0.029f),
    deltaZangle3D(0.027f),
    zAngle2D(0.0f),
    deltaZangle2D(0.019f)
{
    
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::handleTickEvent()
{
    static int time = 0;
    time++;
    
    // Update angle values
    xAngle3D += deltaXangle3D;
    yAngle3D += deltaYangle3D;
    zAngle3D += deltaZangle3D;

    zAngle2D += deltaZangle2D;
}

