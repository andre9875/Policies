#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include "nxm_timelib.h"

namespace nxm_mail{

class SendMail
{
   public:
    void send_plain_email(std::string*,std::string*,std::string*);
    void send_html_email(std::string*,std::string*,std::string*,std::string*);
    void send_attach_email(std::string*,std::string*,std::string*);
    void write_error_log(const std::string*,std::string*, std::string*);
    void write_runtime_log(const std::string*,std::string*, std::string*);
   private:
    int status;
  };

};
#endif
