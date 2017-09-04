#ifndef PINYINENGINE_H
#define PINYINENGINE_H

#include <QtCore/qstring.h>
#include "scim/Big5pinyin_char.h"
#include "phrase/Big5phrase.h"

class PinyinEngine
{
public:
        PinyinEngine(const char *table_file,const char *phrase_index_file);
        ~PinyinEngine();

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

        //single hanzi
        PinyinTable m_table;
        CharVector  m_chars;
        String      m_table_filename;

        //phrase
        PinyinPhraseTable               m_phrases_table;
        PhraseOffsetFrequencyPairVector m_offset_freq_pairs;
        PhraseStringVector              m_phrases;
        String                          m_phrase_idx_filename;
};

#endif // PINYINENGINE_H
