#include "utils/utils.h"

#include <stdlib.h>

const wchar_t* to_wch(const char *c) {
    const size_t c_size = strlen(c) + 1;
    wchar_t* wc = malloc(sizeof(wchar_t) * c_size);
    mbstowcs (wc, c, c_size);

    return wc;
}