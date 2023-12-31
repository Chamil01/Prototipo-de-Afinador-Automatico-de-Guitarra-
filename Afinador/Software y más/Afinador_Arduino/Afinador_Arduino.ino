
#include <Stepper.h>
#define STEPS 4096 

//////////// Main /////////////

float FrecuenciaRecibida;
int Cuerda=0;

void setup() {
  Serial.begin(9600);
  pinMode(7,OUTPUT);
}


void loop() {
  if (Serial.available() > 0) {
    String floatString = Serial.readStringUntil('\n');
    FrecuenciaRecibida = floatString.toFloat();
    float DiferenciaEntreFrecuencias;
    float AjustarClavija;
    DiferenciaEntreFrecuencias=CompararFrecuencia(FrecuenciaRecibida, Cuerda);
    
    if (abs(DiferenciaEntreFrecuencias)<=0.7){  //Verificamos si la cuerda ya está afinada
      if (Cuerda==5) {
          Cuerda=0;
      }
      else{
          Cuerda=Cuerda+1;
      }

      for(int i = 0; i <= 10; i++) { // De estarlo, se le avisa al usuario mediante una luz LED que debe pasar a la siguiente cuerda
        digitalWrite(7,HIGH);
        delay(200);
        digitalWrite(7,LOW);
        delay(200);
      }
    }
    else{
    MoverServo(DiferenciaEntreFrecuencias); //Si la cuerda no está afinada se le dan indicaciones al motor paso a paso para que gire correctamente
    delay(200);
    }

  }

}



//////////// Funciones /////////////

float CompararFrecuencia(float FrecuenciaDetectada, int Cuerda){   //Esta función compara la frecuencia detectada por el sensor de sonido con la tabulada

// Guardamos en un array las frecuencias con las que tiene que comparar el programa, por el momento únicamente hacemos
// para afinación estándar

float AfinacionEstandar[]={82.41,110.00,146.83,196.00,246.94,329.63}; //El primer elemento es la cuerda mas grave (Mi),
                                                                      // el último la mas aguda (También un Mi)

float DiferenciaEntreFrecuencias; 

DiferenciaEntreFrecuencias=AfinacionEstandar[Cuerda]-FrecuenciaDetectada; //Guardamos en un float la diferencia entre la frecuencia detectada
                                                                           // y la frecuencia tabulada
return DiferenciaEntreFrecuencias; 
}




void MoverServo(float DiferenciaEntreFrecuencias){ //Esta función gira el servo en sentido horario o antihorario según estemos por encima o por 
                                                  // debajo de la frecuencia deseada. 

  Stepper stepper(STEPS, 8, 10, 9, 11); // Constructor, pasamos STEPS y los pines donde tengamos conectado el motor (hay que elegir bien
                                        // los pines para que el servo pueda girar tanto en sentido horario como antihorario, el orden es
                                        // IN1, IN3, IN2, IN4)

  stepper.setSpeed(2); //Seteamos la velocidad de giro del servo en rpm

  // Aca vemos si la frecuencia detectada está por encima o por debajo de la frecuencia tabulada. En base a ello, se decide si el servo gira 
  // en un sentido u otro. 

  if(DiferenciaEntreFrecuencias>0){
    stepper.step(int((STEPS/3)*abs(DiferenciaEntreFrecuencias))); //Mientras mayor sea la diferencia entre frecuencias, mas girará el servo
    delay(100);
    }
  else{
    stepper.step(-int((STEPS/3)*abs(DiferenciaEntreFrecuencias)));
    delay(100);
  } 
  }



