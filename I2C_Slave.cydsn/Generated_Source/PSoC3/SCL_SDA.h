/*******************************************************************************
* File Name: SCL_SDA.h  
* Version 1.60
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PINS_SCL_SDA_H) /* Pins SCL_SDA_H */
#define CY_PINS_SCL_SDA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SCL_SDA_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    SCL_SDA_Write(uint8 value) ;
void    SCL_SDA_SetDriveMode(uint8 mode) ;
uint8   SCL_SDA_ReadDataReg(void) ;
uint8   SCL_SDA_Read(void) ;
uint8   SCL_SDA_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define SCL_SDA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define SCL_SDA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define SCL_SDA_DM_RES_UP          PIN_DM_RES_UP
#define SCL_SDA_DM_RES_DWN         PIN_DM_RES_DWN
#define SCL_SDA_DM_OD_LO           PIN_DM_OD_LO
#define SCL_SDA_DM_OD_HI           PIN_DM_OD_HI
#define SCL_SDA_DM_STRONG          PIN_DM_STRONG
#define SCL_SDA_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define SCL_SDA_MASK               SCL_SDA__MASK
#define SCL_SDA_SHIFT              SCL_SDA__SHIFT
#define SCL_SDA_WIDTH              2u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SCL_SDA_PS                     (* (reg8 *) SCL_SDA__PS)
/* Data Register */
#define SCL_SDA_DR                     (* (reg8 *) SCL_SDA__DR)
/* Port Number */
#define SCL_SDA_PRT_NUM                (* (reg8 *) SCL_SDA__PRT) 
/* Connect to Analog Globals */                                                  
#define SCL_SDA_AG                     (* (reg8 *) SCL_SDA__AG)                       
/* Analog MUX bux enable */
#define SCL_SDA_AMUX                   (* (reg8 *) SCL_SDA__AMUX) 
/* Bidirectional Enable */                                                        
#define SCL_SDA_BIE                    (* (reg8 *) SCL_SDA__BIE)
/* Bit-mask for Aliased Register Access */
#define SCL_SDA_BIT_MASK               (* (reg8 *) SCL_SDA__BIT_MASK)
/* Bypass Enable */
#define SCL_SDA_BYP                    (* (reg8 *) SCL_SDA__BYP)
/* Port wide control signals */                                                   
#define SCL_SDA_CTL                    (* (reg8 *) SCL_SDA__CTL)
/* Drive Modes */
#define SCL_SDA_DM0                    (* (reg8 *) SCL_SDA__DM0) 
#define SCL_SDA_DM1                    (* (reg8 *) SCL_SDA__DM1)
#define SCL_SDA_DM2                    (* (reg8 *) SCL_SDA__DM2) 
/* Input Buffer Disable Override */
#define SCL_SDA_INP_DIS                (* (reg8 *) SCL_SDA__INP_DIS)
/* LCD Common or Segment Drive */
#define SCL_SDA_LCD_COM_SEG            (* (reg8 *) SCL_SDA__LCD_COM_SEG)
/* Enable Segment LCD */
#define SCL_SDA_LCD_EN                 (* (reg8 *) SCL_SDA__LCD_EN)
/* Slew Rate Control */
#define SCL_SDA_SLW                    (* (reg8 *) SCL_SDA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SCL_SDA_PRTDSI__CAPS_SEL       (* (reg8 *) SCL_SDA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SCL_SDA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SCL_SDA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SCL_SDA_PRTDSI__OE_SEL0        (* (reg8 *) SCL_SDA__PRTDSI__OE_SEL0) 
#define SCL_SDA_PRTDSI__OE_SEL1        (* (reg8 *) SCL_SDA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SCL_SDA_PRTDSI__OUT_SEL0       (* (reg8 *) SCL_SDA__PRTDSI__OUT_SEL0) 
#define SCL_SDA_PRTDSI__OUT_SEL1       (* (reg8 *) SCL_SDA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SCL_SDA_PRTDSI__SYNC_OUT       (* (reg8 *) SCL_SDA__PRTDSI__SYNC_OUT) 


#if defined(SCL_SDA__INTSTAT)  /* Interrupt Registers */

    #define SCL_SDA_INTSTAT                (* (reg8 *) SCL_SDA__INTSTAT)
    #define SCL_SDA_SNAP                   (* (reg8 *) SCL_SDA__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins SCL_SDA_H */


/* [] END OF FILE */
