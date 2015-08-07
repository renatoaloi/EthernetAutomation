// seriallink.com.br
// 2015
// by Renato Aloi

#include <SPI.h>
#include <EEPROM.h>
#include <EthernetAutomation.h>
#include <EtherEncLib.h>

#define ACTIVATE_LOGIN 1

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 177 };

const char relay1 = 6; // relays
const char relay2 = 7;
const char relay3 = 8;
const char relay4 = 9;
const char dimmer1 = 97; // real pin 4;
const char dimmer2 = 98; // real pin 5;
const char exitfakebutton = 99;

static volatile unsigned long momentoDisparo = 0;
static volatile unsigned int gatilhoDisparo = 4000;
static volatile unsigned long momentoDisparo2 = 0;
static volatile unsigned int gatilhoDisparo2 = 4000;

const unsigned char dimmer1pin = 4;
const unsigned char dimmer2pin = 5;

EthernetAutomation automation(80);

void setup()
{
  pinMode(10,OUTPUT);	//--- ? -- SS pin must be output # by Renato Aloi

  Serial.begin(115200);
  delay(200);
  Serial.println(F("Serial OK!"));
  
  automation.begin(mac, ip);
  delay(200);
  Serial.println(F("Ethernet OK!"));
  
  if (!automation.isWebDBExists())
  {
    // Criando WebDB
    automation.addDB(); 
    
    // Criando logins
    automation.addLogin("yourname", "yourpass", 5); // 5 = tempo da sessao (session expires)
    automation.addLogin("seuuser", "suasenha", 15); // logins e senhas ate 9 caracteres
    
    // Criando botoes buttons
    automation.addButton(relay1, "Rele 1", ONOFF_BUTTON);
    automation.addButton(relay2, "Rele 2", ONOFF_BUTTON);
    automation.addButton(relay3, "Rele 3", ONOFF_BUTTON);
    automation.addButton(relay4, "Pulsar", PULSE_BUTTON);
    automation.addDimmer(dimmer1, "Dimmer1: ", 127, -1, 25);
    automation.addDimmer(dimmer2, "Dimmer2: ", 127, -1, 25);
    automation.addButton(exitfakebutton, "Sair", MACRO_BUTTON); // fake button
    
    Serial.println(F("DB Add OK!"));
  }
  else
  {
    Serial.println(F("DB Init OK!"));
  }

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(dimmer1, OUTPUT);
  pinMode(dimmer2, OUTPUT);
     
  attachInterrupt(0, zeroCrossInt, CHANGE);
}

void loop()
{
  if (automation.available(ACTIVATE_LOGIN))
  {
    Serial.println(F("ETH avail OK!"));
    
    // Verifica se algum botao foi pressionado
    // Check for button pressed
    int lastButton = automation.getLastClickedButton();
    int buttonState = automation.getButtonState(lastButton);

    Serial.print("Lastbutton: ");
    Serial.println(lastButton, DEC);
    Serial.print("ButtonState: ");
    Serial.println(buttonState, DEC);
		  
    // Executa o comando conforme o botao clicado
    // Do the command for pressed button
    switch(lastButton)
    {
      case relay1:
	Serial.print(buttonState ? "Ativou" : "Desativou"); Serial.println(F(" Rele 1"));
        digitalWrite(relay1, buttonState);
        break;
      case relay2:
	Serial.print(buttonState ? "Ativou" : "Desativou"); Serial.println(F(" Rele 2"));
        digitalWrite(relay2, buttonState);
        break;
      case relay3:
	Serial.print(buttonState ? "Ativou" : "Desativou"); Serial.println(F(" Rele 3"));
        digitalWrite(relay3, buttonState);
        break;
      case relay4: // PULSE Button
	Serial.print(F("Pulsou ")); Serial.println(F(" Rele 4"));
        digitalWrite(relay4, HIGH);
        delay(100);
        digitalWrite(relay4, LOW);
        break;
      case dimmer1:
        Serial.print(F("Dimmer1 value: "));
	Serial.println(automation.getDimmerValue(dimmer1), DEC);
	gatilhoDisparo = map(automation.getDimmerValue(dimmer1), 0, 255, 7000, 1000);
        break;
      case dimmer2:
        Serial.print(F("Dimmer2 value: "));
	Serial.println(automation.getDimmerValue(dimmer2), DEC);
	gatilhoDisparo2 = map(automation.getDimmerValue(dimmer2), 0, 255, 7000, 1000);
        break;
      case exitfakebutton:
        // Logout!
        automation.logOut();
        break;
      default:
        break;
    }
    
    // Closing and printing HTML to client
    automation.close();
  }
  checkDimmerTimeSpan();
}

void checkDimmerTimeSpan()
{
  if (momentoDisparo > micros() || momentoDisparo2 > micros())
      {
        if (momentoDisparo > momentoDisparo2)
        {
          while(momentoDisparo2 > micros());
          digitalWrite(dimmer2pin, HIGH);
          while(momentoDisparo > micros());
          digitalWrite(dimmer1pin, HIGH);
        }
        else if (momentoDisparo2 > momentoDisparo)
        {
          while(momentoDisparo > micros());
          digitalWrite(dimmer1pin, HIGH);
          while(momentoDisparo2 > micros());
          digitalWrite(dimmer2pin, HIGH);
        }
        else
        {
          while(momentoDisparo > micros());
          digitalWrite(dimmer1pin, HIGH);
          digitalWrite(dimmer2pin, HIGH);
        }
      }
}

void zeroCrossInt()
{
  digitalWrite(dimmer1pin, LOW);
  digitalWrite(dimmer2pin, LOW);
  momentoDisparo = micros() + gatilhoDisparo;
  momentoDisparo2 = micros() + gatilhoDisparo2;
}
