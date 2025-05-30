#pragma once

struct KeyUp final
{
    inline KeyUp(int key_code)
        : keycode(key_code)
    {
    }
    int keycode = -1; // defaults to invalid -1
};