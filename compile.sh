#!/bin/bash

# Configurações
EXECUTABLE="./editor"
SRC_DIR="./src"
INCLUDE_DIR="./include"
OBJ_DIR="./obj" # Novo diretório para objetos
LIBS="-lGL -lGLU -lglut -lm"

# Cria diretório de objetos se não existir
mkdir -p "$OBJ_DIR"

# Limpeza
echo "Limpando compilações anteriores..."
rm -f "$OBJ_DIR"/*.o "$EXECUTABLE"

# Lista de arquivos para compilar
SOURCES=(
    "main.cpp"
    "Vector2.cpp"
    "gl_canvas2d.cpp"
    "Bmp.cpp"
    "Layer.cpp"
    "LayerManager.cpp"
    "ImageEditor.cpp"
    "Button.cpp"
    "UIManager.cpp"
    "Checkbox.cpp"
    "ImageLoaderUI.cpp"
)

# Compilação
echo "Compilando..."
for source in "${SOURCES[@]}"; do
    base_name=$(basename "$source")
    obj_name="${base_name%.*}.o"
    echo "- Compilando $source"
    g++ -c -I"$INCLUDE_DIR" "$SRC_DIR/$source" -o "$OBJ_DIR/$obj_name" || exit 1
done

# Linking
echo "Linkando..."
g++ -o "$EXECUTABLE" "$OBJ_DIR"/*.o $LIBS || {
    echo "Erro no linking"
    exit 1
}

# Execução
if [ -f "$EXECUTABLE" ]; then
    echo "Executando..."
    "$EXECUTABLE"
else
    echo "Falha na compilação!"
    exit 1
fi
