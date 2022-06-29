
#define LAST_LED 13

/// @brief recibe el estado del cronometro y lo transforma a binario. Va encendiendo y apagando
///       las luces según corresponda y va asignando el valor a cada elemento del array de enteros
/// 
/// @param cronometro valor del cronometro
/// @param array un array de enteros
/// @param tamanio del array del enteros
void estadoLuces (int cronometro, int array[], int tamanio);


/// @brief imprime por consola los valores del cronometro decimal y 
///        del binario cargado en el array
/// 
/// @param cronometro valor del cronometro
/// @param array un array de enteros
/// @param tamanio del array del enteros
void imprimir (int cronometro, int array, int tamanio);


void setup(){
  
  Serial.begin(9600);
  pinMode (2, INPUT);
  pinMode (3, INPUT);
  
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (8, OUTPUT);
  pinMode (9, OUTPUT);
  pinMode (10, OUTPUT);
  pinMode (11, OUTPUT);
  pinMode (12, OUTPUT);
  pinMode (13, OUTPUT);
  
}//setup

int play = 0;
unsigned long millisAnterior = 0;
int cronometro = 0;
int binario[10] = {0};
int firstTimeFlag = 1;
int botonPlayAntes = 0; 
int botonResetAntes = 0;  
//global

void loop(){
 
  int botonPlay = digitalRead(2);
  int botonReset = digitalRead(3);
  unsigned long millisActual = millis();

  if (botonPlay && !botonPlayAntes)
  {
    play = !play; 
  }

  if (botonReset && !botonResetAntes)
  {
    cronometro = 0;
    binario[10] = {0};
  }
  
  if (cronometro == 1024)
  {
    play = !play;
    binario[10] = {0};
    cronometro = 0;
    estadoLuces(cronometro, binario, 10);
  }

  if(play && (firstTimeFlag || millisActual - millisAnterior >= 1000))
  {	

      estadoLuces (cronometro, binario, 10);
      imprimir(cronometro, binario, 10);
      millisAnterior = millisActual;
      cronometro += 1;
      firstTimeFlag = 0;
  }
  
  botonPlayAntes = botonPlay;
  botonResetAntes = botonReset;
  delay(20);
} //loop

            
void estadoLuces (int cronometro, int array[], int tamanio)
{ 
  
  for(int i = 9; i >= 0; i--){
	
    array[i] = cronometro % 2;
    
    digitalWrite(LAST_LED - i, array[i]);
    
    cronometro /= 2;   
  }  
}
  
void imprimir (int cronometro, int array[], int tamanio)
{
  Serial.print("Segundo: ");
  Serial.print(cronometro); 
  Serial.print(" | Binario: ");
  for( int i = 0; i < 10; i++)
  {
    Serial.print(array[i]);
  }
  Serial.println("");
}