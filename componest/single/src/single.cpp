
#include "../include/single.h"



// gamut 音阶表
#define TABLE_GAMUT_ROW_MAX 11
#define TABLE_GAMUT_COL_MAX 12
static int table_gamut[TABLE_GAMUT_ROW_MAX][TABLE_GAMUT_COL_MAX]=
{
    /* C    C#   D   D#    E   F    F#   G    G#   A    A#   B */
    {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b},     // 0
    {0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17},     // 1
    {0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23},     // 2
    {0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f},     // 3
    {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b},     // 4
    {0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47},     // 5
    {0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53},     // 6
    {0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f},     // 7
    {0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b},     // 8
    {0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77},     // 9
    {0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83}      // 10
};


int CSingle::DynamicByteConversion(int dt)
{
    int tmp = 0;
    int res = 0;
    int n = 0;
    int flagMax = 1;

    tmp = dt;

    while(dt > 0)
    {
        tmp = dt & 0x7F;
        res += (tmp << (8*n));
        //标记最高位    
        if(n>0)
            flagMax = 1<<(8*(n+1)-1);
        else
            flagMax = 0;
        res = res |flagMax;

        dt = dt>>7;
        n++;
    }

    return res;
}

CSingle::CSingle()
{
    syllable.name = (char *)malloc(SYLLABLE_NAME_SIZE);
}

// CSingle::CSingle(Syllable_t *pSyllable,unsigned char syl_channel,unsigned int syl_delaytime,unsigned char syl_strength_hit,unsigned char syl_strength_release)
// {
//     pSyl = pSyllable;

//     syllable.name = pSyl->name;
//     syllable.tone = pSyl->tone;
//     syllable.tone_quality = pSyl->tone_quality;
//     syllable.loudness = pSyl->loudness;
//     syllable.interval = pSyl->interval;

//     channel = syl_channel;
//     delayTime = syl_delaytime;
//     strength_hit = syl_strength_hit;
//     strength_release = syl_strength_release;
    
// }

CSingle::~CSingle()
{
    free(syllable.name);
}

// int CSingle::Play()
// {

//     return 0;
// }

/*
    格式: 音高      通道(0-15)  保持时间（ticks）   延迟时间（ticks）
    格式: pitch    channel     keeptime(ticks)   delaytime(ticks)
    输入: m1        0          480               240
    输出: 
            延迟时间    命令    音高    力度
            DT cmd pitch strength
            81 70 90 3c 64
            83 60 80 3c 40
    返回： 生成的数据字节数
*/
// int CSingle::Write(char *filename)
// {
//     static int cnt = 0;
//     char *pSyllableName = syllable.name;

//     static int rel_pitch = 0;          //相对音高
//     static int abs_pitch = 0;          //绝对音高
//     static int row = 5;                 //默认从中音（中央C开始 3C）
//     static int col = 0;

//     while((*pSyllableName != '\0')&&(*pSyllableName != '\n'))
//     {
//         switch (*pSyllableName)
//         {
//         case '1':
//             col = 0;
//             break;
//         case '2':
//             col = 2;
//             break;
//         case '3':
//             col = 4;
//             break;
//         case '4':
//             col = 5;
//             break;
//         case '5':
//             col = 7;
//             break;
//         case '6':
//             col = 9;
//             break;
//         case '7':
//             col = 11;
//             break;
//         case 'h':
//         case 'H':
//             if(row<TABLE_GAMUT_ROW_MAX)  row++;
//             break;
//         case 'm':
//         case 'M':
//             //row = row;
//             break;
//         case 'l':
//         case 'L':
//             if(row>0)    row--;
//             break;
//         case 'b':       //半音
//         case 'B':
//         case '#':
//             col++;
//         default:
//             break;
//         }
//         pSyllableName++;
//     }

//     //查表
//     abs_pitch = table_gamut[row][col];

//     //cmd & channel
//     static unsigned char cmd= 0x0;

//     //delaytime
//     static unsigned short int delayTime;
//     delayTime = DynamicByteConversion(delayTime);

//     //Keeptime
//     static unsigned short int keeptime;
//     keeptime = DynamicByteConversion(syllable.interval);

//     //strength
// //    static unsigned char strength_hit = 100;
// //    static unsigned char strength_release = 64;

//     //output file
//     //  O_RDWR | O_CREAT | O_APPEND
//     FILE *fp;
//     fp = fopen(filename,"ab+");
//     if(fp == NULL)
//         cout<<"File is not available"<<endl;
    
//     int res = 0;
//     unsigned char buff[5];

//     w2b_t w2bTmp;
//     //写按下键指令
//     w2bTmp.w = delayTime;
//     if (w2bTmp.hsb != 0)
//     {
//         res = fwrite(&(w2bTmp.hsb),1,1,fp);
//         cnt++;
//     }
//     res = fwrite(&(w2bTmp.lsb),1,1,fp);
//     cmd = 0x90 | channel;
//     res = fwrite(&(cmd),1,1,fp);    
//     res = fwrite(&(abs_pitch),1,1,fp);
//     res = fwrite(&(strength_hit),1,1,fp);
//     //写释放键指令
//     w2bTmp.w = keeptime;
//     if (w2bTmp.hsb != 0)
//     {
//         res = fwrite(&(w2bTmp.hsb),1,1,fp);
//         cnt++;
//     }
//     res = fwrite(&(w2bTmp.lsb),1,1,fp);
//     cmd = 0x80 | channel;
//     res = fwrite(&(cmd),1,1,fp);    
//     res = fwrite(&(abs_pitch),1,1,fp);
//     res = fwrite(&(strength_release),1,1,fp);

//     cnt += 8;

//     fclose(fp);    

//     return cnt;
// }

Err_t CSingle::SetAbsPitch(char *pStr)
{
    while((*pStr != '\0')&&(*pStr != '\n'))
    {
        switch (*pStr)
        {
        case '1':
            col = 0;
            break;
        case '2':
            col = 2;
            break;
        case '3':
            col = 4;
            break;
        case '4':
            col = 5;
            break;
        case '5':
            col = 7;
            break;
        case '6':
            col = 9;
            break;
        case '7':
            col = 11;
            break;
        case 'h':
        case 'H':
            if(row<TABLE_GAMUT_ROW_MAX)  row++;
            break;
        case 'm':
        case 'M':
            //row = row;
            break;
        case 'l':
        case 'L':
            if(row>0)    row--;
            break;
        case 'b':       //半音
        case 'B':
        case '#':
            col++;
        default:
            break;
        }
        pStr++;
    }

    abs_pitch = table_gamut[row][col];

    return Err_OK;
}

Err_t CSingle::SetAbsPitch(int tab_row,int tab_col)
{
    row = tab_row;
    col = tab_col;
    abs_pitch = table_gamut[tab_row][tab_col];

    return Err_OK;
}

int CSingle::Conversion2Midi()
{
    int index = 0;
//    char *pSyllableName = syllable.name;

    static int rel_pitch = 0;          //相对音高
//    static int abs_pitch = 0;          //绝对音高
//    static int row = 5;                 //默认从中音（中央C开始 3C）
//    static int col = 0;

    // while((*pSyllableName != '\0')&&(*pSyllableName != '\n'))
    // {
    //     switch (*pSyllableName)
    //     {
    //     case '1':
    //         col = 0;
    //         break;
    //     case '2':
    //         col = 2;
    //         break;
    //     case '3':
    //         col = 4;
    //         break;
    //     case '4':
    //         col = 5;
    //         break;
    //     case '5':
    //         col = 7;
    //         break;
    //     case '6':
    //         col = 9;
    //         break;
    //     case '7':
    //         col = 11;
    //         break;
    //     case 'h':
    //     case 'H':
    //         if(row<TABLE_GAMUT_ROW_MAX)  row++;
    //         break;
    //     case 'm':
    //     case 'M':
    //         //row = row;
    //         break;
    //     case 'l':
    //     case 'L':
    //         if(row>0)    row--;
    //         break;
    //     case 'b':       //半音
    //     case 'B':
    //     case '#':
    //         col++;
    //     default:
    //         break;
    //     }
    //     pSyllableName++;
    // }

    //查表
//    abs_pitch = table_gamut[row][col];

    //cmd & channel
    static unsigned char cmd= 0x0;

    //delaytime
    static unsigned short int delayTime;
    delayTime = DynamicByteConversion(delayTime);

    //Keeptime
    static unsigned short int keeptime;
    keeptime = DynamicByteConversion(syllable.interval);

    //清空buff
    for(int i=0;i<10;i++)
        buff_midi[i] = 0;

    w2b_t w2bTmp;
    //写按下键指令
    w2bTmp.w = delayTime;
    if (w2bTmp.hsb != 0)
    {
        buff_midi[index] = w2bTmp.hsb;
        index++;
    }
    buff_midi[index++] = w2bTmp.lsb;
    cmd = 0x90 | channel;
    buff_midi[index++] = cmd;
    buff_midi[index++] = abs_pitch;
    buff_midi[index++] = strength_hit;

    //写释放键指令
    w2bTmp.w = keeptime;
    if (w2bTmp.hsb != 0)
    {
        buff_midi[index] = w2bTmp.hsb;
        index++;
    }
    buff_midi[index++] = w2bTmp.lsb;
    cmd = 0x80 | channel;
    buff_midi[index++] = cmd;
    buff_midi[index++] = abs_pitch;
    buff_midi[index++] = strength_release;

    count = index;
    
    return index;
}









