# Luis Fernando Antunes - Trabalho 1 de Computação Gráfica - "Photoshop" Caseiro

## Observações

O projeto foi desenvolvido em conformidade com as especificações, utilizando como base a pasta Demos fornecida pelo professor. Para garantir o funcionamento correto, o programa deve ser executado dentro desta pasta, pois ela contém as bibliotecas necessárias (DLLs) e os diretórios include e lib.

Todos os requisitos obrigatórios foram implementados, além de funcionalidades extras conforme detalhado abaixo.

## Funcionalidades Obrigatórias Implementadas

1. **Sistema de Camadas:**
   - Adição/remoção dinâmica de camadas
   - Reordenação de camadas (mover para cima/baixo)
   - Controle de visibilidade (checkbox)
   - Indicação visual da camada ativa
2. **Manipulação de Imagens:**
   - Carregamento de imagens BMP geneéricas (a.bmp, b.bmp, c.bmp)
   - Flip horizontal e vertical
   - Ajuste de brilho (slider)
   - Efeitos cumulativos (aplicação múltipla preservada na renderização)
3. **Ferramentas de Desenho:**

- Pincel com tamanho ajustável (slider)
  - Troca de cores:
    - Teclas: 1 (Vermelho) | 2 (Verde) | 3 (Azul)
    - Seletor de cores (color-picker) no painel inferior direito
  - Desenho com transparência binária sobre imagens

1. **Interface:**
   - Painel lateral com botões, sliders e checkboxes funcionais
   - Fundo quadriculado para visualização de transparência
   - Lista de camadas interativa

## Funcionalidades Opcionais Implementadas

1. Tons de cinza (conversão automática)
2. Ajuste de contraste (slider)
3. Ajuste de saturação (slider)
5. Sistema de posicionamento manual de imagens

## Manual de Uso

1. **Carregamento de Imagens**:
   - Clique em "Carregar Imagem" e selecione uma das imagens pré-carregadas (a.bmp, b.bmp, c.bmp)
   - Posicione a imagem clicando no canvas
   - É possivel adicionais outras imagens dentro da pasta images, o projeto funciona para imagens genéricas
   - Apenas uma imagem por camada é permitida
1. **Gerenciamento de Camadas**:
   - Adicionar/Remover: Use os botões dedicados no painel
   - Ordem de renderização:
   - Mover p/ cima: Aumenta prioridade
   - Mover p/ baixo: Reduz prioridade
   - Controle de visibilidade: Marque/desmarque a checkbox ao lado de cada camada
   - Limpeza: Limpar Camada remove todos os desenhos e efeitos da camada ativa
1. **Ferramentas**:
   - Pincel:
     - Clique + arraste para desenhar
     - Ajuste o tamanho com o slider Brush Size
   - Cores:
     - Teclas rápidas (1/2/3) ou seleção manual no color-picker
3. **Efeitos de Imagem**:
   - Ajuste os sliders de brilho, contraste e saturação para aplicar efeitos na imagem da camada ativa atual.
   - O botão "Tons de Cinza" converte a imagem para tons de cinza
   - Flip: Use os botões de flip horizontal/vertical
   - Reset: Resetar Efeitos restaura a imagem original
