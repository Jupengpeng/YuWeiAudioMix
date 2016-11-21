/************************************************************************/
/*				Audio Mix complement                                                                     */
/************************************************************************/
#include "include/mixAudio.h"

int findVolume(char*buff,char*volume);
#ifdef USE_WITHHEAD_SET
int main(int argc, char** argv){
	if(argc <3 || argc >3){
		printf("param input is invalid! \n");
		return 1;
	}
	const char *input1 = argv[1];
	const char *output = argv[2];

	char *p2 = strstr(argv[0], "mixAudioNoHeadSet");
	if(p2==NULL){
		printf("exc program para is wrong\n");
		return 1;	
	}
	char ffmpegPath[100];
	memcpy(ffmpegPath,argv[0],p2-argv[0]);
	ffmpegPath[p2-argv[0]]='\0';
	printf("ffmpeg path =%s \n",ffmpegPath);
	mixAudioWithOutHeadSet(input1,output,ffmpegPath);

	return 0;
}
#endif



int main(int argc, char** argv){
	if(argc <4 || argc >4){
		printf("param input is invalid! \n");
		return 1;
	}
	const char *input1 = argv[1];
	const char *input2 = argv[2];
	const char *output = argv[3];

	char *p2 = strstr(argv[0], "mixAudio");
	if(p2==NULL){
		printf("exc program para is wrong\n");
		return 1;	
	}
	char ffmpegPath[100];
	memcpy(ffmpegPath,argv[0],p2-argv[0]);
	ffmpegPath[p2-argv[0]]='\0';
	printf("ffmpeg path =%s \n",ffmpegPath);
	mixAudio(input1,input2,output,ffmpegPath);

	return 0;
}

//************************************
// Method:    getVolume   get the volume of file
// FullName:  getVolume
// Access:    public 
// Returns:   double    volume value
// Qualifier:
// Parameter: const char * input   input file path
//************************************
double getVolume(const char*input,char*ffmpegPath){
	FILE *fstream;
	double max_volume = 0.0;
	char command[1024] = { 0 };
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	sprintf(command,
		"%sffmpeg -i %s -af volumedetect -f null /dev/NUL 2>&1 | grep %s",
		ffmpegPath,input, VOLUME);

	if (NULL == (fstream = popen(command, "r"))) {
		fprintf(stderr, "execute command failed: %s", strerror(errno));
		return -1.0;
	}
	if (NULL != fgets(buff, sizeof(buff), fstream)) {
		printf("%s", buff);
		char volume[32] = {0};
		int result = findVolume(buff,volume);
		if (result == 0) {
			max_volume = atof(volume);
		}else{
			max_volume = -1.0;
		}

	} else {
		pclose(fstream);
		return -1.0;
	}
	pclose(fstream);

	return max_volume;
}


//************************************
// Method:    findVolume  find the volume value from system command
// FullName:  findVolume
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: char * buff  system command line buf
// Parameter: char * volume  the volume value find
//************************************
int findVolume(char*buff,char*volume) {
	char *p1 = NULL, *p2 = NULL;
	if (NULL == buff) {
		return -1;
	}
	do {
		p1 = strstr(buff, VOLUME);
		if (p1 != NULL) {
			int nLen = strlen(VOLUME);
			p2 = strstr(p1, " dB");
			if (NULL != p2) {
				memcpy(volume, p1 + nLen, p2 - p1 - nLen);
				volume[p2 - p1 - nLen] = '\0';
				printf("\n szVolume = %s\n", volume);
			}else{
				return -1;
			}
		}else{
			return -1;
		}

	} while (false);

	return 0;
}


//************************************
// Method:    setVolume  set the file of input file
// FullName:  setVolume
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const char * input  input file 
// Parameter: double volume  the volume value want to set
//************************************
void setVolume(const char *input,double volume,char *temp,char*ffmpegPath){
	char cVolume[32];
	char command[1024] = {0};
	//string result = "temp.mp3";
	ifstream fin(temp);
	if (fin){
		char command[200]={0};
		sprintf(command,"rm %s",temp);
		system(command);
	}
	sprintf(cVolume,"%f",volume);
	sprintf(command,"%sffmpeg -y -i %s -af \"volume=%s\" %s",ffmpegPath,input,cVolume,temp);
	printf("command = %s\n",command);
	system(command);
}



//************************************
// Method:    mixAudioWithOutHeadSet  msin interface
// FullName:  mixAudioWithOutHeadSet
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const char * src1
// Parameter: const char * dst
//************************************
int mixAudioWithOutHeadSet(const char* src1,const char*dst,char*ffmpegPath){

	if (src1 ==NULL || dst ==NULL)
	{
		printf("input or output is not find!");
		return 1;
	}
	ifstream fin(dst);
	if (fin){
		char command[200]={0};
		sprintf(command,"rm %s",dst);
		system(command);
	}

	double volume = getVolume(src1,ffmpegPath);
	if (volume ==-1.0)
	{
		printf("getVolume is failed");
		return 1;
	}
	setVolume(src1,(-13.0-volume),(char*)dst,ffmpegPath);
	return 0;
}




//************************************
// Method:    mixAudio  msin interface
// FullName:  mixAudio
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const char * src1
// Parameter: const char * src2
// Parameter: const char * dst
//************************************
int mixAudio(const char* src1,const char *src2,const char*dst,char*ffmpegPath){

	if (src1 ==NULL ||src2 ==NULL || dst ==NULL)
	{
		printf("input or output is not find!");
		return 1;
	}
	ifstream fin(dst);
	if (fin){
		char command[200]={0};
		sprintf(command,"rm -rf %s",dst);
		system(command);
	}

	double volume = getVolume(src1,ffmpegPath);
	if (volume ==-1.0)
	{
		printf("getVolume is failed");
		return 1;
	}
	char tempFile1[100];
	char tempFile[100];

	char *p2 = strstr((char*)dst,".mp3");
	if(p2!=NULL){	
		memcpy(tempFile1,(char*)dst,p2-(char*)dst);
		tempFile1[p2-(char*)dst] = '\0';
		printf("p2=%s tempfile =%s \n",p2,tempFile1);
		sprintf(tempFile,"%stemp.mp3",tempFile1);
	}else {
		sprintf(tempFile,"%s","temp.mp3");	
	}
	setVolume(src1,(-16.0-volume),tempFile,ffmpegPath);

	char command[1024]={0};
	sprintf(command,"%sffmpeg  -i %s -i %s -filter_complex amix=inputs=2:duration=first:dropout_transition=3  -c:a libmp3lame -q:a 1  %s",ffmpegPath,src1,src2,dst);
	system(command);

	char comRm[100];
	sprintf(comRm,"rm %s",tempFile);
	system(comRm);
	return 0;
}
