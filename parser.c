#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>

#include "config.h"

#ifndef max
#define max(a, b) ((a > b) ? a : b)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

typedef enum {
	STATE_START_OF_LINE
,	STATE_COMMENT_OR_ERROR	// Ignores further data until end of line
,	STATE_NAME		// Until the first whitespace or equal sign
,	STATE_PRE_EQUAL		// Only if whitespace before equal sign
,	STATE_POST_EQUAL
,	STATE_VALUE		// First non-whitespace after equal sign
} state;

typedef enum {
	LETTER_INVALID
,	LETTER_WHITESPACE
,	LETTER_END_OF_LINE
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
	syslog(LOG_WARNING, "PAMNSS-MySQL-2017: Expected %s but got %s at line %i, character %i", expect, got, lineline, linechar);
}

void config_parse(char *filename) {
	/* Prevent parsing config file multiple times. */
	static int config_parsed = 0;

	int fd
	  , nameStart
	  , nameStop
	  , valueStart
	  , valueStop
	  , lineR	// Counting \r's
	  , lineN	// Counting \n's
	  , lineT	// Highest of \r's and \n's
	  , lineC	// Character in line
	  , pos;	// Byte position in file
	char *name
	   , *value
	   , *config;
	state state;
	letter l;
	size_t filesize;

	if (config_parsed) {
		return;
	}

	filesize = getFilesize(filename);

	/* Open file */
	fd = open(filename, O_RDONLY, 0);
	if (fd == -1) {
		/* Do not throw an error on EACCES, to allow blindly reading
		 * the 'global' followed by the 'root only' configurations to
		 * simplify overall code.
		 */
		if (errno != EACCES) {
			parser_error("readable file", "unreadable file", -1, -1);
		}

		return;
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
	state = STATE_START_OF_LINE;
	do {
		switch(config[pos]) {
			case '\r':
				lineR++;
				lineT = max(lineR, lineN);
				lineC = 1;
				l = LETTER_END_OF_LINE;
				break;
			case '\n':
				lineN++;
				lineT = max(lineR, lineN);
				lineC = 1;
				l = LETTER_END_OF_LINE;
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
					parser_error("valid character", "invalid character", lineT, lineC);
					state = STATE_COMMENT_OR_ERROR;
					break;
				}
				l = LETTER_LETTER;
				break;
		}

		switch(state) {
		case STATE_COMMENT_OR_ERROR: switch(l) {
			case LETTER_END_OF_LINE:
				state = STATE_START_OF_LINE;
				break;
			default:
				break;
			} break;
		case STATE_START_OF_LINE: switch(l) {
			case LETTER_END_OF_LINE:
			case LETTER_WHITESPACE:
				break;
			case LETTER_EQUAL:
				parser_error("option name", "=", lineT, lineC);
				state = STATE_COMMENT_OR_ERROR;
				break;
			case LETTER_COMMENT:
				state = STATE_COMMENT_OR_ERROR;
				break;
			case LETTER_LETTER:
			default:
				nameStart = pos;
				nameStop = pos;
				state = STATE_NAME;
				break;
			} break;
		case STATE_NAME: switch(l) {
			case LETTER_END_OF_LINE:
				parser_error("=", "end of line", lineT, lineC);
				state = STATE_COMMENT_OR_ERROR;
				break;
			case LETTER_WHITESPACE:
				state = STATE_PRE_EQUAL;
				break;
			case LETTER_EQUAL:
				state = STATE_POST_EQUAL;
				break;
			case LETTER_COMMENT:
				parser_error("=", "comment", lineT, lineC);
				state = STATE_COMMENT_OR_ERROR;
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
				parser_error("= or more whitespace", "gibberish", lineT, lineC);
				state = STATE_COMMENT_OR_ERROR;
				break;
			} break;
		case STATE_POST_EQUAL: switch(l) {
			case LETTER_END_OF_LINE:
				parser_error("option value or more whitespace", "end of line", lineT, lineC);
				state = STATE_COMMENT_OR_ERROR;
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
			case LETTER_END_OF_LINE:
				name = strndup((char *)config + nameStart, nameStop - nameStart + 1);
				value = strndup((char *)config + valueStart, valueStop - valueStart + 1);
				option_set(name, value);
				free(name);
				free(value);
				state = STATE_START_OF_LINE;
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
		parser_error("=", "end of file", lineT, lineC);
		break;
	case STATE_POST_EQUAL:
		parser_error("option value", "end of file", lineT, lineC);
		break;
	case STATE_START_OF_LINE:
	case STATE_COMMENT_OR_ERROR:
	case STATE_VALUE:
	default:
		/* Valid end of file location, no issue. */
		break;
	}

	/* Prevent parsing config file multiple times. */
	config_parsed = !0;

	/* Cleanup */

	munmap(config, filesize);

abort_mmap:
	close(fd);
}
