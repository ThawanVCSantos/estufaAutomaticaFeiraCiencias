// Inclui as bibliotecas do Arduino e LiquidCrystal
// Inclui também o Display, cuja implementação está neste arquivo

#include "Arduino.h"
#include "Display.h"
#include <LiquidCrystal.h>

// Construtor do Display, utilizado para construir a classe mãe LiquidCrystal
Display::Display(int rs, int en, int d4, int d5, int d6, int d7) : LiquidCrystal(rs, en, d4, d5, d6, d7) {

  // Inicia as 'linhas' de lastText como vazias, utilizo de strcpy para copiar o valor, pois a atribuição
  // direta ao array (lastText[0] = "                "), não é permitida
  strcpy(lastText[0], "                ");
  strcpy(lastText[1], "                ");
}


// Função que escreve o valor no display

void Display::println(char firstLineText[17], char secondLineText[17], int animationAux) {

  // Verifica se não há animação, caso o texto da 1° linha seja igual o texto anterior da 1° e
  // o texto da 2° linha seja igual o texto anterior da 2°, retorna, para evitar multiplos ticks iguais

  // Não faz isso caso haja animação, pois como tem um scroll, vai ter tick de qualquer forma na animação,
  // o que não é um problema

  // O strcmp é utilizado para realmente comparar o valor das strings e não o ponteiro das váriaveis, resolve um problema comum
  // de várias linguagens quando se compara arrays ou strings, e como as váriaveis são arrays de caracteres,
  // é provavel de acontecer esses problemas, essa função funciona de forma similiar ao .equals do java

  if(animationAux == 0 && strcmp(lastText[0], firstLineText) == 0 && strcmp(lastText[1], secondLineText) == 0) return;

  // Limpa o display caso não haja animação de scroll no print, pois a animação começa movendo o texto antigo

  if(animationAux == 0) clear();

  // Variável auxiliar calcular a posição central para o texto, na primeira linha

  int centralizerAux = (16 - strlen(firstLineText)) / 2;

  // Define a posição do cursor para centralizar o texto, na primeira linha

  setCursor(centralizerAux + animationAux, 0);

  // Exibe a primeira linha do display

  print(firstLineText);

  // Variável auxiliar calcular a posição central para o texto, na segunda linha

  centralizerAux = (16 - strlen(secondLineText)) / 2;

  // Define a posição do cursor para centralizar o texto, na segunda linha

  setCursor(centralizerAux + animationAux, 1);

  // Exibe a segunda linha do display

  print(secondLineText);

  // Caso não haja animação, salva o valor do último texto para evitar múltiplos ticks de textos iguais.
  // Caso o contrário, não salva o último valor, pois a função de animação se divide em duas etapas,
  // Primeiro faz a animação, porém mantém o texto antigo, logo após a função de print é chamada de novo,
  // porém sem animar novamente, serve apenas para limpar o display e manter o texto.
  // Sendo assim, limpa o valor do último texto, para garantir que não seja executado novamente, devido à
  // proteção contra múltiplos ticks de textos iguais

  if(animationAux == 0) {
    strcpy(lastText[0], firstLineText);
    strcpy(lastText[1], secondLineText);
  } else {
    strcpy(lastText[0], "                ");
    strcpy(lastText[1], "                ");
  }
}

// Função que escreve o valor no display, ativando a animação de scroll

void Display::println(char firstLineText[17], char secondLineText[17], AnimationDirection animationDirection) {

  // Chama a outra função println, mandando o animationAux a depender do valor de animationDirection
  // Utiliza um operador ternário (espécie de if else, inline) que determina o valor do parametro animation aux no print

  println(firstLineText, secondLineText, animationDirection == AnimationDirection::LEFT_SCROLLING ? 24 :
    animationDirection == AnimationDirection::RIGHT_SCROLLING ? 16 : 0);

  // Realiza o scroll do texto para a esquerda ou direita, movendo 24 caracteres com pausas de 75 milissegundos para maior fluidez

  if(animationDirection == AnimationDirection::LEFT_SCROLLING) {
    for (int pos = 0; pos < 24; pos++){
      scrollDisplayLeft();
      delay(75);
    }
  } else if (animationDirection == AnimationDirection::RIGHT_SCROLLING) {
    for (int pos = 0; pos < 24; pos++){
      scrollDisplayRight();
      delay(75);
    }
  }

  println(firstLineText, secondLineText);
}

// Função para animar uma Introdução da Escola

void Display::intro() {

  println("EEJBH", "EM. DESEN. SIST.");

  // Pausa que mostra a primeira parte da introdução durante 1,5 segundos

  delay(1500);

  println("FEIRA DE", "CIENCIAS");

  // Pausa que mostra a segunda parte da introdução durante 1,5 segundos

  delay(1500);
}

// Função para exibir uma animação de loading

void Display::onLoading(char firstLineText[17], char loadingText[17]) {

  // Variáveis estáticas para manter o estado entre as chamadas da função

  // Váriavel auxiliar que mantém a quantidade de pontos

  static String dotAux = "";

  // Tempo Anterior de atualização do loading

  static unsigned long previousTimer = 0;

  // Intervalo para atualização da animação de loading

  const int loadingInterval = 500;

  // Obtém o instante atual

  unsigned long currentTimer = millis();
  
  // Verifica se o intervalo de atualização foi alcançado
  if (currentTimer - previousTimer >= loadingInterval) {
    // Atualiza o timer

    previousTimer = currentTimer;

    // Verifica se há 3 pontos, caso haja apaga os mesmos
    // Caso o contrário adiciona outro ponto à sequência

   if (dotAux.length() >= 3) {
      dotAux = "";
    } else {
      dotAux += ".";
    }

    // Variável auxiliar para concatenação simples

    char loadingAux[17];

    // Transforma o texto loadingText em String para que a concatenação e incrementação do dotAux seja mais simples
    // Logo após transforma em charArray novamente e salva o valor na váriavel loadingAux com tamanho do charArray igual a 17

    (String(loadingText) + dotAux).toCharArray(loadingAux, 17);

    // Printa o Loading na tela

    println(firstLineText, loadingAux);
  }
}
