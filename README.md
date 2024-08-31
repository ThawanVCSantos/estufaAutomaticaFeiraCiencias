# estufaAutomaticaFeiraCiencias

== Projeto Estufa Automática Feira de Ciências ==

Projeto de estufa automátizada feita em arduino para feira de ciências

Todas as bibliotecas utilizadas estão na pasta libraries do projeto, as mesma devem ser colocadas
na pasta libraries do Arduino, para que a IDE consiga encontrar as bibliotecas.

Uma das bibliotecas é pessoal do projeto, ela se chama ScienceFairGreenhouse

Há mais informações sobre o código do projeto no arquivo ScienceFairGreenhouse.ino

Há mais informações sobre a biblioteca ScienceFairGreenhouse no arquivo README.adoc da própria biblioteca

=== Utilização ===

Há dois modos de utilização, automático e manual, que podem ser trocados acionando botões.

	- Modo Manual: Para fins demonstrativos, varia entre manual com irrigação ligada, e manual com irrigação desligada,
	quando está em manual ligado, aciona a bomba d'água por meio do relé, quando está em manual desligado, interrompe a ação da bomba d'água por meio do relé.
	
	- Modo Automático: Utiliza de timers e medições do sensor de umidade do solo para decidir quando acionar e/ou interromper a ação da bomba d'água.
	
Como dito antes, o projeto utiliza um relé, devido a bomba d'agua utilizada ser um modelo 110v.

Outro item utilizado foi um display LCD 16x2, foi utilizado para demontrar o modo em que o programa se encontra, e o status.
	
	- Ex: MODO AUTOMATICO    MODO AUTOMATICO     MODO AUTOMATICO
	        UMIDADE: 65%        REGANDO...       SOLO ENCHARCADO
	        
	- O Status "Umidade" e o status "Regando" está presente em ambos os modos, apenas o "Solo Encharcado" está presente apenas no automático.
	
Também foi utilizado um sensor de umidade do solo, funciona medindo a resistência do solo, que diminui enquanto está úmido.
