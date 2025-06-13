# Luis Fernando Antunes - Trabalho 4 de Computação Gráfica - Geração Objeto 3D com Sweep e Curvas

## Observações

O projeto foi desenvolvido em conformidade com as especificações, utilizando como base a pasta Demos fornecida pelo professor. Para garantir o funcionamento correto, o programa deve ser executado dentro desta pasta, pois ela contém as bibliotecas necessárias (DLLs) e os diretórios include e lib.

Todos os requisitos obrigatórios foram implementados, além de funcionalidades extras conforme detalhado abaixo.

## Funcionalidades Obrigatórias Implementadas

1. **Modelagem com Curvas de Bézier**:
    - Editor interativo para criação de curvas 2D com pontos de controle:
        - Adição/remoção de pontos de controle com clique do mouse.
        - Arrastar para reposicionar pontos existentes.
        - Visualização em tempo real da curva 2D e do objeto 3D gerado.
    - **Renderização:**
        - Curva 2D com linhas ciano e pontos de controle amarelos (vermelho quando selecionado).
        - Objeto 3D gerado por sweep rotacional com base na curva.
2. **Visualização 2D:**
    - Controles de Câmera:
        - Teclas WASD para rotação.
        - Teclas Q/E para zoom.
        - Tecla R/Botão resetar para resetar a câmera.
    - Modos de Exibição:
        - Wireframe (linhas azuis).
        - Superfície preenchida (tons de azul claro).
        - Normais das faces (amarelo, opcional).

## Funcionalidades Opcionais Implementadas

- Exibição do vetores normais de cada face.
  - Checkbox para ativar/desativar a exibição das normais.
- Adição de mais de 4 pontos de controle.
  - Permite adicionar pontos de controle adicionais com o clique do mouse.
- Visualização da curva de bezier e das linhas de controle bases.
  - O projeto é separado em duas telas (2D e 3D), onde a curva de Bézier é visualizada na tela 2D e o objeto 3D gerado por sweep é visualizado na tela 3D.
  - A curva base pode ser visualizada atravez dos pontos de controle, linhas de controle e da curva de Bézier no plano cartesiano.
- Modelo 3D interativo:
  - Controle do número de passos de rotação (voltas do objeto 3D).
  - Controle da exibição das normais, faces e wireframe individualmente.

## Manual de Uso

- Editor de Curvas (Painel Esquerdo)
  - Adicionar Ponto: Clique com botão esquerdo em área vazia.
  - Mover Ponto: Arraste um ponto existente.
  - Remover Ponto: Botão direito no ponto.
  - Controles UI:
    - Reset: Reinicia a curva.
    - Wireframe/Surface/Normals: Alterna modos de visualização 3D.
    - Slider "Voltas": Ajusta a complexidade do objeto 3D.
    - Apagar ultimo ponto: Botão para remover o último ponto adicionado.
    - Reset View: Botão para resetar a visualização do objeto 3D.
- Visualização 3D (Painel Direito)
  - Rotação: Teclas W (cima), S (baixo), A (esquerda), D (direita).
  - Zoom: Teclas Q (aumentar), E (diminuir) ou roda do mouse.
  - Reset: Tecla R.
