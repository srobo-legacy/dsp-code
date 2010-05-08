#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void
useage()
{

	fprintf(stderr, "Usage: dsptest testcases.xml\n");
	return;
}

int
parse_test(xmlDocPtr doc, xmlNodePtr test)
{
	xmlNodePtr subnode;
	const char *testname, *tmp, *endptr;
	int testnum;
	int wanted_retval;
	bool got_retval;

	testname = NULL;
	testnum = -1;
	got_retval = false;

	subnode = test->xmlChildrenNode;
	while (subnode != NULL) {
		if (!xmlStrcmp("name", subnode->name)) {
			testname = xmlNodeListGetString(doc,
					subnode->xmlChildrenNode, 1);
		} else if (!xmlStrcmp("nodenum", subnode->name)) {
			tmp = xmlNodeListGetString(doc,
					subnode->xmlChildrenNode, 1);
			testnum = strtol(tmp, &endptr, 16);
			if (entptr == tmp) {
				fprintf(stderr, "\"%s\" is not a node number\n",
									tmp);
				return -1;
			}
		} else if (!xmlStrcmp("retval", subnode->name)) {
			tmp = xmlNodeListGetString(doc,
					subnode->xmlChildrenNode, 1);
			wanted_retval = strtol(tmp, &endptr, 16);
			got_retval = true;
			if (entptr == tmp) {
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

	return run_test(testname, testnum, wanted_retval);
}

int
main(int argc, char **argv)
{
	int ret, total, passed, failed;
	xmlDocPtr tests;
	xmlNodePtr xnode;

	ret = total = passed = failed = ret = 0;

	if (argc != 2) {
		usage();
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

	if (!xmlStrcmp(cur->name, "testlist")) {
		fprintf(stderr, "Root node of testfile \"%s\" is not "
				"\"testlist\"\n", argv[1]);
		xmlFreeDoc(tests);
		return 1;
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		ret = parse_test(cur);
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
	}

	xmlFreeDoc(tests);
	printf("Ran %d tests, %d passed, %d failed\n", total, passed, failed);
	return failed;
}
