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
main(int argc, char **argv)
{
	int total, passed, failed;
	xmlDocPtr tests;
	xmlNodePtr xnode;

	total = passed = failed = ret = 0;

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

	if (xmlStrcmp(cur->name, "testlist")) {
		fprintf(stderr, "Root node of testfile \"%s\" is not "
				"\"testlist\"\n", argv[1]);
		xmlFreeDoc(tests);
		return 1;
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if (run_test(cur) == 0)
			passed++;
		else
			failed++;

		total++;
	}

	xmlFreeDoc(tests);
	printf("Ran %d tests, %d passed, %d failed\n", total, passed, failed);
	return failed;
}
