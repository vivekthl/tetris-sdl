set(TetrisLib_SRCS 
Matrix.cpp
Painter.cpp
Game.cpp
Tetromino.cpp
TetrisPainter.cpp
Music.cpp
Main.cpp
)

add_executable(tetris ${TetrisLib_SRCS})

target_link_libraries(tetris
${SDL2_LIBRARY}
${SDL2_TTF_LIBRARIES}
${SDL2_MIXER_LIBRARY}
)
