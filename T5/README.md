# Luis Fernando Antunes - Trabalho 5 de Computação Gráfica - Guerra nas estrelas OpenGL

## Observações

O projeto foi desenvolvido em conformidade com as especificações, utilizando como base a pasta Demos fornecida pelo professor. Para garantir o funcionamento correto, o programa deve ser executado dentro desta pasta, pois ela contém as bibliotecas necessárias (DLLs) e os diretórios include e lib.

Todos os requisitos obrigatórios foram implementados, além de funcionalidades extras conforme detalhado abaixo.

## Funcionalidades Obrigatórias Implementadas

- Sistema de Câmera/Nave Espacial:
  - Movimentação suave baseada em vetores (posição, direção e up)
  - Controles completos de rotação (pitch, yaw, roll)
  - Movimento em 6 eixos (frente/trás, esquerda/direita, cima/baixo)
- Ambiente Espacial:
  - Geração procedural de campo de asteroides
  - Renderização de estrelas no fundo como elementos de background
  - Sistema de iluminação com luz direcional
- Modelagem de Asteroides:
  - Diferentes tipos de modelos (esfera, cubo, torus, bule, icosaedro)
  - Variação aleatória de tamanho, cor e velocidade de rotação
  - Detecção de colisão na geração para evitar sobreposição
- Sistemas de Renderização:
  - Alternância entre wireframe e preenchimento de polígonos
  - Iluminação com vetores normais corretos para todos os modelos

## Funcionalidades Opcionais Implementadas

- Sistema de Level of Detail (LOD):
  - Três níveis de detalhe para modelos complexos (esfera, torus)
  - Transição suave baseada na distância da câmera

- Diversidade de Modelos:
  - Cinco tipos diferentes de asteroides com características visuais distintas
  - Variação procedural de cores para maior realismo

- Interface HUD:
  - HUD com informações em tempo real (FPS, posição, modo de renderização)
  - Crosshair central para auxiliar na navegação

## Manual de Uso

### Controles da Nave/Câmera

- Movimento:
  - W/S: Movimento para frente/trás
  - A/D: Movimento para esquerda/direita
  - Q/E: Movimento para cima/baixo

- Rotação:
  - I/K: Rotação para cima/baixo (pitch)
  - J/L: Rotação para esquerda/direita (yaw)
  - U/O: Inclinação lateral (roll)

- Teclas de Ação:
  - F: Alternar entre modo wireframe e sólido
  - H: Mostrar/esconder HUD
  - R: Resetar posição e orientação da câmera
  - ESC: Sair do programa

### Informações do HUD

- O HUD exibe:
  - FPS atual
  - Modo de renderização atual (Wireframe/Solid)
  - Posição atual da câmera (coordenadas x, y, z)
  - Contagem de asteroides na cena
  - Controles básicos (teclas de atalho)
