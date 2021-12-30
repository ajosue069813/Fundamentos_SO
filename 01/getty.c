#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define TAM_CONT 1500   // Cantidad máxima de caracteres leidos por fread


int main()
{
    char contra[6][16];
    char usuarios[6][16]={};
    char usr[10];
    char clave[10];
    char shell[10];
    int  longitud;
    int users_number = 0;
    int  flag_usuario=1;
    FILE *pass;
    FILE *f_sh;
    
    pass=fopen("passwd","r");   // Lectura del archivo de contraseñas
    char contenido[TAM_CONT];
    longitud=fread(contenido,1,TAM_CONT,pass);
    fclose(pass);

    f_sh=fopen("PIDs_GETTY","a");   // Concatenación del PID de la terminal actual en el archivo PIDs_GETTY
    fprintf(f_sh,"%d",getppid());
    fclose(f_sh);
    
    printf("\nBienvenido\n\n");
    int i=0;  // Iterador para renglones de la lectura archivo de contraseñas
    int k=0;  // Iterador para cada caracter dentro del renglon de un arreglo
    
    for(int j=0;j<longitud;j++)
    {  
        if('\n'==contenido[j])  // Santa de renglón y cambia a leer usuarios
        {
          contra[i][k-1]=0;
          i++;  
          flag_usuario=1;
          k=0;
        }
        else 
        {
          if(':'==contenido[j])   // Cambia a leer contraseñas
          {
              flag_usuario=0;
              usuarios[i][k]=0;
              k=0;
              users_number += 1;
          }
          else
          {
            if(flag_usuario)    // Almacenaje de usuario y contraseña en arrays
              usuarios[i][k]=contenido[j];
              else
                contra[i][k-1]=contenido[j];

          }

          k++;
        }     
    }

    while(1){
    int valid_user=0;
    int valid_contra=0;

    printf("INGRESE USUARIO:\n");
    scanf("%s",usr);
    printf("INGRESE CONTRASEÑA\n");
    scanf("%s",clave);

    for(int idx=0;idx<users_number;idx++)
    {
    //printf("%s\n",usuarios[idx]);

    if(strcmp(usr,usuarios[idx])==0)    // Validacion de usuario
      {
          valid_user=1;
          if(strcmp(clave,contra[idx])==0)    // Validacion de contraseña
          {
          printf("\nAcesso correcto :)\n\n");
          valid_contra=1;
          break;
          }
      }
    }

    if (valid_user==0 || valid_contra==0)
      {
       printf("\nUsuario o contraseña incorrectos\n\n");
       continue;
      }

    // Levantamiento de sh en terminal
    int pid;    
    pid=fork();
    
    if (pid==0)
    {
      execlp("./sh","sh",NULL);
    }
    // Para este punto espera a que finalice sh al introducir "exit"
    wait(NULL);
    // Para este punto se terminó sh
    printf("\nSesión terminada\n\n");
    }

    return 0;  
}

