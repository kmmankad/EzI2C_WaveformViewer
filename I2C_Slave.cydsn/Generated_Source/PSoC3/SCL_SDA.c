/*******************************************************************************
* File Name: SCL_SDA.c  
* Version 1.60
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "SCL_SDA.h"


/*******************************************************************************
* Function Name: SCL_SDA_Write
********************************************************************************
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  void 
*  
*******************************************************************************/
void SCL_SDA_Write(uint8 value) 
{
    uint8 staticBits = SCL_SDA_DR & ~SCL_SDA_MASK;
    SCL_SDA_DR = staticBits | ((value << SCL_SDA_SHIFT) & SCL_SDA_MASK);
}


/*******************************************************************************
* Function Name: SCL_SDA_SetDriveMode
********************************************************************************
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  void
*
*******************************************************************************/
void SCL_SDA_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(SCL_SDA_0, mode);
	CyPins_SetPinDriveMode(SCL_SDA_1, mode);
}


/*******************************************************************************
* Function Name: SCL_SDA_Read
********************************************************************************
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro SCL_SDA_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 SCL_SDA_Read(void) 
{
    return (SCL_SDA_PS & SCL_SDA_MASK) >> SCL_SDA_SHIFT;
}


/*******************************************************************************
* Function Name: SCL_SDA_ReadDataReg
********************************************************************************
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 SCL_SDA_ReadDataReg(void) 
{
    return (SCL_SDA_DR & SCL_SDA_MASK) >> SCL_SDA_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(SCL_SDA_INTSTAT) 

    /*******************************************************************************
    * Function Name: SCL_SDA_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  void 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 SCL_SDA_ClearInterrupt(void) 
    {
        return (SCL_SDA_INTSTAT & SCL_SDA_MASK) >> SCL_SDA_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
