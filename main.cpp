

#include "iostream"
#include "iomanip"      //format
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
    
    char buff[64];

    CSingle *pSingle;
    CSingle *pHit;

    while(!feof(fp))
    {
        res = Getline(fp,buff);
        pSingle = new CSingle();
        pHit = new CSingle();
        char tmp[64];
        res = PopAParameter(pSingle->syllable.name,buff);
        pSingle->SetAbsPitch(pSingle->syllable.name);
        res = PopAParameter(tmp,buff);
        pSingle->channel = atoi(tmp);
        res = PopAParameter(tmp,buff);
        pSingle->syllable.interval = atoi(tmp);
        res = PopAParameter(tmp,buff);
        pSingle->delayTime = atoi(tmp);
        pMidi->singleList.push_back(pSingle);
        
        //打击音轨
        pHit->syllable.name = pSingle->syllable.name;
        pHit->SetAbsPitch(pHit->row-1,pHit->col);       //降8度音
        pHit->channel = 0x09;
        pHit->syllable.interval = pSingle->syllable.interval;
        pHit->delayTime = pSingle->delayTime;
        pHit->strength_hit = 0x5a;
        pHit->strength_release = 0x00;
        pMidi->accompanyList.push_back(pSingle);
    }

    int cnt = 0;

    for (std::list<CSingle *>::iterator it = pMidi->singleList.begin(); it != pMidi->singleList.end(); ++it) 
    {
        (*it)->Conversion2Midi();
        cnt += (*it)->count;
    }
    
//   cout<<"All bytes is "<<cnt<<endl;
    //准备音轨数据
    AudioTrackHeader_t audioTrackHeader;
    audioTrackHeader.len = cnt + 3 + 4;     //切换乐器（3） + 音轨结束事件（4）
    pMidi->Write(audioTrackHeader);
    //切换乐器
    MidiEvent_t midiEvent;
    midiEvent.dt = 0;
    midiEvent.code = 0xC0|0x01;
    midiEvent.data1 = 0x73;
    midiEvent.data2 = 0x00;
    pMidi->Write(midiEvent);
    //计算整个音轨数据长度，重写AudioTrackHeader_t数据
    pMidi->WriteSingle();
    //写音轨结束事件
    pMidi->WriteAudioTrackEnd();

    cnt = 0;
    for (list<CSingle *>::iterator it = pMidi->accompanyList.begin(); it != pMidi->accompanyList.end(); ++it) 
    {
        (*it)->Conversion2Midi();
        cnt += (*it)->count;
    }
    
//    cout<<"All bytes is "<<cnt<<endl;
    //准备音轨数据
//    AudioTrackHeader_t audioTrackHeader;
    audioTrackHeader.len = cnt + 3 + 4;     //切换乐器（3） + 音轨结束事件（4）
    pMidi->Write(audioTrackHeader);
    //切换乐器
//    MidiEvent_t midiEvent;
    midiEvent.dt = 0;
    midiEvent.code = 0xC9;
    midiEvent.data1 = 0x21;
    pMidi->Write(midiEvent);
    //计算整个音轨数据长度，重写AudioTrackHeader_t数据
    pMidi->WriteSingle();
    //写音轨结束事件
    pMidi->WriteAudioTrackEnd();

    pMidi->close();    

    fclose(fp);

    cout<<"Midi file create successful."<<endl;

    return 0;
}


