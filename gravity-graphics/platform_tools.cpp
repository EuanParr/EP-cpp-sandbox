#include "platform_tools.h"

namespace platform_tools
{
    struct KeyState // will be used to improve key event system
    {
        bool bIsDown;
        bool bChangedThisFrame;
    };


}
