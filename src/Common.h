#ifndef TETRIS_COMMON_H
#define TETRIS_COMMON_H

#define NO_OF_TETROMINO_SHAPES 7


#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 600

#define SQUARE_SIZE 20

#define NO_OF_ROWS SCREEN_HEIGHT/SQUARE_SIZE
#define NO_OF_COLUMNS SCREEN_WIDTH/SQUARE_SIZE


#define BG_COLOR_RED 1
#define BG_COLOR_GREEN 1
#define BG_COLOR_BLUE 1


namespace Tetris
{
    struct Color
    {
        Color(unsigned red_,
              unsigned green_,
              unsigned blue_)
            :_red(red_),
             _green(green_),
             _blue(blue_)
        {}
            
        unsigned                      _red;
        unsigned                      _green;
        unsigned                      _blue;
    };

    struct Position
    {
        Position(){}
        Position(int row_, int column_)
            :_row(row_),
             _column(column_)
        {}

        int                                _row;
        int                                _column;
    };

    struct Cordinate
    {
        Cordinate(unsigned x_, unsigned y_)
            : _x(x_),
              _y(y_)
        {}

        unsigned                           _x;
        unsigned                           _y;
    };

//    void pauseTime(unsigned milliSeconds_) const { return SDL_Delay(milliSeconds_);}
}

#endif
