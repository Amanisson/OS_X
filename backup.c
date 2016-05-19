//
//  main.c
//  sigidu
//
//  Created by User on 17.05.16.
//  Copyright (c) 2016 User. All rights reserved.
//

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <zlib.h>
#include <stdlib.h>


#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 4194304
#endif

void Copy_File(char Trace_File[]);
void Get_Time_DMY (char dmy_time[]);
void Get_Path_File (char Path_File[],char Trace_Backup_File[]);
void Get_Path_Folder (char Trace_Backup_Folder[]);
int  Create_Conteiner (char *infilename, char *outfilename);
unsigned long file_size(char *filename);


void Copy_File(char Path_File[])
{
    char *F[2];
    char Path_Backup_Folder [256] ="";
    char Path_Backup_File [256] = "";
    
    Get_Path_File(Path_File, Path_Backup_File);
    Get_Path_Folder(Path_Backup_Folder);
    
    mkdir(Path_Backup_Folder,0755);
    F[0] = Path_File;
    F[1] = Path_Backup_File;
    
    int inputFd;
    int outputFd;
    int openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    
    /* open input and outpu files */
    inputFd = open(F[0], O_RDONLY);
    if (inputFd == -1)
        printf("opening file %s", F[0]);
    
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /*rw-rw-rw*/
    
    outputFd = open(F[1], openFlags, filePerms);
    
    if (outputFd == -1)
        printf("opening file %s", F[1]);
    
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        if (write(outputFd, buf, numRead) != numRead)
            printf("couldn't write whole buffer");
    }
    
    if (numRead == -1)
        printf("read");
    
    if (close(inputFd) == -1)
        printf("close input");
    if (close(outputFd) == -1)
        printf("close output");
}

int main()
{
    
    char *F[4];
    F[0] =  "/Users/user/Desktop/File/bear.jpg";
    F[1] =  "/Users/user/Desktop/File/red.txt";
    F[2] =  "/Users/user/Desktop/File/green.jpg";
    F[3] =  "/Users/user/Desktop/File/blue.txt";
    for(int i = 0; i < 4;i++)
    {
        Copy_File(F[i]);
    }
    
    
    time_t t = time(NULL);
    struct tm* aTm = localtime(&t);
    
    //запись логов в файл output.txt
    FILE *file;
    file=fopen("/Users/user/Desktop/output.txt", "w");
    if (file == NULL){
        printf("ERROR!!!");
        exit(1);
    }
    fprintf(file, "[ . . . . . . . . . . . . . . . . . . . . . . . . . .] \n");
    fprintf(file, "     Proramm is work very well ---> [ backup of file ] \n");
    fprintf(file, "     Created by team #4 \n");
    fprintf(file,"      %04d/%02d/%02d %02d:%02d:%02d \n",aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
    fprintf(file, "[ . . . . . . . . . . . . . . . . . . . . . . . . . .]");
    fclose(file);
    
}



void Get_Time_DMY (char dmy_time[])
{
    //Переменная для системного времени
    long int s_time;
    //Указатель на структуру с локальным временем
    struct tm *m_time;
    //Строка для сохранения преобразованного времени
    
    //Считываем системное время
    s_time = time (NULL);
    //Преобразуем системное время в локальное
    m_time = localtime (&s_time);
    
    //Преобразуем локальное время в текстовую строку
    strftime (dmy_time, 128, "_%d_%m_%Y", m_time);
}


void Get_Path_File (char Path_File[],char Trace_Backup_File[])
{
    char str_t[256]; // для строки времени
    Get_Time_DMY(str_t);//заполняем строку
    /* Ищем колличество слешей*/
    int N_sl = 0,i = 0,j=0;
    for( int i = 0;Path_File[i] != '\0';i++)
    {
        if(Path_File[i] == '/')
            N_sl++;
    }
    
    /*Определение путь к файлу и его имя*/
    char Pre_Name_File[150] ="";
    char Post_Name_File[150] = "";
    while (Path_File[i] != '\0')
    {
        if (N_sl > 0) Pre_Name_File[i] = Path_File[i];
        if (N_sl == 0)
        {
            Post_Name_File[j] = Path_File[i];
            j++;
        }
        if ( Path_File[i] == '/') N_sl--;
        i++;
    }
    
    strcat(Trace_Backup_File,"/Users/user/Desktop/Backup1");
    strcat(Trace_Backup_File, str_t);
    strcat(Trace_Backup_File, "/");
    strcat(Trace_Backup_File,Post_Name_File);
}

void Get_Path_Folder (char Trace_Backup_Folder[])
{
    char str_t[256];
    Get_Time_DMY(str_t);
    strcat(Trace_Backup_Folder,"/Users/user/Desktop/Backup1");
    strcat(Trace_Backup_Folder, str_t);
    strcat(Trace_Backup_Folder, "/");
    
}


unsigned long file_size(char *filename)
{
    FILE *pFile = fopen(filename, "rb");
    fseek (pFile, 0, SEEK_END);
    unsigned long size = ftell(pFile);
    fclose (pFile);
    return size;
}