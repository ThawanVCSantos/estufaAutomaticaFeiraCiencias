// Só define caso ainda não esteja definido, evita múltiplas inclusões

#ifndef AnimationDirection_h
#define AnimationDirection_h


// Enum para definir as direções da animação de rolagem
// Utilizado como parâmetro para o método de animação de rolagem no println da classe Display,
// que estende a classe LiquidCrystal.

enum class AnimationDirection {
  LEFT_SCROLLING,  // Animação rolando para a esquerda
  RIGHT_SCROLLING  // Animação rolando para a direita
};

// Fim da proteção contra múltiplas inclusões

#endif
