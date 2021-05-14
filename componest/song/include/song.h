

#ifndef _SONG_H_
#define _SONG_H_

#include "../../common/include/typedef.h"
#include "../../single/include/single.h"

#include "string"
#include "list"

using namespace std;

//曲子
class CSong
{
private:
    
public:
    CSong();
    ~CSong();

    int play();

    //曲子名称
    string name;        

    //描述
    string description;

    //节拍
    string meter;

    //调号
    int tone;

    //小节线

    //连音符

    //特殊标记

    //渐强渐弱符
    
    //显示位置

    CSingle begin;      //起始音符
    CSingle end;        //结束音符

    list<CSingle> single;

    int count;          //音符个数


};






#endif

