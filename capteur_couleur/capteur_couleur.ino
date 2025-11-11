#include <Arduino.h>
#include <math.h>

// Assignment of the sensor pins
#define S0 7
#define S1 6
#define S2 5
#define S3 4
#define sensorOut 3

/* Calibration values (update before each use) */
long int redMin = 40000;
long int redMax = 70000;
long int greenMin = 40000;
long int greenMax = 70000;
long int blueMin = 40000;
long int blueMax = 70000;

const int size_mean = 100;

bool blue_detected=false;
bool yellow_detected=false;

float truth_factor_red_blue =0.0;
float truth_factor_green_blue =0.0;
float truth_factor_blue_blue =0.0;

float truth_factor_blue =0.0;

float truth_factor_blue_ecart_relatif =0.0;

float truth_factor_red_yellow =0.0;
float truth_factor_green_yellow =0.0;
float truth_factor_blue_yellow =0.0;

float truth_factor_yellow =0.0;

float truth_factor_yellow_ecart_relatif =0.0;



// Arrays to store measured frequencies
float tab_red[size_mean];    
float tab_blue[size_mean];
float tab_green[size_mean];

// Mean frequencies
float blue_mean_frequency = 0;
float red_mean_frequency = 0;
float green_mean_frequency = 0;



float blue_mean_frequency_blue = 0;
float red_mean_frequency_blue = 0;
float green_mean_frequency_blue = 0;

float blue_mean_frequency_yellow = 0;
float red_mean_frequency_yellow = 0;
float green_mean_frequency_yellow = 0;


float blue_norm_frequency_blue = 0;
float red_norm_frequency_blue = 0;
float green_norm_frequency_blue = 0;

float blue_norm_frequency_yellow = 0;
float red_norm_frequency_yellow = 0;
float green_norm_frequency_yellow = 0;



// Temporary variables for frequency measurement
float redFrequency = 0;
float redEdgeTime = 0;
float greenFrequency = 0;
float greenEdgeTime = 0;
float blueFrequency = 0;
float blueEdgeTime = 0;

float red_norm_frequency = 0;
float green_norm_frequency = 0;
float blue_norm_frequency = 0;


float measureMeanFrequency(int S2_state, int S3_state, int numValues) {
  float sum = 0.0;
  for (int i = 0; i < numValues; i++) {
    digitalWrite(S2, S2_state);
    digitalWrite(S3, S3_state);
    // Mesure du temps d'impulsion
    float edgeTime = pulseIn(sensorOut, HIGH) + pulseIn(sensorOut, LOW);
    float frequency = 1.0 / (edgeTime / 1000000.0);  // convertir en Hz
    
    sum += frequency;
    delay(50); // petit délai entre mesures
  }
  return sum / numValues;
}



void setup() {
  Serial.begin(9600);
  Serial.println("Begin setup");

  // Definition of the sensor pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Scaling the output frequency S0/S1
  // LOW/LOW=AUS, LOW/HIGH=2%, HIGH/LOW=20%, HIGH/HIGH=100%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);


  Serial.println("Ne rien placer");
  red_mean_frequency=measureMeanFrequency(LOW,LOW, size_mean);
  green_mean_frequency=measureMeanFrequency(HIGH,HIGH, size_mean);
  blue_mean_frequency=measureMeanFrequency(LOW,HIGH, size_mean);

  Serial.println("Placer bleu");
  delay(2000);
  Serial.println("Debut bleu");
  red_mean_frequency_blue=measureMeanFrequency(LOW,LOW, size_mean);
  green_mean_frequency_blue=measureMeanFrequency(HIGH,HIGH, size_mean);
  blue_mean_frequency_blue=measureMeanFrequency(LOW,HIGH, size_mean);
  Serial.println("fin bleu");
  delay(2000);

  Serial.println("Placer jaune");
  delay(2000);
  Serial.println("Debut jaune");
  red_mean_frequency_yellow=measureMeanFrequency(LOW,LOW, size_mean);
  green_mean_frequency_yellow=measureMeanFrequency(HIGH,HIGH, size_mean);
  blue_mean_frequency_yellow=measureMeanFrequency(LOW,HIGH, size_mean);
  Serial.println("fin jaune");
  delay(2000);

  blue_norm_frequency_blue=abs((blue_mean_frequency_blue-blue_mean_frequency)/(green_mean_frequency_blue+red_mean_frequency_blue+blue_mean_frequency_blue));
  red_norm_frequency_blue=abs((red_mean_frequency_blue-red_mean_frequency)/(green_mean_frequency_blue+red_mean_frequency_blue+blue_mean_frequency_blue));
  green_norm_frequency_blue=abs((green_mean_frequency_blue-green_mean_frequency)/(green_mean_frequency_blue+red_mean_frequency_blue+blue_mean_frequency_blue));



  blue_norm_frequency_yellow=abs((blue_mean_frequency_yellow-blue_mean_frequency)/(green_mean_frequency_blue+red_mean_frequency_blue+blue_mean_frequency_blue));
  red_norm_frequency_yellow=abs((red_mean_frequency_yellow-red_mean_frequency)/(green_mean_frequency_blue+red_mean_frequency_blue+blue_mean_frequency_blue));
  green_norm_frequency_yellow=abs((green_mean_frequency_yellow-green_mean_frequency)/(green_mean_frequency_blue+red_mean_frequency_blue+blue_mean_frequency_blue));
  

  // Calculate mean frequencies

  Serial.println("Setup finished");
  Serial.print("Red mean frequency: "); Serial.println(red_mean_frequency);
  Serial.print("Green mean frequency: "); Serial.println(green_mean_frequency);
  Serial.print("Blue mean frequency: "); Serial.println(blue_mean_frequency);
}

void loop() {

  blue_detected=false;
  yellow_detected=false;


  truth_factor_red_blue =0.0;
  truth_factor_green_blue =0.0;
  truth_factor_blue_blue =0.0;

  truth_factor_red_yellow =0.0;
  truth_factor_green_yellow =0.0;
  truth_factor_blue_yellow =0.0;

  truth_factor_yellow =0.0;
  truth_factor_blue =0.0;

  truth_factor_blue_ecart_relatif =0.0;
  truth_factor_yellow_ecart_relatif =0.0;

  

  redFrequency=measureMeanFrequency(LOW,LOW, 10);
  greenFrequency=measureMeanFrequency(HIGH,HIGH, 10);
  blueFrequency=measureMeanFrequency(LOW,HIGH, 10);
 
  delay(100);

  blue_norm_frequency=abs((blueFrequency-blue_mean_frequency)/(redFrequency+greenFrequency+blueFrequency));
  red_norm_frequency=abs((redFrequency-red_mean_frequency)/(redFrequency+greenFrequency+blueFrequency));
  green_norm_frequency=abs((greenFrequency-green_mean_frequency)/(redFrequency+greenFrequency+blueFrequency));



 //Serial.print("Blue Frequency: "); Serial.println(blue_norm_frequency);
 //Serial.print("Red Frequency: "); Serial.println(red_norm_frequency);
 //Serial.print("Green Frequency: "); Serial.println(green_norm_frequency);

 Serial.println("");

  truth_factor_blue_blue =blue_norm_frequency/blue_norm_frequency_blue;
  truth_factor_green_blue =green_norm_frequency/green_norm_frequency_blue;
  truth_factor_red_blue =red_norm_frequency/red_norm_frequency_blue;

  truth_factor_blue =truth_factor_blue_blue+truth_factor_green_blue+truth_factor_red_blue;

 
 // blue_detected = (truth_factor_blue_blue > 0.5 && truth_factor_blue_blue < 1.5) 
   //               && (truth_factor_green_blue > 0.5 && truth_factor_green_blue < 1.5)
     //             &&(truth_factor_red_blue > 0.5 && truth_factor_red_blue < 1.5);






  truth_factor_blue_yellow =blue_norm_frequency/blue_norm_frequency_yellow;
  truth_factor_green_yellow =green_norm_frequency/green_norm_frequency_yellow;
  truth_factor_red_yellow =red_norm_frequency/red_norm_frequency_yellow;

  truth_factor_yellow =truth_factor_blue_yellow+truth_factor_green_yellow+truth_factor_red_yellow;


  //yellow_detected = (truth_factor_blue_yellow > 0.5 && truth_factor_blue_yellow < 1.5) 
    //              && (truth_factor_red_yellow > 0.5 && truth_factor_red_yellow < 1.5)
      //            &&(truth_factor_green_yellow > 0.5 && truth_factor_green_yellow < 1.5);


  
  
  
  truth_factor_blue_ecart_relatif =(abs(truth_factor_blue/3 -1) )*100;   // indique l'écart relatif en % par rappoirt a la mesure attendue pour avoir du blueu
  truth_factor_yellow_ecart_relatif =(abs(truth_factor_yellow/3 -1))*100; // indique l'écart relatif en % par rappoirt a la mesure attendue pour avoir du jaune

  blue_detected = truth_factor_blue_ecart_relatif <15 && truth_factor_yellow_ecart_relatif>30;
  yellow_detected=truth_factor_yellow_ecart_relatif<15 &&truth_factor_blue_ecart_relatif>100;


 if (blue_detected && !yellow_detected){

    Serial.println("blue detected");
    Serial.println("ble tf");
    Serial.println(truth_factor_blue_ecart_relatif);
    Serial.println("");
    Serial.println("yellow tf");
    Serial.println(truth_factor_yellow_ecart_relatif);
 };

 if (!blue_detected && yellow_detected){

    Serial.println("yellow detected");
    Serial.println("yellow tf");
    Serial.println(truth_factor_yellow_ecart_relatif);
    Serial.println("");
    Serial.println("ble tf");
    Serial.println(truth_factor_blue_ecart_relatif);
    
 };

 if (!blue_detected && !yellow_detected){

    Serial.println("nothing detected");
    Serial.println("yellow tf");
    Serial.println(truth_factor_yellow_ecart_relatif);
    Serial.println("");
    Serial.println("ble tf");
    Serial.println(truth_factor_blue_ecart_relatif);
 };




  Serial.println("------------------------------");
  Serial.println("------------------------------");
  delay(500);

  /* Determination of the measured color by comparison
     with the values of the other measured colors
  if (redColor > greenColor and redColor > blueColor) {
    Serial.println("Red detected ");
    delay(100);
  }
  if (greenColor > redColor and greenColor > blueColor) {
    Serial.println("Green detected ");
    delay(100);
  }
  if (blueColor > redColor and blueColor > greenColor) {
    Serial.println("Blue detected ");
    delay(100);
  }
  if (blueColor > 0 and greenColor > 0 and redColor > 0) {
    Serial.println("Please place an Object in front of theSensor");
    delay(100);
  }
  Serial.println("------------------------------");
  delay(1000);
  */
}
