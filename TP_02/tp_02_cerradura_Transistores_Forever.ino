#include <Keypad.h>
#include <LiquidCrystal.h>

#define UNCHECKED 0
#define CHECKED 1
#define CONFIG 2

//--------------------------------------------------------------------------------------------------------------//

//DECLARACION DE FUNCIONES 

/// @brief agrega a la cadena en la posicion index el caracter que recibe, y suma uno al contador index
///
/// @param password cadena de caracteres donde se va a guardar los caracteres ingresados por el usuario
/// @param index entero que va a funcionar como variable contador de iteracion
/// @param key caracter ingresado por el usuario en el keypad
void cargarPassword(char password[], int* index, char key);

/// @brief imprime en LCD la cadena password
///
/// @param password cadena de caracteres donde se va guardando lo que ingresa el usuario
void mostrarIngresoPassword(char password[]);

/// @brief llama a funcion verificar() y les pasa password y master, 
/// 		luego llama a la funcion cargarVerificacion() y les pasa el retorno de verificar() y mensajeDos
/// 		si verificar() retorna 1 cambia el estado a 1 ("checked") si retorna 0 no lo modifica
/// 		llama a la funcion borrarCadena() y le pasa password 
///
/// @param estado puntero a entero que indica el estado en el que se encuentra el sistema
/// @param password cadena de caracteres donde se guarda lo que va ingresando el usuario por keypad
/// @param master cadena de caracteres donde se aloja la password actual del sistema
/// @param mensajeDos cadena de caracteres donde se guardan mensajes para imprimir en LCD
/// @return retorna el retorno de la funcion verificar()
int controlador_verificacion(int* estado, char password[], char master[], char mensajeDos[]);

/// @brief copia en la cadena master el contenido de password y llama a la funcion borrarCadena() y le pasa password por parametro
///
/// @param password cadena de caracteres donde se guarda lo que va ingresando el usuario por keypad
/// @param master cadena de caracteres donde se aloja la password actual del sistema
void controlador_configuracion(char password[], char master[]);

/// @brief compara las cadenas password y master
///
/// @param password cadena de caracteres donde se guarda lo que va ingresando el usuario por keypad
/// @param master cadena de caracteres donde se aloja la password actual del sistema
/// @return 1 si password == master || 0 si password != master
int verificar(char password[], char master[]);

/// @brief limpia el lcd y si verificacion es 0 carga "PASSWORD FAIL" en cadenaDos o 
/// 		si verificacion es != 0 carga "PASSWORD OK"
///
/// @param verificacion entero con el valor de retorno de la funcion verificar()
/// @param mensajeDos cadena de caracteres donde se guardan mensajes para imprimir en la linea inferior del LCD
void cargarVerificacion(int verificacion, char mensajeDos[]);

/// @brief imprime en la linea superior del LCD el contenido de mensajeUno
///
/// @param mensajeUno cadena de caracteres donde se guardan mensajes para imprimir en la linea superior del LCD
void imprimirLineaUno(char mensajeUno[]);

/// @brief imprime en la linea inferior del LCD el contenido de mensajeDos
///
/// @param mensajeDos cadena de caracteres donde se guardan mensajes para imprimir en la linea inferior del LCD
void imprimirLineaDos(char mensajeDos[]);

/// @brief llama a la funcion cargarLineaUnoPassword() y le pasa mensajeUno, luego llama a la funcion imprimirLineaUno() y le pasa mensajeUno
///
/// @param mensajeUno cadena de caracteres donde se guardan mensajes para imprimir en la linea superior del LCD
void imprimirPasswordLiteral(char mensajeUno[]);

/// @brief se copia en la cadena password 6 espacios vacios
///
/// @param password cadena de caracteres donde se guarda lo que ha ingresado el usuario por keypad
void borrarCadena(char password[]);

/// @brief copia en la cadena mensajeUno "Password:       "
///
/// @param mensajeUno cadena de caracteres donde se guardan mensajes para imprimir en la linea superior del LCD
void cargarLineaUnoPassword(char mensajeUno[]);

/// @brief copia en la cadena mensajeDos el contenido de la cadena mensaje		
/// 
/// @param mensajeDos cadena de caracteres donde se guardan mensajes para imprimir en la linea inferior del LCD
/// @param mensaje cadena de caracteres o literal que se quiera cargar en mensajeDos
void cargarLineaDos (char mensajeDos[], char mensaje[]);

/// @brief niega el valor que recibe de estadoLed y se lo asigna mediante digitalWrite al pin
/// 
/// @param pin valor del pin donde está conectado la lampara LED a manipular 
/// @param estadoLed puntero a entero de tipo booleano que va a encender (1) o apagar(0) el led del pin
void intruccionLeds(int pinOn, int* estadoLed);

/// @brief mediante la funcion digitalWrite les pasa a pinUno y pinDos el valor cero para apagarlos
///
/// @param pinUno valor del pin donde está conectado la lampara LED a manipular
/// @param pinDos valor del pin donde está conectado la lampara LED a manipular
void apagarLeds(int pinUno, int pinDos);

//--------------------------------------------------------------------------------------------------------------//

//DECLARACION Y SETEO DEL KEYPAD

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 12,  11, 10, 9}; 
byte colPins[COLS] = {8, 7, 6, A2};

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//--------------------------------------------------------------------------------------------------------------//

//DECLARACION Y SETEO DEL LCD

LiquidCrystal lcd(A5, 1, 2, 3, 4, 5);

//--------------------------------------------------------------------------------------------------------------//

//SETUP

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Password:");
 }

//--------------------------------------------------------------------------------------------------------------//

//DECLARACIO DE VARIABLES

int botonConfigAntes = 0;
int botonResetAntes = 0;
int botonTestAntes = 0;

char password[7]; // cadena para el ingreso de password
char master[7] = "1C2022"; //cadena con password guardada
int index = 0;
int estado = UNCHECKED; 

int failFlag = 0;
int flagAlerta = 0;
int configFlag = 0;
int noPermitidoFlag = 0;
int cambioExitosoFlag = 0;

unsigned long millisAntes = 0;
int segundero = 0;
int estadoLed = 0;

char mensajeUno[17] = "Password:";
char mensajeDos[17] = "                ";

//--------------------------------------------------------------------------------------------------------------//

//LOOP

void loop()
{
  int botonConfig = digitalRead(A0);
  int botonReset = digitalRead(A1);
  int botonTest = digitalRead(13); 
  unsigned long millisAhora = millis();
  char key = kpd.getKey();
  
  if(key && !(flagAlerta || configFlag || noPermitidoFlag || cambioExitosoFlag))
  {
	cargarPassword(password, &index, key);
    mostrarIngresoPassword(password);
  }
  
  if( estado != CONFIG && botonTest && !botonTestAntes)
  {
    index = 6;
  }
  
  if(estado != CONFIG && index == 6)
  {
	if(controlador_verificacion(&estado, password, master, mensajeDos))
    {
      failFlag = 0;
    }
    else
    {
      failFlag = 1;
    }
    index = 0;
    flagAlerta = 1;
    segundero = 0;
  }
  
  if(estado == CONFIG && index == 6)
  {
    controlador_configuracion(password, master);
    index = 0;
    estado = CHECKED;
    cambioExitosoFlag = 1;
  }
  
  if(estado == CHECKED && !failFlag && botonConfig && !botonConfigAntes)
  {
    estado = CONFIG;
    borrarCadena(password);
    configFlag = 1;
  }
	
  if((estado == UNCHECKED || failFlag) && botonConfig && !botonConfigAntes)
  {
    index = 0;
    borrarCadena(password);
    noPermitidoFlag = 1;
  }
  
  if(botonReset && !botonResetAntes)
  {
    borrarCadena(password);
    index = 0;
    imprimirPasswordLiteral(mensajeUno);
    
  }
  
  //IMPRESION MENSAJES CON MILLIS
  
  if(millisAhora - millisAntes >=250 && (flagAlerta || configFlag || noPermitidoFlag || cambioExitosoFlag))
  {
    if(configFlag || noPermitidoFlag || cambioExitosoFlag)
    {
      if(segundero == 6)
      {
        segundero = 0;
        configFlag = 0;
        noPermitidoFlag = 0;
        cambioExitosoFlag = 0;
        strcpy(mensajeDos, "                ");
        imprimirPasswordLiteral(mensajeUno);
      }
      //SI SE INGRESA A ESTADO CONFIGURACION
      else if(configFlag)
      {
        cargarLineaDos(mensajeDos, "CAMBIAR PASSWORD");
      }     
      //SI SE INTENTA CAMBIAR PASSWORD SIN VALIDAR ANTES 
      else if(noPermitidoFlag)
      {
        cargarLineaDos(mensajeDos, "OPCION INVALIDA ");
      }
      //CAMBIO DE PASSWORD EXITOSO
      else if(cambioExitosoFlag)
      {
        cargarLineaDos(mensajeDos, "CAMBIO EXITOSO  ");
      }
    }
    //SI SE VERIFICA PASSWORD
    else
    {
      if(segundero == 20)
      {
        segundero = 0;
        flagAlerta = 0;
        apagarLeds(A3, A4);
		strcpy(mensajeDos, "                ");
        imprimirPasswordLiteral(mensajeUno);
      }
      else if(failFlag) // se verifico MAL
      {
        intruccionLeds(A4, &estadoLed);
      }
      else // se verifico BIEN
      {
        intruccionLeds(A3, &estadoLed);
      }
    }
    imprimirLineaDos(mensajeDos);
    segundero ++;
    millisAntes = millisAhora;
  } 
  botonConfigAntes = botonConfig;
  botonResetAntes = botonReset;
  botonTestAntes = botonReset; 
  delay(50); 
  
}// loop

//--------------------------------------------------------------------------------------------------------------//

//DESARROLLO DE FUNCIONES:

void cargarPassword(char password[], int* index, char key)
{
  password[*index] = key;
  (*index)++;
}

void mostrarIngresoPassword(char password[])
{
  lcd.setCursor(10,0);
  lcd.print(password);
}

int controlador_verificacion(int* estado, char password[], char master[], char mensajeDos[])
{
  int verificacion = verificar(password, master);
  cargarVerificacion(verificacion, mensajeDos);

  if(verificacion)
  {
    *estado = CHECKED;
  }
  borrarCadena(password);
  return verificacion;
}
  
void controlador_configuracion(char password[], char master[])
{
  strcpy(master, password);
  borrarCadena(password);
}

int verificar(char password[], char master[])
{
  int retorno = 0;
  
  if (strcmp(password, master) == 0)
  {
    retorno = 1;
  } 
  return retorno;
}

void cargarVerificacion(int verificacion, char mensajeDos[])
{
  lcd.clear();
  
  if(!verificacion)
  {
    cargarLineaDos(mensajeDos, "PASSWORD FAIL   "); 
  }
  else
  {
    cargarLineaDos(mensajeDos, "PASSWORD OK     "); 
  }
}

void imprimirLineaUno(char mensajeUno[])
{
  lcd.setCursor(0,0);
  lcd.print(mensajeUno);
}

void imprimirLineaDos(char mensajeDos[])
{
  lcd.setCursor(0,1);
  lcd.print(mensajeDos);
}

void imprimirPasswordLiteral(char mensajeUno[])
{
  cargarLineaUnoPassword(mensajeUno);
  imprimirLineaUno(mensajeUno);
}

void borrarCadena(char password[])
{
  strcpy(password, "      ");
}

void cargarLineaUnoPassword(char mensajeUno[])
{
  strcpy(mensajeUno, "Password:       ");
}

void cargarLineaDos (char mensajeDos[], char mensaje[])
{
  strcpy(mensajeDos, mensaje);
}

void intruccionLeds(int pin, int* estadoLed) // A3, A4
{ 
  //Cambio el estado del LED
  *estadoLed = !(*estadoLed);
  digitalWrite(pin, *estadoLed);
}

void apagarLeds(int pinUno, int pinDos)
{
  digitalWrite(pinUno, LOW);
  digitalWrite(pinDos, LOW);
}