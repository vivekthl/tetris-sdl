#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <cstdlib>
#include <SDL2/SDL.h>


#include "Matrix.h"
#include "Tetromino.h"
#include "Common.h"
#include <iostream>
#include "Music.h"

namespace Tetris
{
    class Game
    {
    public:
        Game();
        void start();        

        enum class State
        {
            MENU = 0,
            GAME = 1,
            GAME_OVER = 2
        };


        template<State MyState>
        struct StateTag
        {

        };
                
        
    private:



        
        void init();
        void run();


//        template<State MyState>
//        void runImpl(const StateTag<MyState>&){}
        
        void runImpl(const StateTag<State::MENU>&);
        void runImpl(const StateTag<State::GAME>&);
        void runImpl(const StateTag<State::GAME_OVER>&);        
        
        void exit();
        Tetromino generateRandomTertomino() const;
        void addTetromino();
        void moveTetromino(Tetromino::Action action_);

        SDL_Window*                           _window;
        SDL_Renderer*                         _renderer;
        Position                              _tetrominoPosition;
        unsigned                              _tetrominoCurrentRow;
        unsigned                              _tetrominoCurrentColumn;
        Tetromino                             _tetromino;
        Matrix                                _matrix;
        unsigned                              _score;
        Music                                 _music;
        State                                 _state;
    };

}

#endif
