#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "dsptest.h"

void
usage()
{

	fprintf(stderr, "Usage: dsptest testcases.xml\n");
	return;
}

int
parse_test(xmlDocPtr doc, xmlNodePtr test)
{
	xmlNodePtr subnode;
	const char *testname, *tmp;
	char *endptr, *filename;
	int testnum;
	int wanted_retval;
	bool got_retval;

	testname = NULL;
	filename = NULL;
	testnum = -1;
	got_retval = false;

	subnode = test->xmlChildrenNode;
	while (subnode != NULL) {
		if (subnode->type == XML_TEXT_NODE) {
			subnode = subnode->next;
			continue;
		}

		if (!xmlStrcmp("name", subnode->name)) {
			testname = xmlNodeListGetString(doc,
					subnode->xmlChildrenNode, 1);
		} else if (!xmlStrcmp("filename", subnode->name)) {
			filename = xmlNodeListGetString(doc,
					subnode->xmlChildrenNode, 1);
		} else if (!xmlStrcmp("nodenum", subnode->name)) {
			tmp = xmlNodeListGetString(doc,
					subnode->xmlChildrenNode, 1);

			/* The string is preceeded by a " character */
			if (*tmp != '\0')
				tmp += 1;
			testnum = strtoul(tmp, &endptr, 16);
			if (endptr == tmp) {
				fprintf(stderr, "\"%s\" is not a node number\n",
									tmp);
				return -1;
			}
		} else if (!xmlStrcmp("retval", subnode->name)) {
			tmp = xmlNodeListGetString(doc,
					subnode->xmlChildrenNode, 1);
			wanted_retval = strtoul(tmp, &endptr, 16);
			got_retval = true;
			if (endptr == tmp) {
				fprintf(stderr, "\"%s\" is not a return val\n",
									tmp);
				return -1;
			}
		}
		subnode = subnode->next;
	}

	if (got_retval == false || testnum == -1 || testname == NULL) {
		fprintf(stderr, "Missing node(s) when reading a test\n");
		return -1;
	}

	return run_test(testname, filename, testnum, wanted_retval);
}

int
main(int argc, char **argv)
{
	int ret, total, passed, failed;
	xmlDocPtr tests;
	xmlNodePtr cur;

	ret = total = passed = failed = ret = 0;

	if (argc != 2) {
		usage();
		return 1;
	}

	if (check_dsp_open()) {
		fprintf(stderr, "Failed to open dsp\n");
		return 1;
	}

	tests = xmlParseFile(argv[1]);
	if (tests == NULL) {
		fprintf(stderr, "libxml couldn't parse testfile \"%s\"\n",
								argv[1]);
		return 1;
	}

	cur = xmlDocGetRootElement(tests);
	if (cur == NULL) {
		fprintf(stderr, "No root node in testfile \"%s\"\n", argv[1]);
		xmlFreeDoc(tests);
		return 1;
	}

	if (xmlStrcmp(cur->name, "testlist")) {
		fprintf(stderr, "Root node of testfile \"%s\" is not "
				"\"testlist\"\n", argv[1]);
		xmlFreeDoc(tests);
		return 1;
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if (cur->type == XML_TEXT_NODE) {
			cur = cur->next;
			continue;
		}

		ret = parse_test(tests, cur);
		if (ret == 0) {
			passed++;
		} else if (ret > 0) {
			failed++;
		} else {
			fprintf(stderr, "Error parsing test\n");
			xmlFreeDoc(tests);
			return -1;
		}

		total++;
		cur = cur->next;
	}

	xmlFreeDoc(tests);
	close_dsp();
	printf("Ran %d tests, %d passed, %d failed\n", total, passed, failed);
	return failed;
}
