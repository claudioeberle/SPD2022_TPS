#define LAST_LED 13 
#define TAM 10

/// @brief recibe un entero decimal, lo transforma a binario y lo retorna por referencia a un array de enteros     
///
///	@param entero decimal
/// @param array de enteros
/// @param entero con tamaño de array
///
void decToBin (int decimal, int array[], int tamanio);

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
int binario[TAM] = {0};
int flag = 1;

//global

void loop(){
 
 	int boton1 = digitalRead(2);
	int boton1Antes = 0; 
 	int boton2 = digitalRead(3);
	int boton2Antes = 0;  
 	unsigned long millisActual = millis();
 
 	if (boton1 == 1 && boton1Antes == 0){
    
   		play = !play; 
 	}

 	if (boton2 == 1 && boton2Antes == 0){
    
    	cronometro = 0;
    	binario[TAM] = {0};
      	flag = 0;
  	}
 
	if( play == 1){
    
    	if(flag || millisActual - millisAnterior >= 1000){
    	
      		decToBin (cronometro, binario, TAM);
      	
     		for(int i = 0; i < TAM; i++){
        
          		if(binario[i] == 1){
           
            		digitalWrite(LAST_LED - i, HIGH);
          		}
          		if(binario[i] == 0){
            
            		digitalWrite(LAST_LED - i, LOW);
         		}       	
        
     		}//for	
    
   			millisAnterior = millisActual;
      	
     		Serial.print("Segundo: ");
 	 		Serial.print(cronometro); 
 	 		Serial.print(" | Binario: ");
 	 
     		for (int i = 0; i < TAM; i++){
    
 				Serial.print(binario[i]);
 	 		}
 	 		Serial.println("");
          	
          	cronometro += 1;
          	flag = 0;
      
    	} // if millis
        
 	}//if play
  
 	if ( cronometro == 1023){
    
    	play = !play;
    	cronometro = 0;
    	binario[TAM] = {0};  
   		for(int i = 0; i < TAM; i++){
    	
      		digitalWrite(LAST_LED - i, LOW);    
     	}
      
 	}//if cronometro
  
 	boton1Antes = boton1;
 	boton2Antes = boton2;
  
} //loop

            
void decToBin (int decimal, int array[], int tamanio){
    
    int binarioI[tamanio];
    int k = 0;
    
    
    for(int i = 0; i < tamanio; i++){
        
       binarioI[i] = decimal % 2;
       decimal /= 2; // decimal = decimal/2;
       
       
    }   
    
    for(int i = tamanio -1; i >= 0; i--){
        
            array[k++] = binarioI[i];       
    }

}
