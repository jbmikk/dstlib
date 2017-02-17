#ifndef ARRAYS_H
#define ARRAYS_H

void int_to_array(char *buffer, unsigned int *size, int symbol);
void int_to_padded_array(char *array, int integer);
void int_to_padded_array_le(char *array, int integer);
int array_to_int(char *buffer, unsigned int size);

#endif //ARRAYS_H
