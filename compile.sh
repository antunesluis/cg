#!/bin/bash

# Nome do executável final
EXECUTABLE="./texture"
# Diretórios
SRC_DIR="./src"
INCLUDE_DIR="./include"

# Limpa compilações anteriores
rm -f *.o $EXECUTABLE

# Compila todos os arquivos .cpp
g++ -c -I"$INCLUDE_DIR" "$SRC_DIR/main.cpp" -o main.o
g++ -c -I"$INCLUDE_DIR" "$SRC_DIR/gl_canvas2d.cpp" -o gl_canvas2d.o
g++ -c -I"$INCLUDE_DIR" "$SRC_DIR/bmp.cpp" -o bmp.o
# g++ -c -I"$INCLUDE_DIR" "$SRC_DIR/Layer.cpp" -o Layer.o
g++ -c -I"$INCLUDE_DIR" "$SRC_DIR/ImageEditor.cpp" -o ImageEditor.o

# Linka tudo incluindo a biblioteca matemática
g++ -o "$EXECUTABLE" main.o gl_canvas2d.o bmp.o ImageEditor.o \
    -lGL -lGLU -lglut -lm

# Executa o programa se a compilação foi bem sucedida
if [ -f "$EXECUTABLE" ]; then
    "$EXECUTABLE"
else
    echo "Falha na compilação"
fi
