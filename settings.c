#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ENV_VAR "SETTINGS_PATH"

long file_size(FILE* file)
{
	if (file == NULL) 
		return -1;

	long pos = ftell(file);

	if (fseek(file, 0, SEEK_END) == -1) 
		return -1;

	long size = ftell(file);

	if (size == -1) 
		return -1;

	if (fseek(file, pos, SEEK_SET) == -1) 
		return -1;

	return size;
}

char* getpath(char const *folder, char const *file)
{
	int len = strlen(folder) + strlen(file) + 1;
	char *path = (char *) malloc((len + 1) * sizeof(char));
	strcpy(path, folder);

	if (path[strlen(path)-1] != '/')
	{
		strcat(path, "/");
	}

	strcat(path, file);

	return path;
}

char* readfile(char const *setting)
{
	FILE *file = fopen(setting, "r");
	long size = file_size(file);

	if (size == -1)
		return 0;

	char *buffer = (char *) malloc((size + 1) * sizeof(char));
	fgets(buffer, size + 1, file);
	fclose(file);

	return buffer;
}

void writefile(char const *setting, char const *value)
{
	FILE *file = fopen(setting, "w");
	fputs(value, file);
	fclose(file);
}

void help()
{
	printf("Usage:\n");
	printf("  settings get <name>\n");
	printf("  settings set <name> <value>\n");
	printf("  settings toggle <name> [<value>...]\n\n");

	printf("Options:\n");
	printf("  -s, --session     Use temporary storage\n\n");

	printf("File-based system settings\n");
}

int main(int argc, char const *argv[])
{
	if (argc == 1 || strcmp(argv[1], "-h") == 0 || 
		strcmp(argv[1], "--help") == 0)
	{
		help();
		return 0;
	}

	char* PATH = getenv(ENV_VAR);

	if (PATH == NULL || strlen(PATH) == 0)
	{
		printf("[!] Environment variable '");
		printf(ENV_VAR);
		printf("' is not set.\n");
		return 1;
	}

	if (strcmp(argv[1], "get") == 0)
	{
		if (argc != 3)
		{
			help();
			return 1;
		}

		char *path = getpath(PATH, argv[2]);
		printf(readfile(path));
	}
	else if (strcmp(argv[1], "set") == 0)
	{
		if (argc != 4)
		{
			help();
			return 1;
		}

		char *path = getpath(PATH, argv[2]);
		writefile(path, argv[3]);
	}
	else if (strcmp(argv[1], "toggle") == 0)
	{
		if (argc < 3)
		{
			help();
			return 1;	
		}

		char *path = getpath(PATH, argv[2]);
		char *value = readfile(path);

		if (argc == 3)
		{
			if (strcmp(value, "1") == 0)
			{
				writefile(path, "0");
				printf("0");
			}

			else if(strcmp(value, "0") == 0)
			{
				writefile(path, "1");
				printf("1");
			}
		}
		else
		{
			int offset = 3;
			int pos = 0;

			for (int i = pos; i < argc - offset; i++)
			{
				if (strcmp(argv[offset + i], value) == 0)
				{
					pos = i;
					break;
				}
			}

			pos = offset + (pos + 1) % (argc - offset);
			writefile(path, argv[pos]);
			printf(argv[pos]);
		}
	}

	return 0;
}