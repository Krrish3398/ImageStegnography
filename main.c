/*
Name : Sai Krishna Chaitanya Vadduri
Date : 31 May 2022
Description : The objective of this project is to send a secret text file encoded inside an image of bmp file format. 
Encoded the length (file-size) of the secret text and then encoded the data into the LSB of the image bytes. 
The decoding process involves decoding the length and then decoding the text bit by bit. 
The final output is the secret text after decoding 

Sample Execution
./a.out -e beautiful.bmp secret.txt
*****************Selected Encoding*****************
Read and validate arguments is successful
Opened all the files successfully
.............Started Encoding.............
width = 1024
height = 768
Possible to hide the data in image
Successful :: copied header......
Successful :: Encoded the magic string 
Successful :: Encoded secret file extention size
Successful :: Encoded extension
Successful :: Encoded secret file size
Successful :: Encoded the secret data
Successful :: Copied remaining bytes
Hurrayy!!!!  Encoding is completed

./a.out -d stego.bmp decode.txt
*****************Selected Decoding*****************

Info :: Read and validate is successful
INFO :: Opened stego_image.bmp
Opened required files successfully
.............Started decoding.............
Successful :: Decoded Magic string signature
Successful :: Decoded secret file extension size
Successful :: Decoded secret file extension 
INFO :: Ouput file is not mentioned!! -- Creating decode.txt as default!!
Successful :: Opened output file 
Successful :: Decoded secret file size 
Successful ::  Decoded secret message
Successful :: Hurrayy!!! Decoding Completed :)
You can check text content by using the command --> cat decode.txt
*/
#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"


int main(int argc, char *argv[])
{
    // VALIDATING THE OPTION - -e / -d
    // check_operation_type
    if (check_operation_type(argv) == e_encode)
    {
        EncodeInfo encInfo;
        printf("*****************Selected Encoding*****************\n\n");
        // read and validate the encding arguments/files
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Read and validate arguments is successful\n");
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Hurrayy!!!!  Encoding is completed\n");
            }
            else
            {
                printf("Failure :: Encoding is not done \n");
            }
        }
        else
        {
            printf("Faiure :: Read and validate arguments\n");
        }
    }
    /*decode operation*/
    else if (check_operation_type(argv) == e_decode)
    {
        printf("*****************Selected Decoding*****************\n\n");
        DecodeInfo decInfo;
        if (read_and_validate_decode_args(argv, &decInfo) == d_success)
        {
            printf("INFO :: Read and validate is successful\n");
            if (do_decoding(&decInfo) == d_success)
            {
                printf("Successful :: Hurrayy!!! Decoding Completed :)\n");
                printf("INFO :: You can check decoded text content by using the command --> cat decode.txt\n");
            }
            else
            {
                printf("Failure :: Decoding is not done\n");
            }
        }
        else
        {
            printf("Read and validate is not successful\n");
        }
    }
    /*error for invalid operation in command line arguments*/
    else
    {
        printf("Invalid option\n******************Usage******************\n");
        printf("Encoding : ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
        printf("Key : Decoding ./a.out -d stego.bmp\n");
    }
    fcloseall();
    return 0;
}
/*encoding or decoding check*/
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}