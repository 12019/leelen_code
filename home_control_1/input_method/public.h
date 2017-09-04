#ifndef PUBLIC_H
#define PUBLIC_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<fstream>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <iostream>

#ifdef X86
#define printX86(fmt,args...)       printf(fmt,## args)
#else
#define printX86(fmt,args...)
#endif

std::string trim(std::string &str, char ch = ' ');
typedef std::basic_string<char> String;

#define VERSION		"0.03"
#define FILE_VERSION	"0.03"


#endif // PUBLIC_H
