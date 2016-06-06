#include "stm32f10x.h"
#include "mb.h" 
#include "mbutils.h"

void ENTER_CRITICAL_SECTION( )//���볬�ٽ� �����ж�
{
	__set_PRIMASK(1);
}

void EXIT_CRITICAL_SECTION( )//�˳����ٽ� �����ж�
{
	__set_PRIMASK(0);
}

#define REG_INPUT_START 0
#define REG_INPUT_NREGS 8

#define REG_HOLDING_START 0
#define REG_HOLDING_NREGS 8

#define REG_COILS_START 0
#define REG_COILS_SIZE 8

#define REG_DISCRETE_START 0
#define REG_DISCRETE_SIZE 8

u16 usRegInputBuf[REG_INPUT_NREGS] = {0x8001,0x7002,0x6003,0x5004,0x4005,0x3006,0x2007,0x1008};
u16 usRegHoldingBuf[REG_HOLDING_NREGS]={0x0810,0x0720,0x0630,0x0540,0x0450,0x0360,0x0270,0x0180};
u8 ucRegCoilsBuf[REG_COILS_SIZE]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
u8 ucRegDiscreteBuf[REG_DISCRETE_SIZE]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

//u16 usRegInputBuf[8]={0x0000,0xfe02,0x1314,0x1516,0x1405,0x1506,0x1607,0x1708};
//u16 *usRegHoldingBuf=usRegInputBuf;	//һ�������õ� �Ĵ������� ��ַ0-7
//
//u8 REG_INPUT_START=0,REG_HOLDING_START=0;
//u8 REG_INPUT_NREGS=8,REG_HOLDING_NREGS=8;
//u8 usRegInputStart=0,usRegHoldingStart=0;
//
//u8 ucRegCoilsBuf[8]={0x00,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
//u8 REG_COILS_START=0,REG_DISCRETE_START=0;
//
//u8 ucRegDiscreteBuf[8]={0x00,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
//u8 REG_COILS_SIZE=0,REG_DISCRETE_SIZE=0;

//�����ּĴ��� ������0x04

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

    if( ( (int16_t)usAddress >= REG_INPUT_START )&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - REG_INPUT_START );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;

}
// �Ĵ����Ķ�д���� ֧�ֵ�����Ϊ�� 0x03 ��д0x06

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
//	u16 *PRT=(u16*)pucRegBuffer;

    if( ( (int16_t)usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - REG_HOLDING_START );
        switch ( eMode )
        {
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
//                *PRT++ = __REV16(usRegHoldingBuf[iRegIndex++]); //������ת REV16.W

                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
//                usRegHoldingBuf[iRegIndex++] = __REV16(*PRT++); //������ת REV16.W

                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

//��/д���ؼĴ���  0x01  x05

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
//    ( void )pucRegBuffer;
//    ( void )usAddress;
//    ( void )usNCoils;
//    ( void )eMode;
//    return MB_ENOREG;
    
    //����״̬
    eMBErrorCode eStatus = MB_ENOERR;
    //�Ĵ�������
    int16_t iNCoils = ( int16_t )usNCoils;
    //�Ĵ���ƫ����
    int16_t usBitOffset;
    
    //���Ĵ����Ƿ���ָ����Χ��
    if( ( (int16_t)usAddress >= REG_COILS_START ) &&
    ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
    {
    //����Ĵ���ƫ����
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
    switch ( eMode )
    {
    //������
    case MB_REG_READ:
    while( iNCoils > 0 )
    {
    *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
    ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
    iNCoils -= 8;
    usBitOffset += 8;
    }
    break;
    
    //д����
    case MB_REG_WRITE:
    while( iNCoils > 0 )
    {
    xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
    ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
    *pucRegBuffer++ );
    iNCoils -= 8;
    }
    break;
    }
    
    }
    else
    {
    eStatus = MB_ENOREG;
    }
    return eStatus;
    
}

//�����ؼĴ��� 0x02
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
//    ( void )pucRegBuffer;
//    ( void )usAddress;
//    ( void )usNDiscrete;
//    return MB_ENOREG;
  
    //����״̬
    eMBErrorCode eStatus = MB_ENOERR;
    //�����Ĵ�������
    int16_t iNDiscrete = ( int16_t )usNDiscrete;
    //ƫ����
    uint16_t usBitOffset;
    
    //�жϼĴ���ʱ�����ƶ���Χ��
    if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
    ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
    {
    //���ƫ����
    usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );
    
    while( iNDiscrete > 0 )
    {
    *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
    ( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
    iNDiscrete -= 8;
    usBitOffset += 8;
    }
    
    }
    else
    {
    eStatus = MB_ENOREG;
    }
    return eStatus;  
  
}
