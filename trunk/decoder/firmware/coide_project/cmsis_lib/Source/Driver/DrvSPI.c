//*****************************************************************************
//
//! \file DrvSPI.c
//! \brief Driver for the SPI Controller.
//! \version v1.02.002
//! \date 09/05/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c)  2011, Nuvoton 
//! All rights reserved.
//
//*****************************************************************************

//*****************************************************************************
//
//! Include related headers
//
//*****************************************************************************
#include "M051Series.h"
#include "core_cm0.h"
#include "DrvSPI.h"
#include "DrvSYS.h"

//*****************************************************************************
//
//! Global file scope (static) variables
//
//*****************************************************************************
typedef struct
{
    uint8_t bInUse;
    PFN_DRVSPI_CALLBACK pfncallback;
    uint32_t u32userData;
} S_DRVSPI_HANDLE;

static S_DRVSPI_HANDLE g_sSpiHandler[2];

static SPI_T * SPI_PORT[2]={SPI0, SPI1};

//*****************************************************************************
//
//! Interrupt Handler
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief SPI0 interrupt handler. Clear the SPI interrupt flag and execute the
//!  callback function. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void SPI0_IRQHandler(void)
{
    //
    // write '1' to clear SPI0 interrupt flag
    //
    SPI0->CNTRL.IF = 1; 
    
    if(g_sSpiHandler[0].pfncallback != NULL)
    {
        g_sSpiHandler[0].pfncallback(g_sSpiHandler[0].u32userData);
    }
    
}

//*****************************************************************************
//
//! \brief SPI1 interrupt handler. Clear the SPI interrupt flag and execute the
//!  callback function. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void SPI1_IRQHandler(void)
{
    //
    // write '1' to clear SPI1 interrupt flag
    //
    SPI1->CNTRL.IF = 1; 
    
    if(g_sSpiHandler[1].pfncallback != NULL)
    {
        g_sSpiHandler[1].pfncallback(g_sSpiHandler[1].u32userData);
    }
}

//*****************************************************************************
//
//! \brief Configure the operation mode, transfer type and bit length of a
//!  transaction of the specified SPI port. The timing waveform types: 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param eMode Specify the operation mode (eDRVSPI_MASTER/eDRVSPI_SLAVE)
//!
//! \param eType Specify the transfer type (eDRVSPI_TYPE0 ~ eDRVSPI_TYPE7)
//!
//! \param i32BitLength Specify the bit length in a transaction (1~32) 
//!
//! \return E_DRVSPI_ERR_INIT The specified SPI port has been opened before.
//!         E_DRVSPI_ERR_BUSY The specified SPI port is in busy status.
//!         E_DRVSPI_ERR_BIT_LENGTH The specified bit length is out of range.
//!         E_SUCCESS Success.  
//
//! DRVSPI_TYPE0:          
//!
//!     CS    --|          Active state           |---
//!                _   _   _   _   _   _   _   _  
//!     CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//!               
//!     Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
//!           
//!     Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
//!
//!
//! DRVSPI_TYPE1:          
//!
//!     CS    --|          Active state           |---
//!                _   _   _   _   _   _   _   _  
//!     CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//!               
//!     Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
//!           
//!     Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
//!
//!
//! DRVSPI_TYPE2:          
//!
//!     CS    --|          Active state           |---
//!                _   _   _   _   _   _   _   _  
//!     CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//!               
//!     Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
//!           
//!     Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
//!
//!
//! DRVSPI_TYPE3:          
//!
//!     CS    --|          Active state           |---
//!                _   _   _   _   _   _   _   _  
//!     CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//!               
//!     Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
//!           
//!     Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
//!
//!
//! DRVSPI_TYPE4:          
//!
//!     CS    --|          Active state           |---
//!            ___   _   _   _   _   _   _   _   ______ 
//!     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//!               
//!     Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
//!           
//!     Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
//!
//!
//! DRVSPI_TYPE5:
//!
//!     CS    --|          Active state           |---
//!            ___   _   _   _   _   _   _   _   ______ 
//!     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//!               
//!     Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
//!           
//!     Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
//!
//!
//! DRVSPI_TYPE6:
//!
//!     CS    --|          Active state           |---
//!            ___   _   _   _   _   _   _   _   ______ 
//!     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//!               
//!     Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
//!           
//!     Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
//!
//!
//! DRVSPI_TYPE7:
//!
//!     CS    --|          Active state           |---
//!            ___   _   _   _   _   _   _   _   ______ 
//!     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//!               
//!     Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
//!           
//!     Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
//!
//!
//! Master / Slave Transfer Type Matching Table
//!
//!     DRVSPI_TYPE0 <==> DRVSPI_TYPE3
//!     DRVSPI_TYPE1 <==> DRVSPI_TYPE1
//!     DRVSPI_TYPE2 <==> DRVSPI_TYPE2
//!     DRVSPI_TYPE3 <==> DRVSPI_TYPE0
//!     DRVSPI_TYPE4 <==> DRVSPI_TYPE7
//!     DRVSPI_TYPE5 <==> DRVSPI_TYPE5
//!     DRVSPI_TYPE6 <==> DRVSPI_TYPE6
//!     DRVSPI_TYPE7 <==> DRVSPI_TYPE4
//
//*****************************************************************************

int32_t DrvSPI_Open(E_DRVSPI_PORT eSpiPort, E_DRVSPI_MODE eMode, E_DRVSPI_TRANS_TYPE eType, int32_t i32BitLength)
{
    int32_t i32TimeOut;
    
    if(g_sSpiHandler[eSpiPort].bInUse)
    {
        return E_DRVSPI_ERR_INIT;
    }
    
    //
    // Bit length 1 ~ 32 
    //
    if((i32BitLength <= 0) || (i32BitLength > 32))
    {
        return E_DRVSPI_ERR_BIT_LENGTH;
    }
   
    if(eSpiPort == eDRVSPI_PORT0)
    {
        SYSCLK->APBCLK.SPI0_EN       =1;
        SYS->IPRSTC2.SPI0_RST        =1;
        SYS->IPRSTC2.SPI0_RST        =0;
    }
    else
    {
        SYSCLK->APBCLK.SPI1_EN       =1;
        SYS->IPRSTC2.SPI1_RST        =1;
        SYS->IPRSTC2.SPI1_RST        =0;
    }
    
    //
    // Check busy 
    //
    i32TimeOut = 0x10000;
    while(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY == 1)
    {
        if(i32TimeOut-- <= 0)
            return E_DRVSPI_ERR_BUSY;
    }
    
    g_sSpiHandler[eSpiPort].bInUse = TRUE;
    g_sSpiHandler[eSpiPort].pfncallback = NULL;
    g_sSpiHandler[eSpiPort].u32userData = 0;
    
    //
    // "i32BitLength = 0" means 32 bits 
    //
    if(i32BitLength == 32)
    {
        i32BitLength = 0;
    }
    SPI_PORT[eSpiPort]->CNTRL.TX_BIT_LEN = i32BitLength;
    
    if(eMode == eDRVSPI_SLAVE)
        SPI_PORT[eSpiPort]->CNTRL.SLAVE = 1;
    else
        SPI_PORT[eSpiPort]->CNTRL.SLAVE = 0;
    
    //
    // Automatic slave select 
    //
    SPI_PORT[eSpiPort]->SSR.AUTOSS = 1;
    
    //
    // Timing waveform types 
    //
    if(eType==eDRVSPI_TYPE0)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE1)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE2)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }
    else if(eType==eDRVSPI_TYPE3)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }
    else if(eType==eDRVSPI_TYPE4)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE5)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE6)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }
    else
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Close the specified SPI module and disable the SPI interrupt. 
//!
//! \param eSpiPort :  Specify the SPI port. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_Close(E_DRVSPI_PORT eSpiPort)
{
    int32_t i32TimeOut;
    
    g_sSpiHandler[eSpiPort].bInUse = FALSE;
    g_sSpiHandler[eSpiPort].pfncallback = NULL;
    g_sSpiHandler[eSpiPort].u32userData = 0;

    //
    // Check SPI state 
    //
    i32TimeOut = 0x10000;
    while(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY == 1)
    {
        if(i32TimeOut-- <= 0)
            break;
    }
   
   if(eSpiPort == eDRVSPI_PORT0)
    {
        NVIC_DisableIRQ(SPI0_IRQn);
        SYS->IPRSTC2.SPI0_RST=1;
        SYS->IPRSTC2.SPI0_RST=0;
        SYSCLK->APBCLK.SPI0_EN=0;
    }
    else
    {
        NVIC_DisableIRQ(SPI1_IRQn);
        SYS->IPRSTC2.SPI1_RST=1;
        SYS->IPRSTC2.SPI1_RST=0;
        SYSCLK->APBCLK.SPI1_EN=0;
    }
   
}

//*****************************************************************************
//
//! \brief Dertermine to transfer data with LSB first or MSB first 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param eEndian Specify LSB first or MSB first (eDRVSPI_LSB_FIRST /
//!  eDRVSPI_MSB_FIRST) 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_SetEndian(E_DRVSPI_PORT eSpiPort, E_DRVSPI_ENDIAN eEndian)
{

    if(eEndian == eDRVSPI_LSB_FIRST)
        SPI_PORT[eSpiPort]->CNTRL.LSB = 1;
    else
        SPI_PORT[eSpiPort]->CNTRL.LSB = 0;

}

//*****************************************************************************
//
//! \brief Set the bit length of SPI transfer. 
//!
//! \param eSpiPort Specify the SPI port
//! \param i32BitLength Specify the bit length (1~32 bits) 
//!
//! \return E_SUCCESS Success. 
//!         E_DRVSPI_ERR_BIT_LENGTH The bit length is out of range.  
//
//*****************************************************************************
int32_t DrvSPI_SetBitLength(E_DRVSPI_PORT eSpiPort, int32_t i32BitLength)
{

    if((i32BitLength < 1) || (i32BitLength > 32))
    {
        return E_DRVSPI_ERR_BIT_LENGTH;
    }
    if(i32BitLength == 32)
        i32BitLength = 0;

    SPI_PORT[eSpiPort]->CNTRL.TX_BIT_LEN = i32BitLength;
    
    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Enable/disable Byte Reorder function. The Byte Reorder function is
//!  supported only in 16-, 24- and 32-bit transaction mode. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param eOption the options of Byte Reorder function and Byte Suspend
//!  function. 
//!  eDRVSPI_BYTE_REORDER_SUSPEND_DISABLE both Byte Reorder function
//!  and Byte Suspend function are disabled. 
//!  eDRVSPI_BYTE_REORDER_SUSPEND both Byte Reorder function and Byte Suspend 
//!  function are enabled.
//!  eDRVSPI_BYTE_REORDER : only enable the Byte Reorder function. 
//!  eDRVSPI_BYTE_SUSPEND : only enable the Byte Suspend function. The Byte 
//!  Suspend function is only available in 32-bit transaction. 
//!
//! \return E_SUCCESS Success. 
//!         E_DRVSPI_ERR_BIT_LENGTH The bit length is not 16-, 24- or 32-bit.  
//
//*****************************************************************************
int32_t DrvSPI_SetByteReorder(E_DRVSPI_PORT eSpiPort, E_DRVSPI_BYTE_REORDER eOption)
{
    //
    // The Byte Suspend function is only available in 32-bit transaction. 
    //
    if( (eOption==eDRVSPI_BYTE_REORDER_SUSPEND)||(eOption==eDRVSPI_BYTE_SUSPEND) )
        if( (SPI_PORT[eSpiPort]->CNTRL.TX_BIT_LEN) != 0 )
            return E_DRVSPI_ERR_BIT_LENGTH;
    //
    // The Byte Reorder function is supported only in 16-, 24- and 32-bit
    //  transaction mode. 
    //
    else if( eOption==eDRVSPI_BYTE_REORDER )
    	if( (SPI_PORT[eSpiPort]->CNTRL.TX_BIT_LEN) % 8 )
            return E_DRVSPI_ERR_BIT_LENGTH;

    SPI_PORT[eSpiPort]->CNTRL.REORDER = eOption;

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Set the number of clock cycle of the suspend interval. Only for
//!  master mode. 
//!
//! \param eSpiPort Specify the SPI port.
//!
//! \param i32Interval In burst transfer mode, this value specified the delay
//!  clocks between successive transactions. If the Byte Suspend function is
//!  enabled, it specified the delay clocks among each byte. It could be 2~17
//!  which indicate 2~17 SPI clock cycles. 
//!
//! \return E_DRVSPI_ERR_SUSPEND_INTERVAL The suspend interval setting is out
//!         of range. 
//!         E_SUCCESS Success.  
//
//*****************************************************************************
int32_t DrvSPI_SetSuspendCycle(E_DRVSPI_PORT eSpiPort, int32_t i32Interval)
{
    if((i32Interval < 2) || (i32Interval > 17))
    {
        //
        // If out of range, specify the maximum suspend cycle and return error
        //  code. 
        //
        SPI_PORT[eSpiPort]->CNTRL.SP_CYCLE = 15;
        return E_DRVSPI_ERR_SUSPEND_INTERVAL;
    }

    SPI_PORT[eSpiPort]->CNTRL.SP_CYCLE = i32Interval-2;

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Set the trigger mode of slave select pin. Only for slave mode. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param eSSTriggerMode Specify the trigger mode. (eDRVSPI_EDGE_TRIGGER or
//!  eDRVSPI_LEVEL_TRIGGER) 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_SetTriggerMode(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SSLTRIG eSSTriggerMode)
{
    SPI_PORT[eSpiPort]->SSR.SS_LTRIG = eSSTriggerMode;
}

//*****************************************************************************
//
//! \brief Set the active level of slave select. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param eSSActType Select the active type of slave select pin.
//!  eDRVSPI_ACTIVE_LOW_FALLING Slave select pin is active low in level-trigger
//!  mode; or falling-edge trigger in edge-trigger mode.
//!  eDRVSPI_ACTIVE_HIGH_RISING Slave select pin is active high in level
//! -trigger mode; or rising-edge trigger in edge-trigger mode. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_SetSlaveSelectActiveLevel(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SS_ACT_TYPE eSSActType)
{
    SPI_PORT[eSpiPort]->SSR.SS_LVL = eSSActType;
}

//*****************************************************************************
//
//! \brief Get the level-trigger transmission status. Only for slave mode. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return TRUE The transaction number and the transferred bit length met the
//!  requirements which defines in TX_NUM and TX_BIT_LEN among one transfer.
//!  FALSE: The transaction number or the transferred bit length of one
//!  transaction doesn't meet the requirements.  
//
//*****************************************************************************
uint8_t DrvSPI_GetLevelTriggerStatus(E_DRVSPI_PORT eSpiPort)
{
    if(SPI_PORT[eSpiPort]->SSR.LTRIG_FLAG==1)
        return TRUE;
    else
        return FALSE;
}

//*****************************************************************************
//
//! \brief Enable the automatic slave select function and set the specified
//!  slave select pin. Only for master mode. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_EnableAutoSS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.AUTOSS = 1;
    SPI_PORT[eSpiPort]->SSR.SSR = 1;
}

//*****************************************************************************
//
//! \brief Disable the Automatic Slave Select function and deselect slave select
//!  pins. Only for master mode. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_DisableAutoSS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.AUTOSS = 0;
    SPI_PORT[eSpiPort]->SSR.SSR = 0;
}

//*****************************************************************************
//
//! \brief Set the slave select pins. Only for master mode. In automatic slave
//!  select operation, call this function to select the slave select pin. The
//!  state of slave select pin will be controlled by hardware. In manual slave
//!  select operation, the slave select pin will be set to active state. 
//!
//! \param eSpiPort : Specify the SPI port 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_SetSS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.SSR = 1;
}

//*****************************************************************************
//
//! \brief Set the slave select pin to inactive state. Only for master mode. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_ClrSS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.SSR = 0;
}

//*****************************************************************************
//
//! \brief Check the busy status of the specified SPI port. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return TRUE The SPI port is in busy. 
//!         FALSE The SPI port is not in busy. 
//!  
//
//*****************************************************************************
uint8_t DrvSPI_IsBusy(E_DRVSPI_PORT eSpiPort)
{
    return ((SPI_PORT[eSpiPort]->CNTRL.GO_BUSY)?TRUE:FALSE);
}

//*****************************************************************************
//
//! \brief Configure the burst transfer settings. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param i32BurstCnt Specify the transaction number in one transfer. It
//!  could be 1 or 2.
//!
//! \param i32Interval Specify the delay clocks between successive
//!  transactions. It could be 2~17. 
//!
//! \return E_DRVSPI_ERR_BURST_CNT The transaction number is out of range. 
//!         E_DRVSPI_ERR_SUSPEND_INTERVAL The suspend interval setting is out of range.
//!         E_SUCCESS Success.  
//
//*****************************************************************************
int32_t DrvSPI_BurstTransfer(E_DRVSPI_PORT eSpiPort, int32_t i32BurstCnt, int32_t i32Interval)
{

    if((i32BurstCnt < 1) || (i32BurstCnt > 2))
    {
        return E_DRVSPI_ERR_BURST_CNT;
    }
    
    if((i32Interval < 2) || (i32Interval > 17))
    {
        return E_DRVSPI_ERR_SUSPEND_INTERVAL;
    }

    SPI_PORT[eSpiPort]->CNTRL.TX_NUM = i32BurstCnt-1;
    SPI_PORT[eSpiPort]->CNTRL.SP_CYCLE = i32Interval-2;

    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Configure the SPI clock. Only for master mode. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param u32Clock1 Specify the SPI clock rate in Hz. It's the target clock
//!  rate of SPI base clock and variable clock 1.
//!
//! \param u32Clock2 : Specify the SPI clock rate in Hz. It's the target clock
//!  rate of variable clock 2. 
//!
//! \return The actual clock rate of SPI engine clock is returned. SPI engine
//!  clock rate = APB clock rate / ((DIVIDER + 1) * 2) The actual clock rate may
//!  be different from the target SPI clock rate. For example, if the system
//!  clock rate is 12MHz and the target SPI base clock rate is 7MHz, the actual
//!  SPI clock rate will be 6MHz.  
//
//*****************************************************************************
uint32_t DrvSPI_SetClockFreq(E_DRVSPI_PORT eSpiPort, uint32_t u32Clock1, uint32_t u32Clock2)
{
    uint32_t u32Div;
    uint32_t u32Pclk;
    
    u32Pclk = DrvSYS_GetHCLKFreq();
    
    //
    // Initial value
    //
    u32Div = 0xFFFF; 

    if(u32Clock2!=0)
    {
        if(u32Clock2>u32Pclk)
            u32Div = 0;
        else
        {
            u32Div = (((u32Pclk / u32Clock2) + 1) >> 1) - 1;
            if(u32Div > 65535)
                u32Div = 65535;
        }
        SPI_PORT[eSpiPort]->DIVIDER.DIVIDER2 = u32Div;
    }
    else
        SPI_PORT[eSpiPort]->DIVIDER.DIVIDER2 = 0xFFFF;
    
    if(u32Clock1!=0)
    {
        if(u32Clock1>u32Pclk)
            u32Div = 0;
        else
        {
            u32Div = (((u32Pclk / u32Clock1) + 1) >> 1) - 1;
            if(u32Div > 0xFFFF)
                u32Div = 0xFFFF;
        }
        SPI_PORT[eSpiPort]->DIVIDER.DIVIDER = u32Div;
    }
    else
        SPI_PORT[eSpiPort]->DIVIDER.DIVIDER = 0xFFFF;
    
    return ( u32Pclk / ((u32Div+1)*2) );
}

//*****************************************************************************
//
//! \brief Get the SPI engine clock rate in Hz. Only for master mode. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return The current SPI bus clock frequency in Hz.  
//
//*****************************************************************************
uint32_t DrvSPI_GetClock1Freq(E_DRVSPI_PORT eSpiPort)
{
    uint32_t u32Div;
    uint32_t u32ApbClock;

    u32ApbClock = DrvSYS_GetHCLKFreq();
    u32Div = SPI_PORT[eSpiPort]->DIVIDER.DIVIDER;
    //
    // SPI_CLK = APB_CLK / ((Divider + 1) * 2)
    //
    return ((u32ApbClock >> 1) / (u32Div + 1));   
}

//*****************************************************************************
//
//! \brief Get the clock rate of variable clock 2 in Hz. Only for master mode. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return The frequency of variable clock 2 in Hz.  
//
//*****************************************************************************
uint32_t DrvSPI_GetClock2Freq(E_DRVSPI_PORT eSpiPort)
{
    uint32_t u32Div;
    uint32_t u32ApbClock;

    u32ApbClock = DrvSYS_GetHCLKFreq();
    u32Div = SPI_PORT[eSpiPort]->DIVIDER.DIVIDER2;
    //
    // SPI_CLK = APB_CLK / ((Divider + 1) * 2)
    //
    return ((u32ApbClock >> 1) / (u32Div + 1));   
}

//*****************************************************************************
//
//! \brief Set the variable clock function. Only for master mode. If the bit
//!  pattern of VARCLK is '0', the output frequency of SPICLK is according to
//!  the value of DIVIDER. If the bit pattern of VARCLK is '1', the output
//!  frequency of SPICLK is according to the value of DIVIDER2. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param bEnable TRUE  -- Enable variable clock function FALSE -- Disable
//!  variable clock function
//!
//! \param u32Pattern Specify the variable clock pattern 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_SetVariableClockFunction(E_DRVSPI_PORT eSpiPort, uint8_t bEnable, uint32_t u32Pattern)
{
    if(bEnable)
    {
        SPI_PORT[eSpiPort]->CNTRL.VARCLK_EN = 1;
        SPI_PORT[eSpiPort]->VARCLK = u32Pattern;
    }
    else
        SPI_PORT[eSpiPort]->CNTRL.VARCLK_EN = 0;
}

//*****************************************************************************
//
//! \brief Enable the SPI interrupt of the specified SPI port and install the
//!  callback function. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param pfnCallback The callback function of SPI interrupt.
//!
//! \param u32UserData The parameter which will be passed to the callback
//!  function. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_EnableInt(E_DRVSPI_PORT eSpiPort, PFN_DRVSPI_CALLBACK pfnCallback, uint32_t u32UserData)
{
    
    if(pfnCallback != NULL)
    {
        g_sSpiHandler[eSpiPort].pfncallback = pfnCallback;
        g_sSpiHandler[eSpiPort].u32userData = u32UserData;
    }
    
    SPI_PORT[eSpiPort]->CNTRL.IE = 1;
    
    if(eSpiPort == eDRVSPI_PORT0)
    {
        NVIC_SetPriority(SPI0_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(SPI0_IRQn);
    }
    else
    {
        NVIC_SetPriority(SPI1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(SPI1_IRQn);
    }
}   

//*****************************************************************************
//
//! \brief Disable the SPI interrupt. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_DisableInt(E_DRVSPI_PORT eSpiPort)
{
    g_sSpiHandler[eSpiPort].pfncallback = NULL;
    g_sSpiHandler[eSpiPort].u32userData = 0;
    
    SPI_PORT[eSpiPort]->CNTRL.IE = 0;

    if(eSpiPort == eDRVSPI_PORT0)
    {
        NVIC_DisableIRQ(SPI0_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(SPI1_IRQn);
    }
}

//*****************************************************************************
//
//! \brief Return the SPI interrupt flag 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return 0 SPI interrupt doesn't occur
//!         1 SPI interrupt occurs  
//
//*****************************************************************************
uint32_t DrvSPI_GetIntFlag(E_DRVSPI_PORT eSpiPort)
{
    return (SPI_PORT[eSpiPort]->CNTRL.IF);
}

//*****************************************************************************
//
//! \brief Clear the SPI interrupt flag 
//!
//! \param eSpiPort : Specify the SPI port 
//!
//! \return None  
//
//*****************************************************************************
void DrvSPI_ClrIntFlag(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->CNTRL.IF = 1;
}

//*****************************************************************************
//
//! \brief Read data from SPI Rx registers and trigger SPI for next transfer. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param pu32Data Store the data got from the SPI bus. 
//!
//! \return TRUE The data stored in pu32Data is valid. 
//!         FALSE The data stored in pu32Data is invalid.  
//
//*****************************************************************************
uint8_t DrvSPI_SingleRead(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Data)
{
    if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
        return FALSE;

    *pu32Data = SPI_PORT[eSpiPort]->RX[0];
    SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;
    return TRUE;
}

//*****************************************************************************
//
//! \brief Write data to SPI bus and trigger SPI to start transfer. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param pu32Data Store the data which will be transmitted through the SPI
//!  bus. 
//!
//! \return TRUE The data stored in pu32Data has been transferred.
//!         FALSE The SPI is in busy. The data stored in pu32Data has not been transferred.  
//
//*****************************************************************************
uint8_t DrvSPI_SingleWrite(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Data)
{
    if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
        return FALSE;

    SPI_PORT[eSpiPort]->TX[0] = *pu32Data;
    SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;
    return TRUE;
}

//*****************************************************************************
//
//! \brief Read two words of data from SPI Rx registers and then trigger SPI for
//!  next transfer. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param pu32Buf Store the data got from the SPI bus. 
//!
//! \return TRUE The data stored in pu32Buf is valid.
//!         FALSE The data stored in pu32Buf is invalid.  
//
//*****************************************************************************
uint8_t DrvSPI_BurstRead(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf)
{
    if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
        return FALSE;

    pu32Buf[0] = SPI_PORT[eSpiPort]->RX[0];
    pu32Buf[1] = SPI_PORT[eSpiPort]->RX[1];
    SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;

    return TRUE;
}

//*****************************************************************************
//
//! \brief Write two words of data to SPI bus and then trigger SPI to start
//!  transfer. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param pu32Buf Store the data which will be transmitted through the SPI
//!  bus. 
//!
//! \return TRUE The data stored in pu32Buf has been transferred.
//!         FALSE The SPI is in busy. The data stored in pu32Buf has not been transferred.  
//
//*****************************************************************************
uint8_t DrvSPI_BurstWrite(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf)
{
    if(SPI_PORT[eSpiPort]->CNTRL.GO_BUSY==1)
        return FALSE;

    SPI_PORT[eSpiPort]->TX[0] = pu32Buf[0];
    SPI_PORT[eSpiPort]->TX[1] = pu32Buf[1];
    SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;

    return TRUE;
}

//*****************************************************************************
//
//! \brief Read data from Rx registers. This function will not trigger a SPI
//!  data transfer. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param pu32Buf Store the data got from Rx registers.
//!
//! \param u32DataCount The count of data read from Rx registers. 
//!
//! \return The count of data actually read from Rx registers.  
//
//*****************************************************************************
uint32_t DrvSPI_DumpRxRegister(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf, uint32_t u32DataCount)
{
    uint32_t i;
    
    if(u32DataCount>2)
        u32DataCount = 2;
    
    for(i=0; i<u32DataCount; i++)
    {
        pu32Buf[i] = SPI_PORT[eSpiPort]->RX[i];
    }

    return u32DataCount;
}

//*****************************************************************************
//
//! \brief Write data to Tx registers. This function will not trigger a SPI data
//!  transfer. 
//!
//! \param eSpiPort Specify the SPI port
//!
//! \param pu32Buf Store the data which will be written to Tx registers.
//!
//! \param u32DataCount The count of data write to Tx registers. 
//!
//! \return The count of data actually written to Tx registers.  
//
//*****************************************************************************
uint32_t DrvSPI_SetTxRegister(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf, uint32_t u32DataCount)
{
    uint32_t i;

    if(u32DataCount>2)
        u32DataCount = 2;
    
    for(i=0; i<u32DataCount; i++)
    {
        SPI_PORT[eSpiPort]->TX[i] = pu32Buf[i];
    }

    return u32DataCount;
}

//*****************************************************************************
//
//! \brief Set the GO_BUSY bit to trigger a SPI data trasfer. 
//!
//! \param eSpiPort Specify the SPI port 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_SetGo(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 1;
}

//*****************************************************************************
//
//! \brief Clear the GO_BUSY bit. 
//!
//! \param eSpiPorr Specify the SPI port 
//!
//! \return None.  
//
//*****************************************************************************
void DrvSPI_ClrGo(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->CNTRL.GO_BUSY = 0;
}

//*****************************************************************************
//
//! \brief Get the version number of NUC100 SPI driver. 
//!
//! \param None 
//!
//! \return Version number.  
//
//*****************************************************************************
uint32_t DrvSPI_GetVersion(void)
{
    return DRVSPI_VERSION_NUM;
}
