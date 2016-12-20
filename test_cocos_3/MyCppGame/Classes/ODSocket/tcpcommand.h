#ifndef __TCP_COMMAND_H__
#define __TCP_COMMAND_H__

#define MAX_NET_DATA_LEN (10 * 1024)
#define NET_HEADER_LEN       16

#define ERROR_CODE_SUCCESS                      0x00
#define ERROR_CODE_ERROR                        0xff

#define reversebytes_uint32t(value) ((value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24)//int 大小端转换


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
}__attribute__ ((packed, aligned(4)));
typedef struct _send_package SEND_PACKAGE;


#endif /* #define __TCP_COMMAND_H__ */
