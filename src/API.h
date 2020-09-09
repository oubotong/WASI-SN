#include <stdbool.h>

typedef struct onboard_LED LED;
struct onboard_LED{
	int ID;
	bool status;
	struct device *dev;
	char *p;
	void (*turn)(void *myled, int action);
	void (*blink)(void *myled);
};

void read(char *device,char *env);
void init_led(void *myled);
void turn(void *myled, int action);
void blink(void *myled);
void led();
void unpack(char * buffer,int size);
