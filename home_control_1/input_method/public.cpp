
#include "public.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<fstream>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <iostream>

 std::string trim(std::string &str, char ch)   //删除空白字符
{
        if(str.length() == 0){
                return str;
        }

        int sPos = str.find_first_not_of(ch);
        int ePos = str.find_last_not_of(ch);
        if(sPos == -1){
                return str.erase();
        }

        return str.substr(sPos, ePos - sPos + 1);
}

