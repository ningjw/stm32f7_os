// -alpha_dither yes -dither 2 -non_opaque_image_format ARGB8888 -opaque_image_format RGB565 0xa6880cb9
// Generated by imageconverter. Please, do not edit!

#include <touchgfx/Bitmap.hpp>
#include <BitmapDatabase.hpp>

#ifndef NO_USING_NAMESPACE_TOUCHGFX
using namespace touchgfx;
#endif

extern const unsigned char _bk2[];
extern const unsigned char _blue_buttons_round_edge_icon_button[];
extern const unsigned char _blue_buttons_round_edge_icon_button_pressed[];
extern const unsigned char _blue_buttons_round_icon_button[];
extern const unsigned char _blue_buttons_round_icon_button_pressed[];
extern const unsigned char _blue_icons_back_arrow_32[];
extern const unsigned char _blue_icons_next_arrow_32[];
extern const unsigned char _cancel[];
extern const unsigned char _cancel_pressed[];
extern const unsigned char _image00[];
extern const unsigned char _mn[];
extern const unsigned char _modal_background[];
extern const unsigned char _ok[];
extern const unsigned char _ok_pressed[];

const touchgfx::Bitmap::BitmapData bitmap_database[] =
{
    { _bk2, 0, 480, 272, 0, 0, 480, 272, touchgfx::Bitmap::RGB565 },
    { _blue_buttons_round_edge_icon_button, 0, 60, 60, 7, 6, 46, 46, touchgfx::Bitmap::ARGB8888 },
    { _blue_buttons_round_edge_icon_button_pressed, 0, 60, 60, 7, 6, 46, 46, touchgfx::Bitmap::ARGB8888 },
    { _blue_buttons_round_icon_button, 0, 60, 60, 13, 11, 34, 36, touchgfx::Bitmap::ARGB8888 },
    { _blue_buttons_round_icon_button_pressed, 0, 60, 60, 13, 11, 34, 36, touchgfx::Bitmap::ARGB8888 },
    { _blue_icons_back_arrow_32, 0, 17, 30, 3, 11, 4, 8, touchgfx::Bitmap::ARGB8888 },
    { _blue_icons_next_arrow_32, 0, 17, 30, 10, 11, 4, 8, touchgfx::Bitmap::ARGB8888 },
    { _cancel, 0, 111, 57, 0, 0, 111, 57, touchgfx::Bitmap::RGB565 },
    { _cancel_pressed, 0, 111, 57, 0, 0, 111, 57, touchgfx::Bitmap::RGB565 },
    { _image00, 0, 100, 100, 0, 0, 100, 100, touchgfx::Bitmap::RGB565 },
    { _mn, 0, 480, 272, 0, 0, 480, 272, touchgfx::Bitmap::RGB565 },
    { _modal_background, 0, 280, 180, 0, 0, 280, 180, touchgfx::Bitmap::RGB565 },
    { _ok, 0, 112, 58, 0, 0, 112, 58, touchgfx::Bitmap::RGB565 },
    { _ok_pressed, 0, 112, 58, 0, 0, 112, 58, touchgfx::Bitmap::RGB565 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}
uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
}

