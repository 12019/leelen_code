#include "phrase/Big5phrase.h"
#include "public.h"

#include <map>

/////////////////////////////////////////////////////////////////////////
//utf8 <=> ucs-2 convertion
//
//from linux kernel fs/nls/nls_base.c
extern int utf8_mbtowc(ucs4_t *p, const __u8 *s, int n);
int utf8_mbstowcs(ucs4_t *pwcs, const __u8 *s, int n)
{
        __u16 *op;
        const __u8 *ip;
        int size;

        op = pwcs;
        ip = s;
        while (*ip && n > 0) {
                if (*ip & 0x80) {
                        size = utf8_mbtowc(op, ip, n);
                        if (size == -1) {
                                /* Ignore character and move on */
                                ip++;
                                n--;
                        } else {
                                op++;
                                ip += size;
                                n -= size;
                        }
                } else {
                        *op++ = *ip++;
                }
        }
        return (op - pwcs);
}

extern int utf8_wctomb(__u8 *s, ucs4_t wc, int maxlen);
int utf8_wcstombs(__u8 *s, const ucs4_t *pwcs, int maxlen)
{
        const __u16 *ip;
        __u8 *op;
        int size;

        op = s;
        ip = pwcs;
        while (*ip && maxlen > 0) {
                if (*ip > 0x7f) {
                        size = utf8_wctomb(op, *ip, maxlen);
                        if (size == -1) {
                                /* Ignore character and move on */
                                maxlen--;
                        } else {
                                op += size;
                                maxlen -= size;
                        }
                } else {
                        *op++ = (__u8) *ip;
                }
                ip++;
        }
        return (op - s);
}

std::ostream& utf8_write_phase_string(std::ostream& os,PhraseString& str)
{
        std::vector<__u8> s;
        int size=str.size()*4;
        s.reserve(size); //utf8
        ucs4_t*p=&str[0];
        int len=utf8_wcstombs(&s[0],p,size);
        for(int i=0;i<len;i++){
                os<<s[i];
        }
        return os;
}

/////////////////////////////////////////////////////////////////////////
// overrided operators
//
std::ifstream& operator >> (std::ifstream& is, uint32& value)
{
        char buf[sizeof(uint32)];
        for(unsigned int i=0;i<sizeof(buf);i++){
                is>>buf[i];
        }

        value=*((uint32*)buf);

        return is;
}

std::fstream& operator << (std::fstream& os, uint32& value)
{
        char *p=(char*)(&value);
        os.write(p,sizeof(uint32));
        return os;
}

bool operator == (PhraseString& left,PhraseString& right)
{
        unsigned int size=std::min(left.size(),right.size());
        for(unsigned int i=0;i<size&&left[i]!=0&&right[i]!=0;i++){
                if(left[i]!=right[i])  return false;
        }
        return true;
}



///////////////////////////////////////////////////////////////////////////////////
//

static const char murphytalk_Phrase_index_header [] = "MurphyTalk Pinyin Phrase Index Table";
static const char murphytalk_Phrase_index_version[] = "Ver "FILE_VERSION;
static const char murphutalk_Phrase_file         [] =
"/home/myP/murphypinyindata/murphyPinyin/murphytalk_phrase.dat";




PinyinPhraseTable::PinyinPhraseTable(const char* indexfile)
{
        load_index(indexfile);
}

PinyinPhraseTable::~PinyinPhraseTable()
{
}

bool PinyinPhraseTable::load_index(const char* indexfile)
{
        std::ifstream ifs(indexfile);
        if (!ifs) return false;
        if (input (ifs) && m_phrases.size () != 0) return true;
        return false;
}

bool PinyinPhraseTable::save_index(const char* indexfile)
{
        std::fstream fs(murphutalk_Phrase_file,std::ios::in|std::ios::out);
        if (!fs) return false;

        printX86("updating phrase dat file for frequencies ...\n");
        //update changed frequencies to phrase file
        for(PhraseOffsetToFreqMap::iterator pos=m_recent_hit_cache.begin();
            pos!=m_recent_hit_cache.end();pos++){
                printX86("from hit cache -> offset %d,freq %d\n",pos->first,pos->second);
                fs.seekp(pos->first,std::ios::beg);
                fs<<pos->second;
        }
        m_recent_hit_cache.clear();

        //append new phrases keys and offsets in phrase file to index
        std::ofstream out(indexfile,std::ios::app|std::ios::out);
        if (!out) return false;

        printX86("updating phrase index ...\n");
        for(PinyinPhraseEntryVector::iterator pos=m_new_phrases_cache.begin();
            pos!=m_new_phrases_cache.end();pos++){
                pos->output_text(out);
        }
        m_new_phrases_cache.clear();

        return true;
}

#if 0
//rewrite the whole phrase index file,merge phrase entries with same key into the same line
bool PinyinPhraseTable::output (std::ostream &os)
{
        os<<murphytalk_Phrase_index_header<<std::endl;
        os<<murphytalk_Phrase_index_version<<std::endl;
        os<<m_phrases.size()<<std::endl;

        for(PinyinPhraseEntryVector::iterator pos=m_phrases.begin();pos!=m_phrases.end();pos++){
                pos->output_text(os);
        }
        return true;
}
#endif

bool PinyinPhraseTable::input (std::istream &is)
{
        char header [40];

        if (!is) return false;

        is.getline (header, 40);
        if (strncmp (header,
                murphytalk_Phrase_index_header,
                strlen (murphytalk_Phrase_index_header)) != 0) {
                        std::cout<<"invalidate Phrase table index file"<<std::endl;
                        return false;
        }

        is.getline (header, 40);
        if (strncmp (header,
                murphytalk_Phrase_index_version,
                strlen (murphytalk_Phrase_index_version)) != 0) {
                        std::cout<<"invalidate Phrase table index file version:"<<std::endl;
                        return false;
        }

        typedef std::map<String,unsigned int> ENTRY_MAP;
        ENTRY_MAP lookup;
        ENTRY_MAP::iterator lookup_pos;

 #ifdef DEBUG
        uint32 i=0;
 #endif
        while(!is.eof())
        {
                PinyinPhraseEntry phrase_entry(is);

                if(phrase_entry.isValid()){
                        //is entry with same key avaliable?
                        String keystr=phrase_entry.get_key().get_key_string();
                        lookup_pos=lookup.find(keystr);
                        if(lookup_pos!=lookup.end()){
                                //got you!
                                printX86("-> %s ...",keystr.c_str());
                                //lookup_pos->second->append(phrase_entry);
                                m_phrases[lookup_pos->second].append(phrase_entry);
                                printX86("OK\n");
                        }
                        else{
                                m_phrases.push_back(phrase_entry);
                                //PinyinPhraseEntryVector::iterator last=m_phrases.end();
                                //last--;
                                lookup[keystr]=m_phrases.size()-1;
                        }
                }

#ifdef DEBUG
                i++;
#endif
                if(!is||is.eof()){
#ifdef DEBUG
                        if(!is){
                                printf("No.%d,exception\n",i);
                        }
#endif
                        break;
                }
        }

        std::sort(m_phrases.begin(),m_phrases.end(),PinyinPhraseKeyLessThan());
#ifdef DEBUG
        for(PinyinPhraseEntryVector::iterator pos=m_phrases.begin();pos!=m_phrases.end();pos++){
                printf("%s\n",pos->get_key().get_key_string().c_str());
        }
        printf("%d Phrases entries sorted\n",m_phrases.size());
#endif
        m_last_matched_phrases_range.first=m_last_matched_phrases_range.second=m_phrases.end();
        return true;
}

void PinyinPhraseTable::insert(PinyinPhraseEntry& phrase)
{
        m_phrases.push_back(phrase);
        std::sort(m_phrases.begin(),m_phrases.end(),PinyinPhraseKeyLessThan());
}

unsigned int PinyinPhraseTable::find_phrases(PhraseOffsetFrequencyPairVector& phrases,PinyinPhraseKey& pinyin)
{
        phrases.clear ();

        PinyinPhraseEntryVectorPosRangePair range =
                std::equal_range(m_phrases.begin(), m_phrases.end(), pinyin,PinyinPhraseKeyLessThan());

        m_last_matched_phrases_range=range;

        for (PinyinPhraseEntryVector::const_iterator i = range.first; i!= range.second; i++) {
                PinyinPhraseEntry& entry=const_cast<PinyinPhraseEntry&>(*i);
#ifdef X86
                entry.get_key();
                String s=entry.get_key().get_key_string();
                printf("%s\n",s.c_str());
#endif
                entry.get_all_phrases(phrases);
        }

        if (!phrases.size ()) return 0;

        return phrases.size ();
}


unsigned int PinyinPhraseTable::get_phrases_by_offsets(PhraseOffsetFrequencyPairVector& phrases_pair,
                                                       PhraseStringVector& strs)
{
        std::ifstream in(murphutalk_Phrase_file);
        if(!in){
                return 0;
        }
        else{
                ucs4_t*      p;
                PhraseString temp;
                String       raw;
                uint32       freq;

                //read(from file or file hit cache) frequncies first
                for(PhraseOffsetFrequencyPairVector::iterator pos=phrases_pair.begin();
                    pos!=phrases_pair.end();pos++){
                        //move to the right position
                        in.seekg(pos->first,std::ios::beg);
                        printX86("moved to offset %d ... ",(unsigned int)in.tellg());
                        //read the frequency
                        in>>freq;
                        printX86("freq in file is %d,file get position is %u\n",freq,(unsigned int)in.tellg());

                        //have we inputed this phrase(or:hit this phrase) before?
                        //check our recent hit cache
                        PhraseOffsetToFreqMap::iterator pos_cache = m_recent_hit_cache.find(pos->first);
                        if(pos_cache==m_recent_hit_cache.end()){
                                //not cached,use freq read from file
                                pos->second=freq;
                        }
                        else{
                                //use cached freq
                                pos->second=pos_cache->second;
                                printX86("use cached freq = %d\n",pos_cache->second);
                        }

                }

                //sort by frequency
                std::sort (phrases_pair.begin (), phrases_pair.end (), PhraseOffsetFrequencyPairGreaterThanByFrequency ());

                //now read phrase strings - in the right freqency order
                strs.clear();
                for(PhraseOffsetFrequencyPairVector::iterator pos=phrases_pair.begin();
                    pos!=phrases_pair.end();pos++){
                        //move to the right position
                        in.seekg(pos->first+sizeof(PhraseOffset),std::ios::beg);
                        printX86("get phrase : moved to offset %d\n",(unsigned int)in.tellg());
                        //read the phrase text,which is encoded in utf8
                        in>>raw;
                        //make enough room
                        temp.reserve(raw.size());
                        p=&temp[0];
                        //all set to zero
                        memset(p,0,raw.size());
                        int len=utf8_mbstowcs(p,(const __u8 *)raw.c_str(),raw.size());
                        strs.push_back(PhraseString(p,p+len));
                }
                return strs.size();
        }
}

bool PinyinPhraseTable::append_phrase(PhraseString& str,const char* pinyin)
{
        PinyinPhraseEntry entry(pinyin);

        PinyinPhraseEntryVector::iterator pos=std::find(m_phrases.begin(),m_phrases.end(),entry);
        if(pos!=m_phrases.end()){
                //same entry
                //we have same pinyin here,have to make sure we don't have the exactly same phrase

                //read the matched phrases from file
                PhraseStringVector strs;
                PhraseOffsetFrequencyPairVector temp_phrases;
                for(PhraseOffsetVector::iterator i=pos->m_phrases.begin();i!=pos->m_phrases.end();i++){
                        temp_phrases.push_back(std::make_pair(*i,0));
                }

                get_phrases_by_offsets(temp_phrases,strs);
                for(PhraseStringVector::iterator pos_str=strs.begin();pos_str!=strs.end();pos_str++){
                        if(*pos_str==str){
                                //oops,already got this phrase
                                printX86("duplicated phrase\n");
                                return false;
                        }
                }
        }

        //append phrase to phrase lib file
        std::ofstream out(murphutalk_Phrase_file,std::ios::app|std::ios::out);
        if(!out){
                return false;
        }
        else{
                //out.seekp(0,std::ios::end);
                PhraseOffset offset = out.tellp();
                //save offset to phrase entry
                entry.insert(offset);

                printX86("new phrase %s,offset is %u\n",pinyin,offset);
                if(pos!=m_phrases.end()){
                        pos->insert(offset);
                }
                else{
                        insert(entry);
                }

                //put into new phrase cache
                m_new_phrases_cache.push_back(entry);

                //first of all output a frequency of 1
                uint32 freq=1;
                out.write((char*)&freq,sizeof(freq));

                //this space is used to seperate this phrase with the next one
                str.push_back(0x0020);
                //terminate the string
                str.push_back(0);
                //convert to utf8 and append to file
                utf8_write_phase_string(out,str);

                return true;
        }
}

void PinyinPhraseTable::set_frequency(uint32 offset,uint32 freq)
{
#if 0
        if(m_last_matched_phrases_range.first!=m_phrases.end()&&
           m_last_matched_phrases_range.second!=m_phrases.end()){
                for (PinyinPhraseEntryVector::const_iterator i = m_last_matched_phrases_range.first;
                     i!= m_last_matched_phrases_range.second; i++) {
                        PinyinPhraseEntry& entry=const_cast<PinyinPhraseEntry&>(*i);
                        if(entry.set_frequency(offset,freq))  break;
                }
        }
#else
        printX86("PinyinPhraseTable::set_frequency(%d,%d)\n",offset,freq);

        //has this phrase's frequency been cached yet?
        PhraseOffsetToFreqMap::iterator pos_cache = m_recent_hit_cache.find(offset);
        if(pos_cache==m_recent_hit_cache.end()){
                m_recent_hit_cache[offset]=freq;
        }
        else{
                printX86("found in hit cache\n");
                pos_cache->second=freq;
        }
#endif
}

PinyinPhraseEntry::PinyinPhraseEntry(std::istream &is)
{
        input_text(is);
}

PinyinPhraseEntry::PinyinPhraseEntry(const char*pinyin)
{
        m_key.set_key(pinyin);
}

PinyinPhraseEntry::PinyinPhraseEntry(PinyinPhraseKey& key):m_key(key)
{
}

PinyinPhraseEntry::~PinyinPhraseEntry()
{
}

#if 0
bool PinyinPhraseEntry::set_frequency(uint32 offset,uint32 freq)
{
        for(PhraseOffsetVector::iterator pos=m_phrases.begin();pos!=m_phrases.end();pos++){
                if(*pos==offset){
                        pos->second=freq;
                        return true;
                }
        }
#ifdef X86
        printf("offset %d -> not found this offset in last matched entry\n",offset);
#endif
        return false;
}
#endif

std::istream& PinyinPhraseEntry::input_text (std::istream &is)
{
        String keystr;

        is>>keystr;

        m_key.set_key(keystr.c_str());

        if(m_key.isValid()){
                PhraseOffset offset;

                int count;
                is>>count;
                for(int i=0;i<count;i++){
                        is>>offset;

                        insert(offset);
                }
        }

        return is;
}

std::ostream& PinyinPhraseEntry::output_text(std::ostream &os)
{
        for(PinyinKeyVector::iterator pos=m_key.m_keys.begin();pos!=m_key.m_keys.end();pos++){
                os<<pos->get_key_string();
        }
        os<<"\t"<<m_phrases.size()<<"\t";
        for(PhraseOffsetVector::iterator pos=m_phrases.begin();pos!=m_phrases.end();pos++){
                os<<*pos<<" ";
        }
        os<<std::endl;
        return os;
}

unsigned int PinyinPhraseEntry::get_all_phrases(PhraseOffsetFrequencyPairVector& vect)
{
        for(PhraseOffsetVector::iterator pos=m_phrases.begin();pos!=m_phrases.end();pos++){
                vect.push_back(std::make_pair(*pos,0));
        }
        return 0;
}

PinyinPhraseKey::PinyinPhraseKey()
{
}

PinyinPhraseKey::PinyinPhraseKey(const char *keystr)
{
        set_key(keystr);
}

PinyinPhraseKey::~PinyinPhraseKey()
{
}

void PinyinPhraseKey::set_key(const char *keystr)
{
        PinyinKey::parse_pinyin_key(scim_default_pinyin_validator,m_keys,keystr);
}

String PinyinPhraseKey::get_key_string()
{
        String s;
        for(unsigned int i=0;i<m_keys.size();i++){
                s+=m_keys[i].get_key_string()+" ";
        }
        return s;
}
