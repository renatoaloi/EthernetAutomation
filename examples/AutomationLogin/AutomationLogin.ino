#include <SPI.h>
#include <EEPROM.h>
#include <EthernetAutomation.h>
#include <EtherEncLib.h>

#define ACTIVATE_LOGIN 1

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 177 };

const char portaLampada = 5;
const char portaVentilador = 6;
const char portaSentidoVentilador = 8;
const char portaFechadura = 7;
const char portaSair = 99;

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
    automation.addLogin("seunome", "seuasenha", 5); // 5 = tempo da sessao
    automation.addLogin("outrousr", "outrapas", 15); // logins e senhas ate 9 caracteres
    
    // Criando botoes
    automation.addButton(portaLampada, "Lampada", ONOFF_BUTTON);
    automation.addButton(portaVentilador, "Ventilador", ONOFF_BUTTON);
    automation.addButton(portaSentidoVentilador, "Inv. Vent.", ONOFF_BUTTON);
    automation.addButton(portaFechadura, "Porta", PULSE_BUTTON);
    automation.addButton(portaSair, "Sair", PULSE_BUTTON);
    
    Serial.println(F("DB Add OK!"));
  }
  else
  {
    Serial.println(F("DB Init OK!"));
  }
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
      case portaLampada:
        digitalWrite(portaLampada, buttonState);
        break;
      case portaVentilador:
        digitalWrite(portaVentilador, buttonState);
        break;
      case portaSentidoVentilador:
        digitalWrite(portaSentidoVentilador, buttonState);
        break;
      case portaFechadura:
        digitalWrite(portaFechadura, HIGH);
        delay(100);
        digitalWrite(portaFechadura, LOW);
        break;
      case portaSair:
        // Logout!
        automation.logOut();
        break;
      default:
        break;
    }
    
    // Closing and printing HTML to client
    automation.close();
  }
}
