#ifndef CONFIG_H
#define CONFIG_H

#include<string>
#include<map>
#include <QtCore/QString>

typedef std::map<std::string,std::string>CONFIG_VALUES;

class Config
{
public:
    Config(const char* config_file);
            std::string get_as_string(const char* key_name);
            int get_as_int(const char* key_name);
    private:
            CONFIG_VALUES m_values;
};

#endif // CONFIG_H
