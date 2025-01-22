#include <stdint.h>

struct balioak_barometro
{
    float temperatura;
    float presioa;
};

void Irakurketa_Barometrikoa (int fitx,struct balioak_barometro * emaiztak);
void Sentsorea_Hasieratu_Periodikoki(int fitx);
int Senstorea_Aatzitu_irekita(int fitx,unsigned char helbidea);
int Senstorea_Aatzitu(unsigned char helbidea);



