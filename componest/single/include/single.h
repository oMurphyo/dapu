

#ifndef _SINGLE_H_
#define _SINGLE_H_

#include "../../common/include/typedef.h"
#include "stdio.h"
#include "iostream"

using namespace std;

#define SYLLABLE_NAME_SIZE  64

//音节
struct Syllable_t
{
    char *name;
    int tone;               //音调，基准音(0 1 2 3 4 5 6 7)[C D E F G A B]
    int tone_quality;       //音色，也叫音品，不同乐器的发声区别
    int loudness;           //响度，强弱敏感度
                            //0-20      dB  宁静音
                            //30-40     dB  微弱音
                            //50-70     dB  正常声
                            //80-100    dB  响音声
                            //110-130   dB  极响声
                            //140~      dB  痛阈
    int interval;           //音程

};
typedef struct Syllable_t Syllable_t;

struct Position_t
{
    int x;
    int y;    
};
typedef struct Position_t Position_t;

struct Single_t
{
    unsigned char instrument;       //演奏乐器
    unsigned char channel;          //通道
    Syllable_t syllable;            //音节，包含：音符名称，时值，音量
    Syllable_t *pSyl;
    unsigned int delayTime;         //延迟时间
    unsigned int specialFlag;       //特殊标记
    unsigned int voice;             //音量

    unsigned char strength_hit = 100;
    unsigned char strength_release = 64;


    char * image;                   //图片   
    Position_t position;            //位置
};
typedef struct Single_t Single_t;

//单音符
class CSingle
{
private:
    
public:
    CSingle();
//    CSingle(Syllable_t *pSyllable,unsigned char syl_channel,unsigned int syl_delaytime,unsigned char syl_strength_hit = 100,unsigned char syl_strength_release = 64);
    ~CSingle();

//    int Write(char *filename);
//    int Play();

    int Conversion2Midi();

public:
    unsigned char instrument;       //演奏乐器
    unsigned char channel;          //通道
    Syllable_t syllable;            //音节，包含：音符名称，时值，音量
    Syllable_t *pSyl;
    unsigned int delayTime;         //延迟时间
    unsigned int specialFlag;       //特殊标记
    unsigned int voice;             //音量

    unsigned char strength_hit = 100;   //力度
    unsigned char strength_release = 64;

    char * image;                   //图片   
    Position_t position;            //位置

    unsigned char buff_midi[10];    //转换成midi后的代码存放位置

    int index;                      //音符序号
    int count;                      //midi指令字节个数
private:
    int DynamicByteConversion(int dt);        //动态字节转换

};




#endif

