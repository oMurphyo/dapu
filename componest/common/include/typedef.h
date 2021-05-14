

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

    typedef unsigned char u8;
    typedef signed char s8;
    typedef unsigned short int u16;
    typedef signed short int s16;
    typedef unsigned long int u32;
    typedef signed long int s32;
    typedef float f32;
    typedef double d32;
    typedef long long l64;


    typedef union w2b_t
    {
        u16 w;
        struct{
            u8 lsb;
            u8 hsb;
        };
    }w2b_t;    


    //故障码
    typedef enum
    {
        Err_OK = 0,
        Err_FileOpenFailed = 1,         //文件打开失败
        Err_FileIsNotExist = 2,         //文件不存在
        Err_TypeFault = 3,              //类型错误

        //写文件
        Err_WriteFault,                 //文件写入错误

    }Err_t;


#endif

