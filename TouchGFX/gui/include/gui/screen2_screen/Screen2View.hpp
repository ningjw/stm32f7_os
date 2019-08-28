#ifndef SCREEN2_VIEW_HPP
#define SCREEN2_VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
        
    virtual void handleTickEvent();
    
protected:
    TextureMapper textureMapperImage3D;
    TextureMapper textureMapperImage2D;

    float xAngle3D;
    float yAngle3D;
    float zAngle3D;

    float deltaXangle3D;
    float deltaYangle3D;
    float deltaZangle3D;

    float zAngle2D;

    float deltaZangle2D;
};

#endif // SCREEN2_VIEW_HPP
