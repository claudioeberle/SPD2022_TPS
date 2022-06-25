#include <Keypad.h>
#include <LiquidCrystal.h>

#define UNCHECKED 0
#define CHECKED 1
#define CONFIG 2


//DECLARACION DE FUNCIONES 


/// @brief llama a las funciones verificar y le pasa las cadenas recibidas; y llama a mostrarVerificacion, en caso de que el return de estas sea '1' cambia el Estado del sistema a 'CHECKED'
/// 
/// @param estado: recibe por referencia la variable "estado" que define el estado en el que se encuentra funcionando el sistema en ese momento
/// @param password recibe una string password donde el usuario ingresó el password a verificar
/// @param master recibe una string master donde está guardada la password master que activa/abre la cerradura
/// @return 1 si la funcion verificar retorna 1 | retorna 0 si la funcion verificar retorna 0
int controlador_verificacion(int* estado, char password[], char master[]);

/// @brief copia la cadena password en la cadena master, llama la funcion borrarCadena y la funcion mostrarCambioExitoso
/// 
/// @param password recibe una string password donde el usuario ingresa el password a guardar
/// @param master recibe una string master donde guardará la cadena password
void controlador_configuracion(char password[], char master[]);

/// @brief compara las cadenas recibidas
/// 
/// @param password recibe una string password donde el usuario ingresó el password a verificar
/// @param master recibe una string master donde está guardada la password master que activa/abre la cerradura
/// @return 1 si son iguales || 0 si las cadenas son distintas
int verificar(char password[], char master[]);

/// @brief rellena toda la cadena con espacios vacios
/// 
/// @param password recibe una string password donde el usuario ingresó el password a verificar
void borrarCadena(char password[]);

/// @brief imprime en el lcd las frases: 'PASSWORD FAIL' si el param es 0 || 'PASSWORD OK' si el param es 1
/// 
/// @param esta pensada para recibir 1 o 0 como retorno de la funcion verificar
void mostrarVerificacion(int verificacion);

/// @brief limpia el lcd, y muestra la palabra 'PASSWORD: ' en la posicion (0,0)
/// 
void passwordLiteral(void);

/// @brief limpia el lcd e imprime los msjs 'OPCION INVALIDA' y 'Debe validar su password primero'
/// 
void mostrarNoPermitido(void);

/// @brief limpia el lcd e imprime 'CAMBIO DE PASSWORD'
/// 
void mostrarEstadoConfig(void);

/// @brief limpia el lcd e imprime 'CAMBIO EXITOSO'
/// 
void mostrarCambioExitoso(void);

/// @brief niega la variable estadoLed, llama a la funcion digitalWrite y le pasa como parametros el pinOn y estadoLed
/// 
/// @param pinOn recibe un entero con el pin del LED sobre el que va a actuar
/// @param estadoLed puntero a entero booleano declarado en el scope global
void intruccionLeds(int pinOn, int* estadoLed);

/// @brief llama a la funcion digitalWrite y le pasa como parametro los pines recibidos y el valor 'LOW'
/// 
/// @param pin1 entero con pin de salida a LED
/// @param pin2 entero con pin de salida a LED
void apagarLeds(int pin1, int pin2);



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


//DECLARACION Y SETEO DEL LCD

LiquidCrystal lcd(A5, 1, 2, 3, 4, 5);

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
  passwordLiteral();
 }

int botonConfigAntes = 0;
int botonResetAntes = 0;
int botonTestAntes = 0;

char password[7]; // cadena para el ingreso de password
char master[7] = "1C2022"; //cadena con password guardada
int index = 0;
int estado = UNCHECKED; 

int failFlag = 0;
int okFlag = 0;
int flagAlerta = 1;

unsigned long millisAntes = 0;
int segundero = 0;
int estadoLed = 0;

void loop()
{
  int botonConfig = digitalRead(A0);
  int botonReset = digitalRead(A1);
  int botonTest = digitalRead(13); 
  unsigned long millisAhora = millis();
  char key = kpd.getKey();
  
  if(key)
  {
	password[index] = key;
    lcd.setCursor(9,0);
    lcd.print(password);
    index++;
  }
  
  if( estado != CONFIG && botonTest && !botonTestAntes)
  {
    index = 6;
  }
  
  if(estado != CONFIG && index == 6)
  {
	if(controlador_verificacion(&estado, password, master))
    {
      okFlag = 1;
      failFlag = 0;
    }
    else
    {
      failFlag = 1;
      okFlag = 0;
    }
    index = 0;
    flagAlerta = 0;
    segundero = 0;
  }
  
  if(estado == CONFIG && index == 6)
  {
    controlador_configuracion(password, master);
    index = 0;
    estado = CHECKED;
  }
  
  if(estado == CHECKED && !failFlag && botonConfig && !botonConfigAntes)
  {
    estado = CONFIG;
    mostrarEstadoConfig();
	passwordLiteral();
  }
	
  if((!estado || failFlag) && botonConfig && !botonConfigAntes)
  {
    mostrarNoPermitido();
    passwordLiteral();
  }
  
  if(botonReset && !botonResetAntes)
  {
    borrarCadena(password);
    passwordLiteral();
    index = 0;
    
  }
  
  if(millisAhora - millisAntes >=300 && !flagAlerta)
  {
    if(segundero == 12)
    {
      segundero = 0;
      flagAlerta = 1;
      apagarLeds(A3, A4);
    }
	else if(failFlag)
    {
      intruccionLeds(A4, &estadoLed);
    }
    else if(okFlag)
    {
      intruccionLeds(A3, &estadoLed);
    }
    segundero ++;
    millisAntes = millisAhora;
  }
  
  botonConfigAntes = botonConfig;
  botonResetAntes = botonReset;
  botonTestAntes = botonReset;  
} // loop


//DESARROLLO DE FUNCIONES:

int controlador_verificacion(int* estado, char password[], char master[])
{
    int verificacion = verificar(password, master);
    mostrarVerificacion(verificacion);
  
  	if(verificacion)
    {
    	*estado = CHECKED;
    }
    passwordLiteral();
	borrarCadena(password);
  
  	return verificacion;
}
  
void controlador_configuracion(char password[], char master[])
{
  strcpy(master, password);
  borrarCadena(password);
  mostrarCambioExitoso();
  passwordLiteral();
}
  
  
int verificar(char password[], char master[])
{
  int retorno = 1;
  
  if (strcmp(password, master))
  {
    retorno = 0;
  } 
  return retorno;
}

void mostrarVerificacion(int verificacion)
{
  lcd.clear();
  
  if(!verificacion)
  {
    lcd.print("PASSWORD FAIL");
  }
  else
  {
    lcd.print("PASSWORD OK");
  }
  delay(1000);
}

void passwordLiteral(void)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password: ");
}

void borrarCadena(char password[])
{
  for(int i = 0; password[i] != '\0'; i++)
  {
    password[i] = ' ';
  }
}

void mostrarNoPermitido()
{
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("OPCION");
  lcd.setCursor(4,1);
  lcd.print("INVALIDA");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Debe validar su");
  lcd.setCursor(0,1);
  lcd.print("Password primero");
  delay(1000);
  lcd.clear();
}

void mostrarEstadoConfig()
{
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("CAMBIO DE");
  lcd.setCursor(4,1);
  lcd.print("PASSWORD");
  delay(1000);
}

void mostrarCambioExitoso()
{
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("CAMBIO");
  lcd.setCursor(4,1);
  lcd.print("EXITOSO");
  delay(1000);
  
}

void intruccionLeds(int pinOn, int* estadoLed) // A3, A4
{ 
  //Cambio el estado del LED
  *estadoLed = !(*estadoLed);
  digitalWrite(pinOn, *estadoLed);
}

void apagarLeds(int pin1, int pin2)
{
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

  