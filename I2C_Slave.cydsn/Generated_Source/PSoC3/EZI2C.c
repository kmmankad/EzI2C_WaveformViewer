/*******************************************************************************
* File Name: EZI2C.c
* Version 1.60
*
* Description:
*  This file contains the setup, control and status commands for the EzI2C
*  component.  Actual protocol and operation code resides in the interrupt
*  service routine file.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EZI2C.h"
#include "CyLib.h"

/* Current state of I2C state machine */
extern volatile uint8 EZI2C_curState;

/* Status byte */
extern volatile uint8 EZI2C_curStatus;

/* Pointer to data exposed to I2C Master */
extern volatile uint8 * EZI2C_dataPtrS1;

#if(EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT)

    /* Offset for read and write operations, set at each write sequence */
    extern volatile uint8   EZI2C_rwOffsetS1;

    /* Points to next value to be read or written */
    extern volatile uint8   EZI2C_rwIndexS1;

    /* Offset where data is read only */
    extern volatile uint8   EZI2C_wrProtectS1;

    /* Size of array between 1 and 255 */
    extern volatile uint8   EZI2C_bufSizeS1;

#else   /* 16 bit sub-address */

    /* Offset for read and write operations, set at each write sequence */
    extern volatile uint16  EZI2C_rwOffsetS1;

    /* Points to next value to be read or written */
    extern volatile uint16  EZI2C_rwIndexS1;

    /* Offset where data is read only */
    extern volatile uint16  EZI2C_wrProtectS1;

    /* Size of array between 1 and 65535 */
    extern volatile uint16  EZI2C_bufSizeS1;

#endif  /* End of (EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT) */


#if(EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)

    /* Pointer to data exposed to I2C Master */
    extern volatile uint8 * EZI2C_dataPtrS2;

    /* Software address compare 1 */
    extern volatile uint8   EZI2C_addrS1;

    /* Software address compare 2 */
    extern volatile uint8   EZI2C_addrS2;

    #if(EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT)

        /* Offset for read and write operations, set at each write sequence */
        extern volatile uint8   EZI2C_rwOffsetS2;

        /* Points to next value to be read or written */
        extern volatile uint8   EZI2C_rwIndexS2;

        /* Offset where data is read only */
        extern volatile uint8   EZI2C_wrProtectS2;

        /* Size of array between 1 and 255 */
        extern volatile uint8   EZI2C_bufSizeS2;

    #else /* 16 bit subaddress */

        /* Offset for read and write operations, set at each write sequence */
        extern volatile uint16  EZI2C_rwOffsetS2;

        /* Points to next value to be read or written */
        extern volatile uint16  EZI2C_rwIndexS2;

        /* Offset where data is read only */
        extern volatile uint16  EZI2C_wrProtectS2;

        /* Size of array between 1 and 65535 */
        extern volatile uint16  EZI2C_bufSizeS2;

    #endif /* End of (EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT) */

#endif /* End of (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES) */

uint8 EZI2C_initVar = 0u;


/*******************************************************************************
* Function Name: EZI2C_Start
********************************************************************************
*
* Summary:
*  Starts the component and enables the interupt. If this function is called at
*  first (or EZI2C_initVar was cleared, then EZI2C_Init()
*  function is called and all offsets and pointers are reset. Anyway, the 
*  state machine state is set to IDLE, status variable is cleared and the
*  interrupt is enabled.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_initVar - is used to indicate initial configuration
*  of this component.  The variable is initialized to zero and set to 1
*  the first time EZI2C_Start() is called. This allows for component
*  initialization without re-initialization in all subsequent calls
*  to the EZI2C_Start() routine. 
*
*  EZI2C_dataPtrS1 global variable, which stores pointer to the
*  data exposed to an I2C master for the first slave address is reset if
*  EZI2C_initVar is set 0 by EZI2C_initVar function call.
*
*  EZI2C_rwOffsetS1 - global variable, which stores offset for read
*  and write operations, is set at each write sequence of the first slave
*  address is reset if EZI2C_initVar is 0, by 
*  EZI2C_initVar function call.
*
*  EZI2C_rwIndexS1 - global variable, which stores pointer to the
*  next value to be read or written for the first slave address is reset if
*  EZI2C_initVar is 0, by EZI2C_initVar function call.
*
* EZI2C_wrProtectS1 - global variable, which stores offset where data
*  is read only for the first slave address is reset if
*  EZI2C_initVar is 0, by EZI2C_initVar function call.
*
* EZI2C_bufSizeS1 - global variable, which stores size of data array
*  exposed to an I2C master for the first slave address is reset if
*  EZI2C_initVar is 0, by EZI2C_initVar function call.
*
*  EZI2C_dataPtrS2 - global variable, which stores pointer to the
*  data exposed to an I2C master for the second slave address is reset if
*  EZI2C_initVar is 0, by EZI2C_initVar function call.
*
*  EZI2C_rwOffsetS2 - global variable, which stores offset for read
*  and write operations, is set at each write sequence of the second slave
*  device is reset if EZI2C_initVar is 0, by EZI2C_initVar
*  function call.
*
*  EZI2C_rwIndexS2 - global variable, which stores pointer to the 
*  next value to be read or written for the second slave address is reset if
*  EZI2C_initVar is 0, by EZI2C_initVar function call.
*
* EZI2C_wrProtectS2 - global variable, which stores offset where data
*  is read only for the second slave address is reset if
*  EZI2C_initVar is 0, by EZI2C_initVar function call.
*
* EZI2C_bufSizeS2 - global variable, which stores size of data array
*  exposed to an I2C master for the second slave address is reset if
*  EZI2C_initVar is 0, by EZI2C_initVar function call.
*
* Side Effects:
*  This component automatically enables it's interrupt. If I2C is enabled
*  without the interrupt enabled, it could lock up the I2C bus.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EZI2C_Start(void)
{
    if(0u == EZI2C_initVar)
    {
        /* Initialize component's parameters */
        EZI2C_Init();

        /* Set init flag */
        EZI2C_initVar = 1u;
    }

    /* Enable slave mode for the device */
    EZI2C_Enable();
}


/*******************************************************************************
* Function Name: EZI2C_Stop
********************************************************************************
*
* Summary:
*  Disable the I2C block's slave operation and the corresponding interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void EZI2C_Stop(void) 
{
    uint8 interruptState;
    
    /* Disable Interrupt */
    EZI2C_DisableInt();

    /* Enter critical section */
    interruptState = CyEnterCriticalSection();

    /* Disable I2C block in Active mode template */
    EZI2C_PM_ACT_CFG_REG &= ~EZI2C_ACT_PWR_EN;
    
    /* Disable I2C block in Alternate Active (Standby) mode template */
    EZI2C_PM_STBY_CFG_REG &= ~EZI2C_STBY_PWR_EN;
    
    /* Exit critical section */
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: EZI2C_EnableInt
********************************************************************************
*
* Summary:
*  Enables the interrupt service routine for the component.  This is normally
*  handled with the start command.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void EZI2C_EnableInt(void) 
{
    /* Enable interrupt */
    CyIntEnable(EZI2C_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: EZI2C_DisableInt
********************************************************************************
*
* Summary:
*  Disable I2C interrupts. Normally this function is not required since the
*  Stop function disables the interrupt. If the I2C interrupt is disabled while
*  the I2C master is still running, it may cause the I2C bus to lock up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  If the I2C interrupt is disabled and the master is addressing the current
*  slave, the bus will be locked until the interrupt is re-enabled.
*
*******************************************************************************/
void EZI2C_DisableInt(void) 
{
    /* Disable interrupt */
    CyIntDisable(EZI2C_ISR_NUMBER);
}

#if(EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)
    /*******************************************************************************
    * Function Name: EZI2C_SetAddress1
    ********************************************************************************
    *
    * Summary:
    *  This function sets the main address of this I2C slave device. This value may
    *  be any value between 0 and 127.
    *
    * Parameters:
    *  address:  The 7-bit slave address between 0 and 127.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  EZI2C_addrS1 - the new slave address for the first slave device is
    *  saved in it, if the component is configured to act as two slave devices.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void EZI2C_SetAddress1(uint8 address)
    {
        /* Address is stored in variable */
        EZI2C_addrS1  = address & EZI2C_SADDR_MASK;
    }

#else

    /*******************************************************************************
    * Function Name: EZI2C_SetAddress1
    ********************************************************************************
    *
    * Summary:
    *  This function sets the main address of this I2C slave device. This value may
    *  be any value between 0 and 127.
    *
    * Parameters:
    *  address:  The 7-bit slave address between 0 and 127.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void EZI2C_SetAddress1(uint8 address) 
    {
        /* Address is stored in hardware */
        EZI2C_ADDR_REG = address & EZI2C_SADDR_MASK;
    }

#endif /* End of (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES) */



/*******************************************************************************
* Function Name: EZI2C_GetAddress1
********************************************************************************
*
* Summary:
*  Returns address of the first slave device.
*
* Parameters:
*  None.
*
* Return:
*  If the component is configured to has two slave addresses than primary
*  address is returned, otherwise address from the the address register is
*  returned.
*
* Global variables:
*  EZI2C_addrS1 - if component is configured to has two slave
*  addresses than primary address is saved here, otherwise address is written to
*  the register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 EZI2C_GetAddress1(void)
{
    /* Get 1st slave address */
    #if(EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)

        /* Return address from variable */
        return(EZI2C_addrS1);

    #else

        /* Return address from hardware */
        return(EZI2C_ADDR_REG);

    #endif /* End of (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES) */
}


/*******************************************************************************
* Function Name: EZI2C_GetActivity
********************************************************************************
*
* Summary:
*  This function returns a nonzero value if the I2C read or write cycle
*  occurred since the last time this function was called.  The activity
*  flag resets to zero at the end of this function call.
*  The Read and Write busy flags are cleared when read, but the "BUSY"
*  flag is only cleared by an I2C Stop.
*
* Parameters:
*  None.
*
* Return:
*  A non-zero value is returned if activity is detected:
*   EZI2C_STATUS_READ1   Set if Read sequence is detected for first
*                                   address. Cleared when status read.
*
*   EZI2C_STATUS_WRITE1  Set if Write sequence is detected for first
*                                   address. Cleared when status read.
*
*   EZI2C_STATUS_READ2   Set if Read sequence is detected for second
*                                   address (if enabled). Cleared when status 
*                                   read.
*
*   EZI2C_STATUS_WRITE2  Set if Write sequence is detected for second 
*                                   address (if enabled). Cleared when status
*                                   read.
*
*   EZI2C_STATUS_BUSY    Set if Start detected, cleared when stop 
*                                   detected.
*
*   EZI2C_STATUS_ERR     Set when I2C hardware detected, cleared
*                                   when status read.
*
* Global variables:
*  EZI2C_curStatus - global variable, which stores the current
*  component status.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 EZI2C_GetActivity(void)
{
    uint8 tmpStatus;

    tmpStatus = EZI2C_curStatus;

    /* Clear status, but no Busy one */
    EZI2C_curStatus &= EZI2C_STATUS_BUSY;

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: EZI2C_SetBuffer1
********************************************************************************
*
* Summary:
*  This function sets the buffer, size of the buffer, and the R/W boundry
*  for the memory buffer.
*
* Parameters:
*  size:  Size of the buffer in bytes.
*
*  rwBoundry: Sets how many bytes are writable in the beginning of the buffer.
*  This value must be less than or equal to the buffer size.
*
*  dataPtr:  Pointer to the data buffer.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_dataPtrS1 - stores pointer to the data exposed to an I2C
*  master for the first slave address is modified with the the new pointer to
*  data, passed by function parameter.
*
*  EZI2C_rwOffsetS1 - stores offset for read and write operations, is
*  modified at each write sequence of the first slave address is reset.
*
*  EZI2C_rwIndexS1 - stores pointer to the next value to be read or
*  written for the first slave address is set to 0.
*
* Reentrant:
*  No.
*
* Side Effects:
*  It is recommended to disable component interrupt before calling this function
*  and enable it afterwards for the proper component operation.
*
*******************************************************************************/
void EZI2C_SetBuffer1(uint16 bufSize, uint16 rwBoundry, void * dataPtr)
{
    /* Set pointer to data and clear index and offset. */
    EZI2C_dataPtrS1   = dataPtr;
    EZI2C_rwOffsetS1  = 0u;
    EZI2C_rwIndexS1   = 0u;

    #if(EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT)
        EZI2C_bufSizeS1   = (uint8) bufSize;
        EZI2C_wrProtectS1 = (uint8) rwBoundry;
    #else
        EZI2C_bufSizeS1   = bufSize;
        EZI2C_wrProtectS1 = rwBoundry;
    #endif /* End of (EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT) */
}


#if (EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS)
    /*******************************************************************************
    * Function Name: EZI2C_SlaveSetSleepMode
    ********************************************************************************
    *
    * Summary:
    *  Disables the run time I2C regulator and enables the sleep Slave I2C.
    *  Should be called just prior to entering sleep. This function is only
    *  provided if a single I2C address is used.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  The I2C interrupt will be disabled if Wake up from Sleep mode option is
    *  enabled (only for PSoC3 ES3).
    *
    *******************************************************************************/
    void EZI2C_SlaveSetSleepMode(void) 
    {
        /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
        #if(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)
           
            /* if I2C block will be used as wake up source */
            #if(EZI2C_ENABLE_WAKEUP == 1u)
                
                uint8 interruptState;
                
                /* Enter critical section */
                interruptState = CyEnterCriticalSection();
                
                /* Enable the I2C regulator backup */
                EZI2C_PWRSYS_CR1_REG |= EZI2C_PWRSYS_CR1_I2C_BACKUP;
                
                /* Exit critical section */
                CyExitCriticalSection(interruptState);
                
                /* Set force nack before putting the device to power off mode.
                *  It is cleared on wake up.
                */
                EZI2C_XCFG_REG |= EZI2C_XCFG_FORCE_NACK;
                while(0u == (EZI2C_XCFG_REG & EZI2C_XCFG_SLEEP_READY))
                {
                    /* Waits for ongoing transaction to be completed. */
                }
                
                /* Disable interrupt for proper wake up procedure */
                EZI2C_DisableInt();
            
            #endif  /* End of (EZI2C_ENABLE_WAKEUP == 1u) */

        #endif  /* End of (EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2) */
    }


    /*******************************************************************************
    * Function Name: EZI2C_SlaveSetWakeMode
    ********************************************************************************
    *
    * Summary:
    *  Disables the sleep EzI2C slave and re-enables the run time I2C.  Should be
    *  called just after awaking from sleep.  Must preserve address to continue.
    *  This function is only provided if a single I2C address is used.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  The I2C interrupt will be enabled if Wake up from Sleep mode option is
    *  enabled (only for PSoC3 ES3).
    *
    *******************************************************************************/
    void EZI2C_SlaveSetWakeMode(void) 
    {
        /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
        #if(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)
            
            /* if I2C block will be used as wake up source */
            #if(EZI2C_ENABLE_WAKEUP == 1u)
                
                uint8 interruptState;
            
                /* Enter critical section */
                interruptState = CyEnterCriticalSection();
                
                /* Disable the I2C regulator backup */
                EZI2C_PWRSYS_CR1_REG &= ~EZI2C_PWRSYS_CR1_I2C_BACKUP;
                
                /* Exit critical section */
                CyExitCriticalSection(interruptState);
                
                /* Enable interrupt. The ISR is supposed to ready to be executed. */
                EZI2C_EnableInt();

            #endif  /* End of (EZI2C_ENABLE_WAKEUP == 1u) */ 

        #endif  /* End of (EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2) */ 
    }
    
#endif /* End of (EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS) */


#if (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)
    /*******************************************************************************
    * Function Name: EZI2C_SetBuffer2
    ********************************************************************************
    *
    * Summary:
    *  This function sets the buffer pointer, size and read/write area for the
    *  second slave data. This is the data that is exposed to the I2C Master
    *  for the second I2C address. This function is only provided if two I2C
    *  addresses have been selected in the user parameters.
    *
    * Parameters:
    *  bufSize:  Size of the buffer exposed to the I2C master.
    *
    *  rwBoundry: Sets how many bytes are readable and writable by the the I2C
    *  master. This value must be less than or equal to the buffer size. Data
    *  located at offset rwBoundry and above are read only.
    *
    *  dataPtr:  This is a pointer to the data array or structure that is used
    *  for the I2C data buffer.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  EZI2C_dataPtrS2 - stores pointer to the data exposed to an I2C
    *  master for the second slave address is modified with the the new pointer to
    *  data, passed by function parameter.
    *
    *  EZI2C_rwOffsetS2 - stores offset for read and write operations,
    *  is modified at each write sequence of the second slave address is reset.
    *
    *  EZI2C_rwIndexS2 - stores pointer to the next value to be read or
    *  written for the second slave address is set to 0.
    *
    * Reentrant:
    *  No.
    *
    * Side Effects:
    *  It is recommended to disable component interrupt before calling this
    *  function and enable it afterwards for the proper component operation.
    *
    *******************************************************************************/
    void EZI2C_SetBuffer2(uint16 bufSize, uint16 rwBoundry, void * dataPtr)
    {
        /* Set pointer to data and clear index and offset. */
        EZI2C_dataPtrS2   = dataPtr;
        EZI2C_rwOffsetS2  = 0u;
        EZI2C_rwIndexS2   = 0u;

        #if(EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT)
            EZI2C_bufSizeS2   = (uint8) bufSize;
            EZI2C_wrProtectS2 = (uint8) rwBoundry;
        #else
            EZI2C_bufSizeS2   = bufSize;
            EZI2C_wrProtectS2 = rwBoundry;
        #endif /* End of (EZI2C_SUBADDR_WIDTH == EZI2C_SUBADDR_8BIT) */
    }


    /*******************************************************************************
    * Function Name: EZI2C_SetAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the I2C slave address for the second device. This value may be any 
    *  value between 0 and 127. This function is only provided if two I2C
    *  addresses have been selected in the user parameters.
    *
    * Parameters:
    *  address:  The 7-bit slave address between 0 and 127.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *  EZI2C_addrS2 - the secondary slave address is modified.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void EZI2C_SetAddress2(uint8 address)
    {
        /* Set slave address */
        EZI2C_addrS2  = address & EZI2C_SADDR_MASK;
    }


    /*******************************************************************************
    * Function Name: EZI2C_GetAddress2
    ********************************************************************************
    *
    * Summary:
    *  Returns the I2C slave address for the second device. This function is only
    *  provided if two I2C addresses have been selected in the user parameters.
    *
    * Parameters:
    *  EZI2C_addrS2 - global variable, which stores the second I2C 
    *   address.
    *
    * Return:
    *  The secondary I2C slave address.
    *
    * Global variables:
    *  EZI2C_addrS2 - the secondary slave address is used.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 EZI2C_GetAddress2(void)
    {
        return(EZI2C_addrS2);
    }

#endif  /* End of (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES) */


/*******************************************************************************
* Function Name: EZI2C_Init
********************************************************************************
*
* Summary:
*  Initializes/restores default EZI2C configuration provided with customizer.
*  Usually called in EZI2C_Start().
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_addrS1 - the new slave address for the first slave device is
*   saved.
*
*  EZI2C_addrS2 - the new slave address for the second slave device
*   is saved, if EzI2C component is configured for two slave addresses.
*
* Reentrant:
*  No.
*
* Side Effects:
*  All changes applied by API to the component's configuration will be reset. 
*
*******************************************************************************/
void EZI2C_Init(void)
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)
    
        uint8 clkSel;
        
    #endif  /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */

    /* Clear Status register */
    EZI2C_CSR_REG  = 0x00u;

    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)
        
        /* Enable I2C block's slave operation */
        EZI2C_CFG_REG |= EZI2C_CFG_EN_SLAVE;
        
        /* The resolution of generated I2C bus SCL frequencies is reduced. */

        /* Temprorary set CLK divider to 16 */
        EZI2C_CLKDIV_REG  = EZI2C_CLK_DIV_16;

        /* Set clock divider for the I2C bus clock to be the closest HIGHER 
        *  clock to ensure it can work with an ideal master clock.
        */
        for(clkSel = 0u; clkSel <= 6u; clkSel++ )
        {
            if((1u << clkSel) >= EZI2C_DEFAULT_CLKDIV)
            {
                EZI2C_CLKDIV_REG  = clkSel;
                break;
            }
        }
        
        /* Set clock rate to Fast mode 400k. This will give the Clock 
        *  divider sufficient range to work bewteen 50 and 400 kbps.
        */
        EZI2C_CFG_REG  |= EZI2C_CFG_CLK_RATE_400;

    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)
    
        /* Enable I2C block's slave operation.
        *  These revisions require slave to be enabled for registers to be
        *  written.
        */
        EZI2C_CFG_REG |= EZI2C_CFG_EN_SLAVE;
        
        /* 8 LSB bits of the 10-bit are written with the divide factor */
		EZI2C_CLKDIV1_REG = LO8(EZI2C_DIVIDE_FACTOR);

        /* 2 MSB bits of the 10-bit are written with the divide factor */
		EZI2C_CLKDIV2_REG = HI8(EZI2C_DIVIDE_FACTOR);
		
        /* Define clock rate */
        if(EZI2C_BUS_SPEED <= EZI2C_BUS_SPEED_50KHZ)
        {   /* 50 kHz - 32 samples/bit */
            EZI2C_CFG_REG |= EZI2C_CFG_CLK_RATE;
        }
        else
        {   /* 100kHz or 400kHz - 16 samples/bit */
            EZI2C_CFG_REG &= ~EZI2C_CFG_CLK_RATE;
        }
        
        /* if I2C block will be used as wake up source, this availabe only for
        * PSoC3 ES3, PSoC5 ES2 and later revisions. Refer to errata.
        */
        #if(1u == EZI2C_ENABLE_WAKEUP)

            /* Configure I2C address match to act as wake-up source */
           EZI2C_XCFG_REG |= EZI2C_XCFG_I2C_ON;

            /* Process sio_select and pselect */
            #if(EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS)
                if(EZI2C__ANY != EZI2C_BUS_PORT)
                {
                    /* SCL and SDA lines get their inputs from SIO block */
                    EZI2C_CFG_REG |= EZI2C_CFG_PSELECT;

                    if(EZI2C__I2C0 == EZI2C_BUS_PORT)
                    {
                        /* SCL and SDA lines get their inputs from SIO1 */
                        EZI2C_CFG_REG &= ~EZI2C_CFG_SIO_SELECT;
                    }
                    else /* SIO2 */
                    {
                        /* SCL and SDA lines get their inputs from SIO2 */
                        EZI2C_CFG_REG |= EZI2C_CFG_SIO_SELECT;
                    }
                }
                else    /* GPIO is used */
                {
                    /* SCL and SDA lines get their inputs from GPIO module. */
                    EZI2C_CFG_REG &= ~EZI2C_CFG_PSELECT;
                }
            #endif  /* End if (EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS) */

        #endif /* End of (1u == EZI2C_ENABLE_WAKEUP) */

    #endif  /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */


    #if(EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS)

        /* Set default slave address */
        EZI2C_ADDR_REG  = EZI2C_DEFAULT_ADDR1;

        /* Turn on hardware address detection */
        EZI2C_XCFG_REG  |= EZI2C_XCFG_HDWR_ADDR_EN;

    #else   /* Two devices */

        /* Set default slave addresses */
        EZI2C_addrS1  = EZI2C_DEFAULT_ADDR1;
        EZI2C_addrS2  = EZI2C_DEFAULT_ADDR2;

    #endif  /* End of (EZI2C_ADDRESSES == EZI2C_ONE_ADDRESS) */
    
    /* Reset offsets and pointers */
    EZI2C_dataPtrS1 = (uint8 *)0x0000u;
    EZI2C_rwOffsetS1 = 0u;
    EZI2C_rwIndexS1 = 0u;
    EZI2C_wrProtectS1 = 0u;
    EZI2C_bufSizeS1 = 0u;

    /* Reset offsets and pointers for 2nd slave address */
    #if(EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES)
        EZI2C_dataPtrS2 = (uint8 *)0x0000u;
        EZI2C_rwOffsetS2 = 0u;
        EZI2C_rwIndexS2 = 0u;
        EZI2C_wrProtectS2 = 0u;
        EZI2C_bufSizeS2 = 0u;
    #endif  /* End of (EZI2C_ADDRESSES == EZI2C_TWO_ADDRESSES) */
    
    /* Enable the I2C block clock */
    EZI2C_XCFG_REG  |= EZI2C_XCFG_CLK_EN;
}


/*******************************************************************************
* Function Name: EZI2C_Enable
********************************************************************************
*
* Summary: 
*  Enables the I2C block operation, sets interrupt priority, sets 
*  interrupt vector, clears pending interrupts and enables interrupts. Clears
*  status variables and reset state machine variable.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_curStatus - this global variable are cleared, it stores the
*  current component status.
*
* EZI2C_curState - global variable are cleared, it stores the current
*  state of the state machine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EZI2C_Enable(void)
{
    uint8 interruptState;
    
    /* Enter critical section */
    interruptState = CyEnterCriticalSection();

    /* Enable I2C block in Active mode template */
    EZI2C_PM_ACT_CFG_REG |= EZI2C_ACT_PWR_EN;
    
    /* Enable I2C block in Alternate Active (Standby) mode template */
    EZI2C_PM_STBY_CFG_REG |= EZI2C_STBY_PWR_EN;
    
    /* Exit critical section */
    CyExitCriticalSection(interruptState);
    
    /* Set the interrupt priority */
    CyIntSetPriority(EZI2C_ISR_NUMBER, EZI2C_ISR_PRIORITY);

    /* Set the interrupt vector */
    CyIntSetVector(EZI2C_ISR_NUMBER, EZI2C_ISR);

    /* Clear any pending interrupt */
    CyIntClearPending(EZI2C_ISR_NUMBER);
    
    /* Reset State Machine to IDLE */
    EZI2C_curState = EZI2C_SM_IDLE;

    /* Clear Status variable */
    EZI2C_curStatus = 0x00u;
    
    /* Enable the interrupt */
    EZI2C_EnableInt();
}


/* [] END OF FILE */
