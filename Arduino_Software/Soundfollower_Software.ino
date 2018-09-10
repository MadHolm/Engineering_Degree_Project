#define sound_detector0_pin A0
#define sound_detector1_pin A2
#define sound_detector2_pin A4
#define sound_detector3_pin A8
#define sound_detector4_pin A10

#define encoder_pin2 A7
#define encoder_pin1 A6


int green_led = 18;
int yellow_led = 17;
int red_led = 16;

int delay_value = 50;
int a = 0;
int b = 0;
int ib = 0;
int lockb = 0;
int ia = 0;
int locka = 0;
int is = 0;

int alpha = 0;

int sound_detector0 = 0;
int sound_detector1 = 0;
int sound_detector2 = 0;
int sound_detector3 = 0;
int sound_detector4 = 0;


int treshold = 900;
int side = 0.15;

float conv_const = 0.235;


int number_of_reads = 5;

int req_reads;
int reads = 0;

int reading = 1;


// Fast ADC defined
#define FASTADC 1

// Fast ADC defines for setting and clearing register bits
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))



void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT); 
  pinMode(green_led, OUTPUT); 
  pinMode(yellow_led, OUTPUT); 
  pinMode(red_led, OUTPUT); 
  pinMode(7, OUTPUT); //Sygnał PWM silnika nr 1
  pinMode(19, OUTPUT); //Sygnał sterujacy silnika nr 1
  pinMode(6, OUTPUT); //Sygnał PWM silnika nr 1
  pinMode(20, OUTPUT); //Sygnał sterujacy silnika nr 1
  // set prescale to 16
    sbi(ADCSRA,ADPS2) ;
    cbi(ADCSRA,ADPS1) ;
    cbi(ADCSRA,ADPS0) ;//definicja 13 wyjscia (dioda na plytce Arduino)
}

int MicReading(){
  //READING INPUTS FROM MICS
  sound_detector0 = analogRead( sound_detector0_pin );
  sound_detector1 = analogRead( sound_detector1_pin );   
  sound_detector2 = analogRead( sound_detector2_pin );
  sound_detector3 = analogRead( sound_detector3_pin );
  sound_detector4 = analogRead( sound_detector3_pin );
  //CHECKING IF ANY EXCEEDS TRESHOLD
  if( ( sound_detector0 > treshold )||( sound_detector1 > treshold )||( sound_detector2 > treshold )||( sound_detector3 > treshold )||( sound_detector4 > treshold ) ){
    if( sound_detector0 > treshold ){
      return 60;
    }    
    if( sound_detector1 > treshold ){
      return 120;     
    }
    if( sound_detector2 > treshold ){
      return 180;     
    }
    if( sound_detector3 > treshold ){
      return 240;
    }
    if( sound_detector4 > treshold ){
      return 300;     
    }    
  }
  reading = 0;
  return 0;
}
  
int Encodeb( int n_o_reads_b ){
  while( ib < n_o_reads_b ){
    b = analogRead( encoder_pin2 );
    a = analogRead( encoder_pin1 );
    Serial.println( b );
    if(( b < 100 )||( a < 100 )){
      if( lockb == 0 ){
        digitalWrite(green_led, HIGH);
        ib++;
        lockb = 1;
        Serial.println( ib );
      }
    }
    if(( b > 200 )||( a > 200 )){
      lockb = 0;
      digitalWrite(green_led, LOW);
    }
    /*if( ib >= n_o_reads_b ){
      return 1;
      ib = 0;
    }
    return 0;*/
  }
  return 1;
  ib = 0;
}

void RobotBodyTurn( int angle ){
  req_reads = (int)( ( angle*50 )/360 );
  digitalWrite(19, 0);
  digitalWrite(20, 1);
  analogWrite(7, 50);
  analogWrite(6, 50);
  Encodeb( req_reads );
      digitalWrite(19, 1);
      digitalWrite(20, 0);
      delay(100);
      analogWrite(7, 0);
      analogWrite(6, 0);      
  alpha = 0;
}


void loop() {
  //////////////////////////////////////////////////////////////////////////////////////////
  while( reading == 1 ){
    ///////////////////////////////////////////////////////////READING INPUTS FROM MICS
    sound_detector0 = analogRead( sound_detector0_pin );
    sound_detector1 = analogRead( sound_detector1_pin );   
    sound_detector2 = analogRead( sound_detector2_pin );
    sound_detector3 = analogRead( sound_detector3_pin );
    sound_detector4 = analogRead( sound_detector4_pin );
    ///////////////////////////////////////////////////////////CHECKING IF ANY EXCEEDS TRESHOLD
    if( ( sound_detector0 > treshold )||( sound_detector1 > treshold )||( sound_detector2 > treshold )||( sound_detector3 > treshold )||( sound_detector4 > treshold ) ){
      if( sound_detector0 > treshold ){
        alpha = 60;
        reading = 0;
      }    
      if( sound_detector1 > treshold ){
        alpha = 120;   
        reading = 0; 
      }
      if( sound_detector2 > treshold ){
        alpha = 180;   
        reading = 0;  
      }
      if( sound_detector3 > treshold ){
        alpha = 240;
        reading = 0;
      }
      if( sound_detector4 > treshold ){
        alpha = 300;   
        reading = 0; 
      }    
    }   
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  req_reads = (int)( ( alpha*45 )/360 );
  //////////////////////////////////////////////////////////////////////////////////////////
  if(( alpha > 80 )&&( alpha < 140 )){
    req_reads = req_reads - 2;
  }
  if(( alpha > 140 )&&( alpha < 220 )){
    req_reads = req_reads - 6;
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  if( alpha <= 180 ){
    digitalWrite(19, 1);
    digitalWrite(20, 0);
    analogWrite(7, 200);
    analogWrite(6, 200);
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  while( is < req_reads ){
    b = analogRead( encoder_pin2 );
    a = analogRead( encoder_pin1 );
    if( b < 100 ){
      if( lockb == 0 ){
        digitalWrite(green_led, HIGH);
        ib++;
        lockb = 1;
      }
    }
    if( a < 100 ){
      if( locka == 0 ){
        digitalWrite(yellow_led, HIGH);
        ia++;
        locka = 1;
      }
    }
    if( b > 200 ){
      lockb = 0;
      digitalWrite(green_led, LOW);
    }
    if( a > 200 ){
      locka = 0;
      digitalWrite(yellow_led, LOW);
    }
    is = 0.5*(ia + ib);
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  digitalWrite(green_led, LOW); 
  digitalWrite(yellow_led, LOW);
  //////////////////////////////////////////////////////////////////////////////////////////
  digitalWrite(19, 0);
  digitalWrite(20, 1);
  analogWrite(7, 250);
  analogWrite(6, 250);
  delay(300);
  analogWrite(7, 0);
  analogWrite(6, 0);
  //////////////////////////////////////////////////////////////////////////////////////////
  digitalWrite(green_led, HIGH);
  digitalWrite(yellow_led, HIGH);
  digitalWrite(red_led, HIGH);
  ib = 0;
  ia = 0;
  is = 0;
  reading = 1; 
  delay(300); 
  digitalWrite(green_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(red_led, LOW);
  //////////////////////////////////////////////////////////////////////////////////////////
}
