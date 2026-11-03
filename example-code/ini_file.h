#if !defined(_INI_FILE_H)
#define _INI_FILE_H_

#define INI_MAX_LINE_LENGTH	128	// bytes/characters
#define INI_MAX_LINE_COUNT	64

void ini_initialize(char* str_name);
void ini_read();
void ini_write();
int is_ini_valid();

char* ini_get_value(const char* str_key);
void ini_set_value(const char* str_key, const char* str_value);

#endif // _INI_FILE_H
