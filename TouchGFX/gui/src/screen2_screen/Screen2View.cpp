#include <gui/screen2_screen/Screen2View.hpp>
#include <BitmapDatabase.hpp>
Screen2View::Screen2View():    
    zAngle2D(0.0f),
    deltaZangle2D(0.0f)
{
    
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
    // Extra space for the image to rotate in
    int borderWidth = 40;
    int borderHeight = 40;
    
    // Setup the texture mapper image that does 2D rotation
    textureMapperImage2D.setBitmap(Bitmap(BITMAP_IMAGE00_ID));
    
    int imageWidth = textureMapperImage2D.getBitmap().getWidth();//获图片的宽度
    int imageHeight = textureMapperImage2D.getBitmap().getHeight();//获取图片的高度

    textureMapperImage2D.setXY(100, 0);
    textureMapperImage2D.setWidth(imageWidth + borderWidth * 2);
    textureMapperImage2D.setHeight(imageHeight + borderHeight * 2);
    textureMapperImage2D.setBitmapPosition(borderWidth, borderHeight);
    textureMapperImage2D.setOrigo(textureMapperImage2D.getBitmapPositionX() + 97, textureMapperImage2D.getBitmapPositionY() + 108);

    add(textureMapperImage2D);
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::handleTickEvent()
{
    zAngle2D += deltaZangle2D;
    
    // Update the images with the new angles
    // The image is automatically invalidated (the optimal minimal area).
    // If any of the set..() methods (e.g. setOrigo, setCamera, setRenderingAlgorithm...) are used
    // remember to manually invalidate the image afterwards (textureMapperImage2D.invalidate()).
    textureMapperImage2D.updateAngles(0.0F, 0.0F, zAngle2D);
}

void Screen2View::addFanSpeed()
{
    deltaZangle2D += 0.1f;
    if(deltaZangle2D > 1)
        deltaZangle2D = 1;
}

void Screen2View::minusFanSpeed()
{
    deltaZangle2D -= 0.1f;
    if(deltaZangle2D < 0){
        deltaZangle2D = 0;
    }
}

