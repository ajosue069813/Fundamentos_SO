#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define pid_length 4  // Consideramos que cada pid tiene un tamaño de 4
#define TAM_CONT 100

void getstr(char *str)		// Evaluación y busqueda de comandos introducidos
{
	fgets(str,80,stdin);   
	while(*str!='\n' && *str!='\r')
		str++;
	*str='\0';
}

int main()
{   int pid;
	char cmd[80];
	int continua=1;
	int longitud,lo;
	char array[pid_length];		// Se usa para guardar un PID leido del archivo PIDs_GETTY 
	char array_ppid[pid_length];	// Se usa para guardar un PID a terminar 
	FILE *pass;
	FILE *p;

	while(continua)
	{   
		printf("sh>");
		getstr(cmd);
		
		if(strcmp(cmd,"exit")==0)	
		    {
			continua=0;			// Termina sh
			}
		else if(strcmp(cmd,"shutdown")==0) 
		    {			
			printf("asesino\n");		// Termina todos los procesos GETTY creados

            pass=fopen("PIDs_GETTY","r");	// Lectura de PIDs_GETTY
    		char contenido[TAM_CONT];
    		longitud=fread(contenido,1,TAM_CONT,pass);
            fclose(pass);

			if(longitud>=6*pid_length)
	           { 
				   for(int i=0;i<6;i++)
				   { 
                     for(int idx=0;idx<pid_length;idx++)		// Se almacenan un array los caracteres de un PID
					 {
						 array[idx]=contenido[(i*pid_length)+idx];
					 }

					 int pid_kill=atoi(array);		

					 	if(pid_kill!=getppid())		// Nos aseguramos de que no se mate a si mismo
					 		kill(pid_kill,9);			// Se termina un proceso getty
				   }
			   }

			   // Para este punto todos los procesos getty ya fueron extermindos

            p=fopen("PID_INIT","r");		// Lectura del PID ultra padre
    		char cont[TAM_CONT];
    		lo=fread(cont,1,TAM_CONT,p);
            fclose(p);

			for(int i=0;i<lo;i++)		// Almacenaje en array del PID ultra padre
				{
				  array_ppid[i]=cont[i];
				}

				int ppid_kill=atoi(array_ppid);
				
				kill(ppid_kill,9);		// Terminacion de ultra padre
				kill(getppid(),9);		// Se termina a si mismo
			
			continua=0;
			}
		
		else
		{
			pid=fork();		// Ejecución del comando introducido por el usuario
			
			if(pid==0){
        	execlp(cmd,cmd,NULL);
			//system(cmd); 	// No lo podemos usar
			}
		
			wait(NULL);		// Espera a que el comando del usuario termine	
		}			  
	}
}
