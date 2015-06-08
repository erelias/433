/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x4d8, 0x3f, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %s\n", wstr);

	// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 0x80;
	res = hid_write(handle, buf, 65);

	//request user input for row and string data and send to PIC
	printf("Input a string:\n");
	unsigned char message[25];
	memset(message, ' ', 25);
	scanf("%[^\n]s", message);

	printf("Input the row for the string to be displayed:\n");
	int row=0;
	scanf("%d", &row);

	buf[0] = 0x0;
	buf[1] = 0x3;
	buf[2] = row;

	for (int i = 0; i < 25; i++){
		buf[3 + i] = message[i];
	}
	



	res = hid_write(handle, buf, 65);
	


	// Request state (cmd 0x81). The first byte is the report number (0x0).
	//buf[0] = 0x0;
	//buf[1] = 0x81;
	//res = hid_write(handle, buf, 65);

	// Read requested state

	buf[0] = 0x0;
	buf[1] = 0x2;

	int data_points = 100;
	int max_accel_reading = 16000;
	unsigned char data[100];
	char negative = '-';

	res = hid_write(handle, buf, 65);


	FILE *ofp;
	ofp = fopen("accels.txt", "w");
	
	while (i <= data_points){

		res = hid_read(handle, buf, 65);

		if (buf[1] == 0x01){

			//print x value to file
			if (buf[2] == 1){
				fprintf(ofp, "%c%d   ", negative, buf[3]);
			}
			else {
				fprintf(ofp, "%d   ", buf[3]);
			}

			//print y value to file
			if (buf[4] == 1){
				fprintf(ofp, "%c%d   ", negative, buf[5]);
			}
			else {
				fprintf(ofp, "%d   ", buf[5]);
			}

			//print z value to file
			if (buf[6] == 1){
				fprintf(ofp, "%c%d\n", negative, buf[7]);
			}
			else {
				fprintf(ofp, "%d\n", buf[7]);
			}

			i++;

		}

		buf[0] = 0x0;
		buf[1] = 0x2;
		res = hid_write(handle, buf, 65);

	}

	i = 0;
		



	//res = hid_read(handle, buf, 65);

	// Print out the returned buffer.
	//for (i = 0; i < 8; i++)
	//	printf("buf[%d]: %d\n", i, buf[i]);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}
