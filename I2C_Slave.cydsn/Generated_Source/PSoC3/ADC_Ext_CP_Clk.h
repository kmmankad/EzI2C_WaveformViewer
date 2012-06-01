/*******************************************************************************
* File Name: ADC_Ext_CP_Clk.h
* Version 1.60
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_CLOCK_ADC_Ext_CP_Clk_H)
#define CY_CLOCK_ADC_Ext_CP_Clk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/

void ADC_Ext_CP_Clk_Start(void) ;
void ADC_Ext_CP_Clk_Stop(void) ;

#if(!(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3 && \
    CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES2) && \
	!(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 && \
	CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_ES1))
void ADC_Ext_CP_Clk_StopBlock(void) ;
#endif

void ADC_Ext_CP_Clk_StandbyPower(uint8 state) ;
void ADC_Ext_CP_Clk_SetDividerRegister(uint16 clkDivider, uint8 reset) ;
uint16 ADC_Ext_CP_Clk_GetDividerRegister(void) ;
void ADC_Ext_CP_Clk_SetModeRegister(uint8 modeBitMask) ;
void ADC_Ext_CP_Clk_ClearModeRegister(uint8 modeBitMask) ;
uint8 ADC_Ext_CP_Clk_GetModeRegister(void) ;
void ADC_Ext_CP_Clk_SetSourceRegister(uint8 clkSource) ;
uint8 ADC_Ext_CP_Clk_GetSourceRegister(void) ;
#if defined(ADC_Ext_CP_Clk__CFG3)
void ADC_Ext_CP_Clk_SetPhaseRegister(uint8 clkPhase) ;
uint8 ADC_Ext_CP_Clk_GetPhaseRegister(void) ;
#endif

#define ADC_Ext_CP_Clk_Enable()                       ADC_Ext_CP_Clk_Start()
#define ADC_Ext_CP_Clk_Disable()                      ADC_Ext_CP_Clk_Stop()
#define ADC_Ext_CP_Clk_SetDivider(clkDivider)         ADC_Ext_CP_Clk_SetDividerRegister(clkDivider, 1)
#define ADC_Ext_CP_Clk_SetDividerValue(clkDivider)    ADC_Ext_CP_Clk_SetDividerRegister((clkDivider) - 1, 1)
#define ADC_Ext_CP_Clk_SetMode(clkMode)               ADC_Ext_CP_Clk_SetModeRegister(clkMode)
#define ADC_Ext_CP_Clk_SetSource(clkSource)           ADC_Ext_CP_Clk_SetSourceRegister(clkSource)
#if defined(ADC_Ext_CP_Clk__CFG3)
#define ADC_Ext_CP_Clk_SetPhase(clkPhase)             ADC_Ext_CP_Clk_SetPhaseRegister(clkPhase)
#define ADC_Ext_CP_Clk_SetPhaseValue(clkPhase)        ADC_Ext_CP_Clk_SetPhaseRegister((clkPhase) + 1)
#endif


/***************************************
*           API Constants
***************************************/

/* Constants SetPhase(), clkPhase parameter. Only valid for PSoC 3 ES2 and earlier. See datasheet for details. */
#if CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3 && \
   (CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES1 || \
    CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES2)
#define CYCLK_2_5NS             0x01 /* 2.5 ns delay. */
#define CYCLK_3_5NS             0x02 /* 3.5 ns delay. */
#define CYCLK_4_5NS             0x03 /* 4.5 ns delay. */
#define CYCLK_5_5NS             0x04 /* 5.5 ns delay. */
#define CYCLK_6_5NS             0x05 /* 6.5 ns delay. */
#define CYCLK_7_5NS             0x06 /* 7.5 ns delay. */
#define CYCLK_8_5NS             0x07 /* 8.5 ns delay. */
#define CYCLK_9_5NS             0x08 /* 9.5 ns delay. */
#define CYCLK_10_5NS            0x09 /* 10.5 ns delay. */
#define CYCLK_11_5NS            0x0A /* 11.5 ns delay. */
#define CYCLK_12_5NS            0x0B /* 12.5 ns delay. */
#endif


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define ADC_Ext_CP_Clk_CLKEN              (* (reg8 *) ADC_Ext_CP_Clk__PM_ACT_CFG)
#define ADC_Ext_CP_Clk_CLKEN_PTR          ((reg8 *) ADC_Ext_CP_Clk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define ADC_Ext_CP_Clk_CLKSTBY            (* (reg8 *) ADC_Ext_CP_Clk__PM_STBY_CFG)
#define ADC_Ext_CP_Clk_CLKSTBY_PTR        ((reg8 *) ADC_Ext_CP_Clk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define ADC_Ext_CP_Clk_DIV_LSB            (* (reg8 *) ADC_Ext_CP_Clk__CFG0)
#define ADC_Ext_CP_Clk_DIV_LSB_PTR        ((reg8 *) ADC_Ext_CP_Clk__CFG0)
#define ADC_Ext_CP_Clk_DIV_PTR            ((reg16 *) ADC_Ext_CP_Clk__CFG0)

/* Clock MSB divider configuration register. */
#define ADC_Ext_CP_Clk_DIV_MSB            (* (reg8 *) ADC_Ext_CP_Clk__CFG1)
#define ADC_Ext_CP_Clk_DIV_MSB_PTR        ((reg8 *) ADC_Ext_CP_Clk__CFG1)

/* Mode and source configuration register */
#define ADC_Ext_CP_Clk_MOD_SRC            (* (reg8 *) ADC_Ext_CP_Clk__CFG2)
#define ADC_Ext_CP_Clk_MOD_SRC_PTR        ((reg8 *) ADC_Ext_CP_Clk__CFG2)

#if defined(ADC_Ext_CP_Clk__CFG3)
/* Analog clock phase configuration register */
#define ADC_Ext_CP_Clk_PHASE              (* (reg8 *) ADC_Ext_CP_Clk__CFG3)
#define ADC_Ext_CP_Clk_PHASE_PTR          ((reg8 *) ADC_Ext_CP_Clk__CFG3)
#endif


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define ADC_Ext_CP_Clk_CLKEN_MASK         ADC_Ext_CP_Clk__PM_ACT_MSK
#define ADC_Ext_CP_Clk_CLKSTBY_MASK       ADC_Ext_CP_Clk__PM_STBY_MSK

/* CFG2 field masks */
#define ADC_Ext_CP_Clk_SRC_SEL_MSK        ADC_Ext_CP_Clk__CFG2_SRC_SEL_MASK
#define ADC_Ext_CP_Clk_MODE_MASK          (~(ADC_Ext_CP_Clk_SRC_SEL_MSK))

#if defined(ADC_Ext_CP_Clk__CFG3)
/* CFG3 phase mask */
#define ADC_Ext_CP_Clk_PHASE_MASK         ADC_Ext_CP_Clk__CFG3_PHASE_DLY_MASK
#endif

#endif /* CY_CLOCK_ADC_Ext_CP_Clk_H */


/* [] END OF FILE */
