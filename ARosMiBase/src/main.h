#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <linux/joystick.h>
#include <time.h>
#include <pthread.h>


#include "utilities.h"
#include "peripherals.h"

extern struct JoY JoYAct;
extern int gRawKeys[6];

void UpdateIO();
void SystemInit();
void SystemExit();
void DataPrepare();
void ClearScreen();
void DrawObjects();
void PrintDiagnosticInfo();

#define MaxObj 9000
int ActMaxObj;
int dx[MaxObj];
int dy[MaxObj];
int s[MaxObj];
int x[MaxObj];
int y[MaxObj];
int xt[MaxObj];
int yt[MaxObj];


int height=480;
int width=640;

unsigned int GRAPH[640 * 480];


int mode = 0;
int stage = 0;
volatile int flash_ligth=0;
volatile long globalTimer_ms=0;
long startTime_ms;
int frame_count = 0;
pthread_t tID;

int Jxt = 0, Jyt = 0, Jx = 0, Jy = 0, Jz = 0, JRz = 0;


char *fbname;
int FrameBufferFD, inputEventsFD, inputJoyFD;
struct fb_fix_screeninfo fix_info;
struct fb_var_screeninfo var_info;
void *framebuffer;

#endif /* SRC_MAIN_H_ */
