#ifndef PRINT_H
#define PRINT_H

// Defining functions

void print(char b)         // SENDING BACK DATA
{
  SBUF=b;
  while(!TI);
  TI=0;
}      
void transmit(char *c)     // SENDING BACK DATA
{
  while(*c)
  {
    print(*c);
    c++;
  }
  print(0x0d);
}
void delay(int t)          // FOR DELAY FOR ANY MILLISEC AT 11.05MHZ CRYSTAL
{
  unsigned int i,j;
  for(i=0;i<t;i++)
  for(j=0;j<1275;j++);
}

#endif