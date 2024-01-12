/*
    (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/*
    MCC USB CDC Demo Usage:
 
    Call the MCC_USB_CDC_DemoTasks() function from your main loop.
    It will read data sent from the host and echo it back +1.  If you open
    up a terminal window and type 'a' then 'b' will be echoed back.
 */
#include <stdint.h>

#include "usb.h"
#include "../pin_manager.h"

static uint8_t readBuffer[64];
static uint8_t writeBuffer[64];

uint8_t usb_flag = 0;
uint8_t dlc = 8;
uint8_t rtr = 0;

void USB_Decode(){
    
}

void MCC_USB_CDC_DemoTasks(void)
{
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        LED2_SetLow();
        return;
    }else{
        LED2_SetHigh();
    }

    if( USBIsDeviceSuspended()== true )
    {
        LED3_Toggle();
        return;
    }

    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        for(i=0; i<numBytesRead; i++)
        {
            LED4_Toggle();
            uint8_t temp_data = readBuffer[i];
            
            if(usb_flag == 1){
//                enqueue(temp_data);
                if(temp_data == 0x03 && ((~rtr & 0x01) * dlc) + 5 < i){
                    usb_flag = 0;
                    USB_Decode();
                }
            }else{
                if(temp_data == 0x02){
                    usb_flag = 1;
                    dlc = 8;
                    rtr = 0;
                }
            }
        }

        if(numBytesRead > 0)
        {
            putUSBUSART(writeBuffer,numBytesRead);
        }
    }

    CDCTxService();
}
