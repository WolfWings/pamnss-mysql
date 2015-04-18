#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "config.h"

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

typedef enum {
	STATE_SOL
,	STATE_COMMENT
,	STATE_NAME
,	STATE_PRE_EQUAL
,	STATE_POST_EQUAL
,	STATE_VALUE
} state;

typedef enum {
	LETTER_INVALID
,	LETTER_WHITESPACE
,	LETTER_EOL
,	LETTER_EQUAL
,	LETTER_COMMENT
,	LETTER_LETTER
} letter;

static size_t getFilesize(const char* filename) {
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

static void parser_error(const char *expect, const char *got, int lineline, int linechar) {
	char buffer[1024];
	snprintf(buffer, 1024, "Expected %s but got %s at line %i, character %i\n", expect, got, lineline, linechar);
	if (write(1, buffer, strlen(buffer)) != -1) {
		fsync(1);
	}
}

int fileline;
int filechar;

void parse_config(char *filename) {
	int fd
	  , nameStart
	  , nameStop
	  , valueStart
	  , valueStop
	  , lineR
	  , lineN
	  , lineC
	  , pos;
	char *name
	   , *value
	   , *config;
	state state;
	letter l;
	size_t filesize;

	filesize = getFilesize(filename);

	/* Open file */
	fd = open(filename, O_RDONLY, 0);
	if (fd == -1) {
		parser_error("readable file", "unreadable file", -1, -1);
		goto abort_open;
	}

	/* Execute mmap */
	config = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
	if (config == MAP_FAILED) {
		parser_error("mmapable file", "error mmapping file", -1, -1);
		goto abort_mmap;
	}

	name = value = NULL;
	nameStart = nameStop = valueStart = valueStop = pos = 0;
	lineR = lineN = lineC = 1;
	state = STATE_SOL;
	do {
		switch(config[pos]) {
			case '\r':
				lineR++;
				lineC = 1;
				l = LETTER_EOL;
				break;
			case '\n':
				lineN++;
				lineC = 1;
				l = LETTER_EOL;
				break;
			case ' ':
			case '\t':
				l = LETTER_WHITESPACE;
				break;
			case '=':
				l = LETTER_EQUAL;
				break;
			case '#':
				l = LETTER_COMMENT;
				break;
			default:
				if ((config[pos] < ' ')
				 || (config[pos] > '~')) {
					l = LETTER_INVALID;
					parser_error("valid character", "invalid character", min(lineR, lineN), lineC);
					state = STATE_COMMENT;
					break;
				}
				l = LETTER_LETTER;
				break;
		}

		switch(state) {
		case STATE_COMMENT: switch(l) {
			case LETTER_EOL:
				state = STATE_SOL;
				break;
			default:
				break;
			} break;
		case STATE_SOL: switch(l) {
			case LETTER_EOL:
			case LETTER_WHITESPACE:
				break;
			case LETTER_EQUAL:
				parser_error("option name", "=", min(lineR, lineN), lineC);
				state = STATE_COMMENT;
				break;
			case LETTER_COMMENT:
				state = STATE_COMMENT;
				break;
			case LETTER_LETTER:
			default:
				nameStart = pos;
				nameStop = pos;
				state = STATE_NAME;
				break;
			} break;
		case STATE_NAME: switch(l) {
			case LETTER_EOL:
				parser_error("=", "end of line", min(lineR, lineN), lineC);
				state = STATE_COMMENT;
				break;
			case LETTER_WHITESPACE:
				state = STATE_PRE_EQUAL;
				break;
			case LETTER_EQUAL:
				state = STATE_POST_EQUAL;
				break;
			case LETTER_COMMENT:
				parser_error("=", "comment", min(lineR, lineN), lineC);
				state = STATE_COMMENT;
				break;
			case LETTER_LETTER:
			default:
				nameStop = pos;
				break;
			} break;
		case STATE_PRE_EQUAL: switch(l) {
			case LETTER_WHITESPACE:
				break;
			case LETTER_EQUAL:
				state = STATE_POST_EQUAL;
				break;
			default:
				parser_error("= or more whitespace", "gibberish", min(lineR, lineN), lineC);
				state = STATE_COMMENT;
				break;
			} break;
		case STATE_POST_EQUAL: switch(l) {
			case LETTER_EOL:
				parser_error("option value or more whitespace", "end of line", min(lineR, lineN), lineC);
				state = STATE_COMMENT;
				break;
			case LETTER_WHITESPACE:
				break;
			case LETTER_EQUAL:
			case LETTER_COMMENT:
			case LETTER_LETTER:
			default:
				valueStart = pos;
				valueStop = pos;
				state = STATE_VALUE;
				break;
			} break;
		case STATE_VALUE: switch(l) {
			case LETTER_EOL:
				name = strndup((char *)config + nameStart, nameStop - nameStart + 1);
				value = strndup((char *)config + valueStart, valueStop - valueStart + 1);
				option_set(name, value);
				free(name);
				free(value);
				state = STATE_SOL;
				break;
			case LETTER_WHITESPACE:
			case LETTER_EQUAL:
			case LETTER_COMMENT:
			case LETTER_LETTER:
			default:
				valueStop = pos;
				break;
			} break;
		}
		pos++;
		lineC++;
	} while (pos < filesize);

	/* End-of-File handling. */
	switch(state) {
	case STATE_NAME:
	case STATE_PRE_EQUAL:
		parser_error("=", "end of file", min(lineR, lineN), lineC);
		break;
	case STATE_POST_EQUAL:
		parser_error("option value", "end of file", min(lineR, lineN), lineC);
		break;
	case STATE_SOL:
	case STATE_COMMENT:
	case STATE_VALUE:
	default:
		/* Valid end of file location, no issue. */
		break;
	}

	/* Cleanup */

	munmap(config, filesize);

abort_mmap:
	close(fd);

abort_open:
	return;
}
