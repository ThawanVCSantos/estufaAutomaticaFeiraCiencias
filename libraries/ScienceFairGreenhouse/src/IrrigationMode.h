// Só define caso ainda não esteja definido, evita múltiplas inclusões

#ifndef IrrigationMode_h
#define IrrigationMode_h

// Enum para definir os modos do sistema de irrigação
// Utilizado para configurar o modo de operação do sistema de irrigação.

enum class IrrigationMode {
  MANUAL_MODE_OFF,  // Modo Manual, irrigação desligada
  MANUAL_MODE_ON,   // Modo Manual, irrigação ligada
  AUTOMATIC_MODE    // Modo Automático
};

// Fim da proteção contra múltiplas inclusões

#endif
