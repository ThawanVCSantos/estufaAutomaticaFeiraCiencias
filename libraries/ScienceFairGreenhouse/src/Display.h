// Só define caso ainda não esteja definido, evita múltiplas inclusões

#ifndef Display_h
#define Display_h

// Inclui a Biblioteca do Arduino e do LCD (LiquidCrystal), a qual será estendida

#include "Arduino.h"
#include "AnimationDirection.h"
#include <LiquidCrystal.h>

// Declara a classe Display que estende a biblioteca LiquidCrystal
class Display : public LiquidCrystal {
   // Define os métodos e variáveis privadas
  private:
    // Será uma váriavel auxiliar para previnir múltiplos ticks de textos iguais, o que além de piscar a tela
    // do display muitas vezes, atrapalhando a visão, gera redundância

    char lastText[2][17];
  // Define os métodos e variáveis públicas
  public:
    // Construtor da classe
    Display(int rs, int en, int d4, int d5, int d6, int d7);

    // Protótipo das funções
    // A função println tem sobrecarga (função com assinaturas diferentes)
    // A primeira versão de println tem um parâmetro opcional animationAux, que é 0 se não houver animação
    // A utilização das funções está melhor detalhada no arquivo cpp

    void println(char firstLineText[17], char secondLineText[17], int animationAux = 0);
    void println(char firstLineText[17], char secondLineText[17], AnimationDirection animationDirection);
    void intro();
    void onLoading(char firstLineText[17], char loadingText[17]);
};

// Fim da proteção contra múltiplas inclusões

#endif
