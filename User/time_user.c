#include "main.h"

void msleep(int num)
{
  for(int i=0;i<num;i++){
    for(int j=0;j<0x21FF;j++);
  }
}