namespace platformTools
{
    struct KeyState
    {
        bool bIsDown;
        bool bChangedThisFrame;
    };

    enum
    {
        KEY_UP_ARROW,
        KEYB_DOWN_ARROW,
        KEY_LEFT_ARROW,
        KEY_RIGHT_ARROW,

        NUMBER_OF_KEYS
    };
}
