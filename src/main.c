//                         !!!!!!!!!!!!!!!!!! BU KOD PD12'Yİ HER 500 TICK'TE TOGGLE EDER !!!!!!!!!!!!!!!!!!!!!

#include <stdint.h>

#define SCB_CPACR (*(volatile unsigned int*)0xE000ED88)
#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define GPIOD_MODER (*(volatile uint32_t*)0x40020C00)
#define GPIOD_ODR (*(volatile uint32_t*)0x40020C14)

#define SYST_CSR (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR (*(volatile uint32_t*)0xE000E018)

volatile uint32_t counter;
volatile uint32_t last_change=0;

void FPU_Enable(void) {
    SCB_CPACR |= ((3UL << 20) | (3UL << 22));
}

void Configure_Green_LED(void){
	RCC_AHB1ENR |= (1<<3); 
    GPIOD_MODER &= ~(3<<(12*2));
    GPIOD_MODER |= (1<<(12*2));
}


void SysTick_Handler(void){
	counter++;
}

void Configure_SysTick(void){
	SYST_RVR = 15999;// LOAD değeri ayarlanır.
	SYST_CVR = 0;// SysTick'in o anki değeri temizlenir.
	SYST_CSR |= (1<<0) | (1<<2) | (1<<1);// CLKSOURCE, TICKINT, ENABLE bitleri set edilir.
}


int main(void)
{
    FPU_Enable();// İşlemci içindeki FPU birimini aktif eder.FPU birimi, ondalıklı sayı işlemlerini donanımda yapan birim.
    Configure_SysTick();//SysTick register'ları ayarlanır.
    Configure_Green_LED();


    while(1){
    	if((counter-last_change)>=500){
    		GPIOD_ODR ^= (1<<12);
    		last_change=counter;
    	}
    }
}
