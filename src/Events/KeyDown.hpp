#pragma once

struct KeyDown final
{
    inline KeyDown(int key_code)
        : keycode(key_code)
    {
    }
    int keycode = -1; // defaults to invalid -1
};