#include "Common.h"
//Function headers
void uart0_init(void);
void uart2_init(void);
char uart0_getchar(void);
char uart2_getchar(void);
void uart0_putchar(char ch);
void uart2_putchar(char ch);
void uart0_put(char *ptr_str);
void uart2_put(char *ptr_str);
BOOLEAN uart2_dataAvailable(void);
BOOLEAN uart0_dataAvailable(void);
void clearBuffer(char *buffer, int bufferLength);
