/******************************************************************************
 * fidostat - Log file analyser for binkd
 * Copyright (c) 1999 Gabriel Plutzar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <huskylib/compiler.h>
#include <huskylib/huskylib.h>

#include <fidoconf/fidoconf.h>
#include <fidoconf/version.h>

#include "cvsdate.h"

typedef struct {
	char node_aka[50];
	char node_descr[100];
	int session_count;
} session_count_type;

static char month_names[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

int sessionsort(const void* a, const void* b)
{
	hs_addr addra, addrb;

	// Initialize all struct members to 0 in order to prevent occasional strange sorting behavior:
	memset(&addra, 0, sizeof(hs_addr));
	memset(&addrb, 0, sizeof(hs_addr));

	parseFtnAddrZS(a, &addra);
	parseFtnAddrZS(b, &addrb);

	if (addra.point == 0 && addrb.point > 0)
		return -1;
	if (addra.point > 0 && addrb.point == 0)
		return 1;

	if (addra.zone != addrb.zone) {
		if (addra.zone > addrb.zone)
			return 1;
		else
			return -1;
	} else if (addra.net != addrb.net) {
		if (addra.net > addrb.net)
			return 1;
		else
			return -1;
	} else if (addra.node != addrb.node) {
		if (addra.node > addrb.node)
			return 1;
		else
			return -1;
	} else if (addra.point != addrb.point) {
		if (addra.point > addrb.point)
			return 1;
		else
			return -1;
	}

	return 0;
}

int valid_args(int argc, char* argv[])
{
	if (argc == 2) {
		if (stricmp(argv[1], "binkdall") == 0)
			return 1;
		if (stricmp(argv[1], "binkdstat") == 0)
			return 1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	char hlp[200];
	char* hlp1;
	char session_with[200];
	char firstaddr[200];
	int getaddr = 0;
	FILE* binkdlog;
	int i;

	char* version_str = GenVersionStr("fidostat", fidoconf_VER_MAJOR, fidoconf_VER_MINOR,
			fidoconf_VER_PATCH, fidoconf_VER_BRANCH, cvs_date);

	if (!valid_args(argc, argv)) {
		fprintf(stderr, "%s - Log file analyser for binkd\n", version_str);
		fprintf(stderr, "Copyright (c) 1999 Gabriel Plutzar\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "USAGE: fidostat <option>\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "   options:  binkdall   List all binkd connections (one line per connection)\n");
		fprintf(stderr, "             binkdstat  Summarize binkd connections (one line per node)\n");
		return EXIT_FAILURE;
	}

	s_fidoconfig* config = readConfig(NULL);
	if (config == NULL)
		exit(3);

	time_t current_time = time(NULL);
	struct tm* current_date = localtime(&current_time);
	char current_date_str[7];
	sprintf(current_date_str, "%02u %s", current_date->tm_mday,
			month_names[current_date->tm_mon]); // For example: "09 Jan"

	printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	printf("%s - Log file analyser for binkd\n", version_str);
	printf("\n");
	printf("   Date:   %s", ctime(&current_time));
	printf("   System: %u:%u/%u.%u, %s\n",
			config->addr[0].zone,
			config->addr[0].net,
			config->addr[0].node,
			config->addr[0].point,
			config->sysop);
	printf("\n");
	printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	printf("\n");

	session_count_type* nodes = malloc(500 * sizeof(session_count_type));

	strcpy(hlp, config->logFileDir);
	strcat(hlp, "/binkd.log");
	binkdlog = fopen(hlp, "r");

	if (binkdlog == NULL) {
		printf("Cannot open %s !\n", hlp);
		exit(3);
	}

	int session_found;
	size_t node_count = 0;
	while (!feof(binkdlog)) {

		fgets(hlp, 200, binkdlog);
		hlp[strlen(hlp) - 1] = 0;

		if (strncmp(hlp + 2, current_date_str, 6) != 0)
			continue;

		hlp1 = strstr(hlp, "session with ");
		if (hlp1 != NULL) {
			// We have found a new session
			getaddr = 1;
			strcpy(session_with, hlp1 + 13); // For example: "78-66-161-203-no236.tbcn.telia.com [78.66.161.203] (2996)"
			continue;
		}

		hlp1 = strstr(hlp, "addr: ");
		if (hlp1 != NULL && getaddr == 1) {

			getaddr = 0;
			strcpy(firstaddr, hlp1 + 6);

			if (strcasecmp(argv[1], "binkdall") == 0) {
				printf("%s, %s\n", session_with, firstaddr);
			}

			session_found = 0;
			for (i = 0; i < node_count && i < 499; i++) {
				if (strcmp(firstaddr, nodes[i].node_aka) == 0) {
					session_found = 1;
					nodes[i].session_count++;
					break;
				}
			}

			if (session_found == 0) {
				strcpy(nodes[node_count].node_aka, firstaddr);
				strcpy(nodes[node_count].node_descr, session_with);
				nodes[node_count].session_count = 1;
				node_count++;
			}
		}
	}

	if (strcasecmp(argv[1], "binkdstat") == 0) {

		qsort(nodes, node_count, sizeof(session_count_type), sessionsort);

		for (i = 0; i < node_count; i++) {
			printf("%3u Sessions with %s, %s\n",
					nodes[i].session_count,
					nodes[i].node_aka,
					nodes[i].node_descr);
		}
	}

	fclose(binkdlog);
	disposeConfig(config);

	return 0;
}
