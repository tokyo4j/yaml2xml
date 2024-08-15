// SPDX-License-Identifier: GPL-2.0-only
#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static bool is_attribute;

static int
depth(xmlNode *node)
{
	int count = 0;

	while (node && node->parent) {
		if (strcmp((char *)node->name, "text") != 0) {
			++count;
		}
		node = node->parent;
	}

	/* Ignore root element */
	--count;

	return count;
}

static void
indent(int level)
{
	if (level > 1) {
		--level;
		printf("%*c", 2 * level, ' ');
	}
	printf("- ");
}

static void
entry(xmlNode *node, const char *name, const char *content)
{
	if (getenv("DEBUG")) {
		printf("%d: %s-%s\n", depth(node), name, content);
		return;
	}

	static bool in_element;
	int level = depth(node);
	static const char *last;

	if (strcmp(name, "text")) {
		/* Element */
		if (in_element) {
			/* Consequtive element without text since last one */
			printf("%s: ", last);
			printf("\n");
		}
		in_element = true;
		indent(level);
		last = name;
	} else {
		/* Value */
		if (is_attribute) {
			if (in_element) {
				/* With <foo bar="" baz=""> we need to print foo here */
				printf("%s:\n", (char *)node->parent->parent->name);
			}
			indent(level);
		}
		in_element = false;
		printf("%s: %s\n", (char *)node->parent->name, content);
	}
}

static void
process_node(xmlNode *node)
{
	if (!node || xmlIsBlankNode(node) || !node->name) {
		return;
	}
	if (!strcasecmp((char *)node->name, "labwc_config")) {
		return;
	}
	entry(node, (const char *)node->name, (const char *)node->content);
}

static void xml_tree_walk(xmlNode *node);

static void
traverse(xmlNode *n)
{
	process_node(n);
	for (xmlAttr *attr = n->properties; attr; attr = attr->next) {
		is_attribute = true;
		xml_tree_walk(attr->children);
	}
	is_attribute = false;
	xml_tree_walk(n->children);
}

static void
xml_tree_walk(xmlNode *node)
{
	for (xmlNode *n = node; n && n->name; n = n->next) {
		if (!strcasecmp((char *)n->name, "labwc_config")) {
			printf("---\n");
			traverse(n);
			printf("...\n");
			continue;
		}
		if (!strcasecmp((char *)n->name, "comment")) {
			continue;
		}
		traverse(n);
	}
}

static void
parse_xml(const char *filename)
{
	xmlDoc *d = xmlReadFile(filename, NULL, XML_PARSE_NOBLANKS);
	if (!d) {
		fprintf(stderr, "warn: xmlReadFile('%s')\n", filename);
		exit(1);
	}
	xml_tree_walk(xmlDocGetRootElement(d));
	xmlFreeDoc(d);
	xmlCleanupParser();
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s input.rc > output.yaml\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	LIBXML_TEST_VERSION
	parse_xml(argv[1]);
	return 0;
}
