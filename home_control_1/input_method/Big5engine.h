#ifndef BIG5ENGINE_H
#define BIG5ENGINE_H
#include <QtCore/qstring.h>
#include "phrase/Big5phrase.h"
#include "scim/Big5pinyin_char.h"

typedef std::basic_string<char> String;

class Big5Engine
{
public:
    Big5Engine(const char *table_file,const char *phrase_index_file);
    ~Big5Engine();

    unsigned int search(const char* pinyin);
    inline unsigned int get_char_count(){
            return m_chars.size();
    }
    QChar get_char(unsigned int index);
    QString get_phrase(unsigned int index);
    void hit(unsigned int index);
    void save_table();
    void append_phrase(QString&,const char* pinyin);
    inline bool isPhrase(){
            return m_key.get_key_count()>1;
    }
    inline String get_formatted_pinyin(){
            return m_key.get_key_string();
    }
    inline void clear_key(){
            m_key.clear_key();
    }
private:
    //PinyinKeyVector   m_keys;
    PinyinPhraseKey   m_key;

    //single hanzi                                  //单个汉字
    PinyinTable m_table;
    CharVector  m_chars;                            //字符向量,在Big5pinyin_char.h中定义CharVector类
    String      m_table_filename;

    //phrase                                        //词组
    PinyinPhraseTable               m_phrases_table;
    PhraseOffsetFrequencyPairVector m_offset_freq_pairs;
    PhraseStringVector              m_phrases;
    String                          m_phrase_idx_filename;
};

#endif // BIG5ENGINE_H
