
/*
  WiFi UDP Send and Receive String

 This sketch wait an UDP packet on localPort using the CC3200 launchpad
 When a packet is received an Acknowledge packet is sent to the client on port remotePort


 created 30 December 2012
 by dlf (Metodo2 srl)
 
 DATE MODIGIED
 Neesarg Patel

 */
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
// your network name also called SSID
char ssid[] = "Rogers01853";
// your network password
char password[] = "EMlearntra";
const int ARMED = 0;
const int DISARMED = 1;
const int STAY = 2;
int systemState;
unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyHigh[] = "forte";       // a string to send back
char ReplyLow[] = "piano";
byte dataByteOne = B00000000;
byte dataByteTwo = B00000000;
byte origDataByteOne = B00000000;
byte origDataByteTwo = B00000000;
bool gotThree = false;
String armSystem, stayButton, disarmSystem, startOne, startTwo, startThree, startFour, startFive, startSix, startSeven, startEight, startSpeaker, checkStatus, recievedPacket;
bool signedIn[9];
IPAddress ipAddresses[9];
uint16_t ports[9];
IPAddress speakerIP;
uint16_t speakerPort;
IPAddress myIP(192, 168, 0, 177);

const char smpt_server[] = "smtp.gmail.com";
const int smpt_port = 465;
const char userid[] = "househomesystems"; // gmail userid 
const char pswd[] = "cc3200Simple";// gmail password 
const char mailTo[] = "<househomesystems@gmail.com>";
const char mailFrom[] = "<househomesystems@gmail.com>";

char base64_userid[64]; 
char base64_pswd[64];

WiFiUDP Udp;

void setup() {
  systemState = DISARMED;
  startSpeaker = String("Bj6zf2MRmFGxxWPS");
  startOne = String("5M4Z2MSTSc6vAMb6");
  startTwo = String("3pTWV7KtcjXpJQff");
  startThree = String("Rj2y2RZTGjtbux7t");
  startFour = String("FAzVFRfut4q3guH6");
  startFive = String("MsZJ3BKaBwa4qskj");
  startSix = String("zEn9Scnk7JSzDt43");
  startSeven = String("7LjcTT4DrpuXsrwz");
  startEight = String("Pk26eV8CKJN3bmgq");
  armSystem = String("xKFAtMaRA4HkcCPt");
  disarmSystem = String("eF3wWyF5XgMJxMzL");
  stayButton = String("KabMRhNnDhJ89svL");
  checkStatus = String ("Cd8swWdJwxyZNUST");

  for (int i = 0; i < 7; i++) {
      signedIn[i] == false;
  }
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(9, OUTPUT); // yellow led
  pinMode(10, OUTPUT); // green led
  pinMode(29, OUTPUT); // red led
  
  //pinMode(2, INPUT_PULLUP);   
  //pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(28, INPUT_PULLUP);
  //pinMode(30, INPUT_PULLUP);
  //pinMode(32, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    digitalWrite(10, HIGH);
    delay(150);
    digitalWrite(10, LOW);
    Serial.print(".");
    delay(150);
  }
  
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    digitalWrite(9, HIGH);
    delay(150);
    digitalWrite(9, LOW);
    Serial.print(".");
    delay(150);
  }

  Serial.println("\nIP Address obtained");
  printWifiStatus();
  int someInt;
  // encode the userid and password
  someInt = base64_encode(userid, (sizeof userid)-1, base64_userid, sizeof base64_userid);
  base64_userid[someInt] = '\0';
  someInt = base64_encode(pswd, (sizeof pswd)-1, base64_pswd, sizeof base64_pswd);
  base64_pswd[someInt] = '\0';
  Serial.print("userid ");
  Serial.print(userid);
  Serial.print(", base64: ");
  Serial.println(base64_userid);
  Serial.println("\nWaiting for a connection from a client...");
  Udp.begin(localPort);
}

void loop() {
	// BYTE ONE
	if (digitalRead(4) == HIGH){
		dataByteOne = (dataByteOne | B00100000);
	} else {
		dataByteOne = (dataByteOne & B11011111);
	}
	if (digitalRead(5) == HIGH){
		dataByteOne = (dataByteOne | B00010000);
	} else {
		dataByteOne = (dataByteOne & B11101111);
	}
	if (digitalRead(6) == HIGH){
		dataByteOne = (dataByteOne | B00001000);
	} else {
		dataByteOne = (dataByteOne & B11110111);
	}
	if (digitalRead(7) == HIGH){
		dataByteOne = (dataByteOne | B00000100);
	} else {
		dataByteOne = (dataByteOne & B11111011);
	}
	if (digitalRead(8) == HIGH){
		dataByteOne = (dataByteOne | B00000010);
	} else {
		dataByteOne = (dataByteOne & B11111101);
	}
	if (digitalRead(27) == HIGH){
		dataByteOne = (dataByteOne | B00000001);
	} else {
		dataByteOne = (dataByteOne & B11111110);
	}

	
	// BYTE TWO
	if (digitalRead(28) == HIGH){
		dataByteTwo = (dataByteTwo | B10000000);
	} else {
		dataByteTwo = (dataByteTwo & B01111111);
	}
	if (digitalRead(19) == HIGH){
		dataByteTwo = (dataByteTwo | B00010000);
	} else {
		dataByteTwo = (dataByteTwo & B11101111);
	}
	if (digitalRead(18) == HIGH){
		dataByteTwo = (dataByteTwo | B00001000);
	} else {
		dataByteTwo = (dataByteTwo & B11110111);
	}
	if (digitalRead(17) == HIGH){
		dataByteTwo = (dataByteTwo | B00000100);
	} else {
		dataByteTwo = (dataByteTwo & B11111011);
	}
	if (digitalRead(15) == HIGH){
		dataByteTwo = (dataByteTwo | B00000010);
	} else {
		dataByteTwo = (dataByteTwo & B11111101);
	}
	if (digitalRead(14) == HIGH){
		dataByteTwo = (dataByteTwo | B00000001);
	} else {
		dataByteTwo = (dataByteTwo & B11111110);
	}

	if (dataByteOne != origDataByteOne || dataByteTwo != origDataByteTwo) {
	    Serial.println(digitalRead(4));
	    Serial.println(digitalRead(5));
	    Serial.println(digitalRead(6));
	    Serial.println(digitalRead(7));
	    Serial.println(digitalRead(8));
	    Serial.println(digitalRead(27));
	    Serial.println(digitalRead(28));
	    Serial.println(digitalRead(19));
	    Serial.println(digitalRead(18));
	    Serial.println(digitalRead(17));
	    Serial.println(digitalRead(15));
	    Serial.println(digitalRead(14));
	   	if (systemState == DISARMED) {
    	    Serial.println("disarmed sending packet");
            for (int x = 0; x < 9; x++) {
                for (int y = 0; y < 2; y++) {
                    if (signedIn[x]) {
                        Udp.beginPacket(ipAddresses[x], ports[x]);
                        Udp.write(dataByteOne);
                        Udp.write(dataByteTwo);
                        Udp.endPacket();
                    }
                }
    	    }
    	}
    	
    	if (systemState == ARMED) {
    	    Serial.println("armed sending packet");
    	    if (digitalRead(4) == HIGH || digitalRead(5) == HIGH || digitalRead(6) == HIGH || digitalRead(7) == HIGH ||
    	        digitalRead(8) == HIGH || digitalRead(27) == HIGH || digitalRead(28) == HIGH ||
    	        digitalRead(19) == HIGH || digitalRead(18) == HIGH || digitalRead(17) == HIGH) {
    	            intruderPresent();
    	        }
    	    if (digitalRead(14) == HIGH || digitalRead(15) == HIGH) {
    	        allowUserToDisarm();
    	    }
    	}

    	if (systemState == STAY) {
    	    Serial.println("stay sending packet");
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 2; y++) {
                    if (signedIn[x]) {
                        Udp.beginPacket(ipAddresses[x], ports[x]);
                        Udp.write(dataByteOne);
                        Udp.write(dataByteTwo);
                        Udp.endPacket();
                    }
                }
    	    }
    	    if (digitalRead(4) == HIGH || digitalRead(5) == HIGH || digitalRead(6) == HIGH || digitalRead(7) == HIGH ||
    	        digitalRead(8) == HIGH || digitalRead(27) == HIGH || digitalRead(28) == HIGH  ||
    	        digitalRead(19) == HIGH || digitalRead(18) == HIGH || digitalRead(17) == HIGH) {
    	            intruderPresent();
    	        }
    	    if (digitalRead(14) == HIGH || digitalRead(15) == HIGH) {
    	        allowUserToDisarm();
    	    }
    	}
	    origDataByteOne = dataByteOne;
	    origDataByteTwo = dataByteTwo;

	}
	int packetSize = Udp.parsePacket();
	if (packetSize) {
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        
        int len = Udp.read(packetBuffer, 255);
        if (len > 0) packetBuffer[len] = 0;
        Serial.println("Contents:");
        Serial.println(packetBuffer);
        recievedPacket = String(packetBuffer);
    
        
        if (recievedPacket.equals(startOne)) {
            Serial.println("Start one");
            signedIn[0] = true;
            ipAddresses[0] = Udp.remoteIP();
            Serial.print(ipAddresses[0]);
            ports[0] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[0]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[0], ports[0]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startTwo)) {
            Serial.println("Start two");
            signedIn[1] = true;
            ipAddresses[1] = Udp.remoteIP();
            Serial.print(ipAddresses[1]);
            ports[1] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[1]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[1], ports[1]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startThree)) {
            Serial.println("Start three");
            signedIn[2] = true;
            ipAddresses[2] = Udp.remoteIP();
            Serial.print(ipAddresses[2]);
            ports[2] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[2]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[2], ports[2]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startFour)) {
            Serial.println("Start four");
            signedIn[3] = true;
            ipAddresses[3] = Udp.remoteIP();
            Serial.print(ipAddresses[3]);
            ports[3] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[3]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[3], ports[3]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startFive)) {
            Serial.println("Start five");
            signedIn[4] = true;
            ipAddresses[4] = Udp.remoteIP();
            Serial.print(ipAddresses[4]);
            ports[4] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[4]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[4], ports[4]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startSix)) {
            Serial.println("Start six");
            signedIn[5] = true;
            ipAddresses[5] = Udp.remoteIP();
            Serial.print(ipAddresses[5]);
            ports[5] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[5]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[5], ports[5]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startSeven)) {
            Serial.println("Start seven");
            signedIn[6] = true;
            ipAddresses[6] = Udp.remoteIP();
            Serial.print(ipAddresses[6]);
            ports[6] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[6]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[6], ports[6]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startEight)) {
            Serial.println("Start eight");
            signedIn[7] = true;
            ipAddresses[7] = Udp.remoteIP();
            Serial.print(ipAddresses[7]);
            ports[7] = Udp.remotePort();
            Serial.print(", port ");
            Serial.println(ports[7]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[7], ports[7]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(startSpeaker)) {
            Serial.println("Start speaker");
            speakerIP = Udp.remoteIP();
            Serial.print("speaker");
            speakerPort = Udp.remotePort();
            signedIn[8] = true;
            ipAddresses[8] = speakerIP;
            Serial.print(speakerIP);
            ports[8] = speakerPort;
            Serial.print(", port ");
            Serial.println(ports[8]);
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(ipAddresses[8], ports[8]);
                Udp.write(dataByteOne);
                Udp.write(dataByteTwo);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(checkStatus)) {
            Serial.println("Check Status");
            for (int i = 0; i < 2; i++) {
                Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                Udp.write(systemState);
                Udp.endPacket();
            }
        }
        else if (recievedPacket.equals(disarmSystem)) {
            Serial.println("Disarm system");
            systemState = DISARMED;
            byte theDataOne = B10000000;
            byte theDataTwo = B00000000;
            
            Udp.beginPacket(speakerIP, speakerPort);
            Udp.write(theDataOne);
            Udp.write(theDataTwo);
            Udp.endPacket();
            // send disarmed notification 
            // TODO send email saying disarmed
        }
        else if (recievedPacket.equals(armSystem)) {
            Serial.println("Arm system");
            systemState = ARMED;
            byte theDataOne = B00000000;
            byte theDataTwo = B01000000;
            Udp.beginPacket(speakerIP, speakerPort);
            Udp.write(theDataOne);
            Udp.write(theDataTwo);
            Udp.endPacket();
        }
        else if (recievedPacket.equals(stayButton)) {
            Serial.println("Stay button");
            systemState = STAY;
            byte theDataOne = B01000000;
            byte theDataTwo = B00000000;
            Udp.beginPacket(speakerIP, speakerPort);
            Udp.write(theDataOne);
            Udp.write(theDataTwo);
            Udp.endPacket();
        }
	}
	delay(200);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void allowUserToDisarm() {
    Serial.println("Allow user to disarm");
    Udp.beginPacket(speakerIP, speakerPort);
    byte theDataOne = B00000000;
    byte theDataTwo = B00100000;
    
    Udp.beginPacket(speakerIP, speakerPort);
    Udp.write(theDataOne);
    Udp.write(theDataTwo);
    Udp.endPacket();
    int countDown = 0;
    while (countDown < 85) {
        int packetSize = Udp.parsePacket();
    	if (packetSize) {
            Serial.print("Received packet of size ");
            Serial.println(packetSize);
            Serial.print("From ");
            
            int len = Udp.read(packetBuffer, 255);
            if (len > 0) packetBuffer[len] = 0;
            Serial.println("Contents:");
            Serial.println(packetBuffer);
            recievedPacket = String(packetBuffer);
        
            
            if (recievedPacket.equals(disarmSystem)) {
                // send disarmed notification
                // send email saying disarmed
                systemState = DISARMED;
                byte theDataOne = B10000000;
                byte theDataTwo = B00000000;
                
                Udp.beginPacket(speakerIP, speakerPort);
                Udp.write(theDataOne);
                Udp.write(theDataTwo);
                Udp.endPacket();
                return;
                
            }
    	}
    	delay(200);
    	countDown++;
    }
    intruderPresent();
}

void intruderPresent() {
    Serial.println("Intruder present");
    // say intruder alert, the authorities have been notified
    Udp.beginPacket(speakerIP, speakerPort);
    byte theDataOne = B11000000;
    byte theDataTwo = B00000000;
    
    Udp.beginPacket(speakerIP, speakerPort);
    Udp.write(theDataOne);
    Udp.write(theDataTwo);
    Udp.endPacket();
    // send email saying intruder
    sendEmail();
    while (1 == 1) {
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
        digitalWrite(29, HIGH);
        int packetSize = Udp.parsePacket();
    	if (packetSize) {
            Serial.print("Received packet of size ");
            Serial.println(packetSize);
            Serial.print("From ");
            
            int len = Udp.read(packetBuffer, 255);
            if (len > 0) packetBuffer[len] = 0;
            Serial.println("Contents:");
            Serial.println(packetBuffer);
            recievedPacket = String(packetBuffer);
        
            
            if (recievedPacket.equals(disarmSystem)) {
                // send disarmed notification
                // send email saying disarmed
                systemState = DISARMED;
                byte theDataOne = B10000000;
                byte theDataTwo = B00000000;
                
                Udp.beginPacket(speakerIP, speakerPort);
                Udp.write(theDataOne);
                Udp.write(theDataTwo);
                Udp.endPacket();
                digitalWrite(9, LOW);
            	digitalWrite(10, LOW);
            	digitalWrite(29, LOW);
                return;
            }
    	}
    	delay(100);
    	digitalWrite(9, LOW);
    	digitalWrite(10, LOW);
    	digitalWrite(29, LOW);
    	delay(100);
    }
}


void sendEmail () {
  char buf[1025];
  int i;
  size_t total = 0;
  WiFiClient smtp;

  Serial.println("Attempting SSL connection to smtp server");
  if (smtp.sslConnect(smpt_server, smpt_port)) {
    Serial.println("Connected");
    getResponse(&smtp);
    Serial.println("Sending EHLO");
    smtp.println("helo google");
    getResponse(&smtp);
    smtp.println("ehlo google");
    getResponse(&smtp);
    Serial.println("Sending AUTH LOGIN");
    smtp.println("AUTH LOGIN");
    getResponse(&smtp);
    Serial.println("Sending userid");
    smtp.println(base64_userid);
    getResponse(&smtp);
    Serial.println("Sending pswd");
    smtp.println(base64_pswd);
    getResponse(&smtp);
    Serial.println("Sending MAIL FROM");
    smtp.print("MAIL FROM:"); // must be first
    smtp.println(mailFrom);
    getResponse(&smtp);
    Serial.println("Sending RCPT TO");
    smtp.print("RCPT TO:");
    smtp.println(mailTo);
    getResponse(&smtp);
    Serial.println("Sending DATA");
    smtp.println("DATA");
    getResponse(&smtp);
    Serial.println("Sending actual data...");
    smtp.print("From: CC3100");
    smtp.println(mailFrom);
    smtp.println("Subject: INTRUDER!");
    smtp.print("To:");
    smtp.println(mailTo);
    smtp.println(" ");
    smtp.println("There is an intruder!");
    smtp.println(" ");
    smtp.println("(This is an automated message)");
    smtp.println(".");
    delay(1000);
    getResponse(&smtp);
    Serial.println("Sending QUIT");
    smtp.println("QUIT");
    getResponse(&smtp);
    
    Serial.println("Done");
  } else {
    Serial.println("Error connecting to smtp server");
  }
}

int base64_encode(char const* bytes_to_encode, unsigned int in_len, char* encoded_bytes, unsigned int outbuf_len) {
  //std::string ret;
  const unsigned char base64_chars[] = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
  int i = 0;
  int j = 0;
  int output_index = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        encoded_bytes[output_index++] = base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      encoded_bytes[output_index++] = base64_chars[char_array_4[j]];

    while((i++ < 3))
      encoded_bytes[output_index++] = '\=';

  }
  return output_index;
}

void getResponse(WiFiClient* client) {
  const int TIME_INCR = 10;
  const int TIME_MAX = 10000;
  char buf[1025];
  int i;
  int time_elapsed = 0;
  WiFiClient smtp = *client;
  
  if (smtp.connected()) {
    while (!smtp.available() && time_elapsed < TIME_MAX) {
      delay(TIME_INCR);
      time_elapsed += TIME_INCR;
    }
    if (!smtp.available()) {
      Serial.print("No response (");
      Serial.print(TIME_MAX);
      Serial.println("ms)");
      return;
    } else {
      Serial.print("Server response (");
      Serial.print(time_elapsed);
      Serial.println("ms):");
      while (smtp.available()) {
        i = smtp.read((uint8_t *)buf, 1024);
        buf[i] = '\0';
        Serial.print(buf);
        delay(100);
      }
    }
  } else {
    Serial.println("Server is not connected");
  }
}