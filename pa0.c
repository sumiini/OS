/**********************************************************************
 * Copyright (c) 2019
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_NR_TOKENS 32	/* Maximum number of tokens in a command */
#define MAX_TOKEN_LEN 64	/* Maximum length of single token */
#define MAX_COMMAND	256		/* Maximum length of command string */
#pragma warning (disable : 4996)
#define _CRT_SECURE_NO_WARNINGS

 /***********************************************************************
  * parse_command
  *
  * DESCRIPTION
  *	parse @command, and put each command token into @tokens[] and the number of
  *	tokes into @nr_tokens.
  *
  * A command token is defined as a string without any whitespace (i.e., *space*
  * and *tab* in this programming assignment). For exmaple,
  *   command = "  Hello world   Ajou   University!!  "
  *
  * then, nr_tokens = 4 and tokens should be
  *
  *   tokens[0] = "Hello"
  *   tokens[1] = "workd"
  *   tokens[2] = "Ajou"
  *   tokens[3] = "University!!"
  *
  * Another exmaple is;
  *   command = "ls  -al   /home/sanghoon /etc  "
  *
  * then, nr_tokens = 4, and tokens is
  *   tokens[0] = "ls"
  *   tokens[1] = "-al"
  *   tokens[2] = "/home/sanghoon"
  *   tokens[3] = "/etc"
  *
  *
  * RETURN VALUE
  *	Return 0 after filling in @nr_tokens and @tokens[] properly
  *
  */
static int parse_command(char *command, int *nr_tokens, char *tokens[])
{
	/* TODO
	 * Followings are example code. You should delete them and implement
	 * your own code here
	 */

	int command_len = 0;
	for (int p = 0;; p++) {
		if (command[p] != NULL) {
			command_len++;
		}
		else {
			break;

		}

	}
	//공백 아닌거 찾기

	int a = 0;
	int cnt1 = 0;
	int cnt2 = 0;

	for (a = 0;a<command_len;a++) {
		int i;
		for (i = a; i < command_len; i++) {
			if (command[i] != 32 && command[i] != '\t' && command[i] != '\n' && command[i] != '\r') {
				break;
			}

		}
		cnt1 = i;

		int k ;
		for (k = cnt1; k < command_len; k++) {
			if (command[k] == 32 || command[k] == '\t' || command[k] == '\n' || command[k] == '\r' || command[k] == NULL) {
				
				break;
			}
		}
		cnt2 = k;
		int length = 0;
		length = cnt2 - cnt1;
		if (length != 0) {
			char* baby_token = (char*)malloc(sizeof(char)*(length));
			int tmp = 0;
			for (int b = cnt1; b < cnt2; b++) {

				baby_token[tmp] = command[b];
				tmp++;
			}

			tokens[*nr_tokens] = baby_token;
			(*nr_tokens)++;
		}
		a =+ cnt2;

	}

	return 0;
}



/***********************************************************************
 * The main function of this program. SHOULD NOT CHANGE THE CODE BELOW
 */
int main(int argc, const char *argv[])
{
	char line[MAX_COMMAND] = { '\0' };
	FILE *input = stdin;

	if (argc == 2) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[1]);
			return -EINVAL;
		}
	}

	while (fgets(line, sizeof(line), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;

		parse_command(line, &nr_tokens, tokens);

		fprintf(stderr, "nr_tokens = %d\n", nr_tokens);
		for (int i = 0; i < nr_tokens; i++) {
			fprintf(stderr, "tokens[%d] = %s\n", i, tokens[i]);
		}
		printf("\n");
	}

	if (input != stdin) fclose(input);

	return 0;
}
