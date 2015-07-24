#ifndef _OSS_H_
#define _OSS_H_

extern int audiofd;
extern void (*m1sdr_Callback)(unsigned long dwUser, signed short *smp);

// function protos

INT16 m1sdr_Init(int sample_rate);
void m1sdr_Exit(void);
void m1sdr_PlayStart(void);
void m1sdr_PlayStop(void);
void m1sdr_TimeCheck(void);
void m1sdr_SetSamplesPerTick(UINT32 spf);
void m1sdr_SetCallback(void *fn);
INT32 m1sdr_HwPresent(void);
void m1sdr_FlushAudio(void);
void m1sdr_Pause(int);
void m1sdr_SetNoWait(int nw);
#endif
