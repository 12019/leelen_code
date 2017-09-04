
#include "PinyinEngine.h"
#include "public.h"

PinyinEngine::PinyinEngine(const char *table_file,const char *phrase_index_file)
        :m_table(NULL,table_file),m_table_filename(table_file),
         m_phrases_table(phrase_index_file),m_phrase_idx_filename(phrase_index_file)
{
}

PinyinEngine::~PinyinEngine()
{
        save_table();
}

unsigned int PinyinEngine::search(const char* pinyin)
{
        m_key.set_key(pinyin);

        if(isPhrase()){     //拼音组词,isPhrase返回m_key.get_key_count()>1
                unsigned int count=m_phrases_table.find_phrases(m_offset_freq_pairs,m_key);
                m_phrases_table.get_phrases_by_offsets(m_offset_freq_pairs,m_phrases);
                return count;                       //count为找到的词组
        }
        else{            //查找单个字
                return m_table.find_chars(m_chars,m_key.get_key_by_index(0));
        }
        }

QChar PinyinEngine::get_char(unsigned int index)
{
        //if(index>=m_chars.size()) return QChar();
        QChar c(m_chars[index]);
        return c;
}

QString PinyinEngine::get_phrase(unsigned int index)  //取词组
{
        QString str;
        for(unsigned int i = 0;i<m_phrases[index].size();i++){
                str+=QChar(m_phrases[index][i]);         //取出的词组用字符串表示
        }
        return str;
}

void PinyinEngine::hit(unsigned int index)
{
        if(isPhrase()){
                m_phrases_table.set_frequency(m_offset_freq_pairs[index].first,
                                              m_offset_freq_pairs[index].second+1);
        }
        else{
                PinyinKey& key=m_key.get_key_by_index(0);
                uint32 freq=m_table.get_char_frequency(m_chars[index],key)+1;
                m_table.set_char_frequency(m_chars[index],freq,key);
        }
}

void PinyinEngine::save_table()        //储存常用字符
{
        m_table.save_table(m_table_filename.c_str());
        m_phrases_table.save_index(m_phrase_idx_filename.c_str());
        printf("table saved\n");
}

void PinyinEngine::append_phrase(QString& phrase,const char* pinyin)   //显示拼音字母和汉字词组
{
        if(phrase.length()<2)  return;

        String sPinyin(pinyin);
        //trim(sPinyin);

        if(sPinyin.size()==0)  return;

        PhraseString str;
        for(unsigned int i=0;i<phrase.length();i++){
                str.push_back(phrase[i].unicode());
        }
        m_phrases_table.append_phrase(str,sPinyin.c_str());
}

