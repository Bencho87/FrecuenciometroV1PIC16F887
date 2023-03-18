#include <ejemplotemporizador1.h>

#BYTE intcon = 0x0B  //LA VARIABLE INTCON TOMA LA DIRECCIÓN 0X0B


int flag = 0;
unsigned int16 pulsos=0;
float periodo=0.0;
float frecuencia=0.0;

#INT_EXT
void  EXT_isr(void) 
{
   if(flag == 1)
   {
   setup_timer_1(T1_DISABLED); //detener timer 
   flag = 2;
   //pulsos =get_timer1();
   disable_interrupts(INT_EXT); //apagar interrupción externa
   disable_interrupts(GLOBAL);
   }
   if(flag == 0) //el primer flanco positivo
   {
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);  //poner a contar
   set_timer1(0);
   flag = 1;
   }

   output_toggle(PIN_D3);
}

#define LCD_DATA_PORT getenv("SFR:PORTD")
#include <lcd.c>




void main()
{
  setup_oscillator( OSC_8MHZ );

   //setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);      //131 ms overflow
   setup_timer_1(T1_DISABLED);

   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);

   lcd_init();

   lcd_putc("\fReady...\n");
   
   while(TRUE)
   {
      if(flag == 2)
      {
      pulsos = 0;
      pulsos =get_timer1(); //copiamos hasta donde contó
      periodo = pulsos * 0.000002; //número de pulsos por resolución
      frecuencia = 1.0/periodo;
      printf(lcd_putc,"\f f = %0.6f\n",frecuencia);
      printf(lcd_putc,"%lu",pulsos);
      delay_ms(500);
      set_timer1(0); //reiniciar el timer 1 a 0
      flag = 0; //reiniciar la bandera
      intcon = intcon & 0XFD;//BORRAMOS LA BANDERA DE LA INTERRUPCIÓN EXTERNA  //11111101 
      enable_interrupts(INT_EXT); //habilitar la interrupción externa
      enable_interrupts(GLOBAL);
      }
   
   }

}
