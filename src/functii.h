#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

void get_operations(void **operations);

void bubbleSort(sensor *arr, int nr)
{
	int i = 0, j = 0;
	sensor aux;

	for (i = 0; i < nr - 1; i++) {
		for (j = 0; j < nr - i - 1; j++) {
			if (arr[j].sensor_type < arr[j + 1].sensor_type) {
				aux = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = aux;
			}
		}
	}
}

void clear(sensor *arr, int index, int dim)
{
	int i = 0;
	free(arr[index].operations_idxs);
	free(arr[index].sensor_data);
	

	for (i = index; i < dim - 1; i++) {
		arr[i] = arr[i + 1];
	}
	//arr = (sensor *)realloc(arr, sizeof(sensor) * dim);
	dim--;
	free(arr[dim]);
	arr = (sensor *)realloc(arr, sizeof(sensor) * dim);

}

void memfree(sensor *arr, int dim)
{
	int i = 0;

	for (i = 0; i < dim; i++) {
		free(arr[i].operations_idxs);
		free(arr[i].sensor_data);
	}
	free(arr);
}
