#include "arrays.h"

void int_to_array(unsigned char *array, unsigned int *size, int integer)
{
    int remainder = integer;
    int i;

    for (i = 0; i < sizeof(int); i++) {
        array[i] = remainder & 0xFF;
        remainder >>= 8;
        if(remainder == 0)
        {
            i++;
            break;
        }
    }
    *size = i;
}

int array_to_int(unsigned char *array, unsigned int size)
{
	int symbol = 0;
	int i;

	for (i = size-1; i >= 0; i--) {
		symbol <<= 8;
		symbol = symbol | array[i];
	}
	return symbol;
}

void int_to_padded_array(unsigned char *array, int integer)
{
	int remainder = integer;
	int i;

	for (i = 0; i < sizeof(int); i++) {
		array[i] = remainder & 0xFF;
		remainder >>= 8;
	}
}

/*
 * Convert int to padded array in little-endian format
 */
void int_to_padded_array_le(unsigned char *array, int integer)
{
	int remainder = integer;
	int i;

	for (i = sizeof(int)-1; i >= 0; i--) {
		array[i] = remainder & 0xFF;
		remainder >>= 8;
	}
}

/*
 * Convert int to padded array in little-endian format
 */
void intptr_to_padded_array_le(unsigned char *array, intptr_t integer)
{
	int remainder = integer;
	int i;

	for (i = sizeof(intptr_t)-1; i >= 0; i--) {
		array[i] = remainder & 0xFF;
		remainder >>= 8;
	}
}
