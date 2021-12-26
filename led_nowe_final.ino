//uzywane piny
#define BLUEPIN 5
#define GREENPIN 9
#define REDPIN 3
#define SOUNDPIN A0

//sterowanie lampami
#define FADESPEED 50     // make this higher to slow down
#define BELOWISZERO 400 //pozniej tego wartosc zmiennej diff bedzie zerowana, lampy sie wyłączą
#define ZEROMAXVALUETIME 10 //co kzde 20 sekund bedzie zerowana wartosc lastmax
#define MINLEDVAL 0 
#define MAXLEDVAL 255
#define MILLISECONDS 1000
#define MILLISFORONEMEASUERMENT 200

//intensywanosc koloru
int RedBrig=0;
int GreenBrig=0;
int BlueBrig=0;

int lastRedBrig=0;
int lastGreenBrig=0;
int lastBlueBrig=0;


void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  //pinMode(8, OUTPUT); //????
  Serial.begin(9600);
  
  randomSeed(analogRead(4));
}

int ColorMode = random(0,7);

int last = 0;
int lastdiff = 0;

int lastmax = 0; // co kzde 20 sekund bedzie zerowana wartosc lastmax
 unsigned long lastReset; //liczy czas zerowania

bool brakMuzyki = false;

void wypiszKolor(String nazwa, int wartosc){
    Serial.print(nazwa);
    Serial.print(wartosc);
    Serial.println(" ");
}

void teSameCoPoprzednio(){
      if(RedBrig == lastRedBrig && BlueBrig == lastBlueBrig && GreenBrig == lastGreenBrig){
      int modulo = RedBrig % 3;
  
        switch (modulo) {
          case 0:
            RedBrig = 0;
            break;
          case 1:
            BlueBrig = 0;
            break;
          case 2:
            GreenBrig = 0;
            break;
        }   
    }
}

void zerujLastMax(int differenece){
   if (millis() - lastReset > ZEROMAXVALUETIME * MILLISECONDS) {
    lastReset += ZEROMAXVALUETIME * MILLISECONDS;

/*
    if(lastmax ==0 && differenece == 0){
      RedBrig = 0;
      GreenBrig = 0;
      BlueBrig = 0;
    }
    else{
      lastmax = 0;
    }
   */ 
   lastmax = 0;
    ColorMode = differenece % 6;
  }
}

void wypiszWszystkieKolory(){
  
  wypiszKolor("blue: ",BlueBrig); 
  wypiszKolor("green: ",GreenBrig);
  wypiszKolor("red: ",RedBrig);
}




 
void loop() {

  //Serial.println("--new--");
  int mic = 0;

  //pobierz dzwiek
  for(int i=0; i<MILLISFORONEMEASUERMENT; i++){
      mic += analogRead(SOUNDPIN);
  }
  Serial.print("mic: ");
  //Serial.print(mic);
  Serial.println(" ");

  //oblicz roznice dzwieku wzgledem poprzedniego
  int diff = abs(mic - last);

  if(diff < BELOWISZERO){ //10 procent
      diff=0;
  }

  Serial.print("diff: ");
  Serial.println(diff);
  Serial.println(" ");

  //zapisz maskymalne
  if(diff > lastmax){
    lastmax = diff;
  }
  

 
  
  lastdiff = diff;
  last = mic; //zapisz synal zeby porownac w kolejnym obiegu petli

  //zapalanie lamp RGB
  //if pozwala wyalczyc lampy gdy nie leci dzwiek
 // if(diff != 0 && lastmax != 0){
    RedBrig = abs(map(diff, BELOWISZERO , lastmax, 0, 255));
    
    GreenBrig = abs(map(diff, BELOWISZERO , lastmax, 0, 255));
   
    BlueBrig = abs(map(diff, BELOWISZERO , lastmax, 0, 255));
 // }
  
   
  


//jesli wszstkie kolory sa 255 to ktroez z nich ustaw na zero (zrob modulo ktorejs z wartosi )

    //teSameCoPoprzednio();

  
  //co kazde 20 sekund bedzie zerowana wartosc lastmax
  zerujLastMax(diff);

  //do terminalu
  wypiszWszystkieKolory();

  if(lastmax ==0 && diff == 0){
      brakMuzyki = true;
      analogWrite(REDPIN, 0);
      analogWrite(BLUEPIN, 0);
      analogWrite(GREENPIN, 0);

      
  }
  if(diff != 0 && brakMuzyki == true){
         brakMuzyki = false;
         ColorMode = millis() % 6;
      }
  
  if(brakMuzyki == false){
        switch (ColorMode) {
          case 0:
            analogWrite(REDPIN, RedBrig);
            break;
          case 1:
            analogWrite(BLUEPIN, BlueBrig);
            break;
          case 2:
            analogWrite(GREENPIN, GreenBrig);
            break;
          case 3:
            analogWrite(REDPIN, RedBrig);
            analogWrite(BLUEPIN, BlueBrig);
            break;
          case 4:
            analogWrite(BLUEPIN, BlueBrig);
            analogWrite(GREENPIN, GreenBrig);
            break;
          case 5:
            analogWrite(GREENPIN, GreenBrig);
            analogWrite(REDPIN, RedBrig);
            break;
           
        } 
  } 

/*
  if(lastmax == 0){
    lastmax = 300;
  }
 */
  
  //konec petli
  delay(FADESPEED); //opoznienie petli
 

}
