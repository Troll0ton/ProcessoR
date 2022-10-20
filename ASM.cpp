#include "stack.h"

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

void assembler (char *buffer, double *code)  //code 1 10 20 3 4 2
{
    struct File *File_input = file_reader (file_in);    //Читаем исходный текст пользователя
    struct Line *Text = lines_separator   (File_input);

    while (1)
    {                                                   //Переводим в код команды и записываем это в массив - файл?
        char cmd[100] = "";                             //В начало этого добавляем сигнатуру, которая покажет успешность ассемблирования

        sscanf (text[line], "%s", cmd);

        if(stricmp(cmd, "push") == 0)
        {
            double val = 0;
            sscanf (text[line] + 4?, "%s", &val);
            fprintf (code, "%d %d", CMD_PUSH, val);
        }

        else if(stricmp(cmd, "halt") == 0)
        {
            fprintf (code, "%d", CMD_HLT);
        }

        else if(stricmp(cmd, "add") == 0)
        {
            fprintf (code, "%d", CMD_ADD);
        }

        else if(stricmp(cmd, "mul") == 0)
        {
            fprintf (code, "%d", CMD_MUL);
        }

        else if(stricmp(cmd, "div") == 0)
        {
            fprintf (code, "%d", CMD_DIV);
        }

        else if(stricmp(cmd, "out") == 0)
        {
            fprintf (code, "%d", CMD_OUT);
        }
    }

}

//-----------------------------------------------------------------------------


