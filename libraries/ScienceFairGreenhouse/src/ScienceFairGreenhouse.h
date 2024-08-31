// Só define caso ainda não esteja definido, evita múltiplas inclusões

#ifndef ScienceFairGreenhouse_h
#define ScienceFairGreenhouse_h

#include "Arduino.h"

// Inclui a bibilioteca do LCD (LiquidCrystal.h) customizada
// A biblioteca customizada adiciona a Classe Display que estende a classe LiquidCrystal
// Essa customização adiciona novas funções para abstrair o código, tornando-o o mais limpo e sintático

#include "Display.h"

// Inclui o enum dos estados de irrigação
#include "IrrigationMode.h"

// Fim da proteção contra múltiplas inclusões

#endif
