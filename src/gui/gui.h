#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/base/base.h"

Maybe main_window_open();
Maybe main_window_loop_start();
Maybe main_window_close();

/// converts hex color to float vector
#define RGBAHexToF(R8,G8,B8,A8) ImVec4(((u8)35)/255.0f, ((u8)35)/255.0f, ((u8)50)/255.0f, ((u8)255)/255.0f)
/// converts float vector to hex color
#define RGBAFToHex(VEC4) {(u8)(VEC4.x*255), (u8)(VEC4.y*255), (u8)(VEC4.z*255), (u8)(VEC4.w*255)}

#if __cplusplus
}
#endif
