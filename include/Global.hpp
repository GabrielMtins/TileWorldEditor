#ifndef GLOBAL_HPP
#define GLOBAL_HPP


#define CGLM_NO_ANONYMOUS_STRUCT
#include <cglm/struct.h>

#include <utility>
#include <imgui.h>
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

#define IMVEC2_TO_VEC2S(v) vec2s{(v).x, (v).y}
#define POINT2I_TO_VEC2S(v) vec2s{(float) (v).first, (float) (v).second}

#define VEC2S_TO_POINT2I(v) Point2i(int((v).raw[0]), int((v).raw[1]))

#include "EditMode.hpp"

using Point2i = std::pair<int, int>;

#endif
