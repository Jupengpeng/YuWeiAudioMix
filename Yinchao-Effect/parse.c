#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <io.h>
#include "iniparser.h"
int getEchoArgs(dictionary *d,char*echoArgs);
int getReverbArgs(dictionary*d,char*reverbArgs);
int getEqualizerArgs(dictionary*d,char*equalizerArgs);

int main(int argc, char * argv[])
{
	dictionary * ini ;
	char  ini_name[100]={0} ;
	char  *finName;
	char  *foutName;
	char commad[1024]={0};
	char echoArgs[1024]={0};
	char reverbArgs[1024]={0};
	char equlizerArgs[1024]={0};
	int secNum=0;
	enum effectType{normal,profession,singer,magic,ktv};
	enum effectType eType;
	int iType =0;
	const char *tempSecName;
	char *p2;
	char exePath[100];
	int i =0;

	if(argc <4 || argc >4){
		printf("param input is invalid! \n");
		return 1;
	}

	finName = argv[1];
	foutName = argv[2];
	if( (access( foutName, 0 )) != -1 )
	{
		char command[200]={0};
		printf("out file is exists\n");
		sprintf(command,"rm %s",foutName);
		system(command);
	}

	p2 = strstr(argv[0], "effect");
	if(p2==NULL){
		printf("exc program para is wrong\n");
		return 1;	
	}

	memcpy(exePath,argv[0],p2-argv[0]);
	exePath[p2-argv[0]]='\0';


	iType = atoi(argv[3]);
	eType =(enum effectType)iType;
	sprintf(ini_name,"%seffect.ini",exePath);


	//load ini file
	ini = iniparser_load(ini_name);
	secNum =iniparser_getnsec(ini);

	for (i =0;i<secNum;i++)
	{
		tempSecName = iniparser_getsecname(ini,i);
		if (strcmp(tempSecName,"echo")==0)
		{
			getEchoArgs(ini,echoArgs);
		}
		else if (strcmp(tempSecName,"reverb")==0)
		{
			getReverbArgs(ini,reverbArgs);
		}
		else if(strcmp(tempSecName,"equalizer")==0)
		{
			getEqualizerArgs(ini,equlizerArgs);
		}
	}
	switch (eType)
	{
	case normal:
		break;
	case profession:
		sprintf(commad,"%ssox %s %s %s",exePath,finName,foutName,equlizerArgs);
		break;
	case singer:
		sprintf(commad,"%ssox %s %s %s %s",exePath,finName,foutName,echoArgs,equlizerArgs);
		break;
	case magic:
		sprintf(commad,"%ssox %s %s %s %s",exePath,finName,foutName,reverbArgs,equlizerArgs);
		break;
	case ktv:
		sprintf(commad,"%ssox %s %s %s %s %s",exePath,finName,foutName,reverbArgs,echoArgs,equlizerArgs);
		break;
	default:
		printf("type is not support!");
		break;
	}
	printf("command = %s\n",commad);
	system(commad);
	iniparser_freedict(ini);
	return 0 ;
}
int  getEchoArgs(dictionary *ini,char*echoArgs)
{
	int echoNum = iniparser_getint(ini,"echo_num",1);
	strcpy(echoArgs,"echo ");
	if (echoNum >0)
	{
		strcat(echoArgs,iniparser_getstring(ini,"echo:in_gain","0"));
		strcat(echoArgs," ");
		strcat(echoArgs,iniparser_getstring(ini,"echo:out_gain","0"));
		strcat(echoArgs," ");
		strcat(echoArgs,iniparser_getstring(ini,"echo:delay","0"));
		strcat(echoArgs," ");
		strcat(echoArgs,iniparser_getstring(ini,"echo:decay","0"));
	}
	return 1;
}

int getReverbArgs(dictionary*ini,char*reverbArgs)
{
	strcpy(reverbArgs,"reverb ");
	strcat(reverbArgs,iniparser_getstring(ini,"reverb:reverberance","0"));
	strcat(reverbArgs," ");
	strcat(reverbArgs,iniparser_getstring(ini,"reverb:hf_damping","0"));
	strcat(reverbArgs," ");
	strcat(reverbArgs,iniparser_getstring(ini,"reverb:stereo_depth","0"));
	strcat(reverbArgs," ");
	strcat(reverbArgs,iniparser_getstring(ini,"reverb:room_scale","0"));
	strcat(reverbArgs," ");
	strcat(reverbArgs,iniparser_getstring(ini,"reverb:pre_delay_ms","0"));
	strcat(reverbArgs," ");
	strcat(reverbArgs,iniparser_getstring(ini,"reverb:wet_gain_dB","0"));
	return 1;
}

int getEqualizerArgs(dictionary*ini,char*equalizerArgs)
{
	int equalizerNum = iniparser_getint(ini,"equalizer:equalizer_num",1);
	int i =0;
	strcpy(equalizerArgs," ");
	if (equalizerNum >0)
	{
		for (i =1 ;i<=equalizerNum;i++)
		{
			char frc[100]={0};
			char width[100]={0};
			char gain[100]={0};
			strcat(equalizerArgs,"equalizer ");
			sprintf(frc,"equalizer:frequency:%d",i);
			strcat(equalizerArgs,iniparser_getstring(ini,frc,"0"));
			strcat(equalizerArgs," ");
			sprintf(width,"equalizer:width:%d",i);
			strcat(equalizerArgs,iniparser_getstring(ini,width,"0"));
			strcat(equalizerArgs," ");
			sprintf(gain,"equalizer:gain:%d",i);
			strcat(equalizerArgs,iniparser_getstring(ini,gain,"0"));
			strcat(equalizerArgs," ");
		}
	}
	return 1;
}