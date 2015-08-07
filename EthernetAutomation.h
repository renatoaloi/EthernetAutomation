/*
  EthernetAutomation.h - Library for Curso Arduino Automation Kit.
  Created by Renato Aloi, Nov, 2014.
  Released into the public domain.
*/
#ifndef EthernetAutomation_h
#define EthernetAutomation_h

#include <stdint.h>
#include "WebDB.h"
#include "EtherEncLib.h"
#include "html.h"

#define DEFAULT_WEB_PORT 80
#define PARAMS_LEN       50
#define BUFF_TMP_LEN     11

#define ONOFF_BUTTON    1
#define PULSE_BUTTON    0
#define DIMMER_BUTTON   2
#define MACRO_BUTTON    3


typedef uint8_t uchar;
typedef uint16_t uint;
typedef uint32_t ulong;

class EthernetAutomation
{
private:
	uint  	    m_port;
	WebDB 	    m_db;
	EtherEncLib m_net;
	uint8_t     pgmChar;
	char        buffTmp[BUFF_TMP_LEN];
	uint        m_contador;
	char        m_login[LOGIN_MAX];
	char        m_pass[PASS_MAX];
	int         m_lastButtonId;
	uchar       m_lastButtonValue;

	//void printP(EtherEncLib &client, const prog_uchar *str);
	//static int GetIntFromStringStream(char* c);


	void returnLoginFormToClient(void);
	void returnHtmlToClient(void);
	bool checkLogin(void);
	bool checkAdmin(void);
	void init(void) {
        for (unsigned i = 0; i < BUFF_TMP_LEN; i++) buffTmp[i] = 0;
        for (unsigned i = 0; i < LOGIN_MAX; i++) m_login[i] = 0;
        for (unsigned i = 0; i < PASS_MAX; i++) m_pass[i] = 0;
	};

public:
	EthernetAutomation(uint port) : m_lastButtonId(-1), m_lastButtonValue(0), m_contador(0), m_port(port), m_db(WebDB()), m_net(EtherEncLib(port)) { init(); };
	EthernetAutomation() : m_lastButtonId(-1), m_contador(0), m_port(DEFAULT_WEB_PORT), m_db(WebDB()), m_net(EtherEncLib(DEFAULT_WEB_PORT))  { init(); };

	void begin(uchar* mac, uchar* ip) { begin(mac, ip, DEFAULT_WEB_PORT); };
	void begin(uchar* mac, uchar* ip, uint port) { begin(mac, ip, port, false); };
	void begin(uchar* mac, uchar* ip, uint port, bool createDB);
	void close(void);
	void logOut(void) { m_lastButtonId = -1; for (unsigned i = 0; i < LOGIN_MAX; i++) m_login[i] = 0;
        for (unsigned i = 0; i < PASS_MAX; i++) m_pass[i] = 0; };

	bool isWebDBExists(void) { return m_db.isDBExists(); };
	void addDB(void) { m_db.initDB(); };
	void addLogin(char* _login, char* _pass, ulong timeout) { m_db.createLogin(_login, _pass, timeout); };

	//int WebDB::createButton(char _type, bool _state, char* _text, char _step, uchar _value)
	void addButton(char pin, char *texton, char type) { addButton(pin, texton, type, (type == PULSE_BUTTON) ? (char)-1 : (char)0); };
	void addButton(char pin, char *texton, char type, char state) { m_db.createButton(type, state, texton, 0, pin, 0); };

    	void addDimmer(char id, char *textdim, unsigned char iniValue, char direction) { addDimmer(id, textdim, iniValue, direction, 1, DIMMER_BUTTON); };
    	void addDimmer(char id, char *textdim, unsigned char iniValue, char direction, char step) { addDimmer(id, textdim, iniValue, direction, step, DIMMER_BUTTON); };
    	void addDimmer(char id, char *textdim, unsigned char iniValue, char direction, char step, char type) {
        	//m_db.createDimmer(pin, textdim, iniValue, direction, step, type);
        	m_db.createButton(type, direction, textdim, step, id, iniValue);
    	};

	bool available() { strcpy(m_login, "_no_auth_"); available(0); };
	bool available(const char activateLogin);

	int getLastClickedButton(void) { return m_lastButtonId; }; //return m_db.findButton(m_lastButtonId); };



	uchar getButtonState(uint id) /*{ return 0; };*/ {
          int btid = m_db.findButton(m_lastButtonId);
          if (btid >= 0)
              return m_db.getButtonState(btid);
          else
              return 255;
        };


        uchar getDimmerValue(uint id) /*{ return 0; };*/ {
          int btid = m_db.findButton(m_lastButtonId);
          if (btid >= 0)
              return m_db.getDimmerValue(btid);
          else
              return 0;
        };


	/*
	void getInfoFromDB(void);
	void getInfoFromRootLogin(void);
	void getInfoFromRootPass(void);
	int createLogin(char* _login, char* _pass, ulong timeout) { return m_db.createLogin(_login, _pass, timeout); };
	void getInfoLogin(uint _id);
	*/
};

#endif // EthernetAutomation
