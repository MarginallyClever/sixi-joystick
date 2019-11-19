// joystick firmware
// dan@marginallyclever.com 2019-09-11

#define RANGE      (280.0)
#define RANGE_DIFF (360.0-RANGE)
#define RANGE_MAX  (180-RANGE_DIFF/2.0)
#define RANGE_MIN  (-RANGE_MAX)
#define MIDPOINT   (0.0)

#define NUM_SENSORS (6)
#define NUM_AVERAGE (5)

// uncomment this line if your cables are exactly backwards
//#define REVERSE_WIRING

float averages[NUM_SENSORS][NUM_AVERAGE];

void setup() {
  Serial.begin(57600);
  Serial.println("\nJ0 J1 J2 J3 J4 J5 min max");
  
  // set pull-up on analog pins
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
}

void loop() {
  int i,j,k;
  float f,sum;
  
  for(i=0;i<NUM_SENSORS;++i) {
    #ifdef REVERSE_WIRING
      j=NUM_SENSORS-1-i;
    #else
      j=i;
    #endif
    
    // "throw away a few reads while the internal capacitor gets it wrong" -- IRC timemage & cehteh
    analogRead(j);
    analogRead(j);
    
    f = (float)analogRead(j)/1024.0;
    f = f * RANGE + RANGE_MIN;
    f -= MIDPOINT;
    if(i==1) f=-f;
    // average samples
    sum=0;
    for(k=0;k<NUM_AVERAGE-1;++k) {
      averages[j][k]=averages[j][k+1];
      sum+=averages[j][k];
    }
    averages[j][k]=f;
    sum+=f;
    sum/=NUM_AVERAGE;
    
    Serial.print(sum);
    Serial.print("\t");
  }
  Serial.print("-180\t180\n");
  delay(30);
}
