#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "functii.h"

int main(int argc, char const *argv[])
{
	int i = 0, j = 0, k = 0, nr_sensor = 0;
	FILE *infile = NULL;
	const char *file = argv[1];
	infile = fopen(file, "rb");

	if (infile == NULL) {
		printf("Eroare\n");
		return -1;
	}

	// se citesc numarul de senzori

	fread(&nr_sensor, sizeof(int), 1, infile);

	// se aloca memorie pentru vectorii de senzori

	sensor *vect = (sensor *)malloc(sizeof(sensor) * nr_sensor);

	for (i = 0; i < nr_sensor; i++) {
		fread(&vect[i].sensor_type, sizeof(int), 1, infile);

		if (vect[i].sensor_type == 0) {
			vect[i].sensor_data =
				(tire_sensor *)malloc(sizeof(tire_sensor));

			tire_sensor *t =
				(tire_sensor *)vect[i].sensor_data;

			// se citeste din fisierul binar datele pentru senzorul specific

			fread(&t->pressure, sizeof(float), 1, infile);
			fread(&t->temperature, sizeof(float), 1, infile);
			fread(&t->wear_level, sizeof(int), 1, infile);
			fread(&t->performace_score, sizeof(int), 1, infile);

			fread(&vect[i].nr_operations, sizeof(int), 1, infile);

			// mai intai se cistesc numarul de operatii si dupa se aloca memorie
			// Memoria este alocata pentru fiecare index al operatiei

			vect[i].operations_idxs =
				(int *)malloc(sizeof(int) * vect[i].nr_operations);

			// se introduc indexii operatiilor in vector

			for (j = 0; j < vect[i].nr_operations; j++) {
				fread(&vect[i].operations_idxs[j], sizeof(int), 1, infile);
			}
		} else {
			vect[i].sensor_data =
				(power_management_unit *)malloc(sizeof(power_management_unit));

			power_management_unit *pmu =
				((power_management_unit *)vect[i].sensor_data);

			fread(&pmu->voltage, sizeof(float), 1, infile);
			fread(&pmu->current, sizeof(float), 1, infile);
			fread(&pmu->power_consumption, sizeof(float), 1, infile);
			fread(&pmu->energy_regen, sizeof(int), 1, infile);
			fread(&pmu->energy_storage, sizeof(int), 1, infile);

			fread(&vect[i].nr_operations, sizeof(int), 1, infile);
			vect[i].operations_idxs =
				(int *)malloc(sizeof(int) * vect[i].nr_operations);

			for (j = 0; j < vect[i].nr_operations; j++) {
				fread(&vect[i].operations_idxs[j], sizeof(int), 1, infile);
			}
		}
	}

	// Se sorteaza senzorii descrescator dupa tip (mai intai de tip 1 "pmu")
	// Pentru acest lucru s a folosit BubbleSort

	bubbleSort(vect, nr_sensor);

	char input[100], str[100] = {0};
	int index = 0;
	void *oper[8];

	// daca string ul dat de la tastatura este diferit de "exit"
	// se apeleaza bucla while

	while (strcmp(str, "exit") != 0) {

		// in vectorul input se salveaza un string inclusiv cu spatii
		// se desparte string ul cu ajutorul "sscanf"

		fgets(input, 100, stdin);
		sscanf(input, "%s %d", str, &index);

		if (strcmp(str, "print") == 0) {

			// se afiseaza datele citite anterior

			if (index <= nr_sensor && index >= 0) {
				if (vect[index].sensor_type == 1) {
					power_management_unit *pmu =
						((power_management_unit *)vect[index].sensor_data);

					printf("Power Management Unit\n");
					printf("Voltage: %.2f\n", pmu->voltage);
					printf("Current: %.2f\n", pmu->current);
					printf("Power Consumption: %.2f\n", pmu->power_consumption);
					printf("Energy Regen: %d%%\n", pmu->energy_regen);
					printf("Energy Storage: %d%%\n", pmu->energy_storage);
				} else {
					tire_sensor *t =
						(tire_sensor *)vect[index].sensor_data;

					printf("Tire Sensor\n");
					printf("Pressure: %.2f\n", t->pressure);
					printf("Temperature: %.2f\n", t->temperature);
					printf("Wear Level: %d%%\n", t->wear_level);

					if (t->performace_score == 0) {
						printf("Performance Score: Not Calculated\n");
					} else {
						printf("Performance Score: %d\n", t->performace_score);
					}
				}
			} else {
				printf("Index not in range!\n");
			}
		} else if (strcmp(str, "clear") == 0) {

		// Se verifica daca macar o valoarea a senzorului este eronata
		// Se apeleaza o functie de stergere din vector (dealocand si memoria)
		// se folosesc 2 for uri doarece urmatorul senzor dupa un senzor
		// cu o valoare eronata doarece poate avea la fel o valoare eronata

			for (k = 0; k < nr_sensor; k++) {
				for (i = 0; i < nr_sensor; i++) {
					if (vect[i].sensor_type == 1) {
						power_management_unit *pmu =
							((power_management_unit *)vect[i].sensor_data);

						if (pmu->voltage < 10 ||
							pmu->voltage > 20 ||
							pmu->current < -100 ||
							pmu->current > 100 ||
							pmu->power_consumption < 0 ||
							pmu->power_consumption > 1000 ||
							pmu->energy_regen < 0 ||
							pmu->energy_regen > 100 ||
							pmu->energy_storage < 0 ||
							pmu->energy_storage > 100) {

							clear(vect, i, nr_sensor);
						}
					} else {
						tire_sensor *t =
							(tire_sensor *)vect[i].sensor_data;

						if (t->pressure < 19 ||
							t->pressure > 28 ||
							t->temperature < 0 ||
							t->temperature > 120 ||
							t->wear_level < 0 ||
							t->wear_level > 100) {

							clear(vect, i, nr_sensor);
						}
					}
				}
			}
		} else if (strcmp(str, "analyze") == 0) {
			get_operations(oper);

			// se apeleaza functia get_operations pentru analyze

			for (j = 0; j < vect[index].nr_operations; j++) {
				void *data = vect[index].sensor_data;
				int idxs = vect[index].operations_idxs[j];

				((void (*)(void *))oper[idxs])(data);
			}
		} else if (strcmp(str, "exit") == 0) {

			// se elibereaza toata memoria alocata

			memfree(vect, nr_sensor);
			fclose(infile);
		}
	}

	return 0;
}
