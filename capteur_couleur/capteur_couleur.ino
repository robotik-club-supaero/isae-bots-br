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
const int facteur = 2;
const int facteur_red = 2;

bool red_detected;
bool green_detected;
bool blue_detected;


// Arrays to store measured frequencies
float tab_red[size_mean];    
float tab_blue[size_mean];
float tab_green[size_mean];

// Mean frequencies
float blue_mean_frequency = 0;
float red_mean_frequency = 0;
float green_mean_frequency = 0;

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



  red_mean_frequency=measureMeanFrequency(LOW,LOW, size_mean);
  green_mean_frequency=measureMeanFrequency(HIGH,HIGH, size_mean);
  blue_mean_frequency=measureMeanFrequency(LOW,HIGH, size_mean);

 
  

  // Calculate mean frequencies

  Serial.println("Setup finished");
  Serial.print("Red mean frequency: "); Serial.println(red_mean_frequency);
  Serial.print("Green mean frequency: "); Serial.println(green_mean_frequency);
  Serial.print("Blue mean frequency: "); Serial.println(blue_mean_frequency);
}

void loop() {

  red_detected=false;
  green_detected=false;
  blue_detected=false;
  
  Serial.println("Begin mesure");
  redFrequency=measureMeanFrequency(LOW,LOW, 10);
  greenFrequency=measureMeanFrequency(HIGH,HIGH, 10);
  blueFrequency=measureMeanFrequency(LOW,HIGH, 10);
 
  delay(100);

  blue_norm_frequency=(blueFrequency-red_mean_frequency)/(redFrequency+greenFrequency+blueFrequency);
  red_norm_frequency=(redFrequency-blue_mean_frequency)/(redFrequency+greenFrequency+blueFrequency);
  green_norm_frequency=(greenFrequency-green_mean_frequency)/(redFrequency+greenFrequency+blueFrequency);



 Serial.print("Blue Frequency: "); Serial.println(blue_norm_frequency);
 Serial.print("Red Frequency: "); Serial.println(red_norm_frequency);
 Serial.print("Green Frequency: "); Serial.println(green_norm_frequency);

 Serial.println("");

 blue_detected= blue_norm_frequency>facteur*abs(red_norm_frequency) && blue_norm_frequency>abs(facteur*green_norm_frequency);

 red_detected=red_norm_frequency>facteur_red*abs(blue_norm_frequency) && red_norm_frequency>abs(facteur_red*green_norm_frequency);

 green_detected=green_norm_frequency>facteur*abs(blue_norm_frequency) && green_norm_frequency>facteur*abs(red_norm_frequency);


 if (blue_detected && !red_detected && ! green_detected){

    Serial.println("blue detected");
 };

 if (!blue_detected && red_detected && ! green_detected){

    Serial.println("red detected");
 };

 if (!blue_detected && !red_detected &&  green_detected){

    Serial.println("green detected");
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
