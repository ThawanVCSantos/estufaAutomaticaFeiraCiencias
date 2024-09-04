// Inclui a biblioteca customizada do projeto

#include <ScienceFairGreenhouse.h>

// Define os pinos que serão utilizados para ligar os botões

#define BTN_MANUAL_PIN 8
#define BTN_AUTO_PIN 9

// Define o pino do sensor de umidade

#define HUMIDITY_SENSOR_PIN A0

// Define o pino do relé que acionará a bomba d'água

#define PUMP_PIN 10

// Limite de umidade. Se a umidade estiver abaixo desse valor, no modo automático, inicia a irrigação.
// Lê o valor analógico do sensor de umidade (0 = mais úmido, 1023 = mais seco).
// Como o sensor mede a resistência elétrica no solo (maior resistência = solo mais seco),
// o valor lido será invertido usando a função map() para que 1023 corresponda a 0% de umidade
// (solo seco) e 0 corresponda a 100% de umidade (solo úmido). Essa conversão é necessária
// para exibir a umidade de forma mais intuitiva na tela.

#define DRY_LIMIT 75

// Tempo (em milissegundos) que ficará irrigando no modo automático (1m)

#define IRRIGATION_DURATION 60000

// Tempo (em milissegundos) que ficará sem irrigar após finalizar a irrigação automatica (12h)

#define IRRIGATION_INTERVAL 43200000

// Tempo (em milissegundos) que ficará sem irrigar após finalizar a irrigação automatica caso esteja com o solo encharcado (1h)

#define IRRIGATION_INTERVAL_SOGGY_SOIL 3600000

#define SOGGY_SOIL_MESSAGE_DURATION 2000

// Tempo (em milissegundos) que ficará sem atualizar o valor da umidade (1s)

#define HUMIDITY_INTERVAL 1000

// Define os pinos que serão utilizados para ligação do display e inicia a variável do display
// A Classe display estende a classe LiquidCrystal

Display display(12, 11, 5, 4, 3, 2);

// Variável que armazenará o modo em que a irrigação está, é um enum importado da lib ScienceFairGreenhouse.h

IrrigationMode irrigationMode = IrrigationMode::MANUAL_MODE_OFF;

// Variável que armazenará o nivel da umidade

int currentHumidity = 0;

// Váriavel auxiliar para formatar a umidade para o display

char formatHumidity[17] = "";

// Váriaveis para controle de tempo sem travar a execução

unsigned long previousHumidityTimer = 0;
unsigned long previousIrrigationTimer = 0;
unsigned long irrigationStartTimer = 0;
unsigned long soggySoilMessageStartTimer = 0;

// Rodará uma vez

void setup() {

  // Define o tipo do pino dos botões (entrada, inverte o HIGH LOW)

  pinMode(BTN_MANUAL_PIN, INPUT_PULLUP);
  pinMode(BTN_AUTO_PIN, INPUT_PULLUP);

  // Define o tipo do pino do sensor de umidade (entrada)

  pinMode(HUMIDITY_SENSOR_PIN, INPUT);

  // Define o tipo de pino da bomba d'água (saída)

  pinMode(PUMP_PIN, OUTPUT);

  // Define o Valor HIGH na bomba d'água, para manter-lá desligada

  digitalWrite(PUMP_PIN, HIGH);

  // Inicia o Display e define as dimensões (quantidade de colunas e de linhas, respectivamente)

  display.begin(16, 2);

  // Chama a função de Introdução

  display.intro();

}

// Rodará em Looping até desligar o arduino

void loop() {

  // Pega o instante atual

  unsigned long currentTimer = millis();

  // Obtém o valor digital (HIGH, LOW) dos botões, não é obrigatório armazenar em uma váriavel mas é um bom costume
  // São declaradas dentro do void loop, pois não são utilizadas em outro escopo, o que deixa o código mais limpo

  int btnManualState = digitalRead(BTN_MANUAL_PIN);
  int btnAutoState = digitalRead(BTN_AUTO_PIN);

  // Lê o estado atual dos botões LOW == botão pressionado devido ao pullUp

  // Se o botão "Manual" for pressionado:

  if(btnManualState == LOW) {
    // Caso o estado de irrigação seja manual desligado, liga a irrigação no modo manual
    // Caso o contrário, apenas coloca em modo manual com irrigação desligada

    irrigationStartTimer = 0;

    if(irrigationMode == IrrigationMode::MANUAL_MODE_OFF) {
      // Seta o Modo de irrigação para manual ligado

      irrigationMode = IrrigationMode::MANUAL_MODE_ON;
    } else {
      // Animação de rolar para direita na troca de modo

      // Só chama a animação caso o modo seja diferente de manual ligado, para evitar que animação seja acionada sem que mude o modo
      // apenas desligue a irrigação ao invés de realmente trocar de automático para manual

      if(irrigationMode != IrrigationMode::MANUAL_MODE_ON)
        display.println("MODO MANUAL", formatHumidity, AnimationDirection::RIGHT_SCROLLING);

      // Seta o Modo de irrigação para manual desligado
      irrigationMode = IrrigationMode::MANUAL_MODE_OFF;
    }
    // Para evitar que faça mais de uma leitura em um clique
    delay(200);
  }

  // Se o botão "Automático" for pressionado, coloca a irrigação no modo automático

  if(btnAutoState == LOW) {
    // Só altera caso o modo de irrigação seja diferente de automático, para evitar que animação e o desligamento da bomba sejam acionados sem que mude o modo

    if(irrigationMode != IrrigationMode::AUTOMATIC_MODE) {
      // Para garantir que a bomba se mantenha desligada após trocar o estado
      digitalWrite(PUMP_PIN, HIGH);

      // Animação de rolar para esquerda na troca de modo

      display.println("MODO AUTOMATICO", formatHumidity, AnimationDirection::LEFT_SCROLLING);

      irrigationMode = IrrigationMode::AUTOMATIC_MODE;
    }
  }

  // Atualiza o valor da umidade a cada o intervalo de verificação (HUMIDITY_INTERVAL)

  if(currentTimer - previousHumidityTimer >= HUMIDITY_INTERVAL) {
    // Atualiza o valor do tempo anterior de atualização da umidade anterior

    previousHumidityTimer = currentTimer;

    // Obtém o valor análogico(0 a 1023) do sensor de umidade, sendo 0 mais úmido e 1023 mais seco.
    // Logo Após se utiliza o map, para inverter o valor, ou seja quanto maior mais úmido, e transforma em um percentual (0 a 100)

    currentHumidity = map(analogRead(HUMIDITY_SENSOR_PIN), 1023, 0, 0, 100);

    // Para formatar o texto da umidade e armazená-lo ná variável formatHumidity

    ("UMIDADE: " + (currentHumidity >= 10 ? currentHumidity : "0" + String(currentHumidity)) + "%").toCharArray(formatHumidity, 17);

  }

  // Modo de Irrigação Automática

  if(irrigationMode == IrrigationMode::AUTOMATIC_MODE) {

    // Verifica se já passou o tempo de espera (IRRIGATION_INTERVAL) desde a última irrigação
    // Separando pois as váriaveis de tempo são unsigned, ou seja apenas números naturais, logo
    // se currentTimer for menor que previousIrrigationTimer ocorreria um erro na subtração, pois
    // resultaria um número negativo, nisso ocorreria um underflow, que resultaria em número positivo
    // gigante que não deveria, resultando na conclusão da condicional em momentos indevidos.

    const bool irregationIntervalAux = (currentTimer < previousIrrigationTimer ? false :
     currentTimer - previousIrrigationTimer >= IRRIGATION_INTERVAL);

    if (irregationIntervalAux) {

      // Atualiza o valor do tempo anterior de espera da irrigação

      // Se a umidade estiver abaixo do limite, liga a bomba

      if (currentHumidity < DRY_LIMIT) {
        // Aciona a bomba d'água

        digitalWrite(PUMP_PIN, LOW);

        // Para garantir que não exiba a mensagem de solo encharcado

        soggySoilMessageStartTimer = 0;

        // Seta os valores auxiliares para que regue por pelo tempo de irrigação defindo e espere o tempo de espera definido, para regar novamente

        irrigationStartTimer = currentTimer;
        previousIrrigationTimer = currentTimer + IRRIGATION_DURATION;
      } else {
        display.println("MODO AUTOMATICO", "SOLO ENCHARCADO");

        // Caso o Solo esteja encharcado, checa novamente após um período menor, designado por (IRRIGATION_INTERVAL_SOGGY_SOIL)

        previousIrrigationTimer = currentTimer - (IRRIGATION_INTERVAL - IRRIGATION_INTERVAL_SOGGY_SOIL);

        // Salva o instante que mostrou a mensagem, para garantir que a mostre durante o tempo designado (soggySoilMessageStartTimer) sem travar a execução

        soggySoilMessageStartTimer = currentTimer;

      }
      // Caso a mensagem tenha sido mostrada durante o tempo designado (soggySoilMessageStartTimer) e esteja ativada (soggySoilMessageStartTimer > 0)
    } else if(soggySoilMessageStartTimer > 0 && (currentTimer - soggySoilMessageStartTimer >= SOGGY_SOIL_MESSAGE_DURATION)) {
      // Parar de mostrar a mensagem do solo encharcado

      soggySoilMessageStartTimer = 0;

      // Para garantir que a umidade esteja visível logo após desativar a mensagem de solo encharcado

      display.println("MODO AUTOMATICO", formatHumidity);

      // Para garantir que a mensagem mostre caso ainda esteja ativada
    } else if(soggySoilMessageStartTimer > 0) {
      display.println("MODO AUTOMATICO", "SOLO ENCHARCADO");
      // Controla apenas a mensagem de irrigação, para garantir que não ocorra nenhum problema
      // na renderização do display
    } else if (irrigationStartTimer > 0 && (currentTimer - irrigationStartTimer <= IRRIGATION_DURATION)) {
      display.onLoading("MODO AUTOMATICO", "REGANDO");
    }
     else {
      display.println("MODO AUTOMATICO", formatHumidity);
    }

    // Caso não tenha completado a irrigação, a mantém ligada pelo tempo determinado
    // Caso tenha completo, desliga a bomba d'água, também garante que
    // a bomba se mantenha desligada durante as múltiplas renderizações.
    // A primeira verificação serve para garantir que não mantenha acionado
    // caso não tenha sido acionado anteriormente.

    if (irrigationStartTimer > 0 && (currentTimer - irrigationStartTimer <= IRRIGATION_DURATION)) {
      digitalWrite(PUMP_PIN, LOW);
    } else {
      digitalWrite(PUMP_PIN, HIGH);
    }


  // Modo de Irrigação Manual

  } else if (irrigationMode == IrrigationMode::MANUAL_MODE_OFF) {
    // Mantém a bomba d'água desligada

    digitalWrite(PUMP_PIN, HIGH);

    // Printa a umidade e o modo na tela

    display.println("MODO MANUAL", formatHumidity);
  } else {
    // Mantém a bomba d'água desligada

    digitalWrite(PUMP_PIN, LOW);

    // Aciona a animação de Loading

    display.onLoading("MODO MANUAL", "REGANDO");
  }

}