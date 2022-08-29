#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * decoding the message present in the image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    //stego image details
    char *stego_file_name;
    FILE *fptr_stego_image;
    char stego_magic_data[MAX_IMAGE_BUF_SIZE];
    char decode_byte[MAX_SECRET_BUF_SIZE];
    char decode__mag_string[MAX_IMAGE_BUF_SIZE];
    char stego_magic_name[MAX_IMAGE_BUF_SIZE];

    //decode.txt details
    char *decode_file_name;
    FILE *fptr_decode_file;
    int decode_extention_size;
    int decode_file_size;
    char decode_extention_name[MAX_IMAGE_BUF_SIZE];

} DecodeInfo;

// Decode functions prototypes
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

// Do decoding function
Status do_decoding(DecodeInfo *decInfo);

// Open decoding files function
Status open_files_decoding(DecodeInfo *decInfo);

// Decoding magic string
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

// Decoding string from lsb of byte
Status decode_byte_from_lsb(char *magic_data, DecodeInfo *decInfo, int byte_pos);

// Decoding secret file extension size
Status decode_secret_ext_size(DecodeInfo *decInfo);

// Decoding size to lsb
int decode_size_to_lsb(char *str, DecodeInfo *decInfo);

// Decoding secret file extension name
Status decode_secret_extention_name(DecodeInfo *decInfo);

// Open decode file
Status open_output_file(DecodeInfo *decInfo);

//Fetching secret file size
Status decode_file_size(DecodeInfo *decInfo);

//Decoding secret data
Status decode_secret_data(DecodeInfo *decInfo);

#endif
