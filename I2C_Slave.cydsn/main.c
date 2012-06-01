/* EzI2C based waveform viewer
 Author : Kartik Mankad
 Date : 01-Jan-2012
 This code is licensed as CC-BY-SA 3.0
 Description : Uses MiniProg3 and Bridge Control Panel to plot incoming analog(converted to digital) data over I2C.
*/

#include <device.h>

#define BUFFER_SIZE             (0x01u)
#define BUFFER_RW_AREA_SIZE     (0x00u)//Read Only.

uint8 EZI2C_Buffer; // 1 Byte EzI2C Memory Space

CY_ISR(BufferUpdate){
	EZI2C_Buffer=ADC_GetResult8(); //Update EzI2C buffer with the latest ADC result
}



void main()
{
	ISR_UpBuff_Start(); //Start the Interrupt Component.
	ISR_UpBuff_SetVector(BufferUpdate);//Set Vector to the above ISR.
	
    CyGlobalIntEnable;//Enable Global Interrupts,EzI2C ad ADC require them.
	
	EZI2C_Start();//Start the EzI2C component.
	EZI2C_SetBuffer1(BUFFER_SIZE,BUFFER_RW_AREA_SIZE,(void *) (&EZI2C_Buffer));//Configure the Buffer for the EzI2C Component.
	
	ADC_Start();//Start the ADC
	ADC_IRQ_Enable();//Enable the EOC Interrupt
	ADC_StartConvert();//Start the Conversion
	
    for(;;);//Everything is done in the ISR.
}

