#ifndef COLORS_H
#define COLORS_H

#include "gl_canvas2d.h"

class Colors
{
  public:
    // Cores base - Paleta moderna
    static void black() { CV::color(0.10f, 0.10f, 0.12f); }
    static void darkGray() { CV::color(0.20f, 0.20f, 0.22f); }
    static void gray() { CV::color(0.40f, 0.40f, 0.45f); }
    static void lightGray() { CV::color(0.75f, 0.75f, 0.78f); }
    static void white() { CV::color(0.95f, 0.95f, 0.97f); }

    // Cores primárias - Tons mais vibrantes
    static void red() { CV::color(0.92f, 0.25f, 0.30f); }
    static void green() { CV::color(0.22f, 0.78f, 0.48f); }
    static void blue() { CV::color(0.20f, 0.52f, 0.95f); }

    // Cores secundárias - Mais saturadas
    static void orange() { CV::color(0.98f, 0.60f, 0.25f); }
    static void yellow() { CV::color(0.98f, 0.90f, 0.35f); }
    static void cyan() { CV::color(0.25f, 0.85f, 0.90f); }
    static void magenta() { CV::color(0.85f, 0.32f, 0.72f); }
    static void purple() { CV::color(0.58f, 0.28f, 0.82f); }

    // Cores terciárias refinadas
    static void olive() { CV::color(0.48f, 0.52f, 0.28f); }
    static void brown() { CV::color(0.52f, 0.35f, 0.25f); }
    static void teal() { CV::color(0.20f, 0.60f, 0.58f); }
    static void coral() { CV::color(0.98f, 0.50f, 0.45f); }

    // Sistema de UI - Tema escuro com acentos
    static void uiBackground() { CV::color(0.15f, 0.15f, 0.18f); }
    static void uiPanel() { CV::color(0.22f, 0.22f, 0.28f); }
    static void uiText() { CV::color(0.92f, 0.92f, 0.94f); }
    static void uiTextHighlight() { CV::color(0.98f, 0.85f, 0.35f); }
    static void uiButton() { CV::color(0.32f, 0.38f, 0.55f); }
    static void uiButtonActive() { CV::color(0.30f, 0.70f, 0.92f); }

    // Elementos do tanque - Design metálico moderno
    static void tankBase() { CV::color(0.38f, 0.45f, 0.32f); }
    static void tankTurret() { CV::color(0.48f, 0.42f, 0.32f); }
    static void tankTracks() { CV::color(0.25f, 0.25f, 0.25f); }
    static void tankOutline() { CV::color(0.15f, 0.15f, 0.15f); }
    static void projectile() { CV::color(0.98f, 0.70f, 0.25f); }

    // Sistema de alvos - Cores mais vivas
    static void targetHealthy() { CV::color(0.30f, 0.85f, 0.42f); }
    static void targetDamaged() { CV::color(0.98f, 0.85f, 0.35f); }
    static void targetCritical() { CV::color(0.98f, 0.42f, 0.35f); }
    static void targetShield() { CV::color(0.35f, 0.65f, 0.92f, 0.6f); }

    // Pista e ambiente - Cores realistas de estrada
    static void roadCenterMarking() { CV::color(0.88f, 0.88f, 0.90f); }
    static void roadEdgeMarking() { CV::color(0.88f, 0.88f, 0.90f); }
    static void trackInner() { CV::color(0.98f, 0.98f, 0.98f); }
    static void trackOuter() { CV::color(0.98f, 0.98f, 0.98f); }
    static void trackCenter() { CV::color(0.88f, 0.88f, 0.90f); }
    static void roadSurface() { CV::color(0.30f, 0.30f, 0.35f); } // Asfalto médio
    static void roadCenterLine() { CV::color(0.95f, 0.95f, 0.40f); } // Linha central amarela
    static void roadEdgeLine() { CV::color(0.95f, 0.95f, 0.95f); } // Linhas de borda brancas
    static void roadShoulder() { CV::color(0.45f, 0.42f, 0.38f); } // Acostamento

    // Fundos temáticos - Mais impacto visual
    static void backgroundDefault() { CV::clear(0.18f, 0.20f, 0.25f); } // Azul escuro
    static void backgroundNight() { CV::clear(0.08f, 0.10f, 0.18f); } // Azul noturno
    static void backgroundDesert() { CV::clear(0.72f, 0.62f, 0.48f); } // Areia
    static void backgroundSpace() { CV::clear(0.05f, 0.06f, 0.12f); } // Espaço profundo

    static void controlPointAnchor() { CV::color(1.0f, 0.5f, 0.0f); } // Laranja
    static void controlPointHandle() { CV::color(0.0f, 0.8f, 1.0f); } // Azul claro
    static void controlPointLabel() { CV::color(1.0f, 1.0f, 1.0f); } // Branco
    static void controlLine() { CV::color(0.7f, 0.7f, 0.7f); }       // Cinza claro

  private:
    // Evita instanciação
    Colors() = delete;
};
#endif
