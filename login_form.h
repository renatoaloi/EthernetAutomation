
/*
  login_form.h - HTML strings for EthernetSup.
  Created by Renato Aloi, Jun 19, 2014.
  Released into the public domain.
*/

P(formOpenTag) = "<FORM method='POST'>\r\n";
P(formCloseTag) = "</FORM>\r\n";

//P(label_ini) = "<p align='center'><span class='normal painel grey' align='center'>";
//P(label_fim) = "</span></p>";

P(formLabelLogin) = "<p align='center'><span class='normal painel grey' align='center'>login: ";
P(formInputLogin) = "<INPUT type=text size=20 name=login id=login /></span></p>\r\n";
P(formLabelSenha) = "<p align='center'><span class='normal painel grey' align='center'>senha: ";
P(formInputSenha) = "<INPUT type=password size=20 name=pass id=pass /></span></p>\r\n";
P(formLabelExpires) = "<p align='center'><span class='normal painel grey' align='center'>expirar (em minutos): ";
P(formInputExpires) = "<INPUT type=text size=5 name=expires id=expires value=30 /></span></p>\r\n";

P(formInputButton) = "<p align='center'><INPUT type=submit name=button id=button value=Enviar class='super button' ></p>\r\n";

//P(button_ini) = "<p align='center'><a href='";
//P(button_mid1) = "' class='super button ";
//P(button_mid2) = "'>";
//P(button_fim) = "</a></p>";
