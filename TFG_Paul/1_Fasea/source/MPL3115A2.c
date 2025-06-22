#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../includes/MPL3115A2.h"


//Sentsorea hasieratu inkesta bidez irakurketak egiteko
void MPL3115A2_Hasieratu_Polling(int fitx)
{
        MPL3115A2_StandbyMode(fitx);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, 0xB8); //Sentsorearen  oversampling ratio 128 (512 ms-tara jarri)
        MPL3115A2_Baimendu_Flagak(fitx); //Beharrezko flag-ak pizten ditu
        MPL3115A2_ActiveMode(fitx); // Hasieratu irakurketak
}

//Sentsorea hasieratu etenen bidez irakurketak egiteko
void MPL3115A2_Hasieratu_Interrupt(int fitx)
{
        MPL3115A2_StandbyMode(fitx);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, 0xB8); //Sentsorearen  oversampling ratio 128 (512 ms-tara jarri)
        MPL3115A2_Baimendu_Flagak(fitx); //Beharrezko flag-ak pizten ditu
        MPL3115A2_Interrupzioak_konfiguratu(fitx);
        MPL3115A2_ActiveMode(fitx); // Hasieratu irakurketak

}

//Sentsorearen Id-a bueltatzen du, erabili daiteke konprobatzeko ondo konektatuta dagoela senstsorea , balio lehenetsia C4 da.
unsigned char MPL3115A2_Id(int fitx)
{
        unsigned char id = MPL3115A2_ReadByte(fitx,WHO_AM_I);
        return id;
}

//1 Bueltatzen du irakurketa moduan dagoenean eta 0 konfigurazio moduan dagoenan. 
unsigned char MPL3115A2_GetMode(int fitx)
{
        unsigned char status = MPL3115A2_ReadByte(fitx,SYSMOD);
        return (status & 0x01) ? 1 : 0;
}

//Konfigutazo moduan jartzen du senstsorea.
void MPL3115A2_StandbyMode(int fitx)
{
      unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1);
      ctrl_reg &= ~SBYB;                                      
      MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg);               
}

//1 bueltatuko du aldaketa egon bada azkeneko irakurketatik bestela 0
int MPL3115A2_Datu_berriaztuak(int fitx)
{

    unsigned char status = MPL3115A2_ReadByte(fitx,STATUS);
    return (status & 0x08) ? 1 : 0;

}

//Irakurketa moduan jartzen du sentsorea
void MPL3115A2_ActiveMode(int fitx)
{
        unsigned char tempSetting = MPL3115A2_ReadByte(fitx,CTRL_REG1); 
        tempSetting |= SBYB; 
        MPL3115A2_WriteByte(fitx,CTRL_REG1, tempSetting);
}

//Senstsorea altitudea irakurtzeko konfiguratzen du.
void MPL3115A2_AltimeterMode(int fitx)
{
        MPL3115A2_StandbyMode(fitx);
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); 

        ctrl_reg = ALT|OS_128;
        MPL3115A2_ActiveMode(fitx);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg);
}

//Altitudearen balioa bueltatzen du.
float MPL3115A2_ReadAltitude(int fitx)
{
        char altpbyte[3] = {0x00};
        MPL3115A2_ReadByteArray(fitx,OUT_P_MSB,altpbyte,3); 
        int m_altitude = altpbyte[0];
        int c_altitude = altpbyte[1];
        int l_altitude = altpbyte[2];
        uint32_t al = (m_altitude << 24)|(c_altitude < 16)| (l_altitude);
        return (float)(al/65536);
}


//Senstsorea presioa irakurtzeko konfiguratzen du.
void MPL3115A2_BarometerMode(int fitx)
{
        MPL3115A2_StandbyMode(fitx);
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings

        ctrl_reg = OS_128 ; //Set ALT bit to zero and enable back Active mode

        MPL3115A2_WriteByte(fitx,CTRL_REG1,ctrl_reg);
        MPL3115A2_ActiveMode(fitx);
}

//Presioaren irakurritako balio baxuena bueltatzen du.
float MPL3115A2_GetMinimumPressure(int fitx)
{
        char minPressure[3] = {0x00};
        MPL3115A2_ReadByteArray(fitx,P_MIN_MSB,minPressure,3);

        unsigned char m_altitude = minPressure[0];
        unsigned char c_altitude = minPressure[1];
        float l_altitude = (float)(minPressure[2]>>4)/4; 
        return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); 

}

//Presioaren irakurritako balio maximoa bueltatzen du.
float MPL3115A2_GetMaximumPressure(int fitx)
{
        char maxPressure[3] = {0x00};
        MPL3115A2_ReadByteArray(fitx,P_MAX_MSB,maxPressure,3);

        unsigned char m_altitude = maxPressure[0];
        unsigned char c_altitude = maxPressure[1];
        float l_altitude = (float)(maxPressure[2]>>4)/4; 
        return((float)(m_altitude<<10 | c_altitude<<2)+l_altitude); 
}


//Presioaren balioa bueltatzen du.
float MPL3115A2_ReadPressure(int fitx)
{
        char pbyte[3];
        char reg = OUT_P_MSB;
        MPL3115A2_ReadByteArray(fitx,reg,pbyte,3);

        int pres = ((pbyte[0] * 65536) + (pbyte[1] * 256 + (pbyte[2] & 0xF0))) / 16;
        float pressure = (pres / 4.0) / 100.0;

        return pressure;
}

//Temperatura bueltatzen du.
float MPL3115A2_ReadTemperature(int fitx)
{
        char temperature[2];
        char reg[1] = {0x04};
        MPL3115A2_ReadByteArray(fitx,0x04,temperature,2);

        int temp = ((temperature[0] * 256) + (temperature[1] & 0xF0)) / 16;
        float cTemp = (temp / 16.0);
       
        return cTemp;
}

//Temperatura irakurritako balio baxuena bueltatzen du.
float MPL3115A2_GetMinimumTemperature(int fitx)
{
        char temperature[2] = {0x00};
        MPL3115A2_ReadByteArray(fitx,T_MIN_MSB,temperature,2);

        float templsb = (temperature[1]>>4) / 16.0; //temp, fraction of a degree
        float minTemp = (float)(temperature[0] + templsb);

        return minTemp;
}

//Temperatura irakurritako balio maximoa bueltatzen du.
float MPL3115A2_GetMaximumTemperature(int fitx)
{
        char temperature[2] = {0x00};
        MPL3115A2_ReadByteArray(fitx,T_MAX_MSB,temperature,2);

        float templsb = (temperature[1]>>4) / 16.0; //temp, fraction of a degree
        float maxTemp = (float)(temperature[0] + templsb);

        return maxTemp;
}

//Presio eta Tenperatura flag-ak konfiguratu
void MPL3115A2_Interrupzioak_konfiguratu(int fitx)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG4,INT_EN_PTH|INT_EN_PCHG|INT_EN_TCHG); 
        MPL3115A2_WriteByte(fitx,CTRL_REG5,INT_EN_DRDY); 
}


/// Presio eta ternperatura  Flag-ak ahalbidetzen ditu.
void MPL3115A2_Baimendu_Flagak(int fitx)
{
        MPL3115A2_WriteByte(fitx,PT_DATA_CFG, 0x07); 
}

//Konprobatzen du ea falg-renbat piztu den
int MPL3115A2_konprobatu_flaga(int fitx)
{
        unsigned char  emaitza = MPL3115A2_ReadByte(fitx,0x12);
        return (emaitza & 0x80) ? 1 : 0;

}

//Etenetarako erabilitako pin-a konfiguratzen du
void MPL3115A2_ConfigureInterruptPin(int fitx,unsigned char intrrpt,unsigned char pin)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG5,(pin << intrrpt));
}

//Konfiguratu Eten pinak beheko hustubide aktiborako
void MPL3115A2_ConfigurePressureInterrupt(int fitx)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG3,PP_OD1|PP_OD2);
}

//Konfiguratu Eten pinak beheko hustubide aktiborako
void MPL3115A2_ConfigureAltitudeInterrupt(int fitx)
{
        MPL3115A2_WriteByte(fitx,CTRL_REG3,PP_OD1|PP_OD2); 
}

//Eten pinen konfigurazioa garbitu
void MPL3115A2_ClearInterrupts(int fitx)
{
        MPL3115A2_ReadByte(fitx,OUT_P_MSB);
        MPL3115A2_ReadByte(fitx,OUT_P_CSB); 
        MPL3115A2_ReadByte(fitx,OUT_P_LSB);
        MPL3115A2_ReadByte(fitx,OUT_T_MSB);
        MPL3115A2_ReadByte(fitx,OUT_T_LSB);
        MPL3115A2_ReadByte(fitx,F_STATUS);
}

//Pasatuz irekitako sentsorearin dagokion fitxategia eta senstsore barruko erregistroa byte bat irakurtzen du.
unsigned char MPL3115A2_ReadByte(int fitx,char reg)
{
        return i2c_smbus_read_byte_data(fitx, reg);
}

//Pasatuz irekitako sentsorearin dagokion fitxategia eta senstsore barruko erregistroa byte array-a irakurtzen du.
void MPL3115A2_ReadByteArray(int fitx,char reg,char *buffer, unsigned int length)
{
        i2c_smbus_read_i2c_block_data(fitx,reg,length,buffer);
}

//Pasatuz irekitako sentsorearin dagokion fitxategia eta senstsore barruko erregistroa byte bat idazten du.
void MPL3115A2_WriteByte(int fitx,char reg, char value)
{
      i2c_smbus_write_byte_data(fitx, reg, value)   ;
}

//Pasatuz irekitako sentsorearin dagokion fitxategia eta senstsore barruko erregistroa byte array bat idazten du.
void MPL3115A2_WriteByteArray(int fitx,char reg, char* buffer, unsigned int length)
{     
        i2c_smbus_write_i2c_block_data(fitx,reg,length, buffer);
}

//Momentuz erabilgarriak ez diren funtzioak

/*void MPL3115A2_SetAltimeterOffset(int fitx,unsigned char H_Offset)
{
        if((H_Offset > -128) || (H_Offset < 128))
        {
                MPL3115A2_WriteByte(fitx,OFF_H,H_Offset);
        }
}*/

 /*void MPL3115A2_SetPressureOffset(int fitx,unsigned char P_Offset)
{
        if((P_Offset > -128) || (P_Offset < 128))
        {
                MPL3115A2_WriteByte(fitx,OFF_P,P_Offset);
        }
}*/

 /*unsigned int MPL3115A2_ReadBarometicPressureInput(int fitx)
{
        unsigned char barMSB = MPL3115A2_ReadByte(fitx,BAR_IN_MSB);
        unsigned char barLSB = MPL3115A2_ReadByte(fitx,BAR_IN_LSB);

        return ((barMSB << 8) | barLSB);
}*/


 /*void MPL3115A2_SetPressureAlarmThreshold(int fitx,unsigned int thresh)
{
        unsigned char threshMSB = (unsigned char) (thresh << 8);
        unsigned char threshLSB = (unsigned char) thresh;
        MPL3115A2_WriteByte(fitx,P_TGT_MSB,threshMSB);
        MPL3115A2_WriteByte(fitx,P_TGT_LSB,threshLSB);
}*/

/*void MPL3115A2_SetPressureTargetWindow(int fitx,unsigned int target,unsigned int window)
{
        unsigned char tmpMSB = (unsigned char) (window << 8);
        unsigned char tmpLSB = (unsigned char) (window);
        MPL3115A2_WriteByte(fitx,P_WND_MSB,tmpMSB);
        MPL3115A2_WriteByte(fitx,P_WND_LSB,tmpLSB);

        tmpMSB = (unsigned char) (target << 8);
        tmpLSB = (unsigned char) (target);
        MPL3115A2_WriteByte(fitx,P_WND_MSB,tmpMSB);
        MPL3115A2_WriteByte(fitx,P_WND_LSB,tmpLSB);
}*/

/*void MPL3115A2_SetTempTargetWindow(int fitx,unsigned int target,unsigned int window)
{
        MPL3115A2_WriteByte(fitx,T_TGT,target);
        MPL3115A2_WriteByte(fitx,T_WND,window);
}*/

/*void MPL3115A2_SetTempOffset(int fitx,char T_Offset)
{
        MPL3115A2_WriteByte(fitx,OFF_T,T_Offset);
}*/

/*void MPL3115A2_OutputSampleRate(int fitx,unsigned char sampleRate)
{
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg &= ~(1 << SBYB);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg); //Put sensor in Standby mode

        if(sampleRate > 7) sampleRate = 7; //OS cannot be larger than 0b.0111
        sampleRate <<= 3; //Align it for the CTRL_REG1 register

        ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg |= sampleRate; //Mask in new Output Sample bits
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg); //Update sample rate settings
}*/

/*void MPL3115A2_SetAcquisitionTimeStep(int fitx,unsigned char ST_Value)
{
        unsigned char ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        ctrl_reg &= ~(1 << SBYB);
        MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg); //Put sensor in Standby mode

        if (ST_Value <= 0xF)
        {
                ctrl_reg = MPL3115A2_ReadByte(fitx,CTRL_REG2); //Read current settings
                MPL3115A2_WriteByte(fitx,CTRL_REG1, ctrl_reg|ST_Value);
        }
}*/

/*void MPL3115A2_ToggleOneShot(int fitx)
{
        unsigned char tempSetting = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings
        tempSetting &= ~(1<<1); //Clear OST bit
        MPL3115A2_WriteByte(fitx,CTRL_REG1, tempSetting);
        tempSetting = MPL3115A2_ReadByte(fitx,CTRL_REG1); //Read current settings to be safe
        tempSetting |= (1<<1); //Set OST bit
        MPL3115A2_WriteByte(fitx,CTRL_REG1, tempSetting);
}*/
