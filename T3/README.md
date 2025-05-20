# Luis Fernando Antunes - Trabalho 3 de Computação Gráfica - Grand Theft Auto 2D

## Observações

O projeto foi desenvolvido em conformidade com as especificações, utilizando como base a pasta Demos fornecida pelo professor. Para garantir o funcionamento correto, o programa deve ser executado dentro desta pasta, pois ela contém as bibliotecas necessárias (DLLs) e os diretórios include e lib.

Todos os requisitos obrigatórios foram implementados, além de funcionalidades extras conforme detalhado abaixo.

## Funcionalidades Obrigatórias Implementadas

1. **Pista usando Bzier:**
    - Modo interativo para criação de pistas com pontos de controle.
    - Renderização: Pista com preenchimento (asfalto cinza), bordas marcadas e linhas centrais tracejadas
    - O jogo inicia com uma pista padrão circular, mas novas pistas podem ser criadas no modo editor.

2. **Tanque Controlável**:
    - Movimentação Física:
        - Tecla A: Rotaciona para a esquerda (movimento circular contínuo).
        - Tecla D: Rotaciona para a direita.
        - Movimento automático para frente (velocidade constante).
    - Torre Independente: Aponta para o cursor do mouse (botão esquerdo para atirar).
    - Barra de Vida: Exibida abaixo do tanque (dano por colisão com bordas/alvos).

3. **Sistema de Tiro e Alvos**:
    - **Projéteis**:
        - Disparo com cooldown (delay de 0.3s entre tiros).
        - Colisão com pista (explosão ao atingir bordas) e alvos.
    - **Alvos Móveis**:
        - Movimento ao longo da pista com velocidade e vida aleatórias.
        - Destruição gera pontuação (+10 pontos) e efeito de explosão.

4. **Colisões**:
    - Tanque vs Pista: Empurrão físico ao colidir com bordas (baseado em normal e penetração).
    - Tanque vs Alvos: Dano recíproco (tanque: -2 HP, alvo: -5 HP).
    - Projéteis vs Pista: Colisão dos projéteis com bordas (explosão).

5. **Interface e Controles**:
    - Estados do Jogo: Menu inicial, modo editor, gameplay, tela de game over.
    - **UI**:
        - Placar (score), onda atual (wave), FPS.
        - Instruções contextuais (ex.: teclas no modo editor).

## Funcionalidades Opcionais Implementadas

1. **Dificuldade Progressiva**:
    - A cada 10 alvos destruídos, nova onda inicia.
    - A cada onda (wave):
        - Aumento de 20% na velocidade e vida dos alvos.
        - Quantidade de alvos: 10 + (wave * 2).
    - Indicador visual de onda atual

2. **Sistema de Partículas**:
    - Explosões com partículas coloridas (laranja/amarelo) para:
        - Tiros na pista (pequena).
        - Colisão tanque-alvo (média).
        - Destruição de alvos (grande).

3. **Detalhes da Pista**:
    - Preenchimento colorido (cinza escuro para asfalto, linhas amarelas tracejadas).

4. **Cores e Texturas**:
    - Todas as estruturas e entidades possuem cores próprias.
    - A cor dos inimigos muda conforme a sua vida.

5. **Movimentação dos inimigos**:
    - Inimigos se movem ao longo da pista, seguindo a tragetória da linha central da pista.
    - Possuem velocidades e sentido diferentes definidos aleatoriamente a partir de um intervalo.

## Manual de Uso

### **Criação de Pistas (modo editor):**

- Novas pistas podem ser definidas no menu utilizando a tecla 2.
- Para a criação de novas pistas é necessário definir pontos de controle clicando na tela.
- O primeiro e ultimo ponto de controle irão se conectar, formando um laço. Ou seja, idealmente as pistas irão apresentar formatos fechados.
- Pistas com maior numero de pontos de controle irão gerar curvas mais suaves e pistas mais equilibradas.
- Idealmente as pistas devem ter um pequeno espaçamento entre os pontos de controle, para que a curva não fique muito apertada, seguindo o formato desejado de pista.

### **Controles**

- **Menu Principal**
  - Tecla `1`: Iniciar jogo com pista padrão
  - Tecla `2`: Acessar modo editor
  - Tecla `ESC`: Sair do jogo
- **Modo Editor**
  - **Construção**:
    - `Click Esquerdo`: Adiciona ponto de controle
    - Tecla `Z`: Remove último ponto adicionado
    - Tecla `C`: Limpa toda a pista
    - Tecla `E`: Salva a pista e retorna ao menu
    - Tecla `M`: Sai sem salvar
- **Modo Jogo**
  - **Movimentação**:
    - Tecla `A`: Rotaciona para esquerda
    - Tecla `D`: Rotaciona para direita
    - Movimento automático para frente
  - **Armas**:
    - `Botão Esquerdo`: Dispara projétil
    - Sistema de cooldown (0.3s entre tiros)
  - **Interface**:
    - Tecla `M`: Retorna ao menu principal
    - Tecla `P`: Pausa o jogo
    - Tecla `ESC`: Sai do jogo
