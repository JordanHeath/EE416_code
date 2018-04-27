 /*
  * Assignment:    Microcontrolller Code for EE416 Senior Project
  * Project:       FSAE Electric Racing Vehicle Benchtop Model
  * Language:      C
  * Board programmed: Arduino Uno
  * Instructions:    http://www.instructables.com/id/Using-the-Arduino-Uno-to-program-ATTINY84-20PU/
  * Microcontroller: ATTiny 84 - 1 MHz - 
  * Team Members:  Matt Maggio, Ethan Wisdom, Jordan Heath,            also       Ben Eltis 
  * Team Name:     Qitai
  * Professor:     Pattrick Pedrow
  * Term;          Fall 2017-Spring 2018
  * Faculty Mentor:Ahmed Abu-Hajar
  * 
  * 
  * 
  * 
  * 
  * 
  * Shummery: This code does checks if things are wrong in the system. 
  * Sensor 1 and sensor 2 are checking if there is a 10% difference. There is code in the main loop for it
  * Brake checks along with the sensors to see if the brake is pressed and there is more than 25% accelerator
  * fromMux is important because it deals with most of the AMS such as....
  *   Battery temp
  *   Battery Voltage- YOU NEED TO FIND A WAY TO SHOW THE JUDGES THAT IT WORKS
  *  
  * Shutdown signal goes to a NMOS transistor and if there is something wrong in the system, it sends a digital 0 to the NMOS thus cutting off vltage to the system
  * chargerRelay turns off the charger as per FSAE rules
  * AMS light triggers when something goes wrong in the battery system.
  * 
  * The select pins are going to the MUX
  * 
  * 
  * I have made macros so you can easily make changes when you swap differrnty batteries...
  * It helps to measure with a multimeter in the actual system, then replicate that voltage with a power supply into an arduino. This will give you the value that it actually is
  * Then just make that value read from your Arduino the value
  * 
  * 
  * 
  */
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////Setting up the pins on the ATTiny.. these values cater to the Tiny... change for debugging on microcontroller///////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sensor1 = 0; // APPS Sensor 1
int sensor2 = 1; //APPS sensor 2
int brake = 2; //brake sensor
int fromMUX = 3; //signal coming from the MUX
int shutdownSignal = 4; //To the NMOS transistor on the safety shutdown system
int chargerRelay = 5; //When shutdown signal, the charger gets cut off
int AMSlight = 6;// is on when the AMS is good

//Select pins
int s3 = 10;
int s2 = 9;
int s1 = 8;
int s0 = 7;

//MACROS 
#define MAX_BATT 20 //
#define MIN_BATT 20 //
#define MAX_TEMP1 650 // was 650
#define MAX_TEMP2 640 // was 640


void setup() {
  // ALL OF THE SETUP... SELF EXPLAINATORY
  pinMode(sensor1, INPUT); 
  pinMode(sensor2, INPUT);
  pinMode(brake, INPUT);
  pinMode(fromMUX, INPUT);//form multiplexer
  pinMode(shutdownSignal, OUTPUT);
  pinMode(chargerRelay, OUTPUT);
  pinMode(AMSlight, OUTPUT);

  //MUX select pins
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  //this resets everything to low at the beginning, they will be set on the first run through
  digitalWrite(AMSlight, LOW);
  digitalWrite(shutdownSignal, HIGH);
  digitalWrite(chargerRelay, HIGH);
  
}

void loop() {
  

  //reset the values
  //consider making them unsigned because negative is never an option
  int sensorValue1=0;
  int sensorValue2=0;
  int brakeValue = 0;


  ///this averages the values for the sensors so we aren't reading 1 value
  //CAn only do so many iterations because the integer value will overflow 
  //if its unsigned,then you can do more iterations
  for(int i=0;i<10;i++) // keep it at 10 because if its larger, then the int gets overloaded....
  {
  sensorValue1 += analogRead(sensor1);// + sensorValue1;
  }
  sensorValue1 = sensorValue1/10;
  
  for(int i=0;i<10;i++)
  {
  sensorValue2 += analogRead(sensor2);// + sensorValue2;
  }
  sensorValue2 = sensorValue2/10;

  //same with this brake value 
  for(int i=0;i<10; i++)
  {
    brakeValue += analogRead(brake);
  }
  brakeValue=brakeValue/10;


  //////////Equation for % difference/////////////
  //consider making these unsigned as well
  double top = abs(sensorValue1 - sensorValue2);
  double bottom = (sensorValue1 + sensorValue2) / 2;
  double total = (top / bottom) *100;


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////Select pins 4 MUX/////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////
  ///////PIN I0////////////
  /////////////////////////
  digitalWrite(s0, LOW);//0
  digitalWrite(s1, LOW);//0
  digitalWrite(s2, LOW);//0
  digitalWrite(s3, LOW);//0

  delay(20); // 5 ms delay in the case of output change

  int temp1Reading = analogRead(fromMUX);
  
  
  /////////////////////////
  ///////PIN I1////////////
  /////////////////////////
  digitalWrite(s0, HIGH);//0
  digitalWrite(s1, LOW);//0
  digitalWrite(s2, LOW);//0
  digitalWrite(s3, LOW);//0

  delay(20);

  
  int temp2Reading = analogRead(fromMUX);

  
  /*
  /////////////////////////
  ///////PIN I2////////////
  /////////////////////////
  digitalWrite(s0, LOW);//0
  digitalWrite(s1, HIGH);//0
  digitalWrite(s2, LOW);//0
  digitalWrite(s3, LOW);//0

  delay(20);
  
  int batt1 = analogRead(fromMUX);//reads val=
////////////FIGURE IT OUT??//////////////////////

*/
  /*
  /////////////////////////
  ///////PIN I3////////////
  /////////////////////////
  digitalWrite(s0, HIGH);//0
  digitalWrite(s1, HIGH);//0
  digitalWrite(s2, LOW);//0
  digitalWrite(s3, LOW);//0

  delay(5);
  
  int batt2 = analogRead(fromMUX);//reads val=
  /////////////////////////
  ///////PIN I4////////////
  /////////////////////////
  digitalWrite(s0, LOW);//0
  digitalWrite(s1, LOW);//0
  digitalWrite(s2, HIGH);//0
  digitalWrite(s3, LOW);//0

  delay(5);
  
  int batt3 = analogRead(fromMUX);
  /////////////////////////
  ///////PIN I5////////////
  /////////////////////////
  digitalWrite(s0, HIGH);//0
  digitalWrite(s1, LOW);//0
  digitalWrite(s2, HIGH);//0
  digitalWrite(s3, LOW);//0

  delay(5);
  
  int batt4 = analogRead(fromMUX);
  /////////////////////////
  ///////PIN I6////////////
  /////////////////////////
  digitalWrite(s0, LOW);//0
  digitalWrite(s1, HIGH);//0
  digitalWrite(s2, HIGH);//0
  digitalWrite(s3, LOW);//0

  delay(5);
  
  int batt5 = analogRead(fromMUX);
  /////////////////////////
  ///////PIN I7////////////
  /////////////////////////
  digitalWrite(s0, HIGH);//0
  digitalWrite(s1, HIGH);//0
  digitalWrite(s2, HIGH);//0
  digitalWrite(s3, LOW);//0

  delay(5);
  
  int batt6 = analogRead(fromMUX);

 */
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////Bools to determine a shutdown signal//////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //This number is calculated earlier.... make sure that you calabrate the brake sensors to be an even angle if you're getting weird values 
  bool APPS = (total >= 10.0 && sensorValue2 >= 250);
  ///THAT 200 IS AN EXCEPTION.... YOU NEED TO GET RID OF THIS TO MAINTAIN INTEEGRITY
  ///THIS WILL NEED TO BE DONE EARLIER IN THE CODE... IT MAY HAVE TO DO WITH THE ANGLEWS OF IT


  //Brake goes from 0-1023... if it goes abouve 5% and 25% throttle, we send a shutdown signlal
  bool BSPD = (sensorValue1 >=250 && brakeValue >= 300);
  


  
  //THESE ARE DETERMINED BY A MACRO... Currently, your fingers will trigget a shutdown

  
  bool TEMP1 = (temp1Reading > MAX_TEMP1) && (sensorValue2 <= 125);
  bool TEMP2 = (temp2Reading > MAX_TEMP2) && (sensorValue2 <= 125);
  
  
  if(TEMP1 || TEMP2 || BSPD || APPS)
  {
    if(TEMP1 || TEMP2)
    {//problem with the AMS means that 
     digitalWrite(AMSlight,HIGH);
    }
     digitalWrite(shutdownSignal, LOW);
     digitalWrite(chargerRelay, LOW);
  }

  //These are also from a macro based on testing.. these will need to be changed

  //
  //
  //
  
  //bool BATT1 = (batt1 > MAX_BATT  || batt1 < MIN_BATT);
  //bool BATT2 = (batt2 > MAX_BATT  || batt2 < MIN_BATT);//(batt2 > MAX_BATT  || batt2 < MIN_BATT);
  //bool BATT3 = false; //(batt3 > MAX_BATT  || batt3 < MIN_BATT);
  //bool BATT4 = false; //(batt4 > MAX_BATT  || batt4 < MIN_BATT);
  //bool BATT5 = false; //(batt5 > MAX_BATT  || batt5 < MIN_BATT);
  //bool BATT6 = false; //(batt6 > MAX_BATT  || batt6 < MIN_BATT);
  //bool condition = (APPS || BSPD || TEMP1 || TEMP2 || BATT1 || BATT2 || BATT3 || BATT4 || BATT5 || BATT6);



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////This checks the AMS to see if all the batteries are at correct voltage///////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  /*
  if(AMS)
    digitalWrite(AMSlight, HIGH);
  else
    digitalWrite(AMSlight, LOW);
*/
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////Condoition means that sensors dont meet criteria///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*
 if(condition) {
  digitalWrite(shutdownSignal, LOW);
  digitalWrite(chargerRelay, HIGH);}
 else
  digitalWrite(shutdownSignal,HIGH);
  */

 
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////Serial prints for debugging....//////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //you can only do these when youre using the actual arduino... ATTiny wont let you use these 
  /*
  Serial.print(n);
  Serial.print(Sensor 1: );
  Serial.print(sensorValue1, DEC);
  Serial.print(n);
  
  Serial.print(Sensor 2: );
  Serial.print(sensorValue2, DEC);
  Serial.print(n);
  
  Serial.print(Brake Value: );
  Serial.print(brakeValue, DEC);
  Serial.print(n);
  
  Serial.print(Percent Difference: );
  Serial.print(total, DEC);
  Serial.print(n);
  
  Serial.print(Temp 1 Reading: );
  Serial.print(temp1Reading);
  Serial.print(n);
  
  Serial.print(Temp 2 Reading: );
  Serial.print(temp2Reading);
  Serial.print(n);
  
  Serial.print(Battery 1 Reading: );
  Serial.print(batt1);
  Serial.print(n);

  Serial.print(Battery 2 Reading: );
  Serial.print(batt2);
  Serial.print(n);

  Serial.print(Battery 3 Reading: );
  Serial.print(batt3);
  Serial.print(n);

  Serial.print(Battery 4 Reading: );
  Serial.print(batt4);
  Serial.print(n);

  Serial.print(Battery 5 Reading: );
  Serial.print(batt5);
  Serial.print(n);

  Serial.print(Battery 6 Reading: );
  Serial.print(batt6);
  Serial.print(n);
  
  delay(3000); // this is so we can read the serial commands
 */
  
    

}

