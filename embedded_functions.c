#include "embedded_functions.h"
#include "garbage_collector.h"
#include "error.h"
#include "scanner.h"

char * readString()
{
    char * tmp = NULL;
    char * string = (char *)mem_alloc(sizeof(char));
    if (string==NULL)
    {
        error_msg(99, "ERROR: Memory could not be allocated. (func. readstring)\n");	// code 99 -> internal error
        return NULL;
    }

    int size = 1;
    int index = 0;
    int c = getc(stdin);

    while (c != EOF && c != '\n')
    {
        string[index]=c;
        index += 1;

        if (index == size)
	{
            size *= 2;
            tmp = (char *)mem_realloc(string, size);
            if (tmp==NULL)
            {
                //free(string);
		        error_msg(99, "ERROR: Memory could not be allocated. (func. readstring)\n");	// code 99 -> internal error
		        return NULL;
            }
            else
                string = tmp;
        }

        c = getc(stdin);
    }

    string[size-1]='\0';
    return string;
}


static int Int_convertor(char *str, int id) {
    if (id == TYPE_INT_HEX || id == TYPE_INT_BIN || id == TYPE_INT_OCTAL || id == TYPE_DOUBLE_HEX) {
        repair_num(str);
    }
    if (id == TYPE_INT_BIN) {
        int num = 0;
        bin2dec(str, &num);
        return num;
    }
    if (id == TYPE_INT_OCTAL) {
        int num = 0;
        octal2dec(str, &num);
        return num;
    }
    if (id == TYPE_INT_HEX) {
        int num = 0;
        hex2dec_int(str, &num);
        return num;
    }
    if (id == TYPE_INT) {
        int num = 0;
        if (sscanf(str, "%i", &num) == EOF) {
            return -1;
        }
        return num;
    }
    return -1;
}
int readInt()
{
    char * string;
    int num;

    string = readString();
	if (string == NULL)
	{
		error_msg(7, "ERROR: Number value could not be read from stdin. (func. readdouble)\n");
		return -1;
	}

    int id = is_num_literal(string, strlen(string));
    if (id == 0 || id == TYPE_DOUBLE || id == TYPE_DOUBLE_HEX) {
        fprintf(stderr, "ERROR: invalid value of number\n");
        return -1;
    }

	num = Int_convertor(string, id);
    if (num < 0) {
        fprintf(stderr, "ERROR: invalid value of number\n");
        return -1;
    }
    return num;

}


static double Double_convertor(char *str, int id) {
    if (id == TYPE_INT_HEX || id == TYPE_INT_BIN || id == TYPE_INT_OCTAL || id == TYPE_DOUBLE_HEX) {
        repair_num(str);
    }
    if (id == TYPE_INT_BIN) {
        int num = 0;
        bin2dec(str, &num);
        return num;
    }
    if (id == TYPE_INT_OCTAL) {
        int num = 0;
        octal2dec(str, &num);
        return num;
    }
    if (id == TYPE_INT_HEX) {
        int num = 0;
        hex2dec_int(str, &num);
        return num;
    }
    if (id == TYPE_INT) {
        int num = 0;
        if (sscanf(str, "%i", &num) == EOF) {
            return -1;
        }
        return num;
    }
    if (id == TYPE_DOUBLE) {
        double num = 0.0;
        if (sscanf(str, "%lf", &num) == EOF) {
            return -1;
        }
        return num;
    }
    if (id == TYPE_DOUBLE_HEX) {
        double num = 0.0;
        hex2dec_double(str, &num);
        return num;
    }
    return -1;
}

double readDouble()
{
    char * string;
    double num;

    string = readString();
	if (string == NULL)
	{
		error_msg(7, "ERROR: Number value could not be read from stdin. (func. readdouble)\n");
		return -1;
	}

    int id = is_num_literal(string, strlen(string));
    if (id == 0) {
        fprintf(stderr, "ERROR: invalid value of number\n");
        return -1;
    }

	num = Double_convertor(string, id);
    if (num < 0) {
        fprintf(stderr, "ERROR: invalid value of number\n");
        return -1;
    }
    return num;
}

void print(char * string)
{
	printf("%s", string);
}
