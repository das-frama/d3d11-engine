#include "math/rect.h"

rect rect_new(int l, int t, int w, int h) {
    rect r = { l, t, w, h };

    return r;
}