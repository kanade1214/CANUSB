/**
  CAN2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    can2.c

  @Summary
    This is the generated driver implementation file for the CAN2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for CAN2.
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

/**
  Section: Included Files
*/

#include "can2.h"
#include "dma.h"

#define CAN2_TX_DMA_CHANNEL DMA_CHANNEL_3
#define CAN2_RX_DMA_CHANNEL DMA_CHANNEL_2

/* Valid options are 4, 6, 8, 12, 16, 24, or 32. */
#define CAN2_MESSAGE_BUFFERS         32

#define CAN2_TX_BUFFER_COUNT 1

#define CAN2_RX_BUFFER_MSG_DATA_SIZE    8U   // CAN RX Buffer Message object data field size

/******************************************************************************/

/******************************************************************************/
/* Private type definitions                                                   */
/******************************************************************************/
typedef struct __attribute__((packed))
{
    unsigned priority                   :2;
    unsigned remote_transmit_enable     :1;
    unsigned send_request               :1;
    unsigned error                      :1;
    unsigned lost_arbitration           :1;
    unsigned message_aborted            :1;
    unsigned transmit_enabled           :1;
} CAN2_TX_CONTROLS;


/**
 Section: Private Variable Definitions
*/

/* This alignment is required because of the DMA's peripheral indirect 
 * addressing mode. */
 // CAN2 with DMA Errata workaround: configured CAN2 message object buffer to "Dual Port RAM"
 // Possible loss of interrupts when DMA is used with CAN.
 __eds__ static unsigned int can2msgBuf[CAN2_MESSAGE_BUFFERS][8] __attribute__((space(eds), aligned(32 * 8 * 2)));

/******************************************************************************/
/* Private function prototypes                                                */
/******************************************************************************/
static void CAN2_DMACopy(uint8_t buffer_number, CAN_MSG_OBJ *message);
static void CAN2_MessageToBuffer(uint_fast8_t i, CAN_MSG_OBJ *message);

// CAN2 Default Interrupt Handler
static void (*CAN2_BusWakeUpActivityInterruptHandler)(void) = NULL;

/**
 Section: Private Function Definitions
*/

/**
  @Summary
    Read the message object from Receive buffer and update to the user message object 
    pointer.

  @Description
    This routine read the message object from Receive buffer and update to the user  
    message object pointer.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    bufferNumber - A buffer number is in the Receive buffer where the message would 
                   be stored.
    message      - pointer to the CAN2 Receive message object. 

  @Returns
    None
    
  @Example
    None
*/
static void CAN2_DMACopy(uint8_t buffer_number, CAN_MSG_OBJ *message)
{
    uint16_t ide=0;
    uint16_t rtr=0;
    uint32_t id=0;

    /* read word 0 to see the message type */
    ide=can2msgBuf[buffer_number][0] & 0x0001U;			

    /* check to see what type of message it is */
    /* message is standard identifier */
    if(ide==0U)
    {
        message->msgId =(can2msgBuf[buffer_number][0] & 0x1FFCU) >> 2U;		
        message->field.idType = CAN_FRAME_STD;
        rtr=can2msgBuf[buffer_number][0] & 0x0002U;
    }
    /* message is extended identifier */
    else
    {
        id=can2msgBuf[buffer_number][0] & 0x1FFCU;		
        message->msgId = id << 16U;
        message->msgId += ( ((uint32_t) can2msgBuf[buffer_number][1] & (uint32_t)0x0FFF) << 6U );
        message->msgId += ( ((uint32_t) can2msgBuf[buffer_number][2] & (uint32_t)0xFC00U) >> 10U );		
        message->field.idType = CAN_FRAME_EXT;
        rtr=can2msgBuf[buffer_number][2] & 0x0200;
    }
    /* check to see what type of message it is */
    /* RTR message */
    if(rtr != 0U)
    {
        /* to be defined ?*/
        message->field.frameType = CAN_FRAME_RTR;	
    }
    /* normal message */
    else
    {
        message->field.frameType = CAN_FRAME_DATA;
        message->data[0] =(uint8_t) can2msgBuf[buffer_number][3];
        message->data[1] =(uint8_t) ((can2msgBuf[buffer_number][3] & 0xFF00U) >> 8U);
        message->data[2] =(uint8_t) can2msgBuf[buffer_number][4];
        message->data[3] =(uint8_t) ((can2msgBuf[buffer_number][4] & 0xFF00U) >> 8U);
        message->data[4] =(uint8_t) can2msgBuf[buffer_number][5];
        message->data[5] =(uint8_t) ((can2msgBuf[buffer_number][5] & 0xFF00U) >> 8U);
        message->data[6] =(uint8_t) can2msgBuf[buffer_number][6];
        message->data[7] =(uint8_t) ((can2msgBuf[buffer_number][6] & 0xFF00U) >> 8U);
        message->field.dlc =(uint8_t) (can2msgBuf[buffer_number][2] & 0x000FU);
    }
}

/**
  @Summary
    Read the message object from user input and update to the CAN2 TX buffer.

  @Description
    This routine Read the message object from user input and update to the CAN2 
    TX buffer.

  @Preconditions
    CAN2_Initialize function should be called before calling this function.

  @Param
    buffer      - pointer to the CAN2 Message object. 
    message     - pointer to the CAN2 transmit message object. 

  @Returns
    None
    
  @Example
    None
*/
static void CAN2_MessageToBuffer(uint_fast8_t i, CAN_MSG_OBJ* message)
{   
    if(message->field.idType == CAN_FRAME_STD)
    {
        can2msgBuf[i][0]= ((message->msgId & 0x000007FF) << 2) + ((uint16_t)(message->field.frameType << 1) & 0x0002);
        can2msgBuf[i][1]= 0;
        can2msgBuf[i][2]= message->field.dlc & 0x0F;
    }
    else
    {
        can2msgBuf[i][0]= ( ( (uint16_t)(message->msgId >> 16 ) & 0x1FFC ) ) | 0x3;
        can2msgBuf[i][1]= (uint16_t)(message->msgId >> 6) & 0x0FFF;
        can2msgBuf[i][2]= (message->field.dlc & 0x0F) + ( (uint16_t)(message->msgId << 10) & 0xFC00) + ((uint16_t)(message->field.frameType << 9) & 0x0200);
    }
    
    if(message->data != NULL)
    {
        can2msgBuf[i][3]= ((message->data[1])<<8) + message->data[0];
        can2msgBuf[i][4]= ((message->data[3])<<8) + message->data[2];
        can2msgBuf[i][5]= ((message->data[5])<<8) + message->data[4];
        can2msgBuf[i][6]= ((message->data[7])<<8) + message->data[6];
    }
}

/**
  Section: CAN2 APIs
*/

void CAN2_Initialize(void)
{
    // Disable interrupts before the Initialization
    IEC3bits.C2IE = 0;
    C2INTE = 0;

    // set the CAN2_initialize module to the options selected in the User Interface

    /* put the module in configuration mode */
    C2CTRL1bits.REQOP = CAN_CONFIGURATION_MODE;
    while(C2CTRL1bits.OPMODE != CAN_CONFIGURATION_MODE);

    /* Set up the baud rate*/	
    // CAN2 Clock Errata workaround: CANCKS bit in C2CTRL1 register is reversed
    C2CFG1 = 0x02;	//BRP TQ = (2 x 3)/FCAN; SJW 1 x TQ; 
    C2CFG2 = 0x1A8;	//WAKFIL disabled; SEG2PHTS Freely programmable; SEG2PH 2 x TQ; SEG1PH 6 x TQ; PRSEG 1 x TQ; SAM Once at the sample point; 
    C2FCTRL = 0xC001;	//FSA Transmit/Receive Buffer TRB1; DMABS 32; 
    C2FEN1 = 0x00;	//FLTEN8 disabled; FLTEN7 disabled; FLTEN9 disabled; FLTEN0 disabled; FLTEN2 disabled; FLTEN10 disabled; FLTEN1 disabled; FLTEN11 disabled; FLTEN4 disabled; FLTEN3 disabled; FLTEN6 disabled; FLTEN5 disabled; FLTEN12 disabled; FLTEN13 disabled; FLTEN14 disabled; FLTEN15 disabled; 
    C2CTRL1 = 0x800;	//CANCKS FOSC/2; CSIDL disabled; ABAT disabled; REQOP Sets Normal Operation Mode; WIN Uses buffer window; CANCAP disabled; 

    /* Filter configuration */
    /* enable window to access the filter configuration registers */
    /* use filter window*/
    C2CTRL1bits.WIN=1;	   
    
    /* Configure the masks */
    C2RXM0SIDbits.SID = 0x0; 
    C2RXM1SIDbits.SID = 0x0; 
    C2RXM2SIDbits.SID = 0x0; 
    
    C2RXM0SIDbits.EID = 0x0; 
    C2RXM1SIDbits.EID = 0x0; 
    C2RXM2SIDbits.EID = 0x0; 
    
    C2RXM0EID = 0x00;     	
    C2RXM1EID = 0x00;     	
    C2RXM2EID = 0x00;     	
    
    C2RXM0SIDbits.MIDE = 0x0; 
    C2RXM1SIDbits.MIDE = 0x0; 
    C2RXM2SIDbits.MIDE = 0x0; 
    
    /* clear window bit to access CAN2 control registers */
    C2CTRL1bits.WIN=0;    

    /*configure CAN2 Transmit/Receive buffer settings*/
    C2TR01CONbits.TXEN0 = 0x1; // Buffer 0 is a Transmit Buffer 
    C2TR01CONbits.TXEN1 = 0x0; // Buffer 1 is a Receive Buffer 
    C2TR23CONbits.TXEN2 = 0x0; // Buffer 2 is a Receive Buffer 
    C2TR23CONbits.TXEN3 = 0x0; // Buffer 3 is a Receive Buffer 
    C2TR45CONbits.TXEN4 = 0x0; // Buffer 4 is a Receive Buffer 
    C2TR45CONbits.TXEN5 = 0x0; // Buffer 5 is a Receive Buffer 
    C2TR67CONbits.TXEN6 = 0x0; // Buffer 6 is a Receive Buffer 
    C2TR67CONbits.TXEN7 = 0x0; // Buffer 7 is a Receive Buffer 

    C2TR01CONbits.TX0PRI = 0x0; // Message Buffer 0 Priority Level
    C2TR01CONbits.TX1PRI = 0x0; // Message Buffer 1 Priority Level
    C2TR23CONbits.TX2PRI = 0x0; // Message Buffer 2 Priority Level
    C2TR23CONbits.TX3PRI = 0x0; // Message Buffer 3 Priority Level
    C2TR45CONbits.TX4PRI = 0x0; // Message Buffer 4 Priority Level
    C2TR45CONbits.TX5PRI = 0x0; // Message Buffer 5 Priority Level
    C2TR67CONbits.TX6PRI = 0x0; // Message Buffer 6 Priority Level
    C2TR67CONbits.TX7PRI = 0x0; // Message Buffer 7 Priority Level

    /* clear the buffer and overflow flags */   
    C2RXFUL1 = 0x0000;
    C2RXFUL2 = 0x0000;
    C2RXOVF1 = 0x0000;
    C2RXOVF2 = 0x0000;	

    /* configure the device to interrupt on the receive buffer full flag */
    /* clear the buffer full flags */ 	
    C2INTFbits.RBIF = 0;  

    /* put the module in normal mode */
    C2CTRL1bits.REQOP = CAN_NORMAL_OPERATION_MODE;
    while(C2CTRL1bits.OPMODE != CAN_NORMAL_OPERATION_MODE);	

    /* Initialize Interrupt Handler*/
    CAN2_SetBusWakeUpActivityInterruptHandler(&CAN2_DefaultBusWakeUpActivityHandler);

    /* Enable CAN2 Interrupt */
    IEC3bits.C2IE = 1;

}

void CAN2_TransmitEnable()
{
    /* setup channel 0 for peripheral indirect addressing mode 
    normal operation, word operation and select as Tx to peripheral */

    /* DMA_PeripheralIrqNumberSet and DMA_TransferCountSet would be done in the 
    DMA */
    
    /* setup the address of the peripheral CAN2 (C2TXD) */ 
    DMA_PeripheralAddressSet(CAN2_TX_DMA_CHANNEL, (uint16_t) &C2TXD);

    /* DPSRAM start address offset value */ 
    DMA_StartAddressASet(CAN2_TX_DMA_CHANNEL, __builtin_dmaoffset(&can2msgBuf));

    /* enable the channel */
    DMA_ChannelEnable(CAN2_TX_DMA_CHANNEL);
}

void CAN2_ReceiveEnable()
{
    /* setup DMA channel for peripheral indirect addressing mode 
    normal operation, word operation and select as Rx to peripheral */

    /* setup the address of the peripheral CAN2 (C2RXD) */     
    /* DMA_TransferCountSet and DMA_PeripheralIrqNumberSet would be set in 
    the DMA_Initialize function */

    DMA_PeripheralAddressSet(CAN2_RX_DMA_CHANNEL, (uint16_t) &C2RXD);

    /* DPSRAM start address offset value */ 
    DMA_StartAddressASet(CAN2_RX_DMA_CHANNEL, __builtin_dmaoffset(&can2msgBuf) );	  

    /* enable the channel */
    DMA_ChannelEnable(CAN2_RX_DMA_CHANNEL);
}

CAN_OP_MODE_STATUS CAN2_OperationModeSet(const CAN_OP_MODES requestMode) 
{
    CAN_OP_MODE_STATUS status = CAN_OP_MODE_REQUEST_SUCCESS;
    
    if((CAN_CONFIGURATION_MODE == CAN2_OperationModeGet()) || (requestMode == CAN_DISABLE_MODE)
            || (requestMode == CAN_CONFIGURATION_MODE))
    {
        C2CTRL1bits.REQOP = requestMode;
        while(C2CTRL1bits.OPMODE != requestMode);
    }
    else
    {
        status = CAN_OP_MODE_REQUEST_FAIL;
    }
    
    return status;
}

CAN_OP_MODES CAN2_OperationModeGet(void) 
{
    return C2CTRL1bits.OPMODE;
}

CAN_TX_MSG_REQUEST_STATUS CAN2_Transmit(CAN_TX_PRIOIRTY priority, CAN_MSG_OBJ *sendCanMsg)
{
    CAN_TX_MSG_REQUEST_STATUS txMsgStatus = CAN_TX_MSG_REQUEST_SUCCESS;
    CAN2_TX_CONTROLS * pTxControls = (CAN2_TX_CONTROLS*)&C2TR01CON;
    uint_fast8_t i;
    bool messageSent = false;
    
    // CAN 2.0 mode DLC supports upto 8 byte 
    if(sendCanMsg->field.dlc > CAN_DLC_8) 
    {
       txMsgStatus |= CAN_TX_MSG_REQUEST_DLC_ERROR;
    }
    
    if(CAN2_TX_BUFFER_COUNT > 0)
    {
        for(i=0; i<CAN2_TX_BUFFER_COUNT; i++)
        {
            if(pTxControls->transmit_enabled == 1)
            {
                if (pTxControls->send_request == 0)
                {
                    // CAN2 with DMA Errata workaround: configured CAN2 message object buffer to "Dual Port RAM"
                    // Possible loss of interrupts when DMA is used with CAN.
                    CAN2_MessageToBuffer(i, sendCanMsg);
                    pTxControls->priority = priority;

                    /* set the message for transmission */
                    pTxControls->send_request = 1; 

                    messageSent = true;
                    break;
                }
            }

            pTxControls++;
        }
    }
    
    if(messageSent == false)
    {
        txMsgStatus |= CAN_TX_MSG_REQUEST_BUFFER_FULL;
    }
    
    return txMsgStatus;
}

bool CAN2_Receive(CAN_MSG_OBJ *recCanMsg) 
{   
    /* We use a static buffer counter so we don't always check buffer 0 first
     * resulting in potential starvation of later buffers.
     */
    static uint_fast8_t currentDedicatedBuffer = 0;
    uint_fast8_t i;
    bool messageReceived = false;
    uint16_t receptionFlags;

    if(recCanMsg->data == NULL)
    {
        return messageReceived;
    }
    
    receptionFlags = C2RXFUL1;
	
    if (receptionFlags != 0)  
    {
        /* check which message buffer is free */  
        for (i=0 ; i < 16; i++)
        {
            if (((receptionFlags >> currentDedicatedBuffer ) & 0x1) == 0x1)
            {           
               CAN2_DMACopy(currentDedicatedBuffer, recCanMsg);
           
               C2RXFUL1 &= ~(1 << currentDedicatedBuffer);
  
               messageReceived = true;
            }
            
            currentDedicatedBuffer++;
            
            if(currentDedicatedBuffer >= 16)
            {
                currentDedicatedBuffer = 0;
            }
            
            if(messageReceived == true)
            {
                break;
            }
        }
    }
        
    return (messageReceived);
}

bool CAN2_IsBusOff() 
{
    return C2INTFbits.TXBO;	
}

bool CAN2_IsRXErrorPassive()
{
    return (C2INTFbits.RXBP);   
}

bool CAN2_IsRxErrorWarning(void)
{
    return (C2INTFbits.RXWAR);
}

bool CAN2_IsRxErrorActive(void)
{
    bool errorState = false;
    if((0 < C2ECbits.RERRCNT) && (C2ECbits.RERRCNT < 128)) 
    {
        errorState = true;
    }
    
    return errorState;
}

bool CAN2_IsTXErrorPassive()
{
    return (C2INTFbits.TXBP);
}

bool CAN2_IsTxErrorWarning(void) 
{
    return (C2INTFbits.TXWAR);
}

bool CAN2_IsTxErrorActive(void)
{
    bool errorState = false;
    if((0 < C2ECbits.TERRCNT) && (C2ECbits.TERRCNT < 128)) 
    {
        errorState = true;
    }
    
    return errorState;
}

uint8_t CAN2_ReceivedMessageCountGet() 
{
    uint_fast8_t messageCount;
    uint_fast8_t currentBuffer;
    uint16_t receptionFlags;
   
    messageCount = 0;

    /* Check any message in buffer 0 to buffer 15*/
    receptionFlags = C2RXFUL1;
    if (receptionFlags != 0) 
    {
        /* check whether a message is received */  
        for (currentBuffer=0 ; currentBuffer < 16; currentBuffer++)
        {
            if (((receptionFlags >> currentBuffer ) & 0x1) == 0x1)
            {
                messageCount++;
            }
        }
    }
            
    return (messageCount);
}

void CAN2_Sleep(void) 
{
    C2INTFbits.WAKIF = 0;
    C2INTEbits.WAKIE = 1;

    /* put the module in disable mode */
    C2CTRL1bits.REQOP = CAN_DISABLE_MODE;
    while(C2CTRL1bits.OPMODE != CAN_DISABLE_MODE);
    
    //Wake up from sleep should set the CAN2 module straight into Normal mode
}

void __attribute__((weak)) CAN2_DefaultBusWakeUpActivityHandler(void) 
{

}

void CAN2_SetBusWakeUpActivityInterruptHandler(void *handler)
{
    CAN2_BusWakeUpActivityInterruptHandler = handler;
}

void __attribute__((__interrupt__, no_auto_psv)) _C2Interrupt(void)
{
    if(C2INTFbits.WAKIF)
    {
        if(CAN2_BusWakeUpActivityInterruptHandler)
        {
            CAN2_BusWakeUpActivityInterruptHandler();
        }
	
        C2INTFbits.WAKIF = 0;
    }
    
    IFS3bits.C2IF = 0;
}

/*******************************************************************************

  !!! Deprecated Definitions and APIs !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

/******************************************************************************
*                                                                             
*    Function:		CAN2_transmit
*    Description:       Transmits the message from user buffer to CAN2 buffer
*                       as per the buffer number allocated.
*                       Allocation of the buffer number is done by user 
*                                                                             
*    Arguments:		priority : priority of the message to be transmitted
*                       sendCanMsg: pointer to the message object
*                                             
*    Return Value:      true - Transmit successful
*                       false - Transmit failure                                                                              
******************************************************************************/
bool CAN2_transmit(CAN_TX_PRIOIRTY priority, uCAN_MSG *sendCanMsg) 
{
    uint8_t msgObjData[8] = {0};
    CAN_MSG_OBJ txCanMsg;
    txCanMsg.data = msgObjData;
    
    txCanMsg.msgId = sendCanMsg->frame.id;
    txCanMsg.field.idType = sendCanMsg->frame.idType;
    txCanMsg.field.frameType = sendCanMsg->frame.msgtype;
    txCanMsg.field.dlc = sendCanMsg->frame.dlc;
    txCanMsg.data[0] = sendCanMsg->frame.data0;
    txCanMsg.data[1] = sendCanMsg->frame.data1;
    txCanMsg.data[2] = sendCanMsg->frame.data2;
    txCanMsg.data[3] = sendCanMsg->frame.data3;
    txCanMsg.data[4] = sendCanMsg->frame.data4;
    txCanMsg.data[5] = sendCanMsg->frame.data5;
    txCanMsg.data[6] = sendCanMsg->frame.data6;
    txCanMsg.data[7] = sendCanMsg->frame.data7;
    
    return (CAN2_Transmit(priority, &txCanMsg));
}

/******************************************************************************
*                                                                             
*    Function:		CAN2_receive
*    Description:       Receives the message from CAN2 buffer to user buffer 
*                                                                             
*    Arguments:		recCanMsg: pointer to the message object
*                                             
*    Return Value:      true - Receive successful
*                       false - Receive failure                                                                              
******************************************************************************/
bool CAN2_receive(uCAN_MSG *recCanMsg) 
{   
    bool messageReceived = false;
    uint8_t msgObjData[8] = {0};
    CAN_MSG_OBJ rxCanMsg;
    rxCanMsg.data = msgObjData;
    
    if(true == CAN2_Receive(&rxCanMsg))
    {
        recCanMsg->frame.id = rxCanMsg.msgId;
        recCanMsg->frame.idType = rxCanMsg.field.idType;
        
        if(rxCanMsg.field.frameType == CAN_FRAME_RTR)
        {
            recCanMsg->frame.msgtype = CAN_MSG_RTR;
        }
        else
        {
            recCanMsg->frame.msgtype = CAN_MSG_DATA;
        }

        recCanMsg->frame.data0 = rxCanMsg.data[0];
        recCanMsg->frame.data1 = rxCanMsg.data[1];
        recCanMsg->frame.data2 = rxCanMsg.data[2];
        recCanMsg->frame.data3 = rxCanMsg.data[3];
        recCanMsg->frame.data4 = rxCanMsg.data[4];
        recCanMsg->frame.data5 = rxCanMsg.data[5];
        recCanMsg->frame.data6 = rxCanMsg.data[6];
        recCanMsg->frame.data7 = rxCanMsg.data[7];
        recCanMsg->frame.dlc = rxCanMsg.field.dlc;
        messageReceived = true;
    }
        
    return (messageReceived);
}

/******************************************************************************
*                                                                             
*    Function:		CAN2_isBusOff
*    Description:       Checks whether the transmitter in Bus off state
*                                                                             
                                             
*    Return Value:      true - Transmitter in Bus Off state
*                       false - Transmitter not in Bus Off state                                                                              
******************************************************************************/
bool CAN2_isBusOff() 
{
    return C2INTFbits.TXBO;	
}

/******************************************************************************
*                                                                             
*    Function:		CAN2_isRXErrorPassive
*    Description:       Checks whether the receive in error passive state
*                                             
*    Return Value:      true - Receiver in Error Passive state
*                       false - Receiver not in Error Passive state                                                                              
******************************************************************************/
bool CAN2_isRXErrorPassive()
{
    return C2INTFbits.RXBP;   
}

/******************************************************************************
*                                                                             
*    Function:		CAN2_isTXErrorPassive
*    Description:       Checks whether the transmitter in error passive state                                                                          
*                                             
*    Return Value:      true - Transmitter in Error Passive state
*                       false - Transmitter not in Error Passive state                                                                              
******************************************************************************/
bool CAN2_isTXErrorPassive()
{
    return (C2INTFbits.TXBP);
}

/******************************************************************************
*                                                                             
*    Function:		CAN2_messagesInBuffer
*    Description:       returns the number of messages that are received                                                                           
*                                             
*    Return Value:      Number of message received
******************************************************************************/
uint8_t CAN2_messagesInBuffer() 
{
    uint_fast8_t messageCount;
    uint_fast8_t currentBuffer;
    uint16_t receptionFlags;
   
    messageCount = 0;

    /* Check any message in buffer 0 to buffer 15*/
    receptionFlags = C2RXFUL1;
    if (receptionFlags != 0) 
    {
        /* check whether a message is received */  
        for (currentBuffer=0 ; currentBuffer < 16; currentBuffer++)
        {
            if (((receptionFlags >> currentBuffer ) & 0x1) == 0x1)
            {
                messageCount++;
            }
        }
    }
            
    return (messageCount);
}

/******************************************************************************
*                                                                             
*    Function:		CAN2_sleep
*    Description:       Puts CAN2 module in disable mode.
*                                                                       
******************************************************************************/
void CAN2_sleep(void) 
{
    C2INTFbits.WAKIF = 0;
    C2INTEbits.WAKIE = 1;

    /* put the module in disable mode */
    C2CTRL1bits.REQOP = CAN_DISABLE_MODE;
    while(C2CTRL1bits.OPMODE != CAN_DISABLE_MODE);
    
    //Wake up from sleep should set the CAN2 module straight into Normal mode
}

/**
 End of File
*/