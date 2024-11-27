#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "mpl3115.h"



void MPL3115A2_Hasieratu_Polling(int fitx)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG1, 0xB8); //Sentsorearen  oversampling ratio 128 (512 ms-tara jarri)
        MPL3115A2_EnableEventFlags(fitx); //Beharrezko flag-ak pizten ditu
        MPL3115A2_ActiveMode(fitx); // Hasieratu irakurketak
}

unsigned char MPL3115A2_Id(int fitx)
{
        unsigned char id = MPL3115A2_ReadByte(fitx,WHO_AM_I);
        return id;
}

unsigned char MPL3115A2_GetMode(int fitx)
{
        unsigned char status = MPL3115A2_ReadByte(fitx,SYSMOD);
        return (status & 0x01) ? 1 : 0;
}

int MPL3115A2_Datu_berriaztuak(int fitx){

    unsigned char status = MPL3115A2_ReadByte(fitx,STATUS);
    return (status & 0x08) ? 1 : 0;

}

void MPL3115A2_ActiveMode(int fitx)
{
        //unsigned char tempSetting = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
//        printf("activate 1 0x%02X\n",tempSetting);        
  //      tempSetting |= SBYB; //Set SBYB bit for Active mode
    //    printf("activate 2 0x%02X\n",tempSetting);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, 0xB9);
}

void MPL3115A2_AltimeterMode(int fitx)
{
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg &= ~(SBYB); //Go to Standby mode
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg);

        ctrl_reg = ALT|OS_128; //Set ALT bit to one and enable back Active mode
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg);
}

float MPL3115A2_ReadAltitude(int fitx)
{
        char altpbyte[3] = {0x00};
        MPL3115A2_ReadByteArray(fitx,OUT_P_MSB,altpbyte,3); //Read altitude data
        int m_altitude = altpbyte[0];
        int c_altitude = altpbyte[1];
        int l_altitude = altpbyte[2];
        uint32_t al = (m_altitude << 24)|(c_altitude < 16)| (l_altitude);
        return (float)(al/65536);
}

void MPL3115A2_SetAltimeterOffset(int fitx,unsigned char H_Offset)
{
        if((H_Offset > -128) || (H_Offset < 128))
        {
                MPL3115A2_WriteByte(fitx,OFF_H,H_Offset);
        }
}

void MPL3115A2_BarometerMode(int fitx)
{
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg &= ~(SBYB); //Set SBYB to 0 and go to Standby mode
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg);

        ctrl_reg = OS_128 ; //Set ALT bit to zero and enable back Active mode
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg);
}

void MPL3115A2_SetPressureOffset(int fitx,unsigned char P_Offset)
{
        if((P_Offset > -128) || (P_Offset < 128))
        {
                MPL3115A2_WriteByte(fitx,OFF_P,P_Offset);
        }
}

float MPL3115A2_GetMinimumPressure(int fitx)
{
        char minPressure[3] = {0x00};
        MPL3115A2_ReadByteArray(fitx,P_MIN_MSB,minPressure,3);

        unsigned char m_altitude = minPressure[0];
        unsigned char c_altitude = minPressure[1];
        float l_altitude = (float)(minPressure[2]>>4)/4; //dividing by 4, since two lowest bits are fractional value
        return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); //shifting 2 to the left to make room for LSB

}

float MPL3115A2_GetMaximumPressure(int fitx)
{
        char maxPressure[3] = {0x00};
        MPL3115A2_ReadByteArray(fitx,P_MAX_MSB,maxPressure,3);

        unsigned char m_altitude = maxPressure[0];
        unsigned char c_altitude = maxPressure[1];
        float l_altitude = (float)(maxPressure[2]>>4)/4; //dividing by 4, since two lowest bits are fractional value
        return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); //shifting 2 to the left to make room for LSB
}

unsigned int MPL3115A2_ReadBarometicPressureInput(int fitx)
{
        unsigned char barMSB = MPL3115A2_ReadByte(fitx,BAR_IN_MSB);
        unsigned char barLSB = MPL3115A2_ReadByte(fitx,BAR_IN_LSB);

        return ((barMSB << 8) | barLSB);
}

float MPL3115A2_ReadBarometricPressure(int fitx)
{
        unsigned char status = MPL3115A2_ReadByte(fitx,STATUS);
        char pbyte[3] = {0x00};

        //Check PDR bit, if it's not set then toggle OST
        if((status & (1<< PDR)) == 0)
        {
                MPL3115A2_ToggleOneShot(fitx); //Toggle the OST bit causing the sensor to immediately take another reading
        }

        MPL3115A2_ReadByteArray(fitx,OUT_P_MSB,pbyte,3);

        //this function takes values from the read buffer and converts them to pressure units
        unsigned long m_altitude = pbyte[0];
        unsigned long c_altitude = pbyte[1];
        float l_altitude = (float)(pbyte[2]>>4)/4; //dividing by 4, since two lowest bits are fractional value
        return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); //shifting 2 to the left to make room for LSB
}

float MPL3115A2_ReadPressure(int fitx,unitsType units)
{
        float pressure = MPL3115A2_ReadBarometricPressure(fitx);

        switch (units)
        {
                case PSI:
                        return pressure * 0.000145037738;
                case INHG:
                        return pressure * 0.00029529983071;
                case MMHG:
                        return pressure * 0.007500615613;
        }
        return pressure;
}

void MPL3115A2_SetPressureAlarmThreshold(int fitx,unsigned int thresh)
{
        unsigned char threshMSB = (unsigned char) (thresh << 8);
        unsigned char threshLSB = (unsigned char) thresh;
        MPL3115A2_WriteByte(fitx,P_TGT_MSB,threshMSB);
        MPL3115A2_WriteByte(fitx,P_TGT_LSB,threshLSB);
}

void MPL3115A2_SetPressureTargetWindow(int fitx,unsigned int target,unsigned int window)
{
        unsigned char tmpMSB = (unsigned char) (window << 8);
        unsigned char tmpLSB = (unsigned char) (window);
        MPL3115A2_WriteByte(fitx,P_WND_MSB,tmpMSB);
        MPL3115A2_WriteByte(fitx,P_WND_LSB,tmpLSB);

        tmpMSB = (unsigned char) (target << 8);
        tmpLSB = (unsigned char) (target);
        MPL3115A2_WriteByte(fitx,P_WND_MSB,tmpMSB);
        MPL3115A2_WriteByte(fitx,P_WND_LSB,tmpLSB);
}

float MPL3115A2_ReadTemperature(int fitx)
{
        char temperature[2] = {0x00};
        MPL3115A2_ReadByteArray(fitx,OUT_T_MSB,temperature,2);

        float templsb = (temperature[1]>>4) / 16.0; //temp, fraction of a degree
        float unpacked = (float)(temperature[0] + templsb);

        return unpacked;
}

float MPL3115A2_GetMinimumTemperature(int fitx)
{
        char temperature[2] = {0x00};
        MPL3115A2_ReadByteArray(fitx,T_MIN_MSB,temperature,2);

        float templsb = (temperature[1]>>4) / 16.0; //temp, fraction of a degree
        float minTemp = (float)(temperature[0] + templsb);

        return minTemp;
}

float MPL3115A2_GetMaximumTemperature(int fitx)
{
        char temperature[2] = {0x00};
        MPL3115A2_ReadByteArray(fitx,T_MAX_MSB,temperature,2);

        float templsb = (temperature[1]>>4) / 16.0; //temp, fraction of a degree
        float maxTemp = (float)(temperature[0] + templsb);

        return maxTemp;
}

void MPL3115A2_SetTempTargetWindow(int fitx,unsigned int target,unsigned int window)
{
        MPL3115A2_WriteByte(fitx,T_TGT,target);
        MPL3115A2_WriteByte(fitx,T_WND,window);
}

void MPL3115A2_SetTemperatureThreshold(int fitx,unsigned char thresh)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG4,INT_EN_TTH); //enable temperature interrupt
        MPL3115A2_WriteByte(fitx,CTRL_REG5,INT_EN_TTH); //map to interrupt
}

void MPL3115A2_SetTempOffset(int fitx,char T_Offset)
{
        MPL3115A2_WriteByte(fitx,OFF_T,T_Offset);
}

void MPL3115A2_OutputSampleRate(int fitx,unsigned char sampleRate)
{
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg &= ~(1 << SBYB);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg); //Put sensor in Standby mode

        if(sampleRate > 7) sampleRate = 7; //OS cannot be larger than 0b.0111
        sampleRate <<= 3; //Align it for the CTRL_REG1 register

        ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg |= sampleRate; //Mask in new Output Sample bits
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg); //Update sample rate settings
}

void MPL3115A2_SetAcquisitionTimeStep(int fitx,unsigned char ST_Value)
{
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg &= ~(1 << SBYB);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg); //Put sensor in Standby mode

        if (ST_Value <= 0xF)
        {
                ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG2); //Read current settings
                MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg|ST_Value);
        }
}

void MPL3115A2_EnableEventFlags(int fitx)
{
        MPL3115A2_WriteByte(fitx,PT_DATA_CFG, 0x07); // Presio eta ternperatura  Flag-ak ahalbidetzen ditu.
}

void MPL3115A2_ToggleOneShot(int fitx)
{
        unsigned char tempSetting = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        tempSetting &= ~(1<<1); //Clear OST bit
        MPL3115A2_WriteByte(fitx,CTRL_REG1, tempSetting);
        tempSetting = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings to be safe
        tempSetting |= (1<<1); //Set OST bit
        MPL3115A2_WriteByte(fitx,CTRL_REG1, tempSetting);
}

void MPL3115A2_ConfigureInterruptPin(int fitx,unsigned char intrrpt,unsigned char pin)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG5,(pin << intrrpt));
}

void MPL3115A2_ConfigurePressureInterrupt(int fitx)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG3,PP_OD1|PP_OD2); //Configure Interrupt pins for open drain active low
}

void MPL3115A2_ConfigureAltitudeInterrupt(int fitx)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG3,PP_OD1|PP_OD2); //Configure Interrupt pins for open drain active low
}

void MPL3115A2_ClearInterrupts(int fitx)
{
        MPL3115A2_ReadByte(fitx,OUT_P_MSB);
        MPL3115A2_ReadByte(fitx,OUT_P_CSB); //Configure Interrupt pins for open drain active low
        MPL3115A2_ReadByte(fitx,OUT_P_LSB);
        MPL3115A2_ReadByte(fitx,OUT_T_MSB);
        MPL3115A2_ReadByte(fitx,OUT_T_LSB);
        MPL3115A2_ReadByte(fitx,F_STATUS);
}


unsigned char MPL3115A2_ReadByte(int fitx,char reg)
{
        char regis[1]={0};
        regis[0]=reg;
        char byte[1];
        read(fitx,byte,1);
        return byte[0];
}

void MPL3115A2_ReadByteArray(int fitx,char reg,char *buffer, unsigned int length)
{
        char regis[1]={0};
        regis[0]=reg;
        write(fitx,regis,1);//helbidea finkatu            
        read(fitx,buffer,length);
        printf("xdddd %s",buffer);
}

void MPL3115A2_WriteByte(int fitx,char reg, char value)
{
        char idaz[2];
        idaz[0] = reg;
        idaz[1] = value;
        write(fitx,idaz,2);            
}

void MPL3115A2_WriteByteArray(int fitx,char reg, char* buffer, unsigned int length)
{
        unsigned char idaz[length+1];
        idaz[0] = reg;
        //idaz[1] = value;
        
        write(fitx,idaz,2);            
        i2c_smbus_write_i2c_block_data(fitx,reg,length, buffer);
}



