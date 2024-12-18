#include <stdint.h>

#define INTERRUPT_MODUA 1    //Irakurketa interrupzion bidez irakurketak egin nahi bada atzitu funtzioan sartu beharreko aldagaia
#define PERIODIKOKI_MODUA 0  //Periodikoki irakurketak egin nahi badira atzitu funtzioan sartu beharreko aldagaia

struct balioak_barometro
{
    float temperatura;
    float presioa;
};

void Irakurketa_Barometrikoa (int fitx,struct balioak_barometro * emaiztak);
void Sentsorea_Hasieratu_Periodikoki(int fitx);
int Senstorea_Aatzituu(int fitx,unsigned char helbidea);
int Senstorea_Aatzitu(unsigned char helbidea);
//int Konprobatu_flaga(int fitx);



