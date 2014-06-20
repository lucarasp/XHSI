
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMNavigation.h"
#include "XPLMDisplay.h"
#include "XPLMMenus.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"


#include "globals.h"


// config variables
int                 dest_enable[NUM_DEST];
char                dest_ip[NUM_DEST][20];
unsigned short int  dest_port[NUM_DEST];

unsigned short int  recv_port;
unsigned long int   recv_rate;
float               recv_delay;

unsigned long int   nav_data_rate;
unsigned long int   fms_data_rate;
unsigned long int   tcas_data_rate;
float               nav_data_delay;
float               fms_data_delay;
float               tcas_data_delay;



// Config settings ---------------------------------------------------------------------------

void defaultSettings() {

	int i;

	for (i=0; i<NUM_DEST; i++) {
		dest_enable[i] = i==0 ? 1 : 0;
		strcpy(dest_ip[i], DEFAULT_DEST_IP);
		dest_port[i] = DEFAULT_DEST_PORT + i;
	}
	recv_port = DEFAULT_RECV_PORT;
	recv_rate = 10;
	recv_delay = 1.0f / (float)recv_rate;

	nav_data_rate = 15;
	nav_data_delay = 1.0f / (float)nav_data_rate;
	fms_data_rate = 2;
	fms_data_delay = 1.0f / (float)fms_data_rate;
	tcas_data_rate = 5;
	tcas_data_delay = 1.0f / (float)tcas_data_rate;

}


void readConfig() {

	FILE	*cfg_file;
	char	cfg_line[120];
	char	param[40];
	char	dest_param[20];
	char	s_value[80];
	int		d_value;
	int i;

	cfg_file = fopen(CFG_FILE, "r");
	if (cfg_file != NULL) {
		XPLMDebugString("XHSI: reading ");
		XPLMDebugString(CFG_FILE);
		XPLMDebugString("\n");
		while (!feof(cfg_file)) {
			fgets(cfg_line, sizeof(cfg_line), cfg_file);
			sscanf(cfg_line, "%s %s", param, s_value);
			sscanf(s_value, "%d", &d_value);

			if (strcmp(param, "recv_port")==0) {
				if (d_value==0) {
					recv_port = DEFAULT_RECV_PORT;
				} else {
					recv_port = d_value;
				}
			}

			if (strcmp(param, "recv_rate")==0) {
				recv_rate = d_value;
				recv_delay = 1.0f / (float)recv_rate;
			}

			if (strcmp(param, "nav_data_rate")==0) {
				nav_data_rate = d_value;
				nav_data_delay = 1.0f / (float)nav_data_rate;
			}

			if (strcmp(param, "fms_data_rate")==0) {
				fms_data_rate = d_value;
				fms_data_delay = 1.0f / (float)fms_data_rate;
			}

			if (strcmp(param, "tcas_data_rate")==0) {
				tcas_data_rate = d_value;
				tcas_data_delay = 1.0f / (float)tcas_data_rate;
			}

			for (i=0; i<NUM_DEST; i++) {

				sprintf(dest_param, "dest_enable[%d]", i);
				if (strcmp(param, dest_param)==0) {
					dest_enable[i] = d_value;
				}

				sprintf(dest_param, "dest_ip[%d]", i);
				if (strcmp(param, dest_param)==0) {
					strcpy(dest_ip[i], s_value);
				}

				sprintf(dest_param, "dest_port[%d]", i);
				if (strcmp(param, dest_param)==0) {
					dest_port[i] = d_value;
				}

			}
		}
		fclose(cfg_file);
	}
}


void initSettings() {

    defaultSettings();
    readConfig();

}



void writeSettings() {

	FILE	*cfg_file;
	int		i;

	XPLMDebugString("XHSI: writing ");
	XPLMDebugString(CFG_FILE);
	// for the "\n", see below...

	cfg_file = fopen(CFG_FILE, "w");
	if (cfg_file != NULL) {

		fprintf(cfg_file, "cfg_version %d\n", PLUGIN_VERSION_NUMBER);

		fprintf(cfg_file, "recv_port %d\n", recv_port);
		fprintf(cfg_file, "recv_rate %ld\n", recv_rate);

		fprintf(cfg_file, "nav_data_rate %ld\n", nav_data_rate);
		fprintf(cfg_file, "fms_data_rate %ld\n", fms_data_rate);
		fprintf(cfg_file, "tcas_data_rate %ld\n", tcas_data_rate);

		for (i=0; i<NUM_DEST; i++) {
			fprintf(cfg_file, "dest_enable[%d] %d\n", i, dest_enable[i]);
			fprintf(cfg_file, "dest_ip[%d] %s\n", i, dest_ip[i]);
			fprintf(cfg_file, "dest_port[%d] %d\n", i, dest_port[i]);
		}

		fprintf(cfg_file, "\n");

		fclose(cfg_file);

		XPLMDebugString(" : OK\n");

	} else {
		XPLMDebugString(" : ERROR!\n");
	}

}

