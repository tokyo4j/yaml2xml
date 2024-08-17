// SPDX-License-Identifier: GPL-2.0-only
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>

__attribute__((unused)) static const char *
get_event_name(yaml_event_type_t type) {
    switch (type) {
    case YAML_NO_EVENT:
        return "YAML_NO_EVENT";
    case YAML_STREAM_START_EVENT:
        return "YAML_STREAM_START_EVENT";
    case YAML_STREAM_END_EVENT:
        return "YAML_STREAM_END_EVENT";
    case YAML_DOCUMENT_START_EVENT:
        return "YAML_DOCUMENT_START_EVENT";
    case YAML_DOCUMENT_END_EVENT:
        return "YAML_DOCUMENT_END_EVENT";
    case YAML_ALIAS_EVENT:
        return "YAML_ALIAS_EVENT";
    case YAML_SCALAR_EVENT:
        return "YAML_SCALAR_EVENT";
    case YAML_SEQUENCE_START_EVENT:
        return "YAML_SEQUENCE_START_EVENT";
    case YAML_SEQUENCE_END_EVENT:
        return "YAML_SEQUENCE_END_EVENT";
    case YAML_MAPPING_START_EVENT:
        return "YAML_MAPPING_START_EVENT";
    case YAML_MAPPING_END_EVENT:
        return "YAML_MAPPING_END_EVENT";
    default:
        return NULL;
    }
}

static void process_mapping(yaml_parser_t *parser);

static void process_sequence(yaml_parser_t *parser, char *key_name) {
    while (1) {
        yaml_event_t event;
        assert(yaml_parser_parse(parser, &event));
        if (event.type == YAML_SEQUENCE_END_EVENT)
            break;
        switch (event.type) {
        case YAML_MAPPING_START_EVENT:
            printf("<%s>", key_name);
            process_mapping(parser);
            printf("</%s>", key_name);
            break;
        case YAML_SEQUENCE_START_EVENT:
            assert(false);
            break;
        case YAML_SCALAR_EVENT:
            printf("<%s>", (char *)key_name);
            printf("%s", (char *)event.data.scalar.value);
            printf("</%s>", (char *)key_name);
            break;
        default:;
        }
    }
}

static void process_mapping(yaml_parser_t *parser) {
    while (1) {
        yaml_event_t key_event, value_event;
        assert(yaml_parser_parse(parser, &key_event));
        if (key_event.type == YAML_MAPPING_END_EVENT)
            break;
        assert(key_event.type == YAML_SCALAR_EVENT);

        assert(yaml_parser_parse(parser, &value_event));

        switch (value_event.type) {
        case YAML_MAPPING_START_EVENT:
            printf("<%s>", (char *)key_event.data.scalar.value);
            process_mapping(parser);
            printf("</%s>", (char *)key_event.data.scalar.value);
            break;
        case YAML_SEQUENCE_START_EVENT:
            process_sequence(parser, (char *)key_event.data.scalar.value);
            break;
        case YAML_SCALAR_EVENT:
            printf("<%s>", (char *)key_event.data.scalar.value);
            printf("%s", (char *)value_event.data.scalar.value);
            printf("</%s>", (char *)key_event.data.scalar.value);
            break;
        default:;
        }
    }
}

static void process_root(yaml_parser_t *parser) {
    yaml_event_t event;
    assert(yaml_parser_parse(parser, &event));
    assert(event.type == YAML_STREAM_START_EVENT);
    assert(yaml_parser_parse(parser, &event));
    assert(event.type == YAML_DOCUMENT_START_EVENT);
    assert(yaml_parser_parse(parser, &event));
    assert(event.type == YAML_MAPPING_START_EVENT);
    printf("<?xml version=\"1.0\"?><labwc_config>");
    process_mapping(parser);
    printf("</labwc_config>");
}

int main(int argc, char **argv) {
    if (argc > 1) {
        printf("usage: %s < input.yaml > output.xml\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    yaml_parser_t parser;
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, stdin);

    // TODO: free objects
    process_root(&parser);

    yaml_parser_delete(&parser);
    return EXIT_SUCCESS;
}
