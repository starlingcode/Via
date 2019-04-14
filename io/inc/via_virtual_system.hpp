/** \file via_virtual_system.hpp
 * \brief Some macros to mimic the hardware GPIO, needs to be updated.
 */

#ifndef IO_INC_VIA_VIRTUAL_SYSTEM_HPP_
#define IO_INC_VIA_VIRTUAL_SYSTEM_HPP_

/// \cond

#ifdef BUILD_VIRTUAL


//@{
/// Method to emulate the hardware GPIO outputs, should be rewritten
#define GPIO_NOP 0

#define GPIO_PIN_13 (1 << 13)
#define GPIO_PIN_12 (1 << 12)
#define GPIO_PIN_8 (1 << 8)
#define GPIO_PIN_9 (1 << 9)

#define ALOGIC_LOW_MASK (uint32_t)GPIO_PIN_13
#define EXPAND_LOGIC_LOW_MASK (uint32_t)GPIO_PIN_12
#define SH_A_TRACK_MASK (uint32_t)GPIO_PIN_8
#define SH_B_TRACK_MASK (uint32_t)GPIO_PIN_9

#define ALOGIC_HIGH_MASK ((uint32_t)GPIO_PIN_13 << 16)
#define EXPAND_LOGIC_HIGH_MASK ((uint32_t)GPIO_PIN_12 << 16)
#define SH_A_SAMPLE_MASK ((uint32_t)GPIO_PIN_8 << 16)
#define SH_B_SAMPLE_MASK ((uint32_t)GPIO_PIN_9 << 16)

#define GET_ALOGIC_MASK(X) (ALOGIC_LOW_MASK << (16*(X)))
#define GET_EXPAND_LOGIC_MASK(X) (EXPAND_LOGIC_LOW_MASK << (16*(X)))
#define GET_SH_A_MASK(X) (SH_A_TRACK_MASK << (16*(X)))
#define GET_SH_B_MASK(X) (SH_B_TRACK_MASK << (16*(X)))

// should return 0 for no-op, 1 for set low, 2 for set high
#define GET_ALOGIC_VIRTUAL_MASK(X) (((X >> 13) | (X >> 28)) & 0b11)
#define GET_EXPAND_LOGIC_VIRTUAL_MASK(X) (((X >> 12) | (X >> 27)) & 0b11)
#define GET_SH_A_VIRTUAL_MASK(X) (((X >> 8) | (X >> 23)) & 0b11)
#define GET_SH_B_VIRTUAL_MASK(X) (((X >> 9) | (X >> 24)) & 0b11)

//@}



/*
 * Touch sensors
 */

//#define BUTTON2SENSOR MyTKeys[0].p_Data->StateId
//#define BUTTON1SENSOR MyTKeys[1].p_Data->StateId
//#define BUTTON3SENSOR MyTKeys[2].p_Data->StateId
//#define BUTTON4SENSOR MyTKeys[3].p_Data->StateId
//#define BUTTON6SENSOR MyTKeys[4].p_Data->StateId
//#define BUTTON5SENSOR MyTKeys[5].p_Data->StateId

//@{
/// Stand ins for the values used by the hardware touch sense library.
#define PRESSED 1
#define RELEASED 0
//@}

#endif


/// \endcond

#endif /* IO_INC_VIA_VIRTUAL_SYSTEM_HPP_ */
