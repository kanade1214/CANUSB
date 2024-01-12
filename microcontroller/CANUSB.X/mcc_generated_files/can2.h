/**
  CAN2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    can2.h

  @Summary
    This is the generated header file for the CAN2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for CAN2.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  dsPIC33EP512MU810
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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

#ifndef _CAN2_H
#define _CAN2_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include "can_types.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: CAN2 Module APIs
*/

/**
  @Summary
    Initialization routine that takes inputs from the CAN2 UI

  @Description
    This routine initializes the CAN2 module.
    This routine must be called before any other CAN2 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Example
    <code>
    int main()
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        
        while(1)
        {      
            
        }
      
        return 0;
    }
    </code> 
*/
void CAN2_Initialize(void);

/**
  @Summary
    Set the CAN2 operation mode

  @Description
    This function set the CAN2 operation mode.

  @Preconditions
    CAN2_Initialize() function should be called before calling this function.
 
  @Param
    requestMode - CAN2 operation modes

  @Returns
    CAN2 Operation mode request to set.
    CAN_OP_MODE_REQUEST_SUCCESS     - Requested Operation mode set successfully
    CAN_OP_MODE_REQUEST_FAIL        - Requested Operation mode set failure. Set configuration 
                                      mode before setting CAN normal or debug operation mode.
    CAN_OP_MODE_SYS_ERROR_OCCURED   - System error occurred while setting Operation mode.

  @Example
    <code>
    int main()
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                //User Application code
            }
        }

        while(1);      
        return 0;
    }
    </code> 
*/
CAN_OP_MODE_STATUS CAN2_OperationModeSet(const CAN_OP_MODES requestMode);

/**
  @Summary
    Get the CAN2 operation mode

  @Description
    This function get the CAN2 operation mode.

  @Preconditions
    CAN2_Initialize() function should be called before calling this function. 
 
  @Param
    None

  @Returns
    Return the present CAN2 operation mode. 

  @Example
    <code>
    int main()
    {
        CAN_OP_MODES mode;
        
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
            
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                mode = CAN2_OperationModeGet();
                //User Application code
            }
        }
    
        while(1);      
        return 0;
    }
    </code> 
*/
CAN_OP_MODES CAN2_OperationModeGet(void);

/**
  @Summary
    Transmits the message object from user buffer to CAN2 Transmit buffer.

  @Description
    This routine Writes a message object to the CAN2 Transmit buffer.

  @Preconditions
    CAN2_Initialize function should be called before calling this function. 
    The transfer status should be checked to see if transmitter is not full 
    before calling this function.

  @Param
    priority     - priority of the message to be transmitted
    sendCanMsg   - pointer to the message object

  @Returns
    CAN2 Transmit message status.
    CAN_TX_MSG_REQUEST_SUCCESS          - Transmit message object successfully placed into Transmit Buffer
    CAN_TX_MSG_REQUEST_DLC_ERROR        - Transmit message object DLC size is more than Transmit Buffer configured DLC size
    CAN_TX_MSG_REQUEST_BUFFER_FULL      - Transmit Buffer is Full

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN transmit APIs usage.
    
    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = CAN_DLC_8;
                msg.data = data;
            
                CAN2_Transmit(CAN_PRIORITY_HIGH, &msg);
            }
        }

        while(1);
        return 0;
    }
    </code>
*/
CAN_TX_MSG_REQUEST_STATUS CAN2_Transmit(CAN_TX_PRIOIRTY priority, CAN_MSG_OBJ *sendCanMsg);

/**
  @Summary
    Reads the message object from CAN2 receive buffer.

  @Description
    This routine reads a message object from the CAN2 receive buffer.

  @Preconditions
    CAN2_Initialize() function should be called before calling this function. 
    The CAN2_ReceivedMessageCountGet() function should be checked to see if the receiver
    is not empty before calling this function.

  @Param
    rxCanMsg    - pointer to the message object

  @Returns
    true        - Receive successful
    false       - Receive failure

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN2 Receive APIs usage. 
    //      The user must have to allocate 8 bytes of space for CAN2 Receive message object data. 
    
    int main(void) 
    {
        uint8_t data[8] = {0};
        CAN_MSG_OBJ msg;
        msg.data = data;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN2_ReceivedMessageCountGet() > 0) 
                    {
                        if(true == CAN2_Receive(&msg))
                        {
                            break;
                        }
                    }                    
                }
            }
        }

        while(1);        
        return 0;
    }
    </code>
*/
bool CAN2_Receive(CAN_MSG_OBJ *recCanMsg);

/**
  @Summary
    Checks whether the transmitter is in bus off state.

  @Description
    This routine checks whether the transmitter is in bus off state.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Bus Off state
    false   - Transmitter not in Bus Off state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      this is a sample code to demonstrate CAN2 transmit APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = CAN_DLC_8;
                msg.data = data;
            
                if(CAN2_IsBusOff() == false)
                {
                    CAN2_Transmit(CAN_PRIORITY_HIGH, &msg);
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
bool CAN2_IsBusOff();

/**
  @Summary
    Checks whether the Receiver is in the error passive state.

  @Description
    This routine checks whether the receive is in the error passive state.
    If Receiver error counter is above 127, then receiver error passive 
    state is set.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error passive state
    false   - Receiver not in Error passive state

  @Example
    <code> 
    
    int main(void) 
    {
        uint8_t data[8] = {0};
        CAN_MSG_OBJ msg;
        msg.data = data;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN2_IsRxErrorPassive() == false)
                    {
                        if(CAN2_ReceivedMessageCountGet() > 0) 
                        {
                            CAN2_Receive(&msg);
                        }
                    }                    
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
bool CAN2_IsRXErrorPassive();

/**
  @Summary
    Checks whether the Receiver is in the error warning state.

  @Description
    This routine checks whether the receive is in the error warning state.
    If Receiver error counter is above 95 to below 128, then receiver error warning 
    state is set.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error warning state
    false   - Receiver not in Error warning state

  @Example
    <code>
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN2_IsRxErrorWarning() == false)
                    {
                        if(CAN2_ReceivedMessageCountGet() > 0) 
                        {
                            CAN2_Receive(&msg);
                        }
                    }
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
bool CAN2_IsRxErrorWarning(void);

/**
  @Summary
    Checks whether the Receiver is in the error active state.

  @Description
    This routine checks whether the receive is in the error active state.
    If Receiver error counter is above 0 to below 128, then receiver error active 
    state is set.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error active state
    false   - Receiver not in Error active state

  @Example
    <code>
    int main(void) 
    {
        CAN_MSG_OBJ msg;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN2_IsRxErrorActive() == false)
                    {
                        if(CAN2_ReceivedMessageCountGet() > 0) 
                        {
                            CAN2_Receive(&msg);
                        }
                    }
                }
            }
        }

        while (1);        
        return 0;
    }
    </code>
*/
bool CAN2_IsRxErrorActive(void);

/**
  @Summary
    Checks whether the transmitter is in the error passive state.

  @Description
    This routine checks whether the transmitter is in the error passive state.
    If Transmitter error counter is above 127, then the transmitter error passive 
    state is set.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error Passive state
    false   - Transmitter not in Error Passive state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    // sample code to demonstrate CAN2 transmit error passive APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                
                
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = CAN_DLC_8;
                msg.data = data;
            
                if(CAN2_IsTxErrorPassive() == false)
                {
                    CAN2_Transmit(CAN_PRIORITY_HIGH, &msg);
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
bool CAN2_IsTXErrorPassive();

/**
  @Summary
    Checks whether the transmitter is in the error warning state.

  @Description
    This routine checks whether the transmitter is in the error warning state.
    If Transmitter error counter is above 95 to below 128, then transmitter error 
    warning state is set.

  @Preconditions
    CAN2_Initialize function should be called 
    before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error warning state
    false   - Transmitter not in Error warning state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      sample code to demonstrate CAN transmit error warning APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN2_IsTxErrorWarning() == false)
                {
                    CAN2_Transmit(CAN_PRIORITY_HIGH, &msg);
                }
            }
        }
        
        while (1);
        return 0;
    }
    </code>
*/
bool CAN2_IsTxErrorWarning(void);

/**
  @Summary
    Checks whether the transmitter is in the error active state.

  @Description
    This routine checks whether the transmitter is in the error active state.
    If Transmitter error counter is above 0 to below 128, then transmitter error 
    active state is set.

  @Preconditions
    CAN2_Initialize function should be called 
    before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error active state
    false   - Transmitter not in Error active state

  @Example
    <code>
    //Note: Example code here is not based on MCC UI configuration, 
    //      sample code to demonstrate CAN transmit error active APIs usage.

    int main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_8;
                msg.data = data;
            
                if(CAN2_IsTxErrorActive() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN2_TransmitFIFOStatusGet(CAN2_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN2_Transmit(CAN2_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while (1);
        return 0;
    }
    </code>
*/
bool CAN2_IsTxErrorActive(void);

/**
  @Summary
    CAN2 Return the number of messages object that are received in RX buffer.

  @Description
    This returns the number of messages object that are received RX buffer.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
     None

  @Returns
    Number of message received.

  @Example
    <code>
    int main(void) 
    {
        uint8_t data[8] = {0};
        CAN_MSG_OBJ msg;
        msg.data = data;
     
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);

        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN2_ReceivedMessageCountGet() > 0) 
                    {
                        CAN2_Receive(&msg);
                    }
                }
            }
        }

        while(1);
        return 0;
    }
    </code>
*/
uint8_t CAN2_ReceivedMessageCountGet();

/**
  @Summary
    Puts CAN2 module in sleep mode.

  @Description
    This routine puts CAN2 module in sleep mode.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    int main(void) 
    {     
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                CAN2_Sleep();
                
                //Check CAN2 module is in CAN_DISABLE_MODE
                if(CAN_DISABLE_MODE == CAN2_OperationModeGet())
                {
                    Sleep(); //Call sleep instruction
                    C1INTLbits.WAKIF = 0; // Clear Wake-Up interrupt flag
                    
                    // Recover to Normal mode
                    if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_CONFIGURATION_MODE))
                    {
                        if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
                        {
                            //User Application code                            
                        }
                    }
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
void CAN2_Sleep();

/**
  @Summary
    Configure the CAN2 Transmit DMA buffer.

  @Description
    This routine configures the CAN2 Transmit DMA buffer and DMA buffer configuration 
    is mandatory forCAN2 2.0 module. Before calling CAN2_Transmit function, 
    CAN2_TransmitEnable function has to be called.

  @Preconditions
    CAN2_Initialize() function should be called before calling this function. 
 
  @Param
    None

  @Returns
    None 

  @Example
    <code>
    int main()
    {
        CAN_OP_MODES mode;
        
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();

        while(1);
        return 0;
    }
    </code> 
*/
void CAN2_TransmitEnable();

/**
  @Summary
    Configure the CAN2 Receive DMA buffer.

  @Description
    This routine configures the CAN2 Receive DMA buffer and DMA buffer configuration 
    is mandatory forCAN2 2.0 module. Before calling CAN2_Receive function, 
    CAN2_ReceiveEnable function has to be called.

  @Preconditions
    CAN2_Initialize() function should be called before calling this function. 
 
  @Param
    None

  @Returns
    None 

  @Example
    <code>
    int main()
    {
        CAN_OP_MODES mode;
        
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();

        while(1);      
        return 0;
    }
    </code> 
*/
void CAN2_ReceiveEnable();

/**
  @Summary
    Callback for CAN2 Bus WakeUp activity.

  @Description
    This routine is callback for CAN2 bus wakeUp activity

  @Param
    None.

  @Returns
    None
 
  @Example 
    <code>
    bool gBusWakeUpOccurred = false;
    
    void CAN2_DefaultBusWakeUpActivityHandler(void)
    {
        gBusWakeUpOccurred = true;
    }
 
    int main(void) 
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                CAN2_Sleep();
                            
                //Check CAN2 module is in CAN_DISABLE_MODE
                if(CAN_DISABLE_MODE == CAN2_OperationModeGet())
                {
                    Sleep(); //Call sleep instruction
                    
                    while(1) 
                    {
                        if(gBusWakeUpOccurred == true)
                        {
                            break;
                        }                        
                    }
                }
            }
        }
        
        while (1);        
        return 0;
    }
    </code>
*/
void CAN2_DefaultBusWakeUpActivityHandler(void);

/**
  @Summary
    Assigns a function pointer with a CAN2 Bus Wake-Up activity callback address.

  @Description
    This routine assigns a function pointer with a CAN2 Bus Wake-Up activity callback address.

  @Param
    Address of the callback routine.

  @Returns
    None
 
  @Example 
    <code>
    bool gBusWakeUpOccurred = false;
    
    void CAN2_BusWakeUpActivity(void)
    {
        gBusWakeUpOccurred = true;
        //CAN Bus WakeUp activity application code
    }
 
    int main(void) 
    {
        // initialize the device
        SYSTEM_Initialize();
        CAN2_TransmitEnable();
        CAN2_ReceiveEnable();
        CAN2_SetBusWakeUpActivityInterruptHandler(&CAN2_BusWakeUpActivity);
        CAN2_OperationModeSet(CAN_CONFIGURATION_MODE);
        
        if(CAN_CONFIGURATION_MODE == CAN2_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN2_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                CAN2_Sleep();
                            
                //Check CAN2 module is in CAN_DISABLE_MODE
                if(CAN_DISABLE_MODE == CAN2_OperationModeGet())
                {
                    Sleep(); //Call sleep instruction
                    
                    while(1) 
                    {
                        if(gBusWakeUpOccurred == true)
                        {
                            break;
                        }                        
                    }
                }
            }
        }
        
        while(1);
        return 0;
    }
    </code>
*/
void CAN2_SetBusWakeUpActivityInterruptHandler(void *handler);

/*******************************************************************************

  !!! Deprecated Definitions and APIs !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

/******************************************************************************
*                                                                             
*    Function:		CAN2_receive
*    Description:       Receives the message from CAN2 buffer to user buffer                                                                         
*    Arguments:		recCanMsg: pointer to the message object
*    Return Value:      true - Receive successful
*                       false - Receive failure                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
"\nUse CAN2_Receive instead."))) CAN2_receive(uCAN_MSG *recCanMsg);

/******************************************************************************
*                                                                             
*    Function:		CAN2_transmit
*    Description:       Transmits the message from user buffer to CAN2 buffer                                                                        
*    Arguments:		priority: priority of the message to be transmitted
*                       sendCanMsg: pointer to the message object                                        
*    Return Value:      true - Transmit successful
*                       false - Transmit failure                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
"\nUse CAN2_Transmit instead."))) CAN2_transmit(CAN_TX_PRIOIRTY priority, 
                                    uCAN_MSG *sendCanMsg);

/******************************************************************************                                                                       
*    Function:          CAN2_isBusOff
*    Description:       Checks whether the transmitter in Bus off state                                                                                                   
*    Return Value:      true - Transmitter in Bus Off state
*                       false - Transmitter not in Bus Off state                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                              "\nUse CAN2_IsBusOff instead."))) CAN2_isBusOff();

/******************************************************************************                                                                 
*    Function:		CAN2_isRXErrorPassive
*    Description:       Checks whether the receive in error passive state                                    
*    Return Value:      true - Receiver in Error Passive state
*                       false - Receiver not in Error Passive state                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                              "\nUse CAN2_IsRXErrorPassive instead."))) CAN2_isRXErrorPassive();

/******************************************************************************                                                                      
*    Function:		CAN2_isTXErrorPassive
*    Description:       Checks whether the transmitter in error passive state                                                                                                             
*    Return Value:      true - Transmitter in Error Passive state
*                       false - Transmitter not in Error Passive state                                                                              
******************************************************************************/
bool __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                              "\nUse CAN2_IsTXErrorPassive instead."))) CAN2_isTXErrorPassive();

/******************************************************************************                                                                    
*    Function:		CAN2_messagesInBuffer
*    Description:       returns the number of messages that are received                                                                                                               
*    Return Value:      Number of message received
******************************************************************************/
uint8_t __attribute__((deprecated("\nThis will be removed in future MCC releases. "
       "\nUse CAN2_ReceivedMessageCountGet instead."))) CAN2_messagesInBuffer();

/******************************************************************************
*                                                                             
*    Function:		CAN2_sleep
*    Description:       Puts CAN2 module in disable mode.
*    Return Value:      None
*                                                                       
******************************************************************************/
void __attribute__((deprecated("\nThis will be removed in future MCC releases. "
                                    "\nUse CAN2_Sleep instead."))) CAN2_sleep();
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  //_CAN2_H
/**
 End of File
*/

