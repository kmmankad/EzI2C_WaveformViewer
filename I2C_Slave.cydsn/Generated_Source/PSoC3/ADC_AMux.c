/*******************************************************************************
* File Name: ADC_AMux.c
* Version 1.50
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "ADC_AMux.h"

uint8 ADC_AMux_initVar = 0u;
uint8 ADC_AMux_lastChannel = ADC_AMux_NULL_CHANNEL;


/*******************************************************************************
* Function Name: ADC_AMux_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_Start(void)
{
    ADC_AMux_DisconnectAll();
    ADC_AMux_initVar = 1u;
}


/*******************************************************************************
* Function Name: ADC_AMux_Init
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_Init(void)
{
    ADC_AMux_DisconnectAll();
}


/*******************************************************************************
* Function Name: ADC_AMux_Stop
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_Stop(void)
{
    ADC_AMux_DisconnectAll();
}


/*******************************************************************************
* Function Name: ADC_AMux_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_Select(uint8 channel) 
{
    ADC_AMux_DisconnectAll();        /* Disconnect all previous connections */
    ADC_AMux_Connect(channel);       /* Make the given selection */
    ADC_AMux_lastChannel = channel;  /* Update last channel */
}


/*******************************************************************************
* Function Name: ADC_AMux_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( ADC_AMux_lastChannel != ADC_AMux_NULL_CHANNEL)   /* Update last channel */
    {
        ADC_AMux_Disconnect(ADC_AMux_lastChannel);
    }

    /* Make the new channel connection */
    ADC_AMux_Connect(channel);
    ADC_AMux_lastChannel = channel;   /* Update last channel */
}


#if(ADC_AMux_MUXTYPE == ADC_AMux_MUX_DIFF)
/*******************************************************************************
* Function Name: ADC_AMux_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_Connect(uint8 channel) 
{
    ADC_AMux_CYAMUXSIDE_A_Set(channel);
    ADC_AMux_CYAMUXSIDE_B_Set(channel);
}


/*******************************************************************************
* Function Name: ADC_AMux_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_Disconnect(uint8 channel) 
{
    ADC_AMux_CYAMUXSIDE_A_Unset(channel);
    ADC_AMux_CYAMUXSIDE_B_Unset(channel);
}
#endif


/*******************************************************************************
* Function Name: ADC_AMux_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void ADC_AMux_DisconnectAll(void) 
{
    uint8 chan;

#if(ADC_AMux_MUXTYPE == ADC_AMux_MUX_SINGLE)
    for(chan = 0; chan < ADC_AMux_CHANNELS ; chan++)
    {
        ADC_AMux_Unset(chan);
    }
#else
    for(chan = 0; chan < ADC_AMux_CHANNELS ; chan++)
    {
        ADC_AMux_CYAMUXSIDE_A_Unset(chan);
        ADC_AMux_CYAMUXSIDE_B_Unset(chan);
    }
#endif
}


/* [] END OF FILE */
