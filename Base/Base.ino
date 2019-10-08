#include <Adafruit_FONA.h>
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

// this is a large buffer for replies
char replybuffer[255];


#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

void setup() {
  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("FONA SMS caller ID test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  // make it slow so its easy to read!
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }
  Serial.println(F("FONA is OK"));

  // Print SIM card IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("SIM card IMEI: "); Serial.println(imei);
  }

  fonaSerial->print("AT+CNMI=2,1\r\n");  //set up the FONA to send a +CMTI notification when an SMS is received

  Serial.println("FONA Ready");
}

  
char fonaNotificationBuffer[64];          //for notifications from the FONA
char smsBuffer[250];


void loop() {
  
  char* bufPtr = fonaNotificationBuffer;    //handy buffer pointer
  
  if (fona.available())      //any data available from the FONA?
  {
    int slot = 0;            //this will be the slot number of the SMS
    int charCount = 0;
    //Read the notification into fonaInBuffer
    do  {
      *bufPtr = fona.read();
      Serial.write(*bufPtr);
      delay(1);
    } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (sizeof(fonaNotificationBuffer)-1)));
    
    //Add a terminal NULL to the notification string
    *bufPtr = 0;

    //Scan the notification string for an SMS received notification.
    //  If it's an SMS message, we'll get the slot number in 'slot'
    if (1 == sscanf(fonaNotificationBuffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) {
      Serial.print("slot: "); Serial.println(slot);
      
      char callerIDbuffer[32];  //we'll store the SMS sender number in here
      
      // Retrieve SMS sender address/phone number.
      if (! fona.getSMSSender(slot, callerIDbuffer, 31)) {
        Serial.println("Didn't find SMS message in slot!");
      }
      Serial.print(F("FROM: ")); Serial.println(callerIDbuffer);

        // Retrieve SMS value.
        uint16_t smslen;
        if (fona.readSMS(slot, smsBuffer, 250, &smslen)) { // pass in buffer and max len!
          Serial.println(smsBuffer);
        }
      Serial.println(smsBuffer);Serial.println(smsBuffer); //Message is stores in smsBuffer which is a char
      //Send back an automatic response
      char lat[10];
      char lon[10];
      char alt[10];
      char aux1[2]; //20
      char aux2[3]; //100
      char aux3[9];
      char aux4[9];
      int comaCounter=0;
      int aux = 0;
      int i=0;
      delay(100);
      while( smsBuffer[i] != ',')
      {
       lat[aux] = smsBuffer[i];
       i++;
       aux++; 
      }
      aux=0;
      i++;
      Serial.println(lat[0]);
      Serial.println(lat[1]);

      
       while( smsBuffer[i] != ',')
      {
       lon[aux] = smsBuffer[i];

       i++;
       aux++; 
      }
      aux=0;
      i++;
      Serial.println(lon[0]);
      Serial.println(lon[1]);
      Serial.println(lon[3]);
      

      
       while( smsBuffer[i] != ',')
      {
       alt[aux] = smsBuffer[i];
       
       i++;
       aux++; 
      }
delay(100);
      aux2[0]=lon[0];
      aux2[1]=lon[1];
      aux2[2]=lon[2];
      int d1=atoi(aux2);
      aux1[0]=lat[0];
      aux1[1]=lat[1];
      int d=atoi(aux1);
      aux3[0]=lat[2];
      aux3[1]=lat[3];
      aux3[2]=lat[4];
      aux3[3]=lat[5];
      aux3[4]=lat[6];
      aux3[5]=lat[7];
      aux3[6]=lat[8];
      aux3[7]=lat[9];
      aux3[8]=lat[10];
      aux3[9]=lat[11];
      float m=atof(aux3);
      
      Serial.println(d);
      Serial.println(m);
      float lati = d + (m/60.0000);
      Serial.println(lati, 6);
      delay(100);
      
      aux4[0]=lon[3];
      aux4[1]=lon[4];
      aux4[2]=lon[5];
      aux4[3]=lon[6];
      aux4[4]=lon[7];
      aux4[5]=lon[8];
      aux4[6]=lon[9];
      aux4[7]=lon[10];
      aux4[8]=lon[11];
      aux4[9]=lon[12];
      float m1=atof(aux4);
      Serial.println(d1);
      Serial.println(m1);
      Serial.println(aux4);
      float longi = (d1 + (m1/60.0000)) * -1;
      Serial.println(longi, 6);
      
      
      
      Serial.println("Latitud: ");
      Serial.println(lati, 8);
      Serial.println("Longitud: ");
      Serial.println(longi, 8);
      int alti=atoi(alt);
      Serial.println("Altitud: ");
      Serial.println(alti, 2);

      
      //Serial.println("Sending reponse...");

      //Serial.println(smsBuffer);
      
      // delete the original msg after it is processed
      //   otherwise, we will fill up all the slots
      //   and then we won't be able to receive SMS anymore
      if (fona.deleteSMS(slot)) {
        Serial.println(F("OK!"));
      } else {
        Serial.print(F("Couldn't delete SMS in slot ")); 
        Serial.println(slot);
        fona.print(F("AT+CMGD=?\r\n"));
      }
    }
  }
}
