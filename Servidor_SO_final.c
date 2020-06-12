#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>
//-std=c99 `mysql_config --cflags --libs'
int contador=0;
typedef struct
{
	char jugador [10];
	int  socket;
} User;

typedef struct
{
	User usuarios [100];
	int num;
} ListaConectados;

ListaConectados lista_Conectados;
typedef struct
{
	char trozo [20];
} Mensaje;

typedef struct
{
	Mensaje mensajes[100];
	int num;
} ListaMensajes;

ListaMensajes   lista_mensajes;
pthread_mutex_t mutex;

int Desregistrarse(char username_desregistrarse [10])
{
	int err;
	char consulta [80];
	MYSQL *conn;
	
	conn = mysql_init(NULL);
	if (conn==NULL) 
	{
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		return 1;
	}
	
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD", 0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);
	if (conn==NULL) 
	{
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		return 1;
	}
	
	sprintf(consulta, "DELETE FROM Jugadores WHERE Jugadores.Nombre = '%s'", username_desregistrarse );
	printf("%s\n", consulta);
	
	err = mysql_query(conn, consulta);
	if (err!=0) 
	{
		printf ("Error al introducir datos la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		return 1;
	}
	// cerrar la conexion con el servidor MYSQL 
	mysql_close (conn);
	return 0;
}







int Registrarse (char username [10], char password [10], int IdentificadorJ)
{   
	int err;
	char consulta [80];
	MYSQL *conn;
	
	conn = mysql_init(NULL);
	if (conn==NULL) 
	{
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		return 1;
	}
	
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD", 0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);
	if (conn==NULL) 
	{
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		return 1;
	}
	
	sprintf(consulta, "INSERT INTO Jugadores VALUES (NULL,'%s','%s');", username, password);
	printf("%s\n", consulta);
	//INSERT INTO jugador VALUES ('nombre', 'contraseÃ±a');
	/*	strcpy (consulta, "INSERT INTO Jugadores VALUES (");*/
	/*	strcat (consulta, IdentificadorJ);*/
	/*	strcat (consulta, ",'");*/
	/*	strcat (consulta, username); */
	/*	strcat (consulta, "','");*/
	/*	strcat (consulta, password); */
	/*	strcat (consulta, "');");*/
	
	err = mysql_query(conn, consulta);
	if (err!=0) 
	{
		printf ("Error al introducir datos la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		return 1;
	}
	// cerrar la conexion con el servidor MYSQL 
	mysql_close (conn);
	return 0;
}

int login (char nombre [15], char password [25])
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [150];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD",0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);	
	if (conn==NULL) {
		printf ("Error al inicializar la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	// consulta SQL para saber si existe el usuario
	// de la base de datos
	strcpy (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='");
	strcat(consulta,nombre);
	strcat(consulta, "' AND Jugadores.password='");
	strcat (consulta, password);
	strcat (consulta, "';");
	// (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='nombre' AND Jugadores.password='%s'", nombre, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	if (row == NULL)
	{
		//No ay ningun jugador con ese nombre y contraseña
		return (2);
	}
	else
	{
		return (0);
	}
	
	
}
int consulta_usuario_existente (char nombre [15])
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [150];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//inicializar la conexion
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD",0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);	
	if (conn==NULL) {
		printf ("Error al inicializar la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	// consulta SQL para saber si existe el usuario
	// de la base de datos
	strcpy (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='");
	strcat(consulta,nombre);
	strcat (consulta, "';");
	// (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='nombre' AND Jugadores.password='%s'", nombre, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	if (row == NULL)
	{
		//No ay ningun jugador con ese nombre y contraseña
		return (2);
	}
	else
	{
		return (0);
	}
	
	
}




int consulta_numero_regitrados()
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//inicializar la conexion
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD",0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	// consulta SQL para saber si existe el usuario
	// de la base de datos
	strcpy (consulta, "SELECT * FROM Jugadores;");
	// (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='nombre' AND Jugadores.password='%s'", nombre, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	int contador_jugadores = mysql_num_rows(resultado);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	if (row == NULL)
	{
		//No ay ningun jugador con ese nombre y contraseña
		return (0);
	}
	else
	{
		return (contador_jugadores);
	}
	
	
}


int consulta_partidas_ganadas_jugador (char nombre [15])
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [150];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//inicializar la conexion
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD",0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	// consulta SQL para saber si existe el usuario
	// de la base de datos
	/*	strcpy (consulta, "SELECT * FROM Partidas WHERE Partida.Ganador =");*/
	/*	strcat(consulta,comillas);*/
	/*	strcat(consulta, nombre);*/
	/*	strcat(consulta,comillas);*/
	/*	strcat(consulta,";");*/
	sprintf (consulta, "SELECT * FROM Partida WHERE Partida.Ganador ='%s';", nombre);
	
	// (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='nombre' AND Jugadores.password='%s'", nombre, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	int partidas_ganadas = mysql_num_rows(resultado);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	if (row == NULL)
	{
		//No ay ningun jugador con ese nombre y contraseña
		return (0);
	}
	else
	{
		return (partidas_ganadas);
	}
	
	
}


int consulta_partidas_jugadas_entre_dos (char nombre1 [15], char nombre2 [15])
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [350];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//inicializar la conexion
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD",0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);	
	if (conn==NULL) {
		printf ("Error al inicializar la conexiï¿ƒï¾³n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	
	sprintf (consulta, "SELECT IdP FROM Partida, Jugadores, Relacion WHERE  Jugadores.Nombre = '%s' AND Jugadores.IdentificadorJ = Relacion.IdJ AND Relacion.IdP = (SELECT IdP FROM Jugadores, Partida, Relacion WHERE Jugadores.Nombre = '%s' AND Jugadores.IdentificadorJ = Relacion.IdJ)", nombre1,nombre2);
	
	
	// (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='nombre' AND Jugadores.password='%s'", nombre, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	int partidas_jugadas = mysql_num_rows(resultado);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	if (row == NULL)
	{
		//No ay ningun jugador con ese nombre y contraseña
		return (0);
	}
	else
	{
		return (partidas_jugadas);
	}
	
	
}


int consulta_anadir_partida_bbdd()
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//inicializar la conexion
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD",0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	// consulta SQL para saber si existe el usuario
	// de la base de datos
	strcpy (consulta, "INSERT into Partida (Ganador) values ('a');"); //valores de prueba que serán machacados
	// (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='nombre' AND Jugadores.password='%s'", nombre, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	strcpy(consulta, "SELECT MAX(IdentificadorP) from Partida");
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	int idpartida = atoi(row[0]);
	return(idpartida);
}	
int abrir_base_datos ()
{
	
	//Conector para acceder al servidor de bases de datos 
	MYSQL *conn;
	int err;
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		
		return (1);
	}
	
	//inicializar la conexion, indicando nuestras claves de acceso
	// al servidor de bases de datos (user,pass)
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", NULL, 0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", NULL,0, NULL, 0);
	if (conn==NULL)
	{
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (1);
	}
	
	// ahora vamos a crear la base de datos, que se llamara mydatabase
	// primero la borramos si es que ya existe (quizas porque hemos
	// hecho pruebas anteriormente
	/*	mysql_query(conn, "drop database if exists mydatabase;"); */
	/*	err=mysql_query(conn, "create database mydatabase;");*/
	/*	if (err!=0) {*/
	/*		printf ("Error al crear la base de datos %u %s\n", */
	/*				mysql_errno(conn), mysql_error(conn));*/
	/*		return (1);*/
	//}
	//indicamos la base de datos con la que queremos trabajar 
	err=mysql_query(conn, "use mydatabase;");
	if (err!=0)
	{
		err=mysql_query(conn, "create database mydatabase;");
		if (err!=0) 
		{
			printf ("Error al crear la base de datos %u %s\n", 
					mysql_errno(conn), mysql_error(conn));
			return (1);
			
			
		}
		err=mysql_query(conn, "use mydatabase;");
		
		// creamos la tabla que define la entidad persona: 
		// 	un DNI (clave principal), nombre y edad 
		err=mysql_query(conn,
						"CREATE TABLE Jugadores (IdentificadorJ integer not null primary key AUTO_INCREMENT, Nombre VARCHAR(15) not null , Password VARCHAR(25) not null)");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			return (1);
		}
		err=mysql_query(conn,
						"CREATE TABLE Partida (IdentificadorP integer primary key AUTO_INCREMENT, Ganador VARCHAR(25))");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		err=mysql_query(conn,
						"CREATE TABLE Relacion (IdJ integer, IdP integer, FOREIGN KEY (IdJ) REFERENCES Jugadores(IdentificadorJ), FOREIGN KEY (IdP) REFERENCES Partida(IdentificadorP))");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			return (1);
		}
		err=mysql_query(conn,"insert into Jugadores (IdentificadorJ, Nombre, Password) values (NULL,'Juan','eetac123');");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			return (1);
		}
		
		err=mysql_query(conn,"insert into Jugadores (IdentificadorJ, Nombre, Password) values (NULL,'Antonio','eetac123');");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			return (1);
		}		
		
		err=mysql_query(conn,"insert into Partida (IdentificadorP, Ganador) values (NULL,'Juan');");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			return (1);
		}	
		
		err=mysql_query(conn,"insert into Relacion (IdJ, IdP) values (1, 1);");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			return (1);
		}	
		
		err=mysql_query(conn,"insert into Relacion (IdJ, IdP) values (2, 1);");
		
		if (err!=0)
		{
			printf ("Error al definir la tabla %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			return (1);
		}	
	}
	
	
	
	// ahora tenemos la base de datos lista en el servidor de MySQL
	// cerrar la conexion con el servidor MYSQL 
	mysql_close (conn);
	
}

int inserta_ganador_partida(int id, char ganador[25])
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	//inicializar la conexion
	//conn = mysql_real_connect (conn, "shiva2.upc.es","root", "mysql", "TG11_BBDD",0, NULL, 0);
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "mydatabase",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	// consulta SQL para saber si existe el usuario
	// de la base de datos
	sprintf (consulta, "UPDATE Partida SET Ganador='%s' WHERE IdentificadorP = '%d';",ganador,id); //valores de prueba que serán machacados
	//strcpy (consulta, "INSERT into Partida (Ganador) values ('a');");
	// (consulta, "SELECT nombre FROM Jugadores WHERE Jugadores.nombre='nombre' AND Jugadores.password='%s'", nombre, password);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		return (-1);
	}
	
	
	
}
void *atender_cliente (void *socket)
{
	char buff[512];
	char buff2[512];
	int ret;
	int *b;
	b=(int *) socket;
	int sock_conn=*b;
	int codigo=0;
	int i=0;
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	conn=mysql_init (NULL);
	if (conn == NULL)
	{
		printf ("Error al crear la conexion: %u %s \n",mysql_errno(conn), mysql_error(conn));
	}
	
	printf ("Escuchando peticiones\n");
	
	int j = 0;
	char username[10];
	
	// Atenderemos todas las peticiones del cliente
	int fin = 0;
	while (fin==0)
	{
		lista_mensajes.num = 0;	
		
		// Ahora recibimos su nombre, que dejamos en buff
		ret=read(sock_conn,buff, sizeof(buff));
		printf ("Recibido\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		buff[ret]='\0';
		
		//Escribimos el nombre en la consola
		
		printf ("Se ha conectado: %s\n",buff);
		i =1;
		
		
		char *mensaje = strtok( buff, "/");
		int codigo =  atoi (mensaje);
		int i = 0;
		while( (mensaje = strtok( NULL, "/" )) != NULL )
		{   
			strcpy (lista_mensajes.mensajes[i].trozo, mensaje);         //Mensaje troceado
			lista_mensajes.num++;
			//printf ("Trozo[%d]: %s\n", i, lista_mensajes.mensajes[i].trozo);
			i = i + 1;
		}
		
		
		if(codigo == 20) //Desregistrarse
		{
			char username_desregistrarse[10];
			strcpy (username_desregistrarse, lista_mensajes.mensajes[0].trozo);
			int d =consulta_usuario_existente(username_desregistrarse);
			if (d == 2) 
			{
				strcpy(buff2,"-20");
				printf("error al desregistrarse");
			}
			else
			{
				int de = Desregistrarse(username_desregistrarse);
				if (de == 0)
				{
					strcpy(buff2,"20");
					printf("Se ha podido desregistrarse");
				}
			}
			write (sock_conn, buff2, strlen(buff2));
		}
		
		
		if (codigo ==1) //Registrarse
		{
			
			char username_registro[10];
			char password_registro[10];
			strcpy (username_registro, lista_mensajes.mensajes[0].trozo);
			strcpy (password_registro, lista_mensajes.mensajes[1].trozo);
			int g=consulta_usuario_existente(username_registro);
			if(g==2)
			{
				int e=Registrarse(username_registro, password_registro, contador);
				contador++;
				strcpy(buff2,"1");
				
			}
			else
			{
				strcpy(buff2,"-1");
				printf("error registro");
			}	
			write (sock_conn, buff2, strlen(buff2));
			
		}
		
		
		if(codigo==2) //login
		{
			char username_login[10];
			char password_login[10];
			strcpy (username_login, lista_mensajes.mensajes[0].trozo);
			strcpy (password_login, lista_mensajes.mensajes[1].trozo);
			int e= login(username_login, password_login);
			if (e == 0)
			{
				//Busca que el usuario ya esta conectado en otro terminal
				j = 0;
				int encontrado = 0;
				
				while ((j < lista_Conectados.num + 1)&&(encontrado == 0))
				{
					if (strcmp(lista_Conectados.usuarios[j].jugador, username_login) == 0)
					{
						encontrado = 1;
					}
					else { j = j + 1;}
				}
				
				//Ha encontrado un duplicado
				if (encontrado == 1)
				{
					strcpy (buff2,"-2");
					
					printf("Respuesta: %s\n", buff2);
					write (sock_conn, buff2, strlen(buff2));
				}
				
				else
				{   //Logeo correcto
					strcpy (buff2,"2");
					
					pthread_mutex_lock(&mutex);
					strcpy (lista_Conectados.usuarios[lista_Conectados.num].jugador, username_login);
					lista_Conectados.usuarios[lista_Conectados.num].socket = sock_conn;
					pthread_mutex_unlock(&mutex);
					
					printf ("Usuarios Conectados: %d\n", lista_Conectados.num + 1);
					j = 0;                      
					while ( j < lista_Conectados.num + 1)   //Muestra la Lista de Conectados
					{
						printf ("  %d  %s\n", lista_Conectados.usuarios[j].socket, lista_Conectados.usuarios[j].jugador);
						j = j + 1;
					}
					lista_Conectados.num = lista_Conectados.num + 1;
					
					printf("Respuesta: %s\n", buff2);
					write (sock_conn, buff2, strlen(buff2));
					
					int j = 0;  
					strcpy(buff2, "0/");
					char numero[10];
					sprintf(numero, "%d", lista_Conectados.num);
					strcat(buff2, numero);
					
					while ( j < lista_Conectados.num)
					{
						strcat(buff2, "/");
						strcat (buff2, lista_Conectados.usuarios[j].jugador);
						j = j + 1;
					}
					
					printf("Lista: %s\n", buff2);
					
					j=0;
					while ( j < lista_Conectados.num + 1 )
					{ 
						write (lista_Conectados.usuarios[j].socket, buff2, strlen(buff2));
						j=j+1;
					}
				}
			}
			else
			{
				strcpy(buff2,"-2");
			}
			strcpy (username, username_login);
			write (sock_conn,buff2, strlen(buff2));
		}
		if(codigo==3) //Consulta Partidas ganadas por un jugador
		{
			char nombre[15];
			strcpy (nombre, lista_mensajes.mensajes[0].trozo);
			int e= consulta_partidas_ganadas_jugador(nombre);
			if (e>-1)
			{
				sprintf (buff2,"3/%d",e);
				
			}
			else
			{
				strcpy(buff2,"-3/1");
			}
			write (sock_conn,buff2, strlen(buff2));
		}
		if(codigo==4) //Consulta numero jugadores registrados
		{
			char nombre1[15];
			char nombre2[15];
			strcpy(nombre1, lista_mensajes.mensajes[0].trozo);
			strcpy(nombre2, lista_mensajes.mensajes[1].trozo);
			
			int e= consulta_partidas_jugadas_entre_dos (nombre1, nombre2);
			if (e>-1)
			{
				sprintf (buff2,"4/%d",e);
				
			}
			else
			{
				strcpy(buff2,"-4/");
			}
			write (sock_conn,buff2, strlen(buff2));
		}
		if(codigo==5) //Consulta numero jugadores registrados
		{
			int numero;
			int e= consulta_numero_regitrados ();
			if (e>-1)
			{
				sprintf (buff2,"5/%d",e);
				
			}
			else
			{
				strcpy(buff2,"-5/");
			}
			write (sock_conn,buff2, strlen(buff2));
		}
		if (codigo == 6)    //Invitacion de partida
		{
			char invitador[10];
			char invitado[10];
			char sock_invitador[5];
			char sock_invitado[5];
			
			strcpy (invitador, lista_mensajes.mensajes[0].trozo);   //Obtener el nombre del usuario que crea la partida
			int i = atoi(lista_mensajes.mensajes[1].trozo);         //Obtenemos la poscion de la lista del usario invitado
			
			sprintf(sock_invitador, "%d", sock_conn);
			sprintf(sock_invitado, "%d", lista_Conectados.usuarios[i].socket);
			strcpy(invitado, lista_Conectados.usuarios[i].jugador);
			
			strcpy(buff2, "6/"); 
			strcat(buff2, invitador);                   //invitador
			
			printf("Respuesta: %s\n", buff2);
			write (lista_Conectados.usuarios[i].socket, buff2, strlen(buff2));
		}
		if (codigo == -6)   //Respuesta de partida
		{
			char confirmacion[1];
			char invitador[10];
			int sock_invitador;
			
			strcpy (confirmacion, lista_mensajes.mensajes[0].trozo);
			strcpy (invitador, lista_mensajes.mensajes[1].trozo);
			
			int encontrado = 0;
			int i = 0;
			
			while ((i < lista_Conectados.num + 1)&&(encontrado == 0))
			{
				if(strcmp(invitador, lista_Conectados.usuarios[i].jugador) == 0)
				{
					sock_invitador = lista_Conectados.usuarios[i].socket;
					encontrado = 1;
				}
				
				i = i + 1;
			}
			
			//Concatenamos el mensaje
			strcpy(buff2, "-6/"); 
			strcat(buff2, confirmacion);    //Respuesta del invitado
			strcat(buff2, "/");
			strcat(buff2, username);        //Nombre del invitado
			
			printf("Respuesta: %s\n", buff2);
			write (sock_invitador, buff2, strlen(buff2));
		}
		if (codigo == 7)    //Chat
		{
			char mensajeador[10];
			char mensajeado[10];
			char mensaje[50];
			char sock_mensajeador[5];
			char sock_mensajeado[5];
			
			strcpy (mensajeador, lista_mensajes.mensajes[0].trozo);   //Obtener el nombre del usuario que crea la partida
			int i = atoi(lista_mensajes.mensajes[1].trozo);         //Obtenemos la poscion de la lista del usario mensajeado
			strcpy (mensaje,lista_mensajes.mensajes[2].trozo);
			
			sprintf(sock_mensajeador, "%d", sock_conn);
			sprintf(sock_mensajeado, "%d", lista_Conectados.usuarios[i].socket);
			strcpy(mensajeado, lista_Conectados.usuarios[i].jugador);
			
			strcpy(buff2, "7/"); 
			strcat(buff2, mensajeador);                   //Mensajeador
			strcat(buff2,"/");
			strcat(buff2, mensaje);
			
			printf("Respuesta: %s\n", buff2);
			write (lista_Conectados.usuarios[i].socket, buff2, strlen(buff2));
		}
		if (codigo == 0) //Desconectar
		{
			
			j = 0;
			int encontrado = 0;
			printf("Desconectar socket %d\n", sock_conn);
			
			while ((j < lista_Conectados.num + 1)&&(encontrado == 0))
			{
				if (strcmp(lista_Conectados.usuarios[j].jugador, username) == 0)
				{
					pthread_mutex_lock(&mutex);
					while (j < lista_Conectados.num + 1)
					{
						strcpy(lista_Conectados.usuarios[j].jugador, lista_Conectados.usuarios[j+1].jugador);
						lista_Conectados.usuarios[j].socket = lista_Conectados.usuarios[j+1].socket;
						j = j + 1;
					}
					encontrado = 1;
					lista_Conectados.num = lista_Conectados.num - 1;
					pthread_mutex_unlock(&mutex);
				}
				else { j = j + 1;}
			}
			
			printf ("Usuarios Conectados: %d\n", lista_Conectados.num);
			j = 0;                      
			while ( j < lista_Conectados.num)   //Muestra la Lista de Conectados
			{
				printf ("  %d  %s\n", lista_Conectados.usuarios[j].socket, lista_Conectados.usuarios[j].jugador);
				j = j + 1;
			}
			
			int j = 0;  
			strcpy(buff2, "0/");
			char numero[10];
			sprintf(numero, "%d", lista_Conectados.num);
			strcat(buff2, numero);
			while ( j < lista_Conectados.num)
			{
				strcat(buff2, "/");
				strcat(buff2, lista_Conectados.usuarios[j].jugador);
				j = j + 1;
			}
			
			printf("Lista: %s\n", buff2);
			
			j=0;
			while(j < lista_Conectados.num + 1)
			{
				write (lista_Conectados.usuarios[j].socket, buff2, strlen(buff2));
				j=j+1;
			}
			fin = 1;
			close(sock_conn);
		}
		if (codigo == 8)	//enviar lista de conectados
		{
			j=0;
			strcpy(buff2, "8/");
			char numero[10];
			sprintf(numero, "%d", lista_Conectados.num);
			strcat(buff2, numero);
			
			while ( j < lista_Conectados.num)
			{
				strcat(buff2, "/");
				strcat (buff2, lista_Conectados.usuarios[j].jugador);
				j = j + 1;
			}
			write (sock_conn,buff2, strlen(buff2));
		}
		
		if(codigo == 12) //Ganador
		{
			int i=inserta_ganador_partida(*lista_mensajes.mensajes[0].trozo,lista_mensajes.mensajes[1].trozo);
			
		}
		
	}
}
int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char buff[512];
	char buff2[512];
	int sockets [100];
	pthread_t thread [100];
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9070);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
	{
		printf ("Error al bind");
	}
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0)
	{
		printf("Error en el Listen");
	}
	int i;
	int e= abrir_base_datos	();
	
	pthread_mutex_init(&mutex, NULL);   //Inicializa el mutex
	for(i=0;i<100;i++)
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexi?n\n");
		//sock_conn es el socket que usaremos para este cliente
		sockets[i]=sock_conn;
		pthread_create(&thread[i], NULL, atender_cliente, &sockets[i]);
		
	}	
	for ( i = 0; i < 100; i++ )
	{
		pthread_join(thread[i],NULL);
	}
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);	
}







