//================================================================================ //
// RELÓGIO DIGITAL - CONFIGURAVEL //
//================================================================================ //
/*
Rafael Stuepp Riegel - Projeto Microcontroladores - Versão 2.0

Descrição:
Desenvolver um projeto utilizando o microcontrolador ATmega328P.

Tema é de sua livre escolha.

A programação deve ser em linguagem C.

Você deve simular previamente no Tinkercad e, em seguida, 
implementar fisicamente usando o Arduino UNO.

Entregas:

Apresentar o projeto em sala(*); e
Gravar um Vídeo (**) de 15 minutos [mín. 10 e máx 20] 
apresentando o projeto (demonstrar o funcionamento e 
explicar brevemente o código, a montagem do circuito, 
componentes, etc.).

Entrega: 02/02/2022

(*)Alunos em AER, deverão apresentar ao vivo na sala virtual 
do Meet.

(**) Você deve subir o vídeo no youtube e postar o link no 
SIGAA.
*/
//================================================================================

//Declaração das Variaveis

//tipo inteiro
int unidHrs, dezHrs, unidMin, dezMin, 
unidSeg, dezSeg, contador, nr, i, calc;
//tipo boleano 1 ou 0
bool statusBotaoLiga, statusBotaoTrocaSeg,
statusBotaoIncre, status, auxiliar;

//Definição das Entradas

#define BotaoLiga 6 
/*define o botão de ligar para o pino digital 6 */
#define BotaoTrocaSeg 7
/*define o botão de Trocar segmento para o pino digital 7*/
#define BotaoIncre A5
/*define o botão de Trocar segmento para o pino analogico A5*/
#define TempoDelay 30
//constante de tempo 

//================================================================================
void setup(){
  
  /*DDR
Os registradores do tipo DDR (Data Direction Register) 
são responsáveis por determinar se os pinos de um determinado 
PORT se comportarão como entrada ou saída. 
Registros Portuarios 
  B (pino 8 a 13 )
  C (pinos de entrada analogica)
  D (pinos digitais 0 a 7)
  
//================================================================================ 
  operador OU booleano || (pipe duplo)
  com o operador OU bitwise | (pipe único).
  operador OU booleano || (pipe duplo)
  com o operador OU bitwise | (pipe único).
//================================================================================
 */
  
  //configuração dos três DDRx
  DDRB = 0xFF;//todas as portas B são de entrada
  DDRC = 0x0F;
  DDRD = 0x3C;
  
//================================================================================
  
  //Define os botões - pullup, puxa nivel para cima
  
  pinMode(BotaoLiga, INPUT_PULLUP); /*Configura pino 6 como 
  entrada e habilita pull up interno;*/
  pinMode(BotaoTrocaSeg, INPUT_PULLUP);/*Configura pino 7 como 
  entrada e habilita pull up interno;*/
  pinMode(BotaoIncre, INPUT_PULLUP);/*Configura pino A5 como 
  entrada e habilita pull up interno;*/
 
  //Aguardando acionar o botão de ligar
  delay(200);
  status = auxiliar = 0; //guarda status recebe auxiliar e auxiliar
  //recebe zero
}

//================================================================================

//Função para Setar os dados no relógio
void setRelogio(){
  //boleano
  bool botaoFinaliza;
  
  //Seta como 0 os numeros e ativa todas as saidas
  PORTB = 0xFF;
  PORTC = 0xFF;
  PORTD = 0;
 
  //Seta as variaveis pra uma nova configuração
  //Zera todas as casas, somento o i, iterador fica como 1
  nr = 0;
  contador = 0;
  i = 1;
  
  unidSeg = 0;
  dezSeg = 0;
  unidMin = 0;
  dezMin = 0;
  unidHrs = 0;
  dezHrs = 0;
  
  do{// faça  
    
    //Definição dos estado das booleanos em relação aos botões
    statusBotaoTrocaSeg = digitalRead(BotaoTrocaSeg);/*lê
    e guarda o status na variavel auxiliar especifica para
    guardar o estado do botão, status recebe dado lido no botão*/
    statusBotaoIncre = digitalRead(BotaoIncre);/*lê
    e guarda o status na variavel auxiliar especifica para
    guardar o estado do botão, status recebe dado lido no botão*/
    botaoFinaliza = digitalRead(BotaoLiga);/*lê
    e guarda o status na variavel auxiliar especifica para
    guardar o estado do botão, status recebe dado lido no botão*/
     //BotaoFinaliza recebe o botaoLiga 
      
    /*movimenta o bit do PORTB para manipular o proximo numero*/
    if (statusBotaoTrocaSeg == 1){//true
      PORTC = 0;
      PORTB = 0;
    if (i == 64){//base 2 (binario) 1000000
        i = 1;
      }
      //Desloca-se para o outro segmento, movimentando o bit
      PORTB = PORTB | i; // or bitwise (binario)
      i = i*2; // iterador vezes 2
      nr = 0; //seta o nr como zero
      //desloca valor
      converteNumero(nr); //Função que converte o numero
      //com ajuda do CI4511
      delay(100);//delay para trocar a casa
    }
    
    //Define o numero do display
  if (statusBotaoIncre == 1){//true
      PORTC = 0;
      if (i == 1 && nr == 0){/*se o i igual a 1 "and lógico" 
        numero igual a 0 */
        PORTB = 0;
        PORTB = PORTB | i; //or bitwise (binario)
        i = i*2; //o i recebe o proprio i nr binario vezes 2
      }
      nr ++; //incrementa +1
      if ((i == 8 && nr == 6) || (i == 32 && nr == 6) || (i == 2 && nr == 3) || (i == 1 && nr == 3)|| (dezHrs == 2 && nr == 4 && i==4) || (nr == 10)){
        /*definição de paramentros dos numeros aceitos nos diplays
        //em binario*/
        nr = 0;
      }
      converteNumero(nr); //função para converter o nume
      setValores(i,nr); //setar valores por parametro
      delay(50);
    }
    
  }while(botaoFinaliza != 1);/*enquanto botãoFinaliza diferente
  de true, Inicia o Relogio */ 
}
//================================================================================

/*Função que recebe os numeros setados na função setRelogio() 
e atribui aos devidos valores*/
void setValores(int a, int b){ /*função void com os parametros 
do tipo inteiro*/
  
  switch(a){ /* escolha, se "a" for tal caso, fazendo com que a
  variavel do segmento, receba b*/
    
    case 1:
    dezHrs = b;    
    break;
    case 2:
    dezHrs = b;
    break;
    case 4:
    unidHrs = b;
    break;
    case 8:
    dezMin = b;
    break;
    case 16:
    unidMin = b;
    break;
    case 32:
    dezSeg = b;
    break;
    case 64:
    unidSeg = b;
    break;
    default:
    break;
  }
}

//================================================================================

//Função com os parametros e comportamento do relogio
void relogio(){
  
  contador ++;// incrementa
 
  if (contador == 10){//se contador igual a 10
    unidSeg ++;//incrementa unidade segundo
    contador = 0;//zera casa atual
  }

  if (unidSeg == 10){//se variavel igual a 10
    dezSeg ++;//incrementa dezena segundo
    unidSeg = 0;//zera casa atual
  }
  
  if (dezSeg == 6){//se variavel igual a 6
    unidMin ++;//incrementa unidade minuto
    dezSeg = 0;//zera casa atual
  }

  if (unidMin == 10){//se variavel igual a 10
    dezMin ++;//incrementa dezena minuto
    unidMin = 0;//zera casa atual
  }
  
  if (dezMin == 6){//se variavel igual a 6
    unidHrs ++;//incrementa unidade Horas
    dezMin = 0;//zera casa atual
  }
  
  if (unidHrs == 10){//se variavel igual a 10
    dezHrs ++;//incrementa Dezena horas
    unidHrs = 0;//zera casa atual
  }
  
  if (unidHrs == 4 && dezHrs == 2){// se 24hrs 
    
    //Zera tudo, ao bater meia noite 24:00 == 00:00
    unidSeg = 0;
    dezSeg = 0;
    unidMin = 0;
    dezMin = 0;
    unidHrs = 0;
    dezHrs = 0;
    contador = 0;
  }
  
//================================================================================

  /*Atribui o valor aos devidos displays 
  sempre habilitando apenas o display necessario*/
  PORTB = 0x20;//binario 100000
  converteNumero(unidSeg);//Função que converte o numero aceito pelo display
  delay(20);
  
  PORTB = 0x10;//10000
  converteNumero(dezSeg);//Função que converte o numero aceito pelo display
  delay(20);
  
  PORTB = 0x08;//1000
  converteNumero(unidMin);//Função que converte o numero aceito pelo display
  delay(20);
  
  PORTB = 0x04;//100
  converteNumero(dezMin);//Função que converte o numero aceito pelo display
  delay(20);
  
  PORTB = 0x02;//10
  converteNumero(unidHrs);//Função que converte o numero aceito pelo display
  delay(20);
  
  PORTB = 0x01;//1
  converteNumero(dezHrs);//Função que converte o numero aceito pelo display
  delay(20);

}
//================================================================================

void loop(){
  //Leitura do estado do BotaoLiga
  statusBotaoLiga = digitalRead(BotaoLiga);
  //Status recebe a leitura e a guarda
 
  /*possiblita atravez de "guardar" na variavel status o 
  mesmo pushButtom ligar e desligar o relógio*/
  if (statusBotaoLiga  == 1){  //true
    status = !status;//variavel status recebe !status
    delay (200);
  }
  
  if (status == 1){ /*safethread para que a função
   fazer com que setRelogio() seja executado apenas uma vez*/
    if (auxiliar == 0){// se a auxiliar igual a 0/falso
    	setRelogio();//chama função set relogio
    }
    
    auxiliar = 1; //variavel recebe 1/true
    PORTC = 0x02; //0000010
    relogio();// chama função relogio
    
    //delay definido no começo
    delay(TempoDelay);
  }
  else{  //senão
    if (auxiliar == 1){// se variavel igual 1/true/HIGH
      //Desliga relogio
    }
    
    /*Desliga os ports de controle e volta a auxiliar para 
    true para executar o setRelogio() novamente*/
    auxiliar = 0; //false
    PORTB = 0;
    PORTC = 0;
  }
}

//================================================================================

/*Função que recebe os valores que devem ser exibidos 
e converte para exibir*/
void converteNumero (int valor){/*Função que converte nr e 
passa por parametro,tipo inteiro*/
  
  switch (valor){ //escolha valor,caso
    
    case 0:
    PORTD = 0x00;
    break;
    case 1:
    PORTD = 0x04;
    break;
    case 2:
    PORTD = 0x08;
    break;
    case 3:
    PORTD = 0x0C;
    break;
    case 4:
    PORTD = 0x10;
    break;
    case 5:
    PORTD = 0x14;
    break;
    case 6:
    PORTD = 0x18;
    break;
    case 7:
    PORTD = 0x1C;
    break;
    case 8:
    PORTD = 0x20;
    break;
    case 9:
    PORTD = 0x24;
    break;
    default:
    break;
  }
}