/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados I
*******************************************************************
* nombre fichero : lavadora.c  
*******************************************************************
* proyecto       : Motor LEGO
* descripcion    : Control de una lavadora simulada
* programador    : JL Villarroel (JLV) 
* fecha          : 06 / 05 / 2020
*******************************************************************/

/******************************************************************
*                     INCLUDES                                    *
*******************************************************************/

#include "derivative.h"

#include "motor.h"
#include "clock.h"

/******************************************************************
*                     PROTOTIPOS FUNCIONES                        *
*******************************************************************/

static void InitSystem (void) ;  
void Control_Motor (void) ;
void Control_Discreto (void) ;
void Visualizacion (void) ;
unsigned char LeerEntrada(void) ;


/******************************************************************
*                     SIMBOLOS                                    *
*******************************************************************/

/* Las entradas se leen del puerto A
     PTA1 --> B0
     PTA2 --> B1
     PTA3 --> B2
     PTA5 --> S0
     PTA6 --> S1
     PTA7 --> S2  */
     

// Pulsadores
#define M       0x02     // 0000_0010
#define S1      0x20     // 0010_0000
#define S2      0x40     // 0100_0000


/******************************************************************
*                     VARIABLES GLOBALES                          *
*******************************************************************/

unsigned char marco = 0, num_marcos = 10;
unsigned int siguiente, m = 10;

enum {Prog1, Prog2, Prog1_5, Prog1_4, Prog1_3,
Prog1_2, Prog1_1, Prog2_4, Prog2_3,Prog2_2, Prog2_1} estado ;
unsigned char Entrada ;

float W, W_ref, U=0.0, E, E_ant = 0.0 ;
volatile unsigned char Paso = 0, Programa = 1 ;
unsigned char V_PASO = 0, V_PROG = 1 ;

float buffer [150] ;
char j = 0 ;


/******************************************************************
*                     MAIN                                        *
*******************************************************************/


void main(void) {
 
 InitSystem () ;
 
 Init_Clock () ;
 
 W_ref = 0;
 estado = Prog1 ;
 /**
  while(1){
  Control_Motor(); 
  }
 **/
 siguiente = Get_Time () ;
 while(1) {
    marco = (marco % num_marcos)+1;
    switch (marco) {
      case 1: Visualizacion(); Control_Discreto(); break;
      case 2: Visualizacion(); Control_Motor(); break;
      case 3: Visualizacion();break;
      case 4: Visualizacion();break;
      case 5: Visualizacion();break;
      case 6: Visualizacion();Control_Discreto();break;
      case 7: Visualizacion();break;
      case 8: Visualizacion();break;
      case 9: Visualizacion();break;
      case 10:Visualizacion();break;
    }
    
    siguiente += m ;
    delay_until (siguiente) ;
   }
}


/******************************************************************
*                     FUNCIONES                                   *
*******************************************************************/

void InitSystem (void) {

  /* Internal Clock Source Configuration */
	if(NVICSTRM == 0xFF)			// If programmed trim value is blank
		ICSTRM = 0x80;				  // load default value
	else
		ICSTRM = NVICSTRM;	  	// else load programmed trim value
  ICSC2_BDIV = 1 ;          // Divides selected clock by 2		
	                          // Bus clock = 5 MHz, CPU clock = 10MHz

  
  SOPT1_COPT = 0 ;          // WATCHDOG disable 
  
  PTBD  = 0b00001000 ;
  PTBDD = 0b00011100 ;      // PTB0..6 as outputs

}

/******************************************************************/

void Control_Motor (void) {

   W = Read_W () ;
   
   buffer[j] = W ;  
   j = (j + 1)%150 ;

   E = W_ref - W;
   U = U + 0.572*E -0.286*E_ant;
   Set_U (U);
   
   E_ant = E ;
   
}

/******************************************************************/

void Visualizacion (void) {
  
  V_PROG = Programa ;
  V_PASO = Paso ;
  
}

/******************************************************************/

void Control_Discreto (void) {
  
  Entrada = LeerEntrada();
    
  switch(estado){
     case Prog1: 
        if(Entrada & M) {
          estado = Prog1_5;
          Set_Timer (4000,0);
          W_ref = 1;
          Paso = 5;
        } 
        else if(Entrada & S2){
          estado = Prog2;
          Programa = 2;
        }
        break;
      case Prog2:
        if(Entrada & M) {
            estado = Prog2_4;
            Set_Timer (2000,0) ;
            W_ref = -0.5;
            Paso = 5;
        } 
        else if(Entrada & S1){
            estado = Prog1;
            Programa = 1;
        }
        break;
      //Programa 1
      case Prog1_5:
        if(Time_Out()){
          Set_Timer(4000,0);
          W_ref = -1;
          Paso = 4;  
          estado = Prog1_4;
        }
        break;
      case Prog1_4:
        if(Time_Out()){
          Set_Timer(4000,0);
          W_ref = 1;
          Paso = 3;  
          estado = Prog1_3;
        }
        break;
      case Prog1_3:
        if(Time_Out()){
          Set_Timer(4000,0);
          W_ref = 0;
          Paso = 2;  
          estado = Prog1_2;
        }
        break;
      case Prog1_2:
        if(Time_Out()){
          Set_Timer(10000,0);
          W_ref = 3;
          Paso = 1;  
          estado = Prog1_1;
        }
        break;
      case Prog1_1:
        if(Time_Out()){
          W_ref = 0;
          Paso = 0;  
          estado = Prog1;
        }
        break;
       //Programa 2
       case Prog2_4:
        if(Time_Out()){
          Set_Timer(4000,0);
          W_ref = 1;
          Paso = 3;  
          estado = Prog2_3;
        }
        break;
      case Prog2_3:
        if(Time_Out()){
          Set_Timer(2000,0);
          W_ref = 0;
          Paso = 2;  
          estado = Prog2_2;
        }
        break;
      case Prog2_2:
        if(Time_Out()){
          Set_Timer(6000,0);
          W_ref = 2;
          Paso = 1;  
          estado = Prog2_1;
        }
        break;
      case Prog2_1:
        if(Time_Out()){
          W_ref = 0;
          Paso = 0;  
          estado = Prog2;
        }
        break; 
  }
}

/******************************************************************/

unsigned char LeerEntrada(void)
{
   return (PTAD)&0b01100010 ;
}
  