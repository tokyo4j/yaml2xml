// SPDX-License-Identifier: GPL-2.0-only
#include <assert.h>
#include <yaml.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static struct element *elements;
static int nr_elements, alloc_elements;

struct element {
	char name[64];
};

static struct element *
push(const char *name)
{
	if (nr_elements == alloc_elements) {
		alloc_elements = (alloc_elements + 16) * 2;
		elements = realloc(elements, alloc_elements * sizeof(struct element));
		if (!elements) {
			perror("out of memeory");
			exit(EXIT_FAILURE);
		}
	}
	struct element *element = elements + nr_elements;
	memset(element, 0, sizeof(*element));
	nr_elements++;
	strncpy(element->name, name, sizeof(element->name));
	element->name[sizeof(element->name) - 1] = '\0';
	return element;
}

static void
pop(char *buffer, size_t len)
{
	strncpy(buffer, elements[nr_elements-1].name, len);
	nr_elements--;
}

static void
indent(int level)
{
	printf("%*c", 2 * level, ' ');
}

void
process_one_event(yaml_event_t *event)
{
	static int level;
	static bool in_scalar;

	switch (event->type) {
	case YAML_NO_EVENT:
	case YAML_ALIAS_EVENT:
	case YAML_MAPPING_START_EVENT:
	case YAML_MAPPING_END_EVENT:
	case YAML_STREAM_START_EVENT:
	case YAML_STREAM_END_EVENT:
		break;
	case YAML_DOCUMENT_START_EVENT:
		printf("<?xml version=\"1.0\"?>\n");
		printf("<labwc_config>\n");
		++level;
		break;
	case YAML_DOCUMENT_END_EVENT:
		printf("</labwc_config>\n");
		--level;
		break;
	case YAML_SCALAR_EVENT:
		/*
		 * scalar-sequence -> <foo>
		 *                      <child></child>
		 *                      ...
		 *                    </foo>
		 *
		 *     ...with the child-elements defined withing a
		 *     subsequent mapping-{start,end}.
		 *
		 * scalar-scalar -> <foo>bar</foo>
		 *
		 *     ...where foo and bar are the names of the first and
		 *     second scalars respectively.
		 */
		if (in_scalar) {
			/* Handle second scalar of pair, so print "bar</foo>" */
			char element[64] = { 0 };
			pop(element, sizeof(element));
			printf("%s</%s>\n", event->data.scalar.value, element);
			in_scalar = false;
		} else {
			/*
			 * Handle first scalar out of a potential pair, so
			 * print "<foo>"
			 */
			char *value = (char *)event->data.scalar.value;
			indent(level);
			printf("<%s>", value);
			push(value);
			in_scalar = true;
		}
		break;
	case YAML_SEQUENCE_START_EVENT:
		/* Get ready for child-elements */
		assert(in_scalar);
		printf("\n");
		++level;
		in_scalar = false;
		break;
	case YAML_SEQUENCE_END_EVENT:
		/*
		 * We're now at the closing element </foo> of
		 * <foo>
		 *   <bar></bar>
		 * </foo>
		 */
		indent(--level);
		char element[64] = { 0 };
		pop(element, sizeof(element));
		printf("</%s>\n", element);
		break;
	}
	if (level < 0) {
		fprintf(stderr, "warn: indentation error\n");
		level = 0;
	}
}

int
main(int argc, char **argv)
{
	if (argc > 1) {
		printf("usage: %s < input.yaml > output.xml\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	yaml_parser_t parser;
	yaml_parser_initialize(&parser);
	yaml_parser_set_input_file(&parser, stdin);

	yaml_event_type_t event_type = 0;
	while (event_type != YAML_STREAM_END_EVENT) {
		yaml_event_t event;
		int success = yaml_parser_parse(&parser, &event);
		if (!success) {
			fprintf(stderr, "fatal: yaml parse: %s\n", parser.problem);
			yaml_parser_delete(&parser);
			return EXIT_FAILURE;
		}
		process_one_event(&event);
		event_type = event.type;
		yaml_event_delete(&event);
	}
	yaml_parser_delete(&parser);
	free(elements);
	nr_elements = 0;
	alloc_elements = 0;
	return EXIT_SUCCESS;
}
