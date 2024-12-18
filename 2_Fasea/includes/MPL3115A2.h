#include <stdint.h>

#define __MPL3115A2_H__
#define TRUE 1
#define FALSE 0

#define MPL3115A2_ADDRESS 0x60  // Sentsorearen helbidea I2c-1 busean

#define STATUS 0x00                 //OUT_P eta OUT_T erregistroen egoerari buruzko informazioa eskaintzen du
#define OUT_P_MSB 0x01          //Presio ala altitude balioa (aukeratutako formatuaren arabera)(19-12)bitak
#define OUT_P_CSB 0x02          //(11-4)bitak
#define OUT_P_LSB 0x03          //(3-0)bitak
#define OUT_T_MSB 0x04          //Tenperatura balioa (11-4)bitak
#define OUT_T_LSB 0x05          //(3-0)bitak
#define DR_STATUS 0x06          //OUT_P eta OUT_T erregistroen egoerari buruzko informazioa eskaintzen du
#define OUT_P_DELTA_MSB 0x07    //Ezberdintasuna hartutako azkenek pRESIO ALA LATITUDEo balioarekiko (19-12)bitak
#define OUT_P_DELTA_CSB 0x08    //(11-4) bitak
#define OUT_P_DELTA_LSB 0x09    //(3-0)bitak
#define OUT_T_DELTA_MSB 0x0A    //Ezberdintasuna hartutako azkeneko tenperatura balioarekiko (19-12)bitak
#define OUT_T_DELTA_LSB 0x0B    //(3-0)
#define WHO_AM_I 0x0C           //SENTSOREAREN id-A

/**********Printzipioz erabiliko ez ditudan erregistroak**********/
#define F_STATUS 0x0D
#define F_DATA 0x0E
#define F_SETUP 0x0F
#define TIME_DLY 0x10
#define SYSMOD 0x11

#define BAR_IN_MSB 0x14
#define BAR_IN_LSB 0x15
#define P_TGT_MSB 0x16
#define P_TGT_LSB 0x17
#define T_TGT 0x18
#define P_WND_MSB 0x19
#define P_WND_LSB 0x1A
#define T_WND 0x1B
#define P_MIN_MSB 0x1C
#define P_MIN_CSB 0x1D
#define P_MIN_LSB 0x1E
#define T_MIN_MSB 0x1F
#define T_MIN_LSB 0x20
#define P_MAX_MSB 0x21
#define P_MAX_CSB 0x22
#define P_MAX_LSB 0x23
#define T_MAX_MSB 0x24
#define T_MAX_LSB 0x25

#define CTRL_REG5 0x2A
#define OFF_P 0x2B
#define OFF_T 0x2C
#define OFF_H 0x2D


/*******************Interrupt modurako erregistroak********************/
#define INT_SOURCE 0x12  // Sentsorearen irakurketak sortutako flag-en helbidea .
#define PT_DATA_CFG 0x13 // Tenperatura eta presioren irakurketak flag-ak sortu dezaten ala ez konfiguratzeko erregistroa.

/*************************Kontrol Erregistroak*************************/
#define CTRL_REG1 0x26 // Senstorea hasieratzeko eta honen zenbait parametro aukeratzeko balio dueen eregustroa.
#define CTRL_REG2 0x27 // Control parametroak ( beste artean konfiguratu zenbat denbora pasa flag-en artean).
#define CTRL_REG3 0x28 // Interrupt moduan erabiltzean konfiguratu daitezken parametroak.
#define CTRL_REG4 0x29 // interrupt anable register.

/**********************DR Status **************************************/
#define PTOW 0x80
#define POW 0x40 //Pressure overwrite
#define TOW 0x20
#define PTDR 0x08 //Pressure/Altitude OR Temperature data ready
#define PDR 0x04 //Pressure/Altitude new data available
#define TDR 0x02 //Temperature new Data Available.


/**********************1.Kontrol Errregistroa****************************/
#define ALT 0x80  // Altimetro modua aktibatu
#define RAW 0x40  //
#define OS2 0x20 //oversampeling
#define OS1 0x10
#define OS0 0x08
#define RST 0x04
#define OST 0x02 //Sentsorea aktibatzean zuzenean irakurketa egin dezan ( ez da beuarrezkoa)
#define SBYB 0x01 //Sentsorea aktibatu

// Oversample ratio (sample arteko milisegundu minimoak)
#define OS_1 0x00 // 6 ms
#define OS_2 OS0 // 10 ms
#define OS_4 OS1 // 18 ms
#define OS_8 OS1 | OS0 // 34 ms
#define OS_16 OS2 // 66 ms
#define OS_32 OS2 | OS0 // 130 ms
#define OS_64 OS2 | OS1 // 258 ms
#define OS_128 OS2 | OS1 | OS0 // 512 ms

#define BAR_MASK 0x80
#define ALT_MASK 0xEF
#define ACTIVE_MASK 0xF1
#define STANDBY_MASK 0xFE

/**********************2.Kontrol Erregistroa****************************/
/*
#define ALARM_SEL 0x20
#define LOAD_OUTPUT 0x10
#define ST3 0x08
#define ST2 0x04
#define ST1 0x02
#define ST0 0x01
#define CLEAR_ST 0xF0*/
/**********************3.Kontrol Erregistroa****************************/
#define IPOL1 0x10 //INT1 flag-a  piztean 1-era jartzea eta ez 0-ra
#define PP_OD1 0x08
#define IPOL2 0x02 ////INT2 flag-a  piztean 1-era jartzea eta ez 0-ra
#define PP_OD2 0x01

/**********************4.Kontrol erregistroa****************************/
#define INT_EN_DRDY 0x80 //Da
#define INT_EN_FIFO 0x40 //FIFO interrupt enabled
#define INT_EN_PW 0x20 //Pressure window interrupt enabled
#define INT_EN_TW 0x10 //Temperature window interrupt enabled
#define INT_EN_PTH 0x08 //Pressure Threshold interrupt enabled
#define INT_EN_TTH 0x04 //Temperature Threshold interrupt enabled
#define INT_EN_PCHG 0x02 //Presioa aldatzea ahalbidetzea flag-a altsatzea
#define INT_EN_TCHG 0x01 //Tenperatura aldatzea ahalbidetzea flag-a altsatzea
#define INT_EN_CLEAR 0x00

/**********************5.Kontrol erregistroa****************************/

#define INT_CFG_DRDY 0x80 //Edozein int1 interrupt seinalean finkatzeko
#define INT_CFG_FIFO 0x40
#define INT_CFG_PW 0x20
#define INT_CFG_TW 0x10
#define INT_CFG_PTH 0x08
#define INT_CFG_TTH 0x04
#define INT_CFG_PCHG 0x02
#define INT_CFG_TCHG 0x01

#define INT_CFG_CLEAR 0x00

#define INT2 0
#define INT1 1
/**************************************************************************/

#define DISABLED 0x00
#define CIRCULAR 0x40
#define FULL_STOP 0x80
#define F_MODE DISABLED
/******************PT_DATA_CFG - Sensor data event flag register (0x13h)***********/
#define DREM 0x04 // Gertaeren detekzioa aktibatu
#define PDEFE 0x02 // Presio gertaera flag-a aktibatu
#define TDEFE 0x01 // Temperature gertaera lag-a aktibatu.

/*************************** INT_SOURCE Register*****************************/

#define SRC_DRDY 0x80
#define SRC_FIFO 0x40
#define SRC_PW 0x20
#define SRC_TW 0x10
#define SRC_PTH 0x08
#define SRC_TTH 0x04
#define SRC_PCHG 0x02
#define SRC_TCHG 0x01

/*******************************************************************************/
typedef enum { PSI,
INHG,
MMHG} unitsType;

typedef enum { SR1 = OS_1,
SR2 = OS_2,
SR3 = OS_4,
SR4 = OS_8,
SR5 = OS_16,
SR6 = OS_32,
SR7 = OS_64,
SR8 = OS_128} OverSample_t;

void MPL3115A2_Hasieratu_Polling(int fitx); //Sentsorea Hasoeratu Polling moduan (Irakurketak eginez programatik eta ez flag-a piztean.)
void MPL3115A2_Hasieratu_Interrupt(int fitx);
unsigned char MPL3115A2_Id(int fitx); //Sentsorearen Id-a bueltatzen du.
unsigned char MPL3115A2_GetMode(int fitx); //Sentsorea StandBy ala active moduan dagoen esaten du.
void MPL3115A2_StandbyMode(int fitx); //StandbyMode
int MPL3115A2_Datu_berriaztuak(int fitx); // Bueltatzen du ea datu berriak dauden azkeneko irakurketatik
void MPL3115A2_StandbyMode(int fitx);  //StandBy moduan jartzen du (ctrl1 erregistroa aldatzeko modua)
void MPL3115A2_ActiveMode(int fitx); // Irakurketak Hasi.
void MPL3115A2_AltimeterMode(int fitx); // Altimetro modua aktibatzen du.
float MPL3115A2_ReadAltitude(int fitx); // Altitudea bueltatzen du(metroak ur mail gainetik)
void MPL3115A2_BarometerMode(int fitx); //Barometro modua aktibatzen du.
float MPL3115A2_GetMinimumPressure(int fitx);
float MPL3115A2_GetMaximumPressure(int fitx);
float MPL3115A2_ReadPressure(int fitx); //Presioaren balio bueltatzen du Paskaletan
float MPL3115A2_ReadTemperature(int fitx); // Tenperatura bueltatzen du Celsiuetan
float MPL3115A2_GetMinimumTemperature(int fitx);
float MPL3115A2_GetMaximumTemperature(int fitx);
void MPL3115A2_Interrupzioak_konfiguratu(int fitx);
void MPL3115A2_Baimendu_Flagak(int fitx); // Beharrezko flag-ak pizten ditu
int MPL3115A2_konprobatu_flaga(int fitx);
void MPL3115A2_ClearInterrupts(int fitx);
void MPL3115A2_ConfigureInterruptPin(int fitx,unsigned char intrrpt,unsigned char pin);
void MPL3115A2_ConfigurePressureInterrupt(int fitx);
void MPL3115A2_ConfigureAltitudeInterrupt(int fitx);
unsigned char MPL3115A2_ReadByte(int fitx,char reg);
void MPL3115A2_ReadByteArray(int fitx,char reg,char *buffer, unsigned int length);
void MPL3115A2_WriteByte(int fitx,char reg, char value);
void MPL3115A2_WriteByteArray(int fitx,char reg, char* buffer, unsigned int length);

//Momentuz erabilera eukiko ez dituzten funtzioak

//void MPL3115A2_SetAltimeterOffset(int fitx,unsigned char H_Offset);
//void MPL3115A2_SetPressureOffset(int fitx,unsigned char P_Offset);
//unsigned int MPL3115A2_ReadBarometicPressureInput(int fitx);
//void MPL3115A2_SetPressureAlarmThreshold(int fitx,unsigned int thresh);
//void MPL3115A2_SetPressureTargetWindow(int fitx,unsigned int target,unsigned int window);
//void MPL3115A2_SetTempTargetWindow(int fitx,unsigned int target,unsigned int window);
//void MPL3115A2_SetTempOffset(int fitx,char T_Offset);
//void MPL3115A2_OutputSamplxeRate(int fitx,unsigned char);
//void MPL3115A2_SetAcquisitionTimeStep(int fitx,unsigned char);
//void MPL3115A2_ToggleOneShot(int fitx);
//


