#include "Game.h"
#include "TetrisPainter.h"

#include <iostream>
#include <random>

namespace Tetris
{
    Game::Game()
        :_window(NULL),
         _renderer(NULL),
         _tetromino(Tetromino::Shape::I,
                    Color(220,255,220)),
         _matrix(Matrix()),
         _score(0),
         _music(),
         _state(State::MENU)
    {}

    void Game::start()
    {
        init();
        run();
        exit();
    }

    void Game::init()
    {        
        _window = SDL_CreateWindow(
            "Game", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        _renderer = 
            SDL_CreateRenderer(_window, 0, SDL_RENDERER_ACCELERATED);
        
        SDL_SetRenderDrawColor(_renderer,
                               BG_COLOR_RED,
                               BG_COLOR_GREEN,
                               BG_COLOR_BLUE,
                               255);
        
        SDL_RenderClear(_renderer);
        
        
        _music.init();
    }


    void Game::runImpl(const StateTag<State::MENU>&)
    {
        std::cout  << "MENU STATE" << std::endl;

        TetrisPainter tetrisPainter(_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        tetrisPainter.clearScreen();

        tetrisPainter.drawText("Tetris",
                               30,
                               Cordinate(SCREEN_WIDTH/2 - 40, 60),
                               Color(192,163,95));        

        tetrisPainter.drawText("Press space to continue...",
                               14,
                               Cordinate(SCREEN_WIDTH/2 - 70, 100),
                               Color(192,163,95)); 
        
        
        SDL_Event event;
        bool quit = false;
        while(!quit)
        {
            while(SDL_PollEvent(&event) != 0)
            {
                switch(event.type)
                {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                    case SDL_QUIT:
                    {
                        quit = true;
                        break;
                    }
                    case SDLK_SPACE:
                    {
                        _state = State::GAME;
                        runImpl(StateTag<State::GAME>());
                        break;
                    }
                    }
                    break;
                }

                if(SDL_QUIT == event.type)
                {
                    quit = true;
                }
            }
        }
    }

    void Game::runImpl(const StateTag<State::GAME>&)
    {
        
        //Game loop starts here
        SDL_Event event;
        bool quit = false;

        Position tetrominoStartPosition(0, SCREEN_WIDTH/SQUARE_SIZE/2);
        _tetrominoPosition = tetrominoStartPosition;
        _tetromino = generateRandomTertomino();

        TetrisPainter tetrisPainter(_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        tetrisPainter.clearScreen();
                
        addTetromino();
        Tetromino nextTetromino = generateRandomTertomino();
           
        
//        tetrisPainter.drawPanel(_score,nextTetromino);
        while(!quit)
        {
            if(_matrix.hasTetrominoHitBottom(_tetromino,_tetrominoPosition))
            {
                _matrix.addTetrominoToHeap(_tetromino,_tetrominoPosition);
                unsigned noOfRowsDeleted = _matrix.deletePackedRows();
                _score += noOfRowsDeleted;
                _tetrominoPosition = tetrominoStartPosition;
                _tetromino = nextTetromino;
                addTetromino();
                nextTetromino = generateRandomTertomino();
//                tetrisPainter.drawPanel(_score,nextTetromino);
                _music.playSound(Sound::FORCE_HIT);
                if(noOfRowsDeleted > 0)
                {
                    _music.playSound(Sound::LINE_REMOVAL);
                }
            }

            if(_matrix.hasTetrominoHitHeap(_tetromino,_tetrominoPosition))
            {
                _matrix.addTetrominoToHeap(_tetromino,_tetrominoPosition);
                unsigned noOfRowsDeleted = _matrix.deletePackedRows();
                _score += noOfRowsDeleted;
                _tetrominoPosition = tetrominoStartPosition;
                _tetromino = nextTetromino;
                addTetromino();
                nextTetromino = generateRandomTertomino();
//                tetrisPainter.drawPanel(_score,nextTetromino);
                _music.playSound(Sound::FORCE_HIT);
                if(noOfRowsDeleted > 0)
                {
                    _music.playSound(Sound::LINE_REMOVAL);
                }
            }
//            std::cout << "SCORE: " << _score << std::endl;
/*
            pauseTime(300);                                 
            _tetrominoPosition._row++;
*/
            while(SDL_PollEvent(&event) != 0)
            {
                switch(event.type)
                {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                    case SDL_QUIT:
                        quit = true; break;
                    case SDLK_UP:
                        moveTetromino(Tetromino::Action::ROTATE);
                        _music.playSound(Sound::ROTATE);
                        break;
                    case SDLK_LEFT:
                        moveTetromino(Tetromino::Action::MOVE_LEFT);
                        break;
                    case SDLK_RIGHT:
                        moveTetromino(Tetromino::Action::MOVE_RIGHT);
                        break;
                    case SDLK_DOWN:
                        moveTetromino(Tetromino::Action::MOVE_DOWN);
                    }
                    break;
                }

                if(SDL_QUIT == event.type)
                {
                    quit = true;
                }
            }
        }
    }

    void Game::runImpl(const StateTag<State::GAME_OVER>&)
    {
        //todo
    }
    

    void Game::run()
    {
        runImpl(StateTag<State::MENU>());
    }

    void Game::exit()
    {
        SDL_DestroyWindow(_window);
        _music.exit();
        TTF_Quit();
        SDL_Quit();  
    }

    Tetromino Game::generateRandomTertomino() const
    {
        size_t tetrominoShapeIndex = rand() % NO_OF_TETROMINO_SHAPES;

        std::array<std::tuple<unsigned, unsigned, unsigned>, 3> colorTable =
        {
            std::make_tuple(230, 230, 230), // light green
            std::make_tuple(160, 160, 160), // light blue
            std::make_tuple(90,90,90), // light red            
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, colorTable.size() - 1);
        unsigned colorTableIndex = dis(gen);

        std::cout << "colorTableIndex : " << colorTableIndex << std::endl;
        
        return Tetromino(static_cast<Tetromino::Shape>(tetrominoShapeIndex),
                         Color(std::get<0>(colorTable[colorTableIndex]),
                               std::get<1>(colorTable[colorTableIndex]),
                               std::get<2>(colorTable[colorTableIndex])));
    }
    
    void Game::addTetromino()
    {
        _matrix.addTetromino(_tetromino,_tetrominoPosition);
        TetrisPainter tetrisPainter(_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        tetrisPainter.drawMatrix(_matrix);
    }

    void Game::moveTetromino(Tetromino::Action action_)
    {
        //Calculate new position of tetromino based on action
        switch(action_)
        {
        case Tetromino::Action::ROTATE:
            if(_matrix.isTetrominoMovable(_tetromino,
                                          Tetromino::Action::ROTATE,
                                          _tetrominoPosition))
            {
                _matrix.deleteTetrmino(_tetromino,_tetrominoPosition);
                _tetromino.rotate();
                addTetromino();
            }
            break;
        case Tetromino::Action::MOVE_LEFT:
            if(_matrix.isTetrominoMovable(_tetromino, 
                                         Tetromino::Action::MOVE_LEFT,
                                          _tetrominoPosition))
            {
                _matrix.deleteTetrmino(_tetromino,_tetrominoPosition);
                _tetrominoPosition._column--;
                addTetromino();
            }
            break;
        case Tetromino::Action::MOVE_RIGHT:
            if(_matrix.isTetrominoMovable(_tetromino,
                                          Tetromino::Action::MOVE_RIGHT,
                                          _tetrominoPosition))
            {
                _matrix.deleteTetrmino(_tetromino,_tetrominoPosition);
                _tetrominoPosition._column++;
                addTetromino();
            }
            break;
        case Tetromino::Action::MOVE_DOWN:
            if(_matrix.isTetrominoMovable(_tetromino,
                                          Tetromino::Action::MOVE_DOWN,
                                          _tetrominoPosition))
            {
                _matrix.deleteTetrmino(_tetromino,_tetrominoPosition);
                _tetrominoPosition._row++;
                addTetromino();
            }
        }
    }
}
