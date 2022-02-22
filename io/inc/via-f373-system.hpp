/** \file via_f373_system.hpp
 * \brief Hardware IO defines
 *
 *  Some hardware implementation macros and global variable definitions to connect with the CMSIS and STM32 libraries used
 */

#ifndef REV5_HARDWARE_IO
#define REV5_HARDWARE_IO


#include "main.h"
#include "stm32f3xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Red pwm timer struct from STM32 HAL library.
extern TIM_HandleTypeDef htim3;
/// Blue pwm timer struct from STM32 HAL library.
extern TIM_HandleTypeDef htim4;
/// Green pwm timer struct from STM32 HAL library.
extern TIM_HandleTypeDef htim5;
/// UI timer struct from STM32 HAL library.
extern TIM_HandleTypeDef htim7;
/// 12 bit ADC struct from STM32 HAL library.
extern ADC_HandleTypeDef hadc1;
/// VCA control DACs struct from STM32 HAL library.
extern DAC_HandleTypeDef hdac1;
/// Signal out DAC struct from STM32 HAL library.
extern DAC_HandleTypeDef hdac2;
/// CV2 SDADC struct from STM32 HAL library.
extern SDADC_HandleTypeDef hsdadc1;
/// CV3 SDADC struct from STM32 HAL library.
extern SDADC_HandleTypeDef hsdadc2;

#ifdef __cplusplus
}
#endif


//@{
/**
 *  GPIO output mapping using the Set/Reset register.
 * The top 16 bits set the corresponding pin in the group low.
 * The bottom 16 bits set it high.
 * The output stage in the circuit inverts the logic state.
 * For each output, define a mask to write to the register for high.
 * One to set it low.
 * One to keep the sate the same (GPIO_NOP)
 */
#define GPIO_NOP 0
#define ALOGIC_LOW_MASK (uint32_t)GPIO_PIN_13
#define EXPAND_LOGIC_LOW_MASK (uint32_t)GPIO_PIN_12
#define SH_A_TRACK_MASK (uint32_t)GPIO_PIN_8
#define SH_B_TRACK_MASK (uint32_t)GPIO_PIN_9
#define ALOGIC_HIGH_MASK ((uint32_t)GPIO_PIN_13 << 16)
#define EXPAND_LOGIC_HIGH_MASK ((uint32_t)GPIO_PIN_12 << 16)
#define SH_A_SAMPLE_MASK ((uint32_t)GPIO_PIN_8 << 16)
#define SH_B_SAMPLE_MASK ((uint32_t)GPIO_PIN_9 << 16)
//}

#define LOGICA_HIGH GPIOC->BRR = (uint32_t)GPIO_PIN_13;
#define LOGICA_LOW GPIOC->BSRR = (uint32_t)GPIO_PIN_13;

//@{
/*  Converts 1 to the high (sample) mask and 0 to the low (track) mask.*/
#define GET_ALOGIC_MASK(X) (ALOGIC_LOW_MASK << (16*(X)))
#define GET_EXPAND_LOGIC_MASK(X) (EXPAND_LOGIC_LOW_MASK << (16*(X)))
#define GET_SH_A_MASK(X) (SH_A_TRACK_MASK << (16*(X)))
#define GET_SH_B_MASK(X) (SH_B_TRACK_MASK << (16*(X)))
#define SET_BLUE_LED_ONOFF(X) GPIOA->BSRR |= ((uint32_t)GPIO_PIN_8 << (16 * !(X)))
//@}

/// Main logic input high GPIO state.
#define TRIGGER_RISING_EDGE ((GPIOA->IDR & GPIO_PIN_15) == (uint32_t) GPIO_PIN_RESET)
/// Expander logic input low GPIO state.
#define EXPANDER_RISING_EDGE ((GPIOA->IDR & GPIO_PIN_11) == (uint32_t) GPIO_PIN_RESET)
/// Control button pressed GPIO state.
#define EXPANDER_BUTTON_PRESSED ((GPIOB->IDR & GPIO_PIN_1) == (uint32_t) GPIO_PIN_RESET)

/// Link button 2 to touch sense library.
#define BUTTON2SENSOR MyTKeys[0].p_Data->StateId
/// Link button 1 to touch sense library.
#define BUTTON1SENSOR MyTKeys[1].p_Data->StateId
/// Link button 3 to touch sense library.
#define BUTTON3SENSOR MyTKeys[2].p_Data->StateId
/// Link button 4 to touch sense library.
#define BUTTON4SENSOR MyTKeys[3].p_Data->StateId
/// Link button 6 to touch sense library.
#define BUTTON6SENSOR MyTKeys[4].p_Data->StateId
/// Link button 5 to touch sense library.
#define BUTTON5SENSOR MyTKeys[5].p_Data->StateId

/// Touch sense library detect state.
#define PRESSED TSL_STATEID_DETECT
/// Touch sense library release state.
#define RELEASED TSL_STATEID_RELEASE

#endif
