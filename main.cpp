

#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "componest/common/include/typedef.h"
#include "componest/single/include/single.h"
#include "componest/midi/include/midi.h"


 using namespace std;

// #include "alsa/asoundlib.h"


int Getline(FILE *fp,char *buff)
{    
    char ch;

    while(!feof(fp))
    {
        ch = fgetc(fp);
        *buff++ = ch;
        if(ch == '\n')
            break;
    }   

    return 0;
}

int JropAParameter(char *src)
{
    char tmp[64] = {0};
    char *ptmp;
    char *psrc;

    ptmp = tmp;
    psrc = src;    
    
    while(*src != ' ')
        src++;

    src++;
    while(*src != '\n')
        *ptmp++ = *src++;

    *ptmp = '\n';

    ptmp = tmp;
    while(*ptmp != '\0')
        *psrc++ = *ptmp++;

    *psrc = '\0';

    return 0;
}

int PopAParameter(char *des,char *src)
{
    char ch;
    char *psrc;

    psrc = src;
    while(*src != ' ')
        *des++ = *src++;

    *des = '\0';

    JropAParameter(psrc);

    return 0;
}

int main (int argc,char **argv)
{
    //syllable  channel keeptime delaytime

    FILE *fp;

    int res;
    char *pInputFileName = (char *)"./test/aa.txt";
    char *pOutputFileName = (char *)"./output/aa.mid";


    fp = fopen(pInputFileName,"r");
    if(fp == NULL)
        cout<<pInputFileName<<"is not available"<<endl;

//    err = CreatMidiFileHeader(pOutputFileName);
    CMidi *pMidi = new CMidi();
    pMidi->fname = pOutputFileName;
    
    if(pMidi == NULL)
        cout<<"Create output midi file fault"<<endl;

    pMidi->open();
    FileHeaderBlock_t fileHeaderBlock;
    fileHeaderBlock.data.ffff = 0x0001;
    fileHeaderBlock.data.nnnn = 0x0002;
    fileHeaderBlock.data.dddd = 0x01E0;
    pMidi->Write(fileHeaderBlock);

    //准备音轨数据
    AudioTrackHeader_t audioTrackHeader;
    pMidi->Write(audioTrackHeader);
    

    char buff[64];

    CSingle *pSingle;

    while(!feof(fp))
    {
        res = Getline(fp,buff);
        pSingle = new CSingle();
        res = PopAParameter(pSingle->syllable.name,buff);
        char tmp[64];
        res = PopAParameter(tmp,buff);
        pSingle->channel = atoi(tmp);
        res = PopAParameter(tmp,buff);
        pSingle->syllable.interval = atoi(tmp);
        res = PopAParameter(tmp,buff);
        pSingle->delayTime = atoi(tmp);

        
        pMidi->singleList.push_back(pSingle);
    }

    int cnt = 0;
     for (std::list<CSingle *>::iterator it = pMidi->singleList.begin(); it != pMidi->singleList.end(); ++it) 
     {
         (*it)->Conversion2Midi();

         cnt += (*it)->count;
     }

    cout<<"all bytes is "<<cnt<<endl;

    // int cnt = 0;
    // while(!feof(fp))
    // {
    //     res = Getline(fp,buff);

    //     res = PopAParameter(pSyl->syllable.name,buff);
    //     char tmp[64];
    //     res = PopAParameter(tmp,buff);
    //     pSyl->channel = atoi(tmp);
    //     res = PopAParameter(tmp,buff);
    //     pSyl->syllable.interval = atoi(tmp);
    //     res = PopAParameter(tmp,buff);
    //     pSyl->delayTime = atoi(tmp);

    //     cnt += pSyl->Write(pOutputFileName);
    // }
    // //写音轨结束事件
    
//    pMidi->WriteAudioTrackEnd();
    //计算整个音轨数据长度，重写AudioTrackHeader_t数据
    
//    pMidi->Write(audioTrackHeader);

    fclose(fp);

    return 0;
}


