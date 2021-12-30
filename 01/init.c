#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define TAM_CONT 100
#define pid_length 4	// Se considera que los PID tienen 4 cifras

void sig_handler(int sig_kill);

int arraypid[6];

int main()
{	
	int pid;
	int i;
	int pid_padre;
	FILE *f_sh;
	
	signal(SIGUSR1,sig_handler);

	// Borra el contenido del archivo PIDs_GETTY
	f_sh=fopen("PIDs_GETTY","w+");
	fclose(f_sh);
	
	// Poner PID ultra padre en un archivo
	f_sh=fopen("PID_INIT","w");
	fprintf(f_sh,"%d",getpid());
	fclose(f_sh);
    
    //printf("\nPID_INIT (PROCESO PADRE): %d\n",getpid());
	
	for(i=0;i<6;i++)
	{
		pid = fork();		
		if(pid == 0)
		{
			printf("\nPID del proceso nuevo %d, %d\n",i,getpid());
			execlp("xterm","xterm","-e","./getty",NULL);
		}	
	}

	while(1)
	{
		int killed_term;	// Aqui se almacena el PID de la terminal que se ha cerrado
        int killed_start=0;

        killed_term = wait(NULL);
        // Para este punto se cerró una consola

        f_sh=fopen("PIDs_GETTY","r");
		char contenido[TAM_CONT];
		char array[pid_length+1];
		int longitud;
		int pid_kill[6];	// Aqui se almacenan los datos del archivo leido (PIDs_GETTY)

	    array[pid_length]=0;     // Es necesario para que el atoi identifique el fin de la cadena
		
		longitud=fread(contenido,1,pid_length*6,f_sh);
        fclose(f_sh);

        for(int i=0;i<6;i++)	// Ciclo para llenar los PID leidos como int
	   { 
         	for(int idx=0;idx<pid_length;idx++)
			 {
				 array[idx]=contenido[(i*pid_length)+idx];
			 }

			pid_kill[i] = atoi(array);

			//printf("\n%d\n", pid_kill[i]);

			if(pid_kill[i]==killed_term){	// Verifica si un PID leido es el que fue terminado
				killed_start = i;
			}	 	
		}

		pid = fork();	// Se crea un nuevo proceso (Terminal) con un nuevo PID

		if(pid==0){
			//printf("\nPID del proceso nuevo: %d\n",getpid());
			execlp("xterm","xterm","-e","./getty",NULL);
		}else{
			f_sh=fopen("PIDs_GETTY","w+"); // Borra el contenido del archivo PIDs_GETTY
    		//fprintf(f_sh,"",NULL);
			fclose(f_sh);
			
			f_sh=fopen("PIDs_GETTY","a");

			for(int i=0; i<6; i++){		// Escritura de los PID nuevos en PIDs_GETTY
				if(pid_kill[i] == killed_term)
					fprintf(f_sh,"%d", pid);
				else
					fprintf(f_sh,"%d", pid_kill[i]);
			}

			fclose(f_sh);
		}
	}
	
	printf("Fin");
	return 0;	
}

void sig_handler(int sig_kill)
{   
	for(int i=0;i<6;i++)
	{
		kill(arraypid[i],9);
	}
	
	kill(0,9);
}
