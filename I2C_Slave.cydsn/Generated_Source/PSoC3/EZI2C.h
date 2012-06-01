/*******************************************************************************
* File Name: EZI2C.h
* Version 1.60
*
* Description:
*  This is the header file for the EzI2C user module.  It contains function
*  prototypes and constants for the users convenience.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_EZI2C_EZI2C_H)
#define CY_EZI2C_EZI2C_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define EZI2C_ADDRESSES         (1u)
#define EZI2C_ONE_ADDRESS       (0x01u)
#define EZI2C_TWO_ADDRESSES     (0x02u)

/* PSoC3 ES2 or early */
#define EZI2C_PSOC3_ES2   ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                     			      (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
                     
/* PSoC5 ES1 or early */
#define EZI2C_PSOC5_ES1   ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                     		    	  (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define EZI2C_PSOC3_ES3   ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                 	  (CYDEV_CHIP_REVISION_USED >= CYDEV_CHIP_REVISION_3A_ES3))
                     
/* PSoC5 ES2 or later*/
#define EZI2C_PSOC5_ES2   ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                  	  (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


#if((EZI2C_PSOC3_ES2) && (EZI2C_isr__ES2_PATCH))
        #include <intrins.h>
        #define EZI2C_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
#endif /* End of ((EZI2C_PSOC3_ES2) && (EZI2C_isr__ES2_PATCH)) */


/***************************************
*   Data Struct Definition
***************************************/

/* Low power modes API Support */
typedef struct _EZI2C_backupStruct
{
    uint8   enableState;
    
    uint8   xcfg;
    uint8   adr;
    uint8   cfg;
    
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)
        
        uint8   clkDiv;
        
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)
        
        uint8   clkDiv1;
        uint8   clkDiv2;
        
    #endif /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */
}   EZI2C_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void    EZI2C_Start(void);
void    EZI2C_Stop(void);
void    EZI2C_EnableInt(void) ;
void    EZI2C_DisableInt(void) ;

#if(EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)
    void EZI2C_SetAddress1(uint8 address);
#else
    void EZI2C_SetAddress1(uint8 address) ;
#endif /* End of (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES) */

uint8   EZI2C_GetAddress1(void);
void    EZI2C_SetBuffer1(uint16 bufSize, uint16 rwBoundry, void * dataPtr);
uint8   EZI2C_GetActivity(void);
void    EZI2C_Init(void);
void    EZI2C_Enable(void);
void    EZI2C_Sleep(void);
void    EZI2C_Wakeup(void);
void    EZI2C_SaveConfig(void);
void    EZI2C_RestoreConfig(void);

CY_ISR_PROTO(EZI2C_ISR);

#if(EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS)

    void    EZI2C_SlaveSetSleepMode(void) ;
    void    EZI2C_SlaveSetWakeMode(void) ;

#endif /* End of (EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS)*/

#if(EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)

    void    EZI2C_SetAddress2(uint8 address);
    uint8   EZI2C_GetAddress2(void);
    void    EZI2C_SetBuffer2(uint16 bufSize, uint16 rwBoundry, void * dataPtr);

#endif  /* End of (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES) */


/***************************************
*   Initial Parameter Constants
***************************************/

#define EZI2C_DEFAULT_ADDR1      (4u)
#define EZI2C_DEFAULT_ADDR2      (5u)
#define EZI2C_ENABLE_WAKEUP      (0u)
#define EZI2C_BUS_SPEED          (400u)
#define EZI2C_SUBADDR_WIDTH      (0u)
#define EZI2C_BUS_PORT           (0u)


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated type*/
#define EZI2C__ANY 0
#define EZI2C__I2C0 1
#define EZI2C__I2C1 2



/***************************************
*              API Constants
***************************************/

/* Status bit definition */

/* A read addr 1 operation occured since last status check */
#define EZI2C_STATUS_READ1       (0x01u)

/* A Write addr 1 opereation occured since last status check */
#define EZI2C_STATUS_WRITE1      (0x02u)

/* A read addr 2 operation occured since last status check */
#define EZI2C_STATUS_READ2       (0x04u)

/* A Write addr 2 opereation occured since last status check */
#define EZI2C_STATUS_WRITE2      (0x08u)

/* A start has occured, but a Stop has not been detected */
#define EZI2C_STATUS_BUSY        (0x10u)

/* Addr 1 read busy */
#define EZI2C_STATUS_RD1BUSY     (0x11u)

/* Addr 1 write busy */
#define EZI2C_STATUS_WR1BUSY     (0x12u)

/* Addr 2 read busy */
#define EZI2C_STATUS_RD2BUSY     (0x14u)

/* Addr 2 write busy */
#define EZI2C_STATUS_WR2BUSY     (0x18u)

/* Mask for status bits. */
#define EZI2C_STATUS_MASK        (0x1Fu)

/* An Error occured since last read */
#define EZI2C_STATUS_ERR         (0x80u)

/* Dummy data to be sent to master */
#define EZI2C_DUMMY_DATA         (0xFFu)

/* The I2C Master bits in I2C cinfiguration register */
#define EZI2C_I2C_MASTER_MASK    (0xDDu)

/* Component's enable/disable state */
#define EZI2C_ENABLED            (0x01u)
#define EZI2C_DISABLED           (0x00u)


/* PSoC3 ES2 or early, PSoC5 ES1*/
#if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)

    /* Samples per bit. Other value is not used */
    #define EZI2C_16_SAMPLES_PER_BIT    (16u)

    /* Default CLK Divider = 
    *  BusClock / (I2C_Bus_Speed * EZI2C_16_SAMPLES_PER_BIT) 
    */
    #define EZI2C_DEFAULT_CLKDIV (BCLK__BUS_CLK__KHZ / \
       (EZI2C_BUS_SPEED * EZI2C_16_SAMPLES_PER_BIT))
    
/* PSoC3 ES3 or later, PSoC5 ES2 or later*/
#elif(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)

    #define EZI2C_BUS_SPEED_50KHZ      (50u)
	
	/* Bus speed grater 50kHz requires 16 oversample rate */
	#if (EZI2C_BUS_SPEED <= EZI2C_BUS_SPEED_50KHZ)

		#define EZI2C_OVER_SAMPLE_RATE       (32u)

	#else

		#define EZI2C_OVER_SAMPLE_RATE       (16u)

	#endif  /* End (EZI2C_BUS_SPEED <= EZI2C_BUS_SPEED_50KHZ) */
    
	/* Divide factor calculation */
	#define EZI2C_DIVIDE_FACTOR_WITH_FRACT_BYTE  \
                    (((uint32) BCLK__BUS_CLK__KHZ << 8u) / ((uint32)EZI2C_BUS_SPEED * \
                    EZI2C_OVER_SAMPLE_RATE))
                    
	#define EZI2C_DIVIDE_FACTOR  (((uint32) EZI2C_DIVIDE_FACTOR_WITH_FRACT_BYTE) >> 8u)

#endif  /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */

/* Following definitions are for the COMPATIBILITY ONLY, they are OBSOLETE. */
#define EZI2C_State          EZI2C_curState
#define EZI2C_Status         EZI2C_curStatus
#define EZI2C_DataPtr        EZI2C_dataPtrS1

#define EZI2C_RwOffset1      EZI2C_rwOffsetS1
#define EZI2C_RwIndex1       EZI2C_rwIndexS1
#define EZI2C_WrProtect1     EZI2C_wrProtectS1
#define EZI2C_BufSize1       EZI2C_bufSizeS1

#if(EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)

    #define EZI2C_DataPtr2   EZI2C_dataPtrS2
    #define EZI2C_Address1   EZI2C_addrS1
    #define EZI2C_Address2   EZI2C_addrS2

    #define EZI2C_RwOffset2  EZI2C_rwOffsetS2
    #define EZI2C_RwIndex2   EZI2C_rwIndexS2
    #define EZI2C_WrProtect2 EZI2C_wrProtectS2
    #define EZI2C_BufSize2   EZI2C_bufSizeS2

#endif /* EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES */

/* Returns 1 if corresponding bit is set, otherwise 0 */
#define EZI2C_IS_BIT_SET(value, mask) (((mask) == ((value) & (mask))) ? 0x01u : 0x00u)

#define EZI2C_ADDRESS_SHIFT      (1u)
#define EZI2C_ADDRESS_LSB_SHIFT  (8u)

/* 8-bit sub-address width */
#define EZI2C_SUBADDR_8BIT       (0x00u)

/* 16-bit sub-address width */
#define EZI2C_SUBADDR_16BIT      (0x01u)


/***************************************
*              Registers
***************************************/

/* I2C Extended Configuration Register */
#define EZI2C_XCFG_REG       (* (reg8 *) EZI2C_I2C_Prim__XCFG )
#define EZI2C_XCFG_PTR       (  (reg8 *) EZI2C_I2C_Prim__XCFG )

/* I2C Slave Adddress Register */
#define EZI2C_ADDR_REG       (* (reg8 *) EZI2C_I2C_Prim__ADR )
#define EZI2C_ADDR_PTR       (  (reg8 *) EZI2C_I2C_Prim__ADR )

/* I2C Configuration Register */
#define EZI2C_CFG_REG        (* (reg8 *) EZI2C_I2C_Prim__CFG )
#define EZI2C_CFG_PTR        (  (reg8 *) EZI2C_I2C_Prim__CFG )

/* I2C Control and Status Register */
#define EZI2C_CSR_REG        (* (reg8 *) EZI2C_I2C_Prim__CSR )
#define EZI2C_CSR_PTR        (  (reg8 *) EZI2C_I2C_Prim__CSR )

/* I2C Data Register */
#define EZI2C_DATA_REG       (* (reg8 *) EZI2C_I2C_Prim__D )
#define EZI2C_DATA_PTR       (  (reg8 *) EZI2C_I2C_Prim__D )

/* PSoC3 ES2 or early, PSoC5 ES1*/
#if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)

    /* I2C Clock Divide Factor Register */
    #define EZI2C_CLKDIV_REG         (* (reg8 *) EZI2C_I2C_Prim__CLK_DIV )
    #define EZI2C_CLKDIV_PTR         (  (reg8 *) EZI2C_I2C_Prim__CLK_DIV )

/* PSoC3 ES3 or later, PSoC5 ES2 or later*/
#elif(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)

     /*  8 LSB bits of the 10-bit Clock Divider */
    #define EZI2C_CLKDIV1_REG        (* (reg8 *) EZI2C_I2C_Prim__CLK_DIV1 )
    #define EZI2C_CLKDIV1_PTR        (  (reg8 *) EZI2C_I2C_Prim__CLK_DIV1 )

    /* 2 MSB bits of the 10-bit Clock Divider */
    #define EZI2C_CLKDIV2_REG        (* (reg8 *) EZI2C_I2C_Prim__CLK_DIV2 )
    #define EZI2C_CLKDIV2_PTR        (  (reg8 *) EZI2C_I2C_Prim__CLK_DIV2 )

#endif  /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */

/* Power System Control Register 1 */
#define EZI2C_PWRSYS_CR1_REG     (* (reg8 *) CYREG_PWRSYS_CR1 )
#define EZI2C_PWRSYS_CR1_PTR     (  (reg8 *) CYREG_PWRSYS_CR1 )

/* I2C operation in Active Mode */
#define EZI2C_PM_ACT_CFG_REG     (* (reg8 *) EZI2C_I2C_Prim__PM_ACT_CFG )
#define EZI2C_PM_ACT_CFG_PTR     (  (reg8 *) EZI2C_I2C_Prim__PM_ACT_CFG )

/* I2C operation in Alternate Active (Standby) Mode */
#define EZI2C_PM_STBY_CFG_REG    (* (reg8 *) EZI2C_I2C_Prim__PM_STBY_CFG )
#define EZI2C_PM_STBY_CFG_PTR    (  (reg8 *) EZI2C_I2C_Prim__PM_STBY_CFG )


/***************************************
*       Register Constants
***************************************/

/* XCFG I2C Extended Configuration Register */
#define EZI2C_XCFG_CLK_EN            (0x80u)
#define EZI2C_XCFG_HDWR_ADDR_EN      (0x01u)

/* PSoC3 ES3 or later, PSoC5 ES2 or later*/
#if(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)

    /* Force nack */
    #define EZI2C_XCFG_FORCE_NACK            (0x10u)
    
    /* Ready to sleep */
    #define EZI2C_XCFG_SLEEP_READY           (0x20u)
    
    /* if I2C block will be used as wake up source */
    #if(EZI2C_ENABLE_WAKEUP == 1u)

        /* Should be set before entering sleep mode */
        #define EZI2C_XCFG_I2C_ON            (0x40u)

        /* Enables the I2C regulator backup */
        #define EZI2C_PWRSYS_CR1_I2C_BACKUP  (0x04u)

    #endif  /* End of (EZI2C_ENABLE_WAKEUP == 1u) */

#endif  /* End of (EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2) */

/* Data I2C Slave Data Register */
#define EZI2C_SADDR_MASK        (0x7Fu)
#define EZI2C_DATA_MASK         (0xFFu)
#define EZI2C_READ_FLAG         (0x01u)

/* CFG I2C Configuration Register */

/* Pin Select for SCL/SDA lines */
#define EZI2C_CFG_SIO_SELECT    (0x80u)

/* Pin Select */
#define EZI2C_CFG_PSELECT       (0x40u)

/* Bus Error Interrupt Enable */
#define EZI2C_CFG_BUS_ERR_IE    (0x20u)

/* Enable Interrupt on STOP condition */
#define EZI2C_CFG_STOP_IE       (0x10u)

/* Enable Interrupt on STOP condition */
#define EZI2C_CFG_STOP_ERR_IE   (0x10u)


/* PSoC3 ES2 or early, PSoC5 ES1*/
#if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)

    /* Clock rate select */
    #define EZI2C_CFG_CLK_RATE_MSK  (0x0Cu)

    /* Clock rate select 100K */
    #define EZI2C_CFG_CLK_RATE_100  (0x00u)

    /* Clock rate select 400K */
    #define EZI2C_CFG_CLK_RATE_400  (0x04u)

    /* Clock rate select 50K */
    #define EZI2C_CFG_CLK_RATE_050  (0x08u)

    /* Clock rate select Invalid */
    #define EZI2C_CFG_CLK_RATE_RSVD (0x0Cu)

/* PSoC3 ES3 or later, PSoC5 ES2 or later*/
#elif(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)

    /* Clock rate mask. 1 for 50K, 0 for 100K and 400K */
    #define EZI2C_CFG_CLK_RATE      (0x04u)

#endif  /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */

/* Enable Slave operation */
#define EZI2C_CFG_EN_SLAVE      (0x01u)

/* CSR I2C Control and Status Register */

/* Active high when bus error has occured */
#define EZI2C_CSR_BUS_ERROR     (0x80u)

/* Set to 1 if lost arbitration in host mode */
#define EZI2C_CSR_LOST_ARB      (0x40u)

/* Set to 1 if Stop has been detected */
#define EZI2C_CSR_STOP_STATUS   (0x20u)

/* ACK response */
#define EZI2C_CSR_ACK           (0x10u)

/* NAK response */
#define EZI2C_CSR_NAK           (0x00u)

/* Set in firmware 0 = status bit, 1 Address is slave */
#define EZI2C_CSR_ADDRESS       (0x08u)

/* Set in firmware 1 = transmit, 0 = receive. */
#define EZI2C_CSR_TRANSMIT      (0x04u)

/* Last received bit. */
#define EZI2C_CSR_LRB           (0x02u)

 /* Last received bit was an ACK */
#define EZI2C_CSR_LRB_ACK       (0x00u)

/* Last received bit was an NAK */
#define EZI2C_CSR_LRB_NAK       (0x02u)

/* Informs that last byte has been sent. */
#define EZI2C_CSR_BYTE_COMPLETE (0x01u)

/* CLK_DIV I2C Clock Divide Factor Register */

/* Status bit, Set at Start and cleared at Stop condition */
#define EZI2C_CLK_DIV_MSK       (0x07u)

/* Divide input clock by  1 */
#define EZI2C_CLK_DIV_1         (0x00u)

/* Divide input clock by  2 */
#define EZI2C_CLK_DIV_2         (0x01u)

/* Divide input clock by  4 */
#define EZI2C_CLK_DIV_4         (0x02u)

/* Divide input clock by  8 */
#define EZI2C_CLK_DIV_8         (0x03u)

/* Divide input clock by 16 */
#define EZI2C_CLK_DIV_16        (0x04u)

/* Divide input clock by 32 */
#define EZI2C_CLK_DIV_32        (0x05u)

/* Divide input clock by 64 */
#define EZI2C_CLK_DIV_64        (0x06u)

/* Active Power Mode CFG Register - power enable mask */
#define EZI2C_ACT_PWR_EN    EZI2C_I2C_Prim__PM_ACT_MSK

/* Alternate Active (Standby) Power Mode CFG Register - power enable mask */
#define EZI2C_STBY_PWR_EN    EZI2C_I2C_Prim__PM_STBY_MSK

/* Number of the EZI2C_isr interrupt. */
#define EZI2C_ISR_NUMBER    EZI2C_isr__INTC_NUMBER

/* Priority of the EZI2C_isr interrupt. */
#define EZI2C_ISR_PRIORITY  EZI2C_isr__INTC_PRIOR_NUM

/* I2C state machine constants */

/* Wait for Start */
#define  EZI2C_SM_IDLE              (0x00u)

/* Default address states */

/* Wait for sub-address */
#define  EZI2C_SM_DEV1_WR_ADDR      (0x01u)

/* Wait for sub-address MSB */
#define  EZI2C_SM_DEV1_WR_ADDR_MSB  (0x01u)

/* Wait for sub-address LSB */
#define  EZI2C_SM_DEV1_WR_ADDR_LSB  (0x02u)

/* Get data from Master */
#define  EZI2C_SM_DEV1_WR_DATA      (0x04u)

/* Send data to Master */
#define  EZI2C_SM_DEV1_RD_DATA      (0x08u)

/* Second address states */

/* Wait for sub-address */
#define  EZI2C_SM_DEV2_WR_ADDR      (0x11u)

/* Wait for sub-address MSB */
#define  EZI2C_SM_DEV2_WR_ADDR_MSB  (0x11u)

/* Wait for sub-address LSB */
#define  EZI2C_SM_DEV2_WR_ADDR_LSB  (0x12u)

/* Get data from Master */
#define  EZI2C_SM_DEV2_WR_DATA      (0x14u)

/* Send data to Master */
#define  EZI2C_SM_DEV2_RD_DATA      (0x18u)

#endif /* CY_EZI2C_EZI2C_H */


/* [] END OF FILE */
