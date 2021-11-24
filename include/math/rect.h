#ifndef MOTOR_RECT_H
#define MOTOR_RECT_H

typedef struct {
    int left, top, width, height;
} rect;

rect rect_new(int l, int t, int w, int h);

#endif // MOTOR_RECT_H