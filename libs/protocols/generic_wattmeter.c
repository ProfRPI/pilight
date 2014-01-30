/*
	Copyright (C) 2014 1000io

	This file is part of pilight.

    pilight is free software: you can redistribute it and/or modify it under the 
	terms of the GNU General Public License as published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later 
	version.

    pilight is distributed in the hope that it will be useful, but WITHOUT ANY 
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR 
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pilight. If not, see	<http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../pilight.h"
#include "common.h"
#include "log.h"
#include "protocol.h"
#include "hardware.h"
#include "binary.h"
#include "gc.h"
#include "generic_wattmeter.h"

void genWattmeterCreateMessage(int id, int watt, int price) {
	generic_wattmeter->message = json_mkobject();
	json_append_member(generic_wattmeter->message, "id", json_mknumber(id));
	if(watt > -999) {
		json_append_member(generic_wattmeter->message, "watt", json_mknumber(watt));
	}
	if(price > -999) {
		json_append_member(generic_wattmeter->message, "price", json_mknumber(price));
	}
}

int genWattmeterCreateCode(JsonNode *code) {
	int id = -999;
	int watt = -999;
	int price = -999;
	char *tmp;

	if(json_find_string(code, "id", &tmp) == 0)
		id=atoi(tmp);
	if(json_find_string(code, "watt", &tmp) == 0)
		watt = atoi(tmp);
	if(json_find_string(code, "price", &tmp) == 0)
		price = atoi(tmp);

	if(id == -999 && watt == -999 && price == -999) {
		logprintf(LOG_ERR, "generic_wattmeter: insufficient number of arguments");
		return EXIT_FAILURE;
	} else {
		genWattmeterCreateMessage(id, watt, price);
	}
	return EXIT_SUCCESS;
}

void genWattmeterPrintHelp(void) {
	printf("\t -w --watt=watt\tset the watts\n");
	printf("\t -p --price=price\t\tset the price\n");
	printf("\t -i --id=id\t\t\tcontrol a device with this id\n");
}

void genWattmeterInit(void) {
	
	protocol_register(&generic_wattmeter);
	protocol_set_id(generic_wattmeter, "generic_wattmeter");
	protocol_device_add(generic_wattmeter, "generic_wattmeter", "Generic watt meter");
	generic_wattmeter->devtype = WATTMETER;

	options_add(&generic_wattmeter->options, 'w', "watt", has_value, config_value, "[0-9]");
	options_add(&generic_wattmeter->options, 'p', "price", has_value, config_value, "[0-9]");
	options_add(&generic_wattmeter->options, 'i', "id", has_value, config_id, "[0-9]");

	protocol_setting_add_number(generic_wattmeter, "decimals", 2);	
	protocol_setting_add_number(generic_wattmeter, "watt", 1);
	protocol_setting_add_number(generic_wattmeter, "price", 1);

	generic_wattmeter->printHelp=&genWattmeterPrintHelp;
	generic_wattmeter->createCode=&genWattmeterCreateCode;
}