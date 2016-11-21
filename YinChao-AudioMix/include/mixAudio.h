/************************************************************************/
/*                Audio Mix Interface                                                    */
/************************************************************************/
#ifndef __MIX_AUDIO__
#define __MIX_AUDIO__
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
using namespace std;

/************************************************************************/
/*                       Macro Define                                               */
/************************************************************************/
#define VOLUME   "mean_volume: "  //"max_volume: "

//************************************
// Method:    mixAudio
// FullName:  mixAudio
// Access:    public 
// Returns:   int 0 success:1 fail
// Qualifier:
// Parameter: const char * src1  record file path
// Parameter: const char * src2  music file path
// Parameter: const char * dst   mixed file path
// Parameter: const char * ffmpegpath  ffmpeg path
// Parameter: double volume
//************************************
int mixAudio(const char* src1,const char *src2,const char*dst,char*ffmpegPath);

//************************************
// Method:    mixAudioWithOutHeadSet
// FullName:  mixAudioWithOutHeadSet
// Access:    public 
// Returns:   int 0 success:1 fail
// Qualifier:
// Parameter: const char * src1  record file path
// Parameter: const char * dst   mixed file path
// Parameter: const char * ffmpegpath  ffmpeg path
//************************************
int mixAudioWithOutHeadSet(const char* src1,const char*dst,char*ffmpegPath);


#endif
