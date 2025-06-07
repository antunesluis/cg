#ifndef KEYCODES_H
#define KEYCODES_H

struct Key
{
    struct Letter
    {
        static constexpr int A = 97;
        static constexpr int D = 100;
        static constexpr int E = 101;
        static constexpr int W = 119;
        static constexpr int S = 115;
    };

    struct Number
    {
        static constexpr int ZERO = 48;
        static constexpr int ONE = 49;
    };

    struct Special
    {
        static constexpr int ESC = 27;
        static constexpr int SPACE = 32;
    };

    struct Arrow
    {
        static constexpr int UP = 101;    // GLUT_KEY_UP
        static constexpr int DOWN = 103;  // GLUT_KEY_DOWN
        static constexpr int LEFT = 100;  // GLUT_KEY_LEFT
        static constexpr int RIGHT = 102; // GLUT_KEY_RIGHT
    };
};

#endif // KEYCODES_H
