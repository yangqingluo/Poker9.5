#ifndef __TCP_COMMAND_H__
#define __TCP_COMMAND_H__

#define NET_HEADER_LEN       16

#define ERROR_CODE_SUCCESS                      0x00
#define ERROR_CODE_ERROR                        0xff

#pragma pack(1)

struct _errorcode_string
{
    unsigned char error_code;
    unsigned char string[32];
};
typedef struct _errorcode_string ERRORCODE_STRING;


struct _send_package
{
    int valueLength;
    unsigned char value[200];
};
typedef struct _send_package SEND_PACKAGE;


#endif /* #define __TCP_COMMAND_H__ */
