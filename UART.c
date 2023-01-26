#include "lib/include.h"

extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<0);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<0);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOA->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOA->DEN = (1<<0) | (1<<1);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART0->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 / (16 * 9600) = 130.2
    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
    */
    UART0->IBRD = 130;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART0->FBRD = 14;
    //  UART Line Control (UARTLCRH) pag.916
    UART0->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART0->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART0->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern void UART_2_CONFIG(void){
    // Initialization of UART
    // UART2 PI N D6 YD7 This pins are connected to serial module
    // UART4 PIN C4 Y C5
    SYSCTL -> RCGCUART |= 0x4;  // 0b100 // Enabling UART MODULE 2, in general (1 << X) where X is number of module
    SYSCTL -> RCGCGPIO |= 0x8; // 0b1000// Port D --> Bit field 3; Port C ---> 2
    GPIOD -> LOCK = 0x4C4F434B;
    GPIOD -> CR = 0xC0; // Bit 6 y 7  
    GPIOD -> AFSEL = (1 << 6) | (1 << 7); // pin D6 and D7
    GPIOD -> PCTL |= (GPIOD -> PCTL & 0x00FFFFFF) | 0x11000000; //(1 << 24) | (1 << 28); // 6 and 7
    GPIOD -> DEN = (1 << 6) | (1 << 7);
    // BAUD_RATE = 40,000,000  / (16 * 115200) = 21.70138889 ~ 22 
    // UARTFBRD[DIVFRAC] = integer(0.70138889 * 64 + 0.5) = 45.388 rounded --> 45
    UART2 -> CTL = (0 << 0) | (0 << 8) | (0 << 9); 
    // Integer portion of brd
    UART2 -> IBRD = 21; 
    // Fractional  portion of brd 
    UART2 -> FBRD = 45; 
    // Desired parameters 
    UART2 -> LCRH = (0x3 << 5) | (1<<4); // 8 bits, by default we get no parity and 1-stop bit and FIFO ENABLE
    UART2 -> CC = 0; //(0 << 0); // SYSTEM CLOCK 
    UART2 -> CTL = (1 << 0) | (1 << 8) | (1 << 9); // bits 8th and 9th should be enabled together

}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART2->FR & (1<<4)) != 0 );
    v = UART2->DR & 0xFF;
    c = v;
    //c = UART2 -> DR; 
    return c;
}
extern void printChar(char c)
{
    while((UART2->FR & (1<<5)) != 0 );
    UART2->DR = c;
}
extern void printString(char *string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern char *readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}




