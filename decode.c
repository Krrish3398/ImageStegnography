#include <stdio.h>
#include <string.h>
#include "common.h"
#include "decode.h"
#include "types.h"

// Function to Open image passed
Status open_files_decoding(DecodeInfo *decInfo)
{
	decInfo->fptr_stego_image = fopen(decInfo->stego_file_name, "r");

	if (decInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "Error : Unable to open file %s\n", decInfo->stego_file_name);
		return d_failure;
	}

	return d_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	// get .bmp and compare with cla
	if (argv[2] != NULL)
	{
		if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
		{
			// hold the name into a pointer
			decInfo->stego_file_name = argv[2];
		}
	}
	else
	{
		return d_failure;
	}

	// Check output file details
	if (argv[3] != NULL)
	{
		// hold the name into a pointer
		decInfo->decode_file_name = argv[3];
	}
	else
	{
		// Suppose file name not passed, creating default file name as decode.txt
		printf("INFO :: Ouput file is not mentioned!! -- decode.txt is created!!\n");
		decInfo->decode_file_name = "decode.txt";
	}
	return d_success;
}

// Function to Decode byte data into image
Status decode_byte_from_lsb(char *magic_data, DecodeInfo *decInfo, int byte_pos)
{

	int mask = 0x1;
	*decInfo->decode_byte = 0;
	for (int i = 0; i < 8; i++)
	{
		*decInfo->decode_byte = *decInfo->decode_byte | ((magic_data[i] & mask) << (7 - i));
	}
	decInfo->decode__mag_string[byte_pos] = *decInfo->decode_byte;
	return d_success;
}
// Function to Decode magic string from the image
Status decode_magic_string(char *data, DecodeInfo *decInfo)
{
	fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

	for (int i = 0; i < strlen(data); i++)
	{
		fread(decInfo->stego_magic_data, 8, sizeof(char), decInfo->fptr_stego_image);
		decode_byte_from_lsb(decInfo->stego_magic_data, decInfo, i);
	}

	decInfo->decode__mag_string[strlen(data)] = '\0';

	strcpy(decInfo->stego_magic_name, decInfo->decode__mag_string);

	if (strcmp(decInfo->decode__mag_string, MAGIC_STRING) == 0)
	{
		return d_success;
	}
	return d_failure;
}
// Function to Decode size from the img
int decode_size_to_lsb(char *str, DecodeInfo *decInfo)
{
	int mask = 0x1;
	int size = 0;
	for (int i = 0; i < 32; i++)
	{
		size = size | ((str[i] & mask) << (31 - i));
	}
	return size;
}

// Function to Decode extension size
Status decode_secret_ext_size(DecodeInfo *decInfo)
{

	char str[32];
	fread(str, 32, sizeof(char), decInfo->fptr_stego_image);
	decInfo->decode_extention_size = (decode_size_to_lsb(str, decInfo));
	return d_success;
}

// Function to Decode extension name
Status decode_secret_extention_name(DecodeInfo *decInfo)
{

	for (int i = 0; i < decInfo->decode_extention_size; i++)
	{
		fread(decInfo->stego_magic_data, 8, sizeof(char), decInfo->fptr_stego_image);
		decode_byte_from_lsb(decInfo->stego_magic_data, decInfo, i);
	}
	decInfo->decode__mag_string[decInfo->decode_extention_size] = '\0';

	strcpy(decInfo->decode_extention_name, decInfo->decode__mag_string);

	return d_success;
}
// Function to Open output decode file
Status open_output_file(DecodeInfo *decInfo)
{
	decInfo->fptr_decode_file = fopen(decInfo->decode_file_name, "w");

	if (decInfo->fptr_decode_file == NULL)
	{
		perror("fopen");
		fprintf(stderr, "Error : Unable to open file %s\n", decInfo->decode_file_name);
		return d_failure;
	}
	return d_success;
}
// Function to Fetch file size
Status decode_file_size(DecodeInfo *decInfo)
{
	char string_size[32];
	fread(string_size, 32, sizeof(char), decInfo->fptr_stego_image);
	decInfo->decode_file_size = (decode_size_to_lsb(string_size, decInfo));
	return d_success;
}
// Function to Get secret message
Status decode_secret_data(DecodeInfo *decInfo)
{
	for (int i = 0; i < decInfo->decode_file_size; i++)
	{
		fread(decInfo->stego_magic_data, 8, sizeof(char), decInfo->fptr_stego_image);
		decode_byte_from_lsb(decInfo->stego_magic_data, decInfo, 0);
		fwrite(decInfo->decode__mag_string, 1, sizeof(char), decInfo->fptr_decode_file);
	}
	return d_success;
}
// Calling rest of the decoding functions
Status do_decoding(DecodeInfo *decInfo)
{
	// Open file
	if (open_files_decoding(decInfo) == d_success)
	{
		printf("INFO :: Opened stego_image.bmp\n");
		printf("Opened required files successfully\n");
		printf("\n.............Started decoding.............\n");

		// Decoding magic string
		if (decode_magic_string(MAGIC_STRING, decInfo) == d_success)
		{
			printf("Successful :: Decoded Magic string signature\n");

			// Decoding extension size
			if (decode_secret_ext_size(decInfo) == d_success)
			{
				printf("Successful :: Decoded secret file extension size\n");

				// Decoding extension name
				if (decode_secret_extention_name(decInfo) == d_success)
				{
					printf("Successful :: Decoded secret file extension \n");

					// Opening output file to write secret message
					if (open_output_file(decInfo) == d_success)
					{
						printf("Successful :: Opened output file \n");

						// Decoding file size
						if (decode_file_size(decInfo) == d_success)
						{
							printf("Successful :: Decoded  file size \n");

							// Decoding secret data
							if (decode_secret_data(decInfo) == d_success)
							{
								printf("Successful :: Decoded secret message\n");
							}
							else
							{
								printf("Failure :: Fetching secret message\n");
								return d_failure;
							}
						}
						else
						{
							printf("Failure :: Unable to ftech secret file size\n");
							return d_failure;
						}
					}
					else
					{
						printf("Failure :: opening output file\n");
						return d_failure;
					}
				}
				else
				{
					printf("Failure :: Decoding secret file extension\n");
					return d_failure;
				}
			}
			else
			{
				printf("Failure :: Decoding secret file extention size\n");
				return d_failure;
			}
		}
		else
		{
			printf("Failure :: Decoding Magic string\n");
			return d_failure;
		}
	}
	else
	{
		printf("Failure :: While opening stego.bmp\n");
		return d_failure;
	}
	return d_success;
}