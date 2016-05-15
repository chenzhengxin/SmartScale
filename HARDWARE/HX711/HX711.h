#include "stm32f10x.h"

void AD_GPIO_Configuration(void);
unsigned long Read_HX711(void);
void Weight_QuPi(float *ad_show_pi);
void Read_Weight_Val(float *AD_show_pi, float *weight);
