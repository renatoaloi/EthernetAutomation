/*
  EthernetAutomation.h - Library for Curso Arduino Automation Kit.
  Created by Renato Aloi, Nov, 2014.
  Released into the public domain.
  seriallink.com.br
*/

#include "EthernetAutomation.h"

extern "C" {
static int GetIntFromStringStream(char* c);
};

void EthernetAutomation::begin(uchar* mac, uchar* ip, uint port, bool createDB)
{
	// Initializing DB
	if (!m_db.isDBExists() && createDB) m_db.initDB();

	// Initializing ETH
	m_net.begin(ip, mac);
}

bool EthernetAutomation::available(const char activateLogin)
{
    	bool ret = false;

	if (m_net.available())
	{
		// GET/POST parameters
        	char *params = m_net.getParams();

		Serial.print(F("Parametros: "));
		Serial.println(params);

		if (params[0] == '?')
		{
            		Serial.println(F("Tratando GET!"));
			// GET
			for (unsigned i = 1; i < PARAMS_LEN; i++)
			{
				if (strncmp(&params[i], "button_id=", 10) == 0)
				{
                    			m_lastButtonId = GetIntFromStringStream(&params[i+10]);

					Serial.print(F("Found! Button ID: "));
					Serial.println(m_lastButtonId, DEC);

				}

				if (strncmp(&params[i], "button_value=up", 15) == 0)
				{
					int btid = m_db.findButton(m_lastButtonId);
					if (m_lastButtonValue + m_db.getButtonStep(btid) < 256)
	                    			m_lastButtonValue += m_db.getButtonStep(btid);

					Serial.print(F("Dimmer Up: "));
					Serial.println(m_lastButtonValue, DEC);

				}
				else if (strncmp(&params[i], "button_value=down", 17) == 0)
				{
					int btid = m_db.findButton(m_lastButtonId);
					if (m_lastButtonValue - m_db.getButtonStep(btid) >= 0)
	                    			m_lastButtonValue -= m_db.getButtonStep(btid);

					Serial.print(F("Dimmer Down: "));
					Serial.println(m_lastButtonValue, DEC);

				}
			}


		}
		else if (params[0] > 0)
		{
            		Serial.println(F("Tratando POST!"));
			// POST
			for (unsigned i = 0; i < PARAMS_LEN; i++)
			{
                		// getting login from POST data
				if (strncmp(&params[i], "login=", 6) == 0)
				{
				    //Serial.println(F("Achei login!"));
				    int j = i;
				    int k = 0;

				    //Serial.print(params[j+6]);
				    //Serial.print(params[j+7]);
				    //Serial.print(params[j+8]);

				    while((params[j+6] >= ' ') && (params[j+6] <= '}') && (params[j+6] != '&'))
				    {
				        //Serial.print(params[j+6]);
				        m_login[k] = params[j+6];
				        j++; k++;
				    }
				    break;
				}
			}

			// getting pass from POST data
            		for (unsigned i = 1; i < PARAMS_LEN; i++)
			{
				if (strncmp(&params[i], "pass=", 5) == 0)
				{
                    			Serial.println(F("Achei pass!"));
				    int j = i;
				    int k = 0;
				    //Serial.print(params[j+5]);
				    //Serial.print(params[j+6]);
				    //Serial.print(params[j+7]);
				    //Serial.print(params[j+8]);

				    while((params[j+5] >= ' ') && (params[j+5] <= '}') && (params[j+5] != '&'))
				    {
				        //Serial.print(params[j+5]);
				        m_pass[k] = params[j+5];
				        j++; k++;
				    }
				    break;
				}
            		}


			Serial.print(F("login: "));
			Serial.println(m_login);
			Serial.print(F("pass: "));
			Serial.println(m_pass);

		}

		if (checkLogin()) ret = true;
		else
		{
			close();
		}
		// Return true if login ok!
		//if (strncmp(m_login, "_no_auth_", LOGIN_MAX) == 0) ret = true;
		//else if (m_login[0] == 0) { returnLoginFormToClient(); checkLogin(); }
	}

	return ret;
}

void EthernetAutomation::close(void)
{
    if (strncmp(m_login, "_no_auth_", LOGIN_MAX) == 0) returnHtmlToClient();
    else if (m_login[0] == 0) returnLoginFormToClient();
    else
    {
        if (checkLogin())
        {
            returnHtmlToClient();
        }
        else
        {
            for (unsigned i = 0; i < LOGIN_MAX; i++) m_login[i] = 0;
            for (unsigned i = 0; i < PASS_MAX; i++) m_pass[i] = 0;
            returnLoginFormToClient();
        }
    }
    m_net.close();
    delay(10);
}

void EthernetAutomation::returnLoginFormToClient(void)
{
	//Serial.println(F("returnLoginFormToClient"));

	// HTML Header
	m_net.print((char *)&resp200Txt[0],strlen_P(&resp200Txt[0]));
	m_net.print((char *)&head_ini[0],strlen_P(&head_ini[0]));
	m_net.print((char *)&stylesheet0[0],strlen_P(&stylesheet0[0]));
	m_net.print((char *)&stylesheet1[0],strlen_P(&stylesheet1[0]));
	m_net.print((char *)&stylesheet2[0],strlen_P(&stylesheet2[0]));
	m_net.print((char *)&stylesheet3[0],strlen_P(&stylesheet3[0]));
	m_net.print((char *)&stylesheet4[0],strlen_P(&stylesheet4[0]));
	m_net.print((char *)&stylesheet5[0],strlen_P(&stylesheet5[0]));
	m_net.print((char *)&stylesheet6[0],strlen_P(&stylesheet6[0]));
	m_net.print((char *)&stylesheet7[0],strlen_P(&stylesheet7[0]));
	m_net.print((char *)&stylesheet8[0],strlen_P(&stylesheet8[0]));
	m_net.print((char *)&stylesheet9[0],strlen_P(&stylesheet9[0]));
	m_net.print((char *)&stylesheet10[0],strlen_P(&stylesheet10[0]));
	m_net.print((char *)&stylesheet11[0],strlen_P(&stylesheet11[0]));
	m_net.print((char *)&head_fim[0],strlen_P(&head_fim[0]));
	m_net.print((char *)&div_ini[0],strlen_P(&div_ini[0]));

	// HTML Login Form
	m_net.print((char *)&formOpenTag[0],strlen_P(&formOpenTag[0]));
	m_net.print((char *)&formLabelLogin[0],strlen_P(&formLabelLogin[0]));
	m_net.print((char *)&formInputLogin[0],strlen_P(&formInputLogin[0]));
	m_net.print((char *)&formLabelSenha[0],strlen_P(&formLabelSenha[0]));
	m_net.print((char *)&formInputSenha[0],strlen_P(&formInputSenha[0]));
	m_net.print((char *)&formInputButton[0],strlen_P(&formInputButton[0]));
	m_net.print((char *)&formCloseTag[0],strlen_P(&formCloseTag[0]));

	// Closing HTML
	m_net.print((char *)&div_fim[0],strlen_P(&div_fim[0]));

	// HTML Header
	/*printP(m_net, head_ini);
	printP(m_net, stylesheet);
	printP(m_net, head_fim);
	printP(m_net, div_ini);

	// here goes the form
	printP(m_net, formOpenTag);
	printP(m_net, formLabelLogin);
	printP(m_net, formInputLogin);
	printP(m_net, formLabelSenha);
	printP(m_net, formInputSenha);
	printP(m_net, formInputButton);
	printP(m_net, formCloseTag);

	// HTML Footer
	printP(m_net, div_fim);*/
}

void EthernetAutomation::returnHtmlToClient(void)
{
	int btid = m_db.findButton(m_lastButtonId);
	//Serial.println(F("returnHtmlToClient"));

	// HTML Header
	m_net.print((char *)&resp200Txt[0],strlen_P(&resp200Txt[0]));
	m_net.print((char *)&head_ini[0],strlen_P(&head_ini[0]));
	m_net.print((char *)&stylesheet0[0],strlen_P(&stylesheet0[0]));
	m_net.print((char *)&stylesheet1[0],strlen_P(&stylesheet1[0]));
	m_net.print((char *)&stylesheet2[0],strlen_P(&stylesheet2[0]));
	m_net.print((char *)&stylesheet3[0],strlen_P(&stylesheet3[0]));
	m_net.print((char *)&stylesheet4[0],strlen_P(&stylesheet4[0]));
	m_net.print((char *)&stylesheet5[0],strlen_P(&stylesheet5[0]));
	m_net.print((char *)&stylesheet6[0],strlen_P(&stylesheet6[0]));
	m_net.print((char *)&stylesheet7[0],strlen_P(&stylesheet7[0]));
	m_net.print((char *)&stylesheet8[0],strlen_P(&stylesheet8[0]));
	m_net.print((char *)&stylesheet9[0],strlen_P(&stylesheet9[0]));
	m_net.print((char *)&stylesheet10[0],strlen_P(&stylesheet10[0]));
	m_net.print((char *)&stylesheet11[0],strlen_P(&stylesheet11[0]));
	m_net.print((char *)&head_fim[0],strlen_P(&head_fim[0]));
	m_net.print((char *)&div_ini[0],strlen_P(&div_ini[0]));

	
	// here goes the automation web panel
	// Verificando o tipo de botao
	if (btid != -1)
	{
		if (m_db.getButtonType(btid) == ONOFF_BUTTON)
		{
			m_db.setButtonState(btid, (m_db.getButtonState(btid) ? 0 : 1));
		}
		else if (m_db.getButtonType(btid) == DIMMER_BUTTON)
		{
			//if (m_db.getButtonState(btid) == 1)
			//{
			//	if (m_db.getButtonValue(btid) + m_db.getButtonStep(btid) < 255)
			//		m_db.setButtonValue(btid, m_db.getButtonValue(btid) + m_db.getButtonStep(btid));
			//	else
					m_db.setDimmerValue(btid,  m_lastButtonValue);
			//}
			//else if (m_db.getButtonState(btid) == 2)
			//{
                //if (m_db.getButtonValue(btid) - m_db.getButtonStep(btid) > 0)
			//		m_db.setButtonValue(btid, m_db.getButtonValue(btid) - m_db.getButtonStep(btid));
			//	else
			//		m_db.setButtonValue(btid,  0);
			//}
		}


	}

	for (unsigned i = 0; i < m_db.getDBButtonMaxRecords(); i++)
	{
		if (m_db.getButtonType(i) != -1)
		{
			if (m_db.getButtonType(i) == DIMMER_BUTTON)
			{
				m_net.print((char *)&dimmer_ini1[0],strlen_P(&dimmer_ini1[0]));
				//printP(m_net, dimmer_ini1);
				m_net.print(m_db.getButtonText(i));

				// converting to percent
				int val1 = map(m_db.getButtonValue(i), 0, 255, 0, 100);
				m_net.print(val1);
				m_net.print("%");

				//printP(m_net, dimmer_ini2);
				m_net.print((char *)&dimmer_ini2[0],strlen_P(&dimmer_ini2[0]));

				// link do dimmer DOWN
				m_net.print((char *)&btnid[0],strlen_P(&btnid[0]));
				//printP(m_net, btnid);
				m_net.print(m_db.getButtonId(i));
				m_net.print((char *)&dimmerdown[0],strlen_P(&dimmerdown[0]));
				//printP(m_net, dimmerdown);
				m_net.print((char *)&dimmer_mid11[0],strlen_P(&dimmer_mid11[0]));
				//printP(m_net, dimmer_mid11);
				m_net.print((char *)&colorgreen[0],strlen_P(&colorgreen[0]));
				//printP(m_net, colorgreen);
				m_net.print((char *)&dimmer_mid12[0],strlen_P(&dimmer_mid12[0]));
				//printP(m_net, dimmer_mid12);
				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print("-");


				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print((char *)&dimmer_mid2[0],strlen_P(&dimmer_mid2[0]));
				//printP(m_net, dimmer_mid2);
			

				// link do dimmer UP
				m_net.print((char *)&btnid[0],strlen_P(&btnid[0]));
				//printP(m_net, btnid);
				m_net.print(m_db.getButtonId(i));
				m_net.print((char *)&dimmerup[0],strlen_P(&dimmerup[0]));
				//printP(m_net, dimmerup);
				m_net.print((char *)&dimmer_mid21[0],strlen_P(&dimmer_mid21[0]));
				//printP(m_net, dimmer_mid21);
				m_net.print((char *)&colorgreen[0],strlen_P(&colorgreen[0]));
				//printP(m_net, colorgreen);
				m_net.print((char *)&dimmer_mid22[0],strlen_P(&dimmer_mid22[0]));
				//printP(m_net, dimmer_mid22);
				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print("+");


				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print((char *)&dimmer_space[0],strlen_P(&dimmer_space[0]));
				//printP(m_net, dimmer_space);
				m_net.print((char *)&dimmer_fim[0],strlen_P(&dimmer_fim[0]));
				//printP(m_net, dimmer_fim);







			}
			else
			{
				//printP(m_net, button_ini);
				m_net.print((char *)&button_ini[0],strlen_P(&button_ini[0]));

				// link do botao
				//printP(m_net, btnid);
				m_net.print((char *)&btnid[0],strlen_P(&btnid[0]));
				m_net.print(m_db.getButtonId(i));
				m_net.print((char *)&button_mid1[0],strlen_P(&button_mid1[0]));
				//printP(m_net, button_mid1);

				// cor do botao
				if (m_db.getButtonState(i) == 1)
				{
					m_net.print((char *)&colorred[0],strlen_P(&colorred[0]));
					//printP(m_net, colorred);
				}
				else
				{
					m_net.print((char *)&colorblue[0],strlen_P(&colorblue[0]));
					//printP(m_net, colorblue);
				}
				m_net.print((char *)&button_mid2[0],strlen_P(&button_mid2[0]));
				//printP(m_net, button_mid2);

				// texto do botao
				m_net.print(m_db.getButtonText(i));
				m_net.print((char *)&button_fim[0],strlen_P(&button_fim[0]));
				//printP(m_net, button_fim);
			}
		}
	}

	// Closing HTML
	m_net.print((char *)&div_fim[0],strlen_P(&div_fim[0]));

}

bool EthernetAutomation::checkLogin(void)
{
    //Serial.println(F("checkLogin"));

    if (m_login[0] == 0) return false;

    if (strncmp(m_login, m_db.getConfigLogin(), LOGIN_MAX) == 0) return checkAdmin();

    int id = m_db.findLogin(m_login);
    if (id != -1)
        return (strncmp(m_pass, m_db.getLoginPass(id), PASS_MAX) == 0);
    else
        return false;

}

bool EthernetAutomation::checkAdmin(void)
{
    //Serial.println(F("checkAdmin"));

    return (strncmp(m_pass, m_db.getConfigPass(), PASS_MAX) == 0);

}

static int GetIntFromStringStream(char* c)
{
    // returns -1 if fails
    int ret = -1;
    unsigned i = 0;
    while(c[i] >= '0' && c[i] <= '9')
    {
        if (ret==-1) ret=0;
        ret = (ret * 10) + (c[i] - '0');
        i++;
    }
    return ret;
}




















/*
void EthernetAutomation::getInfoFromDB(void)
{
	Serial.println();
	Serial.println();

	Serial.println(F("EEPROM Size"));
	Serial.print(F("Size: "));
	Serial.println(m_db.getEEPROMSize(), DEC);
	Serial.println();

	Serial.println(F("Starts"));
	Serial.print(F("DBConfigStart: "));
	Serial.println(m_db.getDBConfigStart(), DEC);
	Serial.print(F("DBLoginStart: "));
	Serial.println(m_db.getDBLoginStart(), DEC);
	Serial.print(F("DBButtonStart: "));
	Serial.println(m_db.getDBButtonStart(), DEC);
	Serial.println();

	Serial.println(F("Sizes"));
	Serial.print(F("DBConfigRecordSize: "));
	Serial.println(m_db.getDBConfigRecordSize(), DEC);
	Serial.print(F("DBLoginRecordSize: "));
	Serial.println(m_db.getDBLoginRecordSize(), DEC);
	Serial.print(F("DBButtonRecordSize: "));
	Serial.println(m_db.getDBButtonRecordSize(), DEC);
	Serial.println();

	Serial.println(F("Max's"));
	Serial.print(F("DBConfigMaxRecords: "));
	Serial.println(m_db.getDBConfigMaxRecords(), DEC);
	Serial.print(F("DBLoginMaxRecords: "));
	Serial.println(m_db.getDBLoginMaxRecords(), DEC);
	Serial.print(F("DBButtonMaxRecords: "));
	Serial.println(m_db.getDBButtonMaxRecords(), DEC);
	Serial.println();

	Serial.print(F("DB Existe? "));
	Serial.println(m_db.isDBExists() ? "Sim" : "Nao");

	if (!m_db.isDBExists()) {
		Serial.println(F("Iniciando banco de dados..."));
		m_db.initDB();
	}

	Serial.print(F("E agora? "));
	Serial.println(m_db.isDBExists() ? "Sim" : "Nao");


}


void EthernetAutomation::getInfoFromRootLogin(void)
{
	Serial.println();
	Serial.print(F("Root login: "));
	Serial.println(m_db.getConfigLogin());
}

void EthernetAutomation::getInfoFromRootPass(void)
{
	Serial.println();
	Serial.print(F("Root senha: "));
	Serial.println(m_db.getConfigPass());
}

void EthernetAutomation::getInfoLogin(uint _id)
{
	Serial.println();
	Serial.print(F("Login Id: "));
	Serial.println(_id, DEC);
	Serial.print(F("Login: "));
	Serial.println(m_db.getLoginLogin(_id));
	Serial.print(F("Pass: "));
	Serial.println(m_db.getLoginPass(_id));
	Serial.print(F("SessionTimeout: "));
	Serial.println(m_db.getLoginSessionTimeout(_id));
}
*/

