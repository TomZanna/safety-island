#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <memory.h>
#include <unistd.h>

#include "system_header.h"
#include "ini_file.h"
#include "mcihw.h"

char str_ini_file_name[PATH_MAX] = { 0 };
char ini_file[INI_MAX_LINE_COUNT][INI_MAX_LINE_LENGTH];
FILE* pf_ini_file = NULL;
int b_ini_modified = 0;
int i_ini_line_count = 0;

extern char gstr_app_folder[PATH_MAX];

void ini_initialize(char* str_name)
{
	strcpy(str_ini_file_name, gstr_app_folder);
	strcat(str_ini_file_name, str_name);

	memset(ini_file, 0, sizeof(ini_file));
}

void ini_uninitialize()
{	
	if (pf_ini_file)
	{
		fclose(pf_ini_file);
		pf_ini_file = NULL;
	}
}

void ini_read()
{
	pf_ini_file = fopen(str_ini_file_name, "r");
	if (pf_ini_file != NULL)
	{
		int i_line = 0;
		char* str_read = NULL;
		do
		{
			char* pch = NULL;
			str_read = fgets(ini_file[i_line], INI_MAX_LINE_LENGTH, pf_ini_file);
			if (str_read == NULL)
				break;
			while ((pch = strrchr(ini_file[i_line], '\n')))
			{
				*pch = 0;
			}
			while ((pch = strrchr(ini_file[i_line], '\r')))
			{
				*pch = 0;
			}
			++i_line;
		} while (i_line < INI_MAX_LINE_COUNT);

		i_ini_line_count = i_line;
		fclose(pf_ini_file);
		pf_ini_file = NULL;
	}
	else
	{
		// failed to open file
		if (gi_debug & DBG_DEFAULT)
			printf(DBGFMT "Error opening initialization data file %s.\n", DBGARGS, str_ini_file_name);
	}
}

void ini_write()
{
	pf_ini_file = fopen(str_ini_file_name, "w");
	if (pf_ini_file != NULL)
	{
		int i_line = 0;
		rewind(pf_ini_file);

		while (i_line < i_ini_line_count)
		{
			if (ini_file[i_line][0] != '\0')
			{
				if (fputs(&ini_file[i_line][0], pf_ini_file) == EOF)
				{
					if (gi_debug & DBG_DEFAULT)
						printf(DBGFMT "error writing file\n", DBGARGS);
				}
				if (fputs("\n", pf_ini_file) == EOF)
				{
					if (gi_debug & DBG_DEFAULT)
						printf(DBGFMT "error writing file\n", DBGARGS);
				}
			}
			else
			{
				// ok, empty line
			}

			++i_line;
		}

		fclose(pf_ini_file);
		pf_ini_file = NULL;
		sync();
		b_ini_modified = 0;
	}
}

char* ini_get_value(const char* str_key)
{
	int i_line = 0;
	int i_key_len = strlen(str_key);

	while (i_line < INI_MAX_LINE_COUNT)
	{
		int i_char = 0;
		int i_start_char = 0;
		int b_is_comment = 0;
		int b_is_empty = 1;
		char* p_start = NULL;

		for (; i_char < INI_MAX_LINE_LENGTH; ++i_char)
		{
			char ch = ini_file[i_line][i_char];
			if (ch == '\0')
			{	// parsed entire line
				break;
			}

			switch (ch)
			{
			case ' ':
			case '\t':
				continue;

			case '#':
				if (b_is_empty)
				{
					b_is_comment = 1;
				}
				break;

			default:
				if (b_is_empty)
				{
					b_is_empty = 0;
					i_start_char = i_char;
					p_start = &ini_file[i_line][i_char];
				}
				break;
			}

			if (b_is_comment)
			{	// no need to parse entire comment
				break;
			}
			if (p_start)
			{	// found the beginning of contents
				break;
			}
		}

		if (b_is_comment || b_is_empty)
		{
			// ok, comment or empty line
		}
		else if (p_start)
		{
			if (strncasecmp(str_key, p_start, i_key_len) == 0)
			{
				int i_equals_found = 0;
				i_char = i_start_char + i_key_len;
				for (; i_char < INI_MAX_LINE_LENGTH; ++i_char)
				{
					char ch = ini_file[i_line][i_char];
					switch (ch)
					{
					case ' ':
					case '\t':
						continue;

					case '=':
						i_equals_found = 1;
						break;

					default:
						if (i_equals_found)
						{
							return &ini_file[i_line][i_char];
						}
					}
				}
			}
		}
		else
		{
		}

		++i_line;
	}

	return NULL;
}

void ini_set_value(const char* str_key, const char* str_value)
{
	int i_line = 0;
	int i_key_len = strlen(str_key);

	while (i_line < INI_MAX_LINE_COUNT)
	{
		int i_char = 0;
		int i_start_char = 0;
		int b_is_comment = 0;
		int b_is_empty = 1;
		char* p_start = NULL;

		for (; i_char < INI_MAX_LINE_LENGTH; ++i_char)
		{
			char ch = ini_file[i_line][i_char];
			if (ch == '\0')
			{	// parsed entire line
				break;
			}

			switch (ch)
			{
			case ' ':
			case '\t':
				continue;

			case '#':
				if (b_is_empty)
				{
					b_is_comment = 1;
				}
				break;

			default:
				if (b_is_empty)
				{
					b_is_empty = 0;
					i_start_char = i_char;
					p_start = &ini_file[i_line][i_char];
				}
				break;
			}

			if (b_is_comment)
			{	// no need to parse entire comment
				break;
			}
			if (p_start)
			{	// found the beginning of contents
				break;
			}
		}

		if (b_is_comment || b_is_empty)
		{
			// ok, comment or empty line
		}
		else if (p_start)
		{
			if (strncasecmp(str_key, p_start, i_key_len) == 0)
			{
				int i_equals_found = 0;
				i_char = i_start_char + i_key_len;
				for (; i_char < INI_MAX_LINE_LENGTH; ++i_char)
				{
					char ch = ini_file[i_line][i_char];
					switch (ch)
					{
					case ' ':
					case '\t':
						continue;

					case '=':
						i_equals_found = 1;
						break;

					default:
						if (i_equals_found)
						{
							strncpy(&ini_file[i_line][i_char], str_value, INI_MAX_LINE_LENGTH-i_char);
							// set modified flag only if we really modified something
							b_ini_modified = 1;
							// we did the job
							return;
						}
					}
				}
			}
		}
		else
		{
		}

		++i_line;
	}
}

int is_ini_valid()
{
	return pf_ini_file != NULL;
}
