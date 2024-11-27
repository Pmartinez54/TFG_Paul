#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>



void  main(){

int file;
int adapter_nr = 1; /* probably dynamically determined */
char filename[20];
char buf[10];


snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
file = open(filename, I2C_RDWR);
if (file < 0) {
  /* ERROR HANDLING; you can check errno to see what went wrong */
printf("xd%i",file);
  exit(1);
}
// print

int addr = 0x60; /* The I2C address */


if (ioctl(file, I2C_SLAVE, addr) < 0) {
  /* ERROR HANDLING; you can check errno to see what went wrong */
  printf("%i", file);
  exit(1);
}
printf("webo");


__u8 reg = 0x00; /* Device register to access */
__s32 res;


if (read(file, buf, 1) != 1) {
  /* ERROR HANDLING: i2c transaction failed */
  printf("xd");
  exit(1);
} else {
  /* buf[0] contains the read byte */
 printf(" xd %s",buf);
}

}



