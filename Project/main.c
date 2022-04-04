#include "tm4c123gh6pm.h"
int main()
{
  //ADC SETUP 
  SYSCTL_RCGCGPIO_R |= (1U<<4) | (1U<<5) | (1U<<1); //clock enable to GPIO port E anf F and B
  SYSCTL_RCGCADC_R |=1; //Analog clock enable to ADC0 module
  ADC0_ACTSS_R &= ~8; //disable sample sequencer 3 during configuration
  ADC0_EMUX_R &= ~0xF000; // software trigger conversion
 // ADC0_PSSI_R 
  //GPIO_PORTF_DIR_R |= (1U<<1) | (1U<<2) | (1U<<3); //output direction for PF1,PF2,PF3 (all LED)
  GPIO_PORTF_DIR_R = 0xFFU; // port F direction as output
  GPIO_PORTE_DIR_R = 0x00U; //port E direction as input
  GPIO_PORTF_DEN_R = 0xFFU; //digital function enable for port F
  GPIO_PORTE_DEN_R = 0x00U; //disabling digital function enable for port E
  GPIO_PORTE_AFSEL_R |= (1U<<3); // alternate function select for pin PE3
  GPIO_PORTE_AMSEL_R |= (1U<<3); // analog mode select for pin PE3
  
  ADC0_SSMUX3_R =0;
  ADC0_SSCTL3_R |=6; //take one sample at a time,set flag at first sample
  ADC0_ACTSS_R |= (1U<<3); //enable ss3
  
  
  //PWM SETUP
  SYSCTL_RCGCPWM_R |= 0x01; //enable clock to PWM0
  SYSCTL_RCC_R &= ~0x00100000; //no predivide for PWM clock   
  GPIO_PORTB_AFSEL_R |= (1U<<4); //alternate function for PB4
  GPIO_PORTB_PCTL_R = 0x00040000;
  GPIO_PORTB_DIR_R |= (1U<<4); //PB4 as output pin
  PWM0_1_CTL_R = 0;               // disable M0PWM1 during configuration
  PWM0_1_GENA_R = 0x0000008C;     
  PWM0_1_LOAD_R = 16000;           // load value set 1 kHz
  PWM0_1_CTL_R = 1;      
  PWM0_ENABLE_R |= 0x04; //enablling M0PWM2
  
   int Values[] = {0, 25, 50, 75, 100};
    int SpeedMul = (int)(16000 / 100);
    int temperature = 0;
    volatile int result;
    while(1)    
    { 
	ADC0_PSSI_R |= 8;                                   // start a conversion sequence 3  
	while((ADC0_RIS_R & 8) == 0)                           // wait for conversion complete  
	{ 
	    result = ADC0_SSFIFO3_R;                          // read conversion result
	    temperature = 147 - (int)((247 * ADC0_SSFIFO3_R) / 4096) - 100;
	    //printf("R: %d\n", result);
	    if(temperature > 35)
	    {
               *((unsigned int*)0x40025008)=0x00U; //red off
               *((unsigned int*)0x40025010)=0x00U; //blue off
               *((unsigned int*)0x40025020)=0x00U; //green off
	        *((unsigned int*)0x40025008)=0x02U; //lighting red LED
    		PWM0_1_CMPA_R = SpeedMul*Values[4];
                
	    }
	    	
	    else if(temperature <= 35 && temperature > 30)
	    {
              *((unsigned int*)0x40025008)=0x00U; //red off
              *((unsigned int*)0x40025010)=0x00U; //blue off
              *((unsigned int*)0x40025020)=0x00U; //green off
	        *((unsigned int*)0x40025010)=0x04U; //lighting blue LED
		PWM0_1_CMPA_R = SpeedMul*Values[3];
	    }
	    else if(temperature <= 30 && temperature > 25)
	    {
              *((unsigned int*)0x40025008)=0x00U; //red off
              *((unsigned int*)0x40025010)=0x00U; //blue off
              *((unsigned int*)0x40025020)=0x00U; //green off
		*((unsigned int*)0x40025020)=0x08U; //lighting green LED
		PWM0_1_CMPA_R = SpeedMul*Values[2];
	    }
	    else
	    {
                *((unsigned int*)0x40025008)=0x00U; //red off
                *((unsigned int*)0x40025010)=0x00U; //blue off
                *((unsigned int*)0x40025020)=0x00U; //green off
		*((unsigned int*)0x40025008)=0x02U; //lighting red LED
                *((unsigned int*)0x40025010)=0x04U; //lighting blue LED
		PWM0_1_CMPA_R = SpeedMul*Values[1];
	    }
	}  
	ADC0_ISC_R = 8;                                     // clear completion flag  
    }
  return 0;
}
