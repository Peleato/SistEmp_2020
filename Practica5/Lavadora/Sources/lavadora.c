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

unsigned char marco = 0, num_marcos =  ;
unsigned int siguiente, m =  ;

enum {Prog1, Prog2, ...} estado ;
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
 
 j = 0 ;
 W_ref =  ;
 estado = Prog1 ;
 ...

 siguiente = Get_Time () ;
  
 while(1) {
 
    marco = (marco % num_marcos)+1 ;
    switch (marco) {
      case 1: Visualizacion(); Control_Discreto(); Control_Motor(); break;
      ...

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

   ...
   
   Set_U (U) ;
   
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
          estado=...;
          Set_Timer (...,0) ;
          W_ref = ... ;
          Paso = ... ;
        } else if (...) {
          estado = ... ;
          Programa = ... ;
        }
        break;
                    
    case 
    }
}

/******************************************************************/

unsigned char LeerEntrada(void)
{
   return (PTAD)&0b01100010 ;
}
  