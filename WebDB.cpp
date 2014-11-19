/*
  WebDB.cpp - Library for EEPROM DataBase for Curso Arduino Automation Kit.
  Created by Renato Aloi, Nov, 2014.
  Released into the public domain.
*/

#include "WebDB.h"

static unsigned contador;
static DBConfigRecord *recordConfig;
static DBLoginRecord *recordLogin;
static DBButtonRecord *recordButton;
static uchar buff[30];

bool WebDB::isDBExists(void)
{
	return (
	       EEPROM.read(getDBWordBeginStart()) == CONTROL_WORD_H
	    && EEPROM.read(getDBWordBeginStart()+1) == CONTROL_WORD_L
	    && EEPROM.read(getDBWordEndStart()) == CONTROL_WORD_H
	    && EEPROM.read(getDBWordEndStart()+1) == CONTROL_WORD_L
	    );
}

void WebDB::initDB(void)
{
	/*static unsigned contador;
	static DBConfigRecord *record = &m_config;*/
	recordConfig = &m_config;
	recordLogin = &m_login;
	recordButton = &m_button;

	// Writing DB Header
	EEPROM.write(getDBWordBeginStart(), CONTROL_WORD_H);
	EEPROM.write(getDBWordBeginStart()+1, CONTROL_WORD_L);

	// Config Record
	recordConfig->sessionExpires = 30;
	for(unsigned i = 0; i < LOGIN_MAX; i++) recordConfig->root[i] = 0;
	recordConfig->root[0] = 'a';
	recordConfig->root[1] = 'd';
	recordConfig->root[2] = 'm';
	recordConfig->root[3] = 'i';
	recordConfig->root[4] = 'n';
	for(unsigned i = 0; i < PASS_MAX; i++) recordConfig->pass[i] = 0;
	recordConfig->pass[0] = '1';
	recordConfig->pass[1] = '2';
	recordConfig->pass[2] = '3';
	recordConfig->pass[3] = '4';

	contador = 0;
	for (int j = getDBConfigStart(); j < getDBConfigStart() + sizeof(DBConfigRecord); j++)
	{
		EEPROM.write(j, ((uchar*)recordConfig)[contador]);
		contador++;
	}


	// Login Records
	recordLogin->id = -1;
	recordLogin->sessionTimeout = 0L;
	for(unsigned i = 0; i < LOGIN_MAX; i++) recordLogin->login[i] = 0;
	for(unsigned i = 0; i < PASS_MAX; i++) recordLogin->pass[i] = 0;
	for (int i = 0; i < getDBLoginMaxRecords(); i++)
	{
		contador = 0;
		for( int j = getDBLoginStart() + (sizeof(DBLoginRecord) * i);
		    j < getDBLoginStart() + ((sizeof(DBLoginRecord) * i) + sizeof(DBLoginRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordLogin)[contador]);
			contador++;
		}
	}

	// Button Records
	recordButton->id = -1;
	recordButton->type = -1;
	recordButton->state = false;
	for(unsigned i = 0; i < TEXT_MAX; i++) recordButton->text[i] = 0;
	recordButton->step = -1;
	recordButton->value = 0;
	for (int i = 0; i < getDBButtonMaxRecords(); i++)
	{
		contador = 0;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * i);
		    j < getDBButtonStart() + ((sizeof(DBButtonRecord) * i) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}

	// Writing DB Footer
	EEPROM.write(getDBWordEndStart(), CONTROL_WORD_H);
	EEPROM.write(getDBWordEndStart()+1, CONTROL_WORD_L);
}








char* WebDB::getConfigLogin(void)
{
	// Config Record
	/*static unsigned contador = 0;
	static DBConfigRecord *record = &m_config;
	static uchar buff[sizeof(DBConfigRecord)];*/
	recordConfig = &m_config;
	contador = 0;
	for (unsigned i = 0; i < sizeof(DBConfigRecord); i++) buff[i] = 0;
	for (int j = getDBConfigStart(); j < getDBConfigStart() + sizeof(DBConfigRecord); j++)
	{
		buff[contador] = EEPROM.read(j);
		contador++;
	}
	recordConfig = (DBConfigRecord*)buff;
	return recordConfig->root;
}

char* WebDB::getConfigPass(void)
{
	// Config Record
	/*static unsigned contador = 0;
	static DBConfigRecord *record = &m_config;
	static uchar buff[sizeof(DBConfigRecord)];*/
	recordConfig = &m_config;
	contador = 0;
	for (unsigned i = 0; i < sizeof(DBConfigRecord); i++) buff[i] = 0;
	for (int j = getDBConfigStart(); j < getDBConfigStart() + sizeof(DBConfigRecord); j++)
	{
		buff[contador] = EEPROM.read(j);
		contador++;
	}
	recordConfig = (DBConfigRecord*)buff;
	return recordConfig->pass;
}

uint WebDB::getConfigSessionExpires(void)
{
	// Config Record
	/*static unsigned contador = 0;
	static DBConfigRecord *record = &m_config;
	static uchar buff[sizeof(DBConfigRecord)];*/
	recordConfig = &m_config;
	contador = 0;
	for (unsigned i = 0; i < sizeof(DBConfigRecord); i++) buff[i] = 0;
	for (int j = getDBConfigStart(); j < getDBConfigStart() + sizeof(DBConfigRecord); j++)
	{
		buff[contador] = EEPROM.read(j);
		contador++;
	}
	recordConfig = (DBConfigRecord*)buff;
	return recordConfig->sessionExpires;
}

/* recordLogin->id = -1;
	recordLogin->sessionTimeout = 0L;
	for(unsigned i = 0; i < LOGIN_MAX; i++) recordLogin->login[i] = 0;
	for(unsigned i = 0; i < PASS_MAX; i++) recordLogin->pass[i] = 0; */


void WebDB::setConfigPass(char *text)
{
	recordConfig = &m_config;
	getConfigLogin();
	contador = 0;
	for (unsigned i = 0; i < LOGIN_MAX; i++) recordConfig->pass[i] = 0;
	for (unsigned i = 0; i < PASS_MAX; i++) {
		recordConfig->pass[i] = text[i];
		if (text[i] <= 0) break;
	}
	for(int j = getDBConfigStart(); j < getDBConfigStart() + sizeof(DBConfigRecord); j++)
	{
		EEPROM.write(j, ((uchar*)recordConfig)[contador]);
		contador++;
	}

}

void WebDB::setConfigSessionExpires(uint expires)
{
	recordConfig = &m_config;
	getConfigLogin();
	contador = 0;
	recordConfig->sessionExpires = expires;
	for(int j = getDBConfigStart(); j < getDBConfigStart() + sizeof(DBConfigRecord); j++)
	{
		EEPROM.write(j, ((uchar*)recordConfig)[contador]);
		contador++;
	}
}














int WebDB::getLoginNextId(void)
{

	recordLogin = &m_login;

	for (int i = 0; i < getDBLoginMaxRecords(); i++)
	{
		contador = 0;
		for (unsigned k = 0; k < sizeof(DBLoginRecord); k++) buff[k] = 0;
		for (int j = getDBLoginStart() + (sizeof(DBLoginRecord) * i);
				j < getDBLoginStart() + ((sizeof(DBLoginRecord) * i) + sizeof(DBLoginRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordLogin = (DBLoginRecord*)buff;
		if (recordLogin->id == -1) return i;
	}

	return -1;
}

int WebDB::createLogin(char* _login, char* _pass, ulong timeout)
{
	int _id = getLoginNextId();

	if (_id != -1)
	{
		// Login Records
		//for (int i = 0; i < getDBLoginMaxRecords(); i++)
		//{
		recordLogin = &m_login;

		contador = 0;
		for (unsigned i = 0; i < LOGIN_MAX; i++) recordLogin->login[i] = 0;
		for (unsigned i = 0; i < PASS_MAX; i++) recordLogin->pass[i] = 0;
		recordLogin->id = _id;
		for (unsigned i = 0; i < LOGIN_MAX; i++) {
			recordLogin->login[i] = _login[i];
			if (_login[i] <= 0) break;
		}
		for (unsigned i = 0; i < PASS_MAX; i++) {
			recordLogin->pass[i] = _pass[i];
			if (_login[i] <= 0) break;
		}
		recordLogin->sessionTimeout = timeout;
		for( int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
			j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordLogin)[contador]);
			contador++;
		}
	}

	return _id;
}

void WebDB::deleteLogin(uint _id)
{
	if (_id < getDBLoginMaxRecords())
	{
		recordLogin = &m_login;
		getLoginLogin(_id);
		contador = 0;
		recordLogin->id = -1;
		for (unsigned i = 0; i < LOGIN_MAX; i++) recordLogin->login[i] = 0;
		for (unsigned i = 0; i < PASS_MAX; i++) recordLogin->pass[i] = 0;
		recordLogin->sessionTimeout = 0L;
		for( int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
			j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordLogin)[contador]);
			contador++;
		}
	}
}

int   WebDB::findLogin(char* _login)
{
    recordLogin = &m_login;
    for (int _id = 0; _id < getDBLoginMaxRecords(); _id++)
    {

		contador = 0;
		for (unsigned i = 0; i < sizeof(DBLoginRecord); i++) buff[i] = 0;
		for (int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
				j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordLogin = (DBLoginRecord*)buff;
		if (strncmp(_login, recordLogin->login, LOGIN_MAX) == 0)
            return recordLogin->id;
    }

    return -1;
}





void WebDB::setLoginLogin(uint _id, char *text)
{
	if (_id < getDBLoginMaxRecords())
	{
		// Login Records
		//for (int i = 0; i < getDBLoginMaxRecords(); i++)
		//{
		recordLogin = &m_login;
		getLoginLogin(_id);
		contador = 0;
		for (unsigned i = 0; i < LOGIN_MAX; i++) recordLogin->login[i] = 0;
		for (unsigned i = 0; i < LOGIN_MAX; i++) {
			recordLogin->login[i] = text[i];
			if (text[i] <= 0) break;
		}
		for( int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
			j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordLogin)[contador]);
			contador++;
		}
	}
}

void WebDB::setLoginPass(uint _id, char *text)
{
	if (_id < getDBLoginMaxRecords())
	{
		// Login Records
		//for (int i = 0; i < getDBLoginMaxRecords(); i++)
		//{
		recordLogin = &m_login;
		getLoginLogin(_id);
		contador = 0;
		for (unsigned i = 0; i < LOGIN_MAX; i++) recordLogin->pass[i] = 0;
		for (unsigned i = 0; i < PASS_MAX; i++) {
			recordLogin->pass[i] = text[i];
			if (text[i] <= 0) break;
		}
		for( int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
			j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordLogin)[contador]);
			contador++;
		}
	}
}

void WebDB::setLoginSessionTimeout(uint _id, ulong timeout)
{
	if (_id < getDBLoginMaxRecords())
	{
		// Login Records
		//for (int i = 0; i < getDBLoginMaxRecords(); i++)
		//{
		recordLogin = &m_login;
		getLoginLogin(_id);
		contador = 0;
		recordLogin->sessionTimeout = timeout;
		for( int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
			j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordLogin)[contador]);
			contador++;
		}
	}
}

char* WebDB::getLoginLogin(uint _id)
{
	if (_id < getDBLoginMaxRecords())
	{
		recordLogin = &m_login;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBLoginRecord); i++) buff[i] = 0;
		for (int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
				j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordLogin = (DBLoginRecord*)buff;
		return recordLogin->login;
	}

	return 0;
}

char* WebDB::getLoginPass(uint _id)
{
	if (_id < getDBLoginMaxRecords())
	{
		recordLogin = &m_login;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBLoginRecord); i++) buff[i] = 0;
		for (int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
				j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordLogin = (DBLoginRecord*)buff;
		return recordLogin->pass;
	}

	return 0;
}

ulong WebDB::getLoginSessionTimeout(uint _id)
{
	if (_id < getDBLoginMaxRecords())
	{
		recordLogin = &m_login;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBLoginRecord); i++) buff[i] = 0;
		for (int j = getDBLoginStart() + (sizeof(DBLoginRecord) * _id);
				j < getDBLoginStart() + ((sizeof(DBLoginRecord) * _id) + sizeof(DBLoginRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordLogin = (DBLoginRecord*)buff;
		return recordLogin->sessionTimeout;
	}

	return 0L;
}




































int WebDB::getButtonNextId(void)
{

	recordButton = &m_button;

	for (int i = 0; i < getDBButtonMaxRecords(); i++)
	{
		contador = 0;
		for (unsigned k = 0; k < sizeof(DBButtonRecord); k++) buff[k] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * i);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * i) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;
		if (recordButton->id == -1) return i;
	}

	return -1;
}

int WebDB::createButton(char _type, bool _state, char* _text, char _step, uchar _value)
{
	int _id = getButtonNextId();

	if (_id != -1)
	{
		// Button Records
		//for (int i = 0; i < getDBButtonMaxRecords(); i++)
		//{
		recordButton = &m_button;

		recordButton->id = _id;
		recordButton->type = _type;
		recordButton->state = _state;
		for (unsigned i = 0; i < TEXT_MAX; i++) recordButton->text[i] = 0;
		for (unsigned i = 0; i < TEXT_MAX; i++) {
			recordButton->text[i] = _text[i];
			if (_text[i] <= 0) break;
		}
		recordButton->step = _step;
		recordButton->value = _value;

		contador = 0;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}

	return _id;
}

void WebDB::deleteButton(uint _id)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		getButtonText(_id);

		recordButton->id = -1;
		recordButton->type = -1;
		recordButton->state = false;
		for (unsigned i = 0; i < TEXT_MAX; i++) recordButton->text[i] = 0;
		recordButton->step = -1;
		recordButton->value = 0;

		contador = 0;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}
}

int WebDB::findButton(uint idVal)
{
    recordButton = &m_button;
    for (int _id = 0; _id < getDBButtonMaxRecords(); _id++)
    {

		contador = 0;
		for (unsigned i = 0; i < sizeof(DBButtonRecord); i++) buff[i] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;

		/*if (recordButton->value != 0)
		{
            Serial.print(F("Button Value: "));
            Serial.println(recordButton->value, DEC);
            Serial.print(F("idVal: "));
            Serial.println(idVal, DEC);
        }*/

		if (idVal == recordButton->value)
            return recordButton->id;
    }

    return -1;
}




void WebDB::setButtonText(uint _id, char *text)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		getButtonText(_id);
		contador = 0;
		for (unsigned i = 0; i < TEXT_MAX; i++) recordButton->text[i] = 0;
		for (unsigned i = 0; i < TEXT_MAX; i++) {
			recordButton->text[i] = text[i];
			if (text[i] <= 0) break;
		}
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}
}

char* WebDB::getButtonText(uint _id)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBButtonRecord); i++) buff[i] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;
		return recordButton->text;
	}

	return 0;
}

void WebDB::setButtonState(uint _id, char val)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		getButtonState(_id);
		contador = 0;
		recordButton->state = val;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}
}

uchar WebDB::getButtonState(uint _id)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBButtonRecord); i++) buff[i] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;
		return recordButton->state;
	}

	return 0;
}



void WebDB::setButtonType(uint _id, char val)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		getButtonType(_id);
		contador = 0;
		recordButton->type = val;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}
}

char WebDB::getButtonType(uint _id)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBButtonRecord); i++) buff[i] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;
		return recordButton->type;
	}

	return -1;
}

void WebDB::setButtonValue(uint _id, uchar val)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		getButtonValue(_id);
		contador = 0;
		recordButton->value = val;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}
}

uchar WebDB::getButtonValue(uint _id)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBButtonRecord); i++) buff[i] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;
		return recordButton->value;
	}

	return 0;
}


void WebDB::setButtonStep(uint _id, char val)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		getButtonType(_id);
		contador = 0;
		recordButton->step = val;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}
}

char WebDB::getButtonStep(uint _id)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBButtonRecord); i++) buff[i] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;
		return recordButton->step;
	}

	return -1;
}


void WebDB::setButtonId(uint _id, uchar val)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		getButtonType(_id);
		contador = 0;
		recordButton->value = val;
		for( int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
			j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			EEPROM.write(j, ((uchar*)recordButton)[contador]);
			contador++;
		}
	}
}

uchar WebDB::getButtonId(uint _id)
{
	if (_id < getDBButtonMaxRecords())
	{
		recordButton = &m_button;
		contador = 0;
		for (unsigned i = 0; i < sizeof(DBButtonRecord); i++) buff[i] = 0;
		for (int j = getDBButtonStart() + (sizeof(DBButtonRecord) * _id);
				j < getDBButtonStart() + ((sizeof(DBButtonRecord) * _id) + sizeof(DBButtonRecord)); j++)
		{
			buff[contador] = EEPROM.read(j);
			contador++;
		}
		recordButton = (DBButtonRecord*)buff;
		return recordButton->value;
	}

	return -1;
}




