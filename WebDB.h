/*
  WebDB.h - Library for EEPROM DataBase for Curso Arduino Automation Kit.
  Created by Renato Aloi, Nov, 2014.
  Released into the public domain.
*/
#ifndef WebDB_h
#define WebDB_h

#include <Arduino.h>
#include <EEPROM.h>
#include <stdint.h>

#define CONTROL_WORD   2
#define CONTROL_WORD_H 0xC0
#define CONTROL_WORD_L 0xA7

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define EEPROMDB_SIZE 2048
#elif defined(__AVR_ATmega328P__)
#define EEPROMDB_SIZE 1024
#else
#define EEPROMDB_SIZE 512
#endif

#define LOGIN_MAX 10
#define PASS_MAX  10
#define TEXT_MAX  20

typedef uint8_t uchar;
typedef uint16_t uint;
typedef uint32_t ulong;

typedef struct DBConfigRecord
{
	uint  sessionExpires;
	char  root[LOGIN_MAX];
	char  pass[PASS_MAX];
}; // 2 + (1 * 10) + (1 * 10) = 22 bytes

typedef struct DBLoginRecord
{
	int   id;
	ulong sessionTimeout;
	char  login[LOGIN_MAX];
	char  pass[PASS_MAX];
}; // 2 + 4 + (1 * 10) + (1 * 10) = 26 bytes

typedef struct DBButtonRecord
{
	int   id;
	char  type;
	bool  state;
	char  text[TEXT_MAX];
	char  step;
	uchar value;
}; // 2 + 1 + 1 + (1 * 20) + 1 + 1 = 26 bytes

enum DBType
{
	DBWord = 0,
	DBConfig = 1,
	DBLogin = 2,
	DBButton = 3
};

enum ButtonType
{
	Button = 0,
	Dimmer = 1,
	Sensor = 2
};

class WebDB
{
private:

	// decided now to add control word at end
	// to ensure DB exists and its size
	const static uint DBWordBeginStart = 0;
	const static uint DBWordEndStart = EEPROMDB_SIZE - (CONTROL_WORD + 1);

	// two first bytes must be control word
	// so DB starts when control word ends
	const static uint DBConfigStart = CONTROL_WORD;
	const static uint DBLoginStart = sizeof(DBLoginRecord) + CONTROL_WORD;
	const static uint DBButtonStart = (EEPROMDB_SIZE / 4) + (CONTROL_WORD*2);

	const static uint DBConfigMaxRecords = 1;
	const static uint DBLoginMaxRecords  = (uint)((EEPROMDB_SIZE / 4) / sizeof(DBLoginRecord)) - 1; // 1 quarto
	const static uint DBButtonMaxRecords = (uint)(((EEPROMDB_SIZE / 4)*3) / sizeof(DBButtonRecord)) - 1; // 3 quartos

	DBConfigRecord m_config;
	DBLoginRecord  m_login;
	DBButtonRecord m_button;

public:
	WebDB() { WebDB(false); };

	WebDB(bool createDB) : m_config(DBConfigRecord()), m_login(DBLoginRecord()), m_button(DBButtonRecord()) {
		if (createDB) {
			if (!isDBExists())
				initDB();
		}
	};

	bool  isDBExists(void);
	void  initDB(void);

	char* getConfigLogin(void);
	char* getConfigPass(void);
	uint  getConfigSessionExpires(void);
	void  setConfigPass(char *text);
	void  setConfigSessionExpires(uint expires);

	int   getLoginNextId(void);
	ulong getLoginSessionTimeout(uint _id);
	char* getLoginPass(uint _id);
	char* getLoginLogin(uint _id);
	void  setLoginSessionTimeout(uint _id, ulong timeout);
	void  setLoginPass(uint _id, char *text);
	void  setLoginLogin(uint _id, char *text);
	void  deleteLogin(uint _id);
	int   createLogin(char* _login, char* _pass, ulong timeout);
	int   findLogin(char* _login);

	int   getButtonNextId(void);
	char* getButtonText(uint _id);
	void  setButtonText(uint _id, char *text);
	void  deleteButton(uint _id);
	int   createButton(char _type, bool _state, char* _text, char _step, uchar _value);
	void  setButtonState(uint _id, char val);
	uchar getButtonState(uint _id);
	int   findButton(uint idVal);
	char  getButtonType(uint _id);
    void  setButtonType(uint _id, char val);
    uchar getButtonValue(uint _id);
    void  setButtonValue(uint _id, uchar val);
    char  getButtonStep(uint _id);
    void  setButtonStep(uint _id, char val);
    uchar getButtonId(uint _id);
    void  setButtonId(uint _id, uchar val);




	// Getters
	uint getDBConfigRecordSize(void) { return sizeof(DBConfigRecord); };
	uint getDBLoginRecordSize(void) { return sizeof(DBLoginRecord); };
	uint getDBButtonRecordSize(void) { return sizeof(DBButtonRecord); };

	uint getDBWordBeginStart(void) { return DBWordBeginStart; };
	uint getDBWordEndStart(void) { return DBWordEndStart; };

	uint getDBConfigStart(void) { return DBConfigStart; };
	uint getDBLoginStart(void) {return DBLoginStart; };
	uint getDBButtonStart(void) { return DBButtonStart; };

	uint getDBConfigMaxRecords(void) { return DBConfigMaxRecords; };
	uint getDBLoginMaxRecords(void) { return DBLoginMaxRecords; };
	uint getDBButtonMaxRecords(void) { return DBButtonMaxRecords; };

	uint getEEPROMSize(void) { return EEPROMDB_SIZE; };
};



#endif // WebDB

