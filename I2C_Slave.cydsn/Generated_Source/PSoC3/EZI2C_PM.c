/*******************************************************************************
* File Name: EZI2C_PM.c
* Version 1.60
*
* Description:
*  This file contains the API for the proper switching to/from the low power
*  modes.
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

/*  Function Prototypes */
void    EZI2C_Enable(void);

/* PSoC3 ES2 or early, PSoC5 ES1*/
#if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)
    
    static EZI2C_BACKUP_STRUCT  EZI2C_backup = 
    {
        /* Enable state - disabled */
        EZI2C_DISABLED,

        /* xcfg: enabled hardware addr detection */
        EZI2C_XCFG_HDWR_ADDR_EN,

        /* addr: default address (0x04) */
        EZI2C_DEFAULT_ADDR1,

        /* cfg: fast mode clock rate */
        EZI2C_CFG_CLK_RATE_400,

        /* clkDiv: set CLK divider to 16 */
        EZI2C_CLK_DIV_16
    };

/* PSoC3 ES3 or later, PSoC5 ES2 or later */
#elif (EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)

    static EZI2C_BACKUP_STRUCT  EZI2C_backup = 
    {
        /* enable state - disabled */
        EZI2C_DISABLED,

        /* xcfg: wakeup disabled, enabled hardware addr detection */
        EZI2C_XCFG_HDWR_ADDR_EN,

        /* addr: default address (0x04) */
        EZI2C_DEFAULT_ADDR1,

        /* cfg: default bus speed - 100kHz, so write 0 (16 samples/bit) */
        0x00u,

        /* clkDiv1 */
        LO8(BCLK__BUS_CLK__KHZ / EZI2C_BUS_SPEED),

        /* clkDiv2 */
        HI8(BCLK__BUS_CLK__KHZ / EZI2C_BUS_SPEED)
    };        

#endif /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */


/*******************************************************************************
* Function Name: EZI2C_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the EZI2C component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_backup - the non retention registers are saved to.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EZI2C_SaveConfig(void)
{
    EZI2C_backup.xcfg = EZI2C_XCFG_REG;
    EZI2C_backup.adr  = EZI2C_ADDR_REG;
    EZI2C_backup.cfg  = EZI2C_CFG_REG;

    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)

        EZI2C_backup.clkDiv  = EZI2C_CLKDIV_REG;

    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)

        EZI2C_backup.clkDiv1  = EZI2C_CLKDIV1_REG;
        EZI2C_backup.clkDiv2  = EZI2C_CLKDIV2_REG;

    #endif  /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: EZI2C_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the previously saved by EZI2C_SaveConfig() or 
*  EZI2C_Sleep() configuration of the EZI2C component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_backup - the non retention registers are restored from.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before EZI2C_SaveConfig() or
*  EZI2C_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void EZI2C_RestoreConfig(void)
{
    /* There are master's configuration bits here */
    EZI2C_CFG_REG |= (EZI2C_backup.cfg & EZI2C_I2C_MASTER_MASK);    
    
    EZI2C_XCFG_REG = EZI2C_backup.xcfg;
    EZI2C_ADDR_REG = EZI2C_backup.adr;

    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if(EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1)

        EZI2C_CLKDIV_REG =EZI2C_backup.clkDiv;

    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif(EZI2C_PSOC3_ES3 || EZI2C_PSOC5_ES2)

        EZI2C_CLKDIV1_REG = EZI2C_backup.clkDiv1;
        EZI2C_CLKDIV2_REG = EZI2C_backup.clkDiv2;

    #endif  /* End of (EZI2C_PSOC3_ES2 || EZI2C_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: EZI2C_Sleep
********************************************************************************
*
* Summary:
*  Saves component enable state and configuration. Stops component operation.
*  Should be called just prior to entering sleep. If "Enable wakeup from the
*  Sleep mode" is properly configured and enabled, this function should not be
*  called.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_backup - the non retention registers are saved to. Changed
*  by EZI2C_SaveConfig() function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EZI2C_Sleep(void)
{
    if(EZI2C_IS_BIT_SET(EZI2C_PM_ACT_CFG_REG, EZI2C_ACT_PWR_EN))
    {
        EZI2C_backup.enableState = EZI2C_ENABLED;
    }
    else /* The I2C block's slave is disabled */
    {
        EZI2C_backup.enableState = EZI2C_DISABLED;
    }
    
    /* Stop component */
    EZI2C_Stop();

    /* Save registers configuration */
    EZI2C_SaveConfig();
}


/*******************************************************************************
* Function Name: EZI2C_Wakeup
********************************************************************************
*
* Summary:
*  Restores component enable state and configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  EZI2C_backup - the non retention registers are restored from.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before EZI2C_SaveConfig() or
*  EZI2C_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void EZI2C_Wakeup(void)
{
    /* Restore registers values */
    EZI2C_RestoreConfig();
    
    if(EZI2C_ENABLED == EZI2C_backup.enableState)
    {
        /* Enable component's operation */
        EZI2C_Enable();

    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */
