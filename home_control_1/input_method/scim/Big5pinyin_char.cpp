#define Uses_STL_AUTOPTR
#define Uses_STL_FUNCTIONAL
#define Uses_STL_VECTOR
#define Uses_STL_IOSTREAM
#define Uses_STL_FSTREAM
#define Uses_STL_ALGORITHM
#define Uses_STL_MAP
#define Uses_STL_UTILITY
#define Uses_STL_IOMANIP
#define Uses_C_STDIO
#define Uses_SCIM_UTILITY
#define Uses_SCIM_SERVER
#define Uses_SCIM_ICONV
#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_CONFIG_PATH
#define Uses_SCIM_LOOKUP_TABLE

//#include <scim.h>
#include <stdio.h>
#include "scim/Big5pinyin_char.h"

struct utf8_table {
        int     cmask;
        int     cval;
        int     shift;
        long    lmask;
        long    lval;
};

static struct utf8_table utf8_table[] =                     //以utf－8编码方式保存
{
    {0x80,  0x00,   0*6,    0x7F,           0,         /* 1 byte sequence */},
    {0xE0,  0xC0,   1*6,    0x7FF,          0x80,      /* 2 byte sequence */},
    {0xF0,  0xE0,   2*6,    0xFFFF,         0x800,     /* 3 byte sequence */},
    {0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,   /* 4 byte sequence */},
    {0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,  /* 5 byte sequence */},
    {0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000, /* 6 byte sequence */},
    {0,						       /* end of table    */}
};

int
utf8_mbtowc(ucs4_t *p, const __u8 *s, int n)
{
        long l;
        int c0, c, nc;
        struct utf8_table *t;

        nc = 0;
        c0 = *s;
        l = c0;
        for (t = utf8_table; t->cmask; t++) {
                nc++;
                if ((c0 & t->cmask) == t->cval) {
                        l &= t->lmask;
                        if (l < t->lval)
                                return -1;
                        *p = l;
                        return nc;
                }
                if (n <= nc)
                        return -1;
                s++;
                c = (*s ^ 0x80) & 0xFF;
                if (c & 0xC0)
                        return -1;
                l = (l << 6) | c;
        }
        return -1;
}

int
utf8_wctomb(__u8 *s, ucs4_t wc, int maxlen)
{
        long l;
        int c, nc;
        struct utf8_table *t;

        if (s == 0)
                return 0;

        l = wc;
        nc = 0;
        for (t = utf8_table; t->cmask && maxlen; t++, maxlen--) {
                nc++;
                if (l <= t->lmask) {
                        c = t->shift;
                        *s = t->cval | (l >> c);
                        while (c > 0) {
                                c -= 6;
                                s++;
                                *s = 0x80 | ((l >> c) & 0x3F);
                        }
                        return nc;
                }
        }
        return -1;
}

std::ostream &
utf8_write_wchar (std::ostream &os, ucs4_t wc)
{
    unsigned char utf8[6];
    int count = 0;

    if ((count=utf8_wctomb (utf8, wc, 6)) > 0)
        os.write ((char*)utf8, count * sizeof (unsigned char));

    return os;
}

/*
// Internal functions
static int
__scim_pinyin_compare_initial (const PinyinCustomSettings &custom,
                                                           PinyinInitial lhs,
                                                           PinyinInitial rhs);

static int
__scim_pinyin_compare_final (const PinyinCustomSettings &custom,
                                                         PinyinFinal lhs,
                                                         PinyinFinal rhs);

static int
__scim_pinyin_compare_tone (const PinyinCustomSettings &custom,
                                                        PinyinTone lhs,
                                                        PinyinTone rhs);
*/

// Data definition

static const char scim_pinyin_table_text_header [] = "SCIM_Pinyin_Table_TEXT";
static const char scim_pinyin_table_binary_header [] = "SCIM_Pinyin_Table_BINARY";
static const char scim_pinyin_table_version [] = "VERSION_0_4";
/*
const PinyinCustomSettings scim_default_custom_settings =
{
        true, false, true,
        {false, false, false, false, false, false, false, false, false, false}
};
*/
const PinyinValidator scim_default_pinyin_validator;

const PinyinToken scim_pinyin_initials[] =
{
        {"", {0}, 0, 0},
        {"b", {0x3105,0}, 1, 1},                        //对应于unicode的0x3105的台湾注音
        {"c", {0x3118,0}, 1, 1},
        {"ch",{0x3114,0}, 2, 1},
        {"d", {0x3109,0}, 1, 1},
        {"f", {0x3108,0}, 1, 1},
        {"g", {0x310d,0}, 1, 1},
        {"h", {0x310f,0}, 1, 1},
        {"j", {0x3110,0}, 1, 1},
        {"k", {0x310e,0}, 1, 1},
        {"l", {0x310c,0}, 1, 1},
        {"m", {0x3107,0}, 1, 1},
        {"n", {0x310b,0}, 1, 1},
        {"p", {0x3106,0}, 1, 1},
        {"q", {0x3111,0}, 1, 1},
        {"r", {0x3116,0}, 1, 1},
        {"s", {0x3119,0}, 1, 1},
        {"sh",{0x3115,0}, 2, 1},
        {"t", {0x310a,0}, 1, 1},
        {"w", {0x3128,0}, 1, 1},
        {"x", {0x3112,0}, 1, 1},
        {"y", {0x3129,0}, 1, 1},
        {"z", {0x3117,0}, 1, 1},
        {"zh",{0x3113,0}, 2, 1}
};

const PinyinToken scim_pinyin_finals[] =                        //改繁体big5，字节数*2
{
        {"", {0}, 0, 0},
        {"a",   {0x311a,0},        1, 1},
        {"ai",  {0x311e,0},        2, 1},
        {"an",  {0x3122,0},        2, 1},
        {"ang", {0x3124,0},        3, 1},
        {"ao",  {0x3120,0},        2, 1},
        {"e",   {0x311c,0},        1, 1},
        {"ei",  {0x311f,0},        2, 1},
        {"en",  {0x3123,0},        2, 1},
        {"eng", {0x3125,0},        3, 1},
        {"er",  {0x3126,0},        2, 1},
        {"i",   {0x3127,0},        1, 1},
        {"ia",  {0x3127,0x311a,0}, 2, 2},
        {"ian", {0x3127,0x3122,0}, 3, 2},
        {"iang",{0x3127,0x3124,0}, 4, 2},
        {"iao", {0x3127,0x3120,0}, 3, 2},
        //{"ie",  {0x3127,0x311c,0}, 2, 2},
        {"ie",  {0x311d,0}, 2, 1},
        {"in",  {0x3127,0x3123,0}, 2, 2},
        {"ing", {0x3127,0x3125,0}, 3, 2},
        {"iong",{0x3129,0x3125,0}, 4, 2},
        {"iou", {0x3127,0x3121,0}, 3, 2},
        {"iu",  {0x3127,0x3121,0}, 2, 2},
        {"ng",  {0x312b,0},        2, 1},
        {"o",   {0x311b,0},        1, 1},
        {"ong", {0x3128,0x3123,0}, 3, 2},
        {"ou",  {0x3121,0},        2, 1},
        {"u",   {0x3128,0},        1, 1},
        {"ua",  {0x3128,0x311a,0}, 2, 2},
        {"uai", {0x3128,0x311e,0}, 3, 2},
        {"uan", {0x3128,0x3122,0}, 3, 2},
        {"uang",{0x3128,0x3124,0}, 4, 2},
        {"ue",  {0x3129,0x311c,0}, 2, 2},
        {"uei", {0x3128,0x311f,0}, 3, 2},
        {"uen", {0x3128,0x3123,0}, 3, 2},
        {"ueng",{0x3128,0x3125,0}, 4, 2},
        {"ui",  {0x3128,0x311f,0}, 2, 2},
        {"un",  {0x3128,0x3123,0}, 2, 2},
        {"uo",  {0x3128,0x311b,0}, 2, 2},
        {"v",   {0x3129,0},        1, 1},
        {"van", {0x3129,0x3122,0}, 3, 2},
        {"ve",  {0x3129,0x311c,0}, 2, 2},
        {"vn",  {0x3129,0x3123,0}, 2, 2}
};

const int scim_number_of_initials = sizeof (scim_pinyin_initials) / sizeof (PinyinToken);
const int scim_number_of_finals = sizeof (scim_pinyin_finals) / sizeof (PinyinToken);

//////////////////////////////////////////////////////////////////////////////
// implementation of PinyinKey

std::ostream&
PinyinKey::output_text (std::ostream &os) const
{
        return os << get_key_string ();
}

std::istream&
PinyinKey::input_text (const PinyinValidator &validator, std::istream &is)
{
        String key;
        is >> key;
        set_key (validator, key.c_str());
        return is;
}
/*
std::ostream&
PinyinKey::output_binary (std::ostream &os) const
{
        unsigned char key [2];
        combine_to_bytes (key);
        os.write ((const char*) key, sizeof (char) * 2);
        return os;
}

std::istream&
PinyinKey::input_binary (const PinyinValidator &validator, std::istream &is)
{
        unsigned char key [2];
        is.read ((char*) key, sizeof (char) * 2);
        extract_from_bytes (key [0], key [1]);
        if (!validator (*this)) {
                m_tone = SCIM_PINYIN_ZeroTone;
                if (!validator (*this)) {
                        m_final = SCIM_PINYIN_ZeroFinal;
                        if (!validator (*this))
                                m_initial = SCIM_PINYIN_ZeroInitial;
                }
        }
        return is;
}
*/
int
PinyinKey::parse_initial (PinyinInitial &initial,
                                                  const char *key,
                                                  int keylen)           //输入法辅音处理
{
        int lastlen = 0;

        for (int i=0; i<scim_number_of_initials; i++) {
                if (keylen >= scim_pinyin_initials [i].len
                        && scim_pinyin_initials [i].len >= lastlen
                        && strncmp (scim_pinyin_initials [i].str, key,
                                                scim_pinyin_initials [i].len) == 0) {
                        initial = static_cast<PinyinInitial>(i);
                        lastlen = scim_pinyin_initials [i].len;
                }
        }

        return lastlen;
}

int
PinyinKey::parse_final (PinyinFinal &final,
                                                const char *key,
                                                int keylen)                 //输入法元音处理
{
        int lastlen = 0;

        for (int i=0; i<scim_number_of_finals; i++) {
                if (keylen >= scim_pinyin_finals[i].len
                        && scim_pinyin_finals[i].len >= lastlen
                        && strncmp (scim_pinyin_finals [i].str, key, scim_pinyin_finals[i].len) == 0) {
                        final = static_cast<PinyinFinal>(i);
                        lastlen = scim_pinyin_finals[i].len;
                }
        }

        return lastlen;
}

int
PinyinKey::parse_tone (PinyinTone &tone,
                                           const char *key)
{
        int kt = (*key) - '0';
        if (kt >= SCIM_PINYIN_First && kt <= SCIM_PINYIN_LastTone) {
                tone = static_cast<PinyinTone>(kt);
                return 1;
        }
        return 0;
}

int
PinyinKey::parse_key (PinyinInitial &initial,
                                          PinyinFinal &final,
                                          PinyinTone &tone,
                                          const char *key,
                                          int keylen)                       //tone表示声调
{
        if (keylen <= 0) return 0;

        initial = SCIM_PINYIN_ZeroInitial;                  //初始化值为SCIM_PINYIN_ZeroInitial=0
        final = SCIM_PINYIN_ZeroFinal;
        tone = SCIM_PINYIN_ZeroTone;

        int initial_len = 0, final_len = 0, tone_len = 0;       //长度初始化为0

        final_len = parse_final (final, key, keylen);
        key += final_len;
        keylen -= final_len;

        // An initial is present
        if (final == SCIM_PINYIN_ZeroFinal) {
                initial_len = parse_initial (initial, key, keylen);
                key += initial_len;
                keylen -= initial_len;
                if (keylen){
                        final_len = parse_final (final, key, keylen);
                        key += final_len;
                        keylen -= final_len;
                }
        }

        if (keylen)
                tone_len = parse_tone (tone, key);

        apply_additional_rules(initial, final);

        return initial_len + final_len + tone_len;
}

int
PinyinKey::set_key (const PinyinValidator &validator,
                                        const char *key,
                                        int keylen)
{
        if (key == NULL || key[0] == 0) {
                return 0;
        }

        m_initial = SCIM_PINYIN_ZeroInitial;
        m_final = SCIM_PINYIN_ZeroFinal;
        m_tone = SCIM_PINYIN_ZeroTone;

        PinyinInitial initial = SCIM_PINYIN_ZeroInitial;
        PinyinFinal final = SCIM_PINYIN_ZeroFinal;
        PinyinTone tone = SCIM_PINYIN_ZeroTone;

        if (keylen < 0) keylen = strlen (key);

        keylen = parse_key (initial, final, tone, key, keylen);

        while (keylen > 0 && !validator (PinyinKey (initial, final, tone)))
                keylen = parse_key (initial, final, tone, key, keylen-1);

        if (keylen) {
                m_initial = initial;                    //
                m_final = final;
                m_tone = tone;
        }

        return keylen;
}

String
PinyinKey::get_key_string () const
{
        char key [16];
        if (m_tone)                     //snprintf指定字符串长度为15
                snprintf (key, 15, "%s%s%d", get_initial_string(), get_final_string(), m_tone);
        else
                snprintf (key, 15, "%s%s", get_initial_string(), get_final_string());

        return String (key);
}
/*
WideString
PinyinKey::get_key_wide_string () const
{
        return WideString (get_initial_wide_string ()) + WideString (get_final_wide_string());
}
*/

void
PinyinKey::apply_additional_rules (PinyinInitial &initial, PinyinFinal &final)
{
        static struct ReplaceRulePair {
                PinyinInitial initial;
                PinyinFinal   final;
                PinyinInitial new_initial;
                PinyinFinal   new_final;
        } rules [] =
        {
        /*
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_I,    SCIM_PINYIN_Yi, SCIM_PINYIN_I},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ia,   SCIM_PINYIN_Yi, SCIM_PINYIN_A},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ian,  SCIM_PINYIN_Yi, SCIM_PINYIN_An},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Iang, SCIM_PINYIN_Yi, SCIM_PINYIN_Ang},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Iao,  SCIM_PINYIN_Yi, SCIM_PINYIN_Ao},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ie,   SCIM_PINYIN_Yi, SCIM_PINYIN_E},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_In,   SCIM_PINYIN_Yi, SCIM_PINYIN_In},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ing,  SCIM_PINYIN_Yi, SCIM_PINYIN_Ing},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Iong, SCIM_PINYIN_Yi, SCIM_PINYIN_Ong},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Iou,  SCIM_PINYIN_Yi, SCIM_PINYIN_Ou},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Iu,   SCIM_PINYIN_Yi, SCIM_PINYIN_U},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_U,    SCIM_PINYIN_Wo, SCIM_PINYIN_U},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ua,   SCIM_PINYIN_Wo, SCIM_PINYIN_A},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Uai,  SCIM_PINYIN_Wo, SCIM_PINYIN_Ai},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Uan,  SCIM_PINYIN_Wo, SCIM_PINYIN_An},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Uang, SCIM_PINYIN_Wo, SCIM_PINYIN_Ang},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Uei,  SCIM_PINYIN_Wo, SCIM_PINYIN_Ei},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Uen,  SCIM_PINYIN_Wo, SCIM_PINYIN_En},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ueng, SCIM_PINYIN_Wo, SCIM_PINYIN_Eng},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ui,   SCIM_PINYIN_Wo, SCIM_PINYIN_Ei},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Un,   SCIM_PINYIN_Wo, SCIM_PINYIN_En},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Uo,   SCIM_PINYIN_Wo, SCIM_PINYIN_O},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ue,   SCIM_PINYIN_Yi, SCIM_PINYIN_Ue},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_V,    SCIM_PINYIN_Yi, SCIM_PINYIN_U},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Van,  SCIM_PINYIN_Yi, SCIM_PINYIN_Uan},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Ve,   SCIM_PINYIN_Yi, SCIM_PINYIN_Ue},
                {SCIM_PINYIN_ZeroInitial, SCIM_PINYIN_Vn,   SCIM_PINYIN_Yi, SCIM_PINYIN_Un},
        */
                {SCIM_PINYIN_Ne,          SCIM_PINYIN_Ve,   SCIM_PINYIN_Ne, SCIM_PINYIN_Ue},
                {SCIM_PINYIN_Le,          SCIM_PINYIN_Ve,   SCIM_PINYIN_Le, SCIM_PINYIN_Ue},
                {SCIM_PINYIN_Ji,          SCIM_PINYIN_V,    SCIM_PINYIN_Ji, SCIM_PINYIN_U},
                {SCIM_PINYIN_Ji,          SCIM_PINYIN_Van,  SCIM_PINYIN_Ji, SCIM_PINYIN_Uan},
                {SCIM_PINYIN_Ji,          SCIM_PINYIN_Ve,   SCIM_PINYIN_Ji, SCIM_PINYIN_Ue},
                {SCIM_PINYIN_Ji,          SCIM_PINYIN_Vn,   SCIM_PINYIN_Ji, SCIM_PINYIN_Un},
                {SCIM_PINYIN_Qi,          SCIM_PINYIN_V,    SCIM_PINYIN_Qi, SCIM_PINYIN_U},
                {SCIM_PINYIN_Qi,          SCIM_PINYIN_Van,  SCIM_PINYIN_Qi, SCIM_PINYIN_Uan},
                {SCIM_PINYIN_Qi,          SCIM_PINYIN_Ve,   SCIM_PINYIN_Qi, SCIM_PINYIN_Ue},
                {SCIM_PINYIN_Qi,          SCIM_PINYIN_Vn,   SCIM_PINYIN_Qi, SCIM_PINYIN_Un},
                {SCIM_PINYIN_Xi,          SCIM_PINYIN_V,    SCIM_PINYIN_Xi, SCIM_PINYIN_U},
                {SCIM_PINYIN_Xi,          SCIM_PINYIN_Van,  SCIM_PINYIN_Xi, SCIM_PINYIN_Uan},
                {SCIM_PINYIN_Xi,          SCIM_PINYIN_Ve,   SCIM_PINYIN_Xi, SCIM_PINYIN_Ue},
                {SCIM_PINYIN_Xi,          SCIM_PINYIN_Vn,   SCIM_PINYIN_Xi, SCIM_PINYIN_Un}
        };

        for (unsigned int i=0; i<sizeof(rules)/sizeof(ReplaceRulePair); i++) {
                if (rules[i].initial == initial && rules[i].final == final) {
                        initial = rules[i].new_initial;
                        final = rules[i].new_final;
                        break;
                }
        }

        if (initial != SCIM_PINYIN_ZeroInitial && final == SCIM_PINYIN_Iou)
                final = SCIM_PINYIN_Iu;
        if (initial != SCIM_PINYIN_ZeroInitial && final == SCIM_PINYIN_Uei)
                final = SCIM_PINYIN_Ui;
        if (initial != SCIM_PINYIN_ZeroInitial && final == SCIM_PINYIN_Uen)
                final = SCIM_PINYIN_Un;
}

int
PinyinKey::parse_pinyin_key (const PinyinValidator &validator,
                                                         PinyinParsedKeyVector &vec,
                                                         const char *key)
{
#if 1
        vec.clear ();

        int usedlen = 0;
        int keylen = strlen (key);

        if (keylen <= 0) return 0;

        PinyinParsedKey aKey;

        while (usedlen < keylen) {
                if (!isalpha (*key)) {
                        key ++;
                        usedlen ++;
                        continue;
                }

                int len = aKey.set_key (validator, key);
                if (len) {
                        aKey.set_pos (usedlen);
                        aKey.set_length (len);
                        vec.push_back (aKey);
                } else {
                        break;
                }

                key += len;
                usedlen += len;
        }
        return usedlen;
#else
        vec.clear ();

        int keylen = strlen (key);
        if (keylen <= 0) return 0;

        PinyinParsedKey aKey;
        int usedlen = 0;
        int len;
        bool found;

        const char *key_start, *key_end;

        key_end = key + keylen;

        while (key_end > key) {

                if (*(key_end-1) == '\'') {
                        --key_end;
                        --keylen;
                        if (keylen == 0) break;
                }

                key_start = std::max (key_end - SCIM_PINYIN_KEY_MAXLEN, key);
                found = false;

                while (key_start < key_end) {
                        if (isalpha (*key_start)) {
                                len = aKey.set_key (validator, key_start, key_end - key_start);
                                if (len == key_end - key_start) {
                                        found = true;
                                        aKey.set_pos (key_start - key);
                                        aKey.set_length (len);
                                        usedlen += len;
                                        key_end = key_start;
                                        vec.push_back (aKey);
                                        break;
                                }
                        }
                        ++ key_start;
                }
                if (!found) {
                        -- keylen;
                        key_end = key + keylen;
                        usedlen = 0;
                        vec.clear ();
                }
        }

        std::reverse (vec.begin (), vec.end ());

        return usedlen;
#endif
}

int
PinyinKey::parse_pinyin_key (const PinyinValidator &validator,
                                                         PinyinKeyVector &vec,
                                                         const char *key)
{
#if 1
        vec.clear ();

        int usedlen = 0;
        int keylen = strlen (key);

        if (keylen <= 0) return 0;

        PinyinKey aKey;

        while (usedlen < keylen) {
                if (!isalpha (*key)) {
                        key ++;
                        usedlen ++;
                        continue;
                }

                int len = aKey.set_key (validator, key);
                if (len && validator (aKey)) {
                        vec.push_back (aKey);
                } else if (!len) break;
                key += len;
                usedlen += len;
        }
        return usedlen;
#else
        vec.clear ();

        int keylen = strlen (key);
        if (keylen <= 0) return 0;

        PinyinKey aKey;
        int usedlen = 0;
        int len;
        bool found;

        const char *key_start, *key_end;

        key_end = key + keylen;

        while (key_end > key) {

                if (*(key_end-1) == '\'') {
                        --key_end;
                        --keylen;
                        if (keylen == 0) break;
                }

                key_start = std::max (key_end - SCIM_PINYIN_KEY_MAXLEN, key);
                found = false;

                while (key_start < key_end) {
                        if (isalpha (*key_start)) {
                                len = aKey.set_key (validator, key_start, key_end - key_start);
                                if (len == key_end - key_start) {
                                        found = true;
                                        usedlen += len;
                                        key_end = key_start;
                                        vec.push_back (aKey);
                                        break;
                                }
                        }
                        ++ key_start;
                }
                if (!found) {
                        -- keylen;
                        key_end = key + keylen;
                        usedlen = 0;
                        vec.clear ();
                }
        }

        std::reverse (vec.begin (), vec.end ());

        return usedlen;
#endif
}

//////////////////////////////////////////////////////////////////////////////
// implementation of PinyinValidator
PinyinValidator::PinyinValidator (/*const PinyinCustomSettings &custom,(*/
                                                                  const PinyinTable *table)
{
        initialize (/*custom, */table);
}

void
PinyinValidator::initialize (/*const PinyinCustomSettings &custom,*/
                                                         const PinyinTable *table)
{
        memset (m_bitmap, 0, PinyinValidatorBitmapSize);

        if (!table || table->size() <=0) return;

        for (int i=0; i<SCIM_PINYIN_InitialNumber; i++) {
                for (int j=0; j<SCIM_PINYIN_FinalNumber; j++) {
                        for (int k=0; k<SCIM_PINYIN_ToneNumber; k++) {
                                PinyinKey key(static_cast<PinyinInitial>(i),
                                                          static_cast<PinyinFinal>(j),
                                                          static_cast<PinyinTone>(k));
                                if (!table->has_key (key)) {
                                        int val = (k * SCIM_PINYIN_FinalNumber + j) * SCIM_PINYIN_InitialNumber + i;
                                        m_bitmap [val >> 3] |= (1 << (val % 8));
                                }
                        }
                }
        }
}

bool
PinyinValidator::operator () (PinyinKey key) const
{
        if (key.get_initial () == SCIM_PINYIN_ZeroInitial && key.get_final () == SCIM_PINYIN_ZeroFinal)
                return false;

        int val = (key.get_tone () * SCIM_PINYIN_FinalNumber + key.get_final ()) *
                                SCIM_PINYIN_InitialNumber + key.get_initial ();

        return  (m_bitmap [ val >> 3 ] & (1 << (val % 8))) == 0;
}

/*
//////////////////////////////////////////////////////////////////////////////
// implementation of PinyinKey comparision classes
static int
__scim_pinyin_compare_initial (const PinyinCustomSettings &custom,
                                                           PinyinInitial lhs,
                                                           PinyinInitial rhs)
{
        // Ambiguity LeRi, NeLe, FoHe will break binary search
        // we treat them as special cases
        if (custom.use_ambiguities [SCIM_PINYIN_AmbLeRi]) {
                if (lhs == SCIM_PINYIN_Ri) lhs = SCIM_PINYIN_Le;
                if (rhs == SCIM_PINYIN_Ri) rhs = SCIM_PINYIN_Le;
        }

        if (custom.use_ambiguities [SCIM_PINYIN_AmbNeLe]) {
                if (lhs == SCIM_PINYIN_Ne) lhs = SCIM_PINYIN_Le;
                if (rhs == SCIM_PINYIN_Ne) rhs = SCIM_PINYIN_Le;
        }

        if (custom.use_ambiguities [SCIM_PINYIN_AmbFoHe]) {
                if (lhs == SCIM_PINYIN_He) lhs = SCIM_PINYIN_Fo;
                if (rhs == SCIM_PINYIN_He) rhs = SCIM_PINYIN_Fo;
        }

        if ((lhs == rhs) ||
                (custom.use_ambiguities [SCIM_PINYIN_AmbZhiZi] &&
                 ((lhs == SCIM_PINYIN_Zhi && rhs == SCIM_PINYIN_Zi) ||
                  (lhs == SCIM_PINYIN_Zi && rhs == SCIM_PINYIN_Zhi))) ||

                (custom.use_ambiguities [SCIM_PINYIN_AmbChiCi] &&
                 ((lhs == SCIM_PINYIN_Chi && rhs == SCIM_PINYIN_Ci) ||
                  (lhs == SCIM_PINYIN_Ci && rhs == SCIM_PINYIN_Chi))) ||

                (custom.use_ambiguities [SCIM_PINYIN_AmbShiSi] &&
                 ((lhs == SCIM_PINYIN_Shi && rhs == SCIM_PINYIN_Si) ||
                  (lhs == SCIM_PINYIN_Si && rhs == SCIM_PINYIN_Shi))))
                return 0;
        else if (lhs < rhs) return -1;
        return 1;
}

static int
__scim_pinyin_compare_final (const PinyinCustomSettings &custom,
                                                         PinyinFinal lhs,
                                                         PinyinFinal rhs)
{
        if(((lhs == rhs) ||
                (custom.use_ambiguities [SCIM_PINYIN_AmbAnAng] &&
                 ((lhs == SCIM_PINYIN_An && rhs == SCIM_PINYIN_Ang) ||
                  (lhs == SCIM_PINYIN_Ang && rhs == SCIM_PINYIN_An))) ||

                (custom.use_ambiguities [SCIM_PINYIN_AmbEnEng] &&
                 ((lhs == SCIM_PINYIN_En && rhs == SCIM_PINYIN_Eng) ||
                  (lhs == SCIM_PINYIN_Eng && rhs == SCIM_PINYIN_En))) ||

                (custom.use_ambiguities [SCIM_PINYIN_AmbInIng] &&
                 ((lhs == SCIM_PINYIN_In && rhs == SCIM_PINYIN_Ing) ||
                  (lhs == SCIM_PINYIN_Ing && rhs == SCIM_PINYIN_In)))))
                return 0;
        else if (custom.use_incomplete && (lhs == SCIM_PINYIN_ZeroFinal || rhs == SCIM_PINYIN_ZeroFinal))
                return 0;
        else if (lhs < rhs) return -1;
        return 1;
}

static int
__scim_pinyin_compare_tone (const PinyinCustomSettings &custom,
                                                        PinyinTone lhs,
                                                        PinyinTone rhs)
{
        if(lhs == rhs || lhs == SCIM_PINYIN_ZeroTone || rhs == SCIM_PINYIN_ZeroTone || !custom.use_tone)
                return 0;
        else if (lhs < rhs) return -1;
        return 1;
}

bool
PinyinKeyLessThan::operator () (PinyinKey lhs, PinyinKey rhs) const
{
        switch (__scim_pinyin_compare_initial (m_custom,
                                static_cast<PinyinInitial>(lhs.m_initial),
                                static_cast<PinyinInitial>(rhs.m_initial))) {
                case 0:
                        switch (__scim_pinyin_compare_final (m_custom,
                                          static_cast<PinyinFinal>(lhs.m_final),
                                          static_cast<PinyinFinal>(rhs.m_final))) {
                                case 0:
                                        switch (__scim_pinyin_compare_tone (m_custom,
                                                         static_cast<PinyinTone>(lhs.m_tone),
                                                         static_cast<PinyinTone>(rhs.m_tone))) {
                                                case -1:
                                                        return true;
                                                default:
                                                        return false;
                                        }
                                case -1:
                                        return true;
                                default:
                                        return false;
                        }
                case -1:
                        return true;
                default:
                        return false;
        }
        return false;
}

bool
PinyinKeyEqualTo::operator () (PinyinKey lhs, PinyinKey rhs) const
{
        if (!__scim_pinyin_compare_initial (m_custom,
                        static_cast<PinyinInitial>(lhs.m_initial),
                        static_cast<PinyinInitial>(rhs.m_initial)) &&
                !__scim_pinyin_compare_final (m_custom,
                        static_cast<PinyinFinal>(lhs.m_final),
                        static_cast<PinyinFinal>(rhs.m_final)) &&
                !__scim_pinyin_compare_tone (m_custom,
                        static_cast<PinyinTone>(lhs.m_tone),
                        static_cast<PinyinTone>(rhs.m_tone)))
                return true;
        return false;
}
*/
//////////////////////////////////////////////////////////////////////////////
// implementation of PinyinEntry
std::ostream&
PinyinEntry::output_text (std::ostream &os) const
{
        m_key.output_text (os) << "\t" << size() << "\t";

        for (std::vector<CharFrequencyPair>::const_iterator i = m_chars.begin(); i != m_chars.end(); i++) {
                utf8_write_wchar (os, i->first);
                os << i->second << ' ';
        }

        os << '\n';

        return os;
}
/*
std::ostream&
PinyinEntry::output_binary (std::ostream &os) const
{
        unsigned char bytes [8];

        m_key.output_binary (os);

        scim_uint32tobytes (bytes, (uint32) size());

        os.write ((char*)bytes, sizeof (unsigned char) * 4);

        for (std::vector<CharFrequencyPair>::const_iterator i = m_chars.begin(); i != m_chars.end(); i++) {
                utf8_write_wchar (os, i->first);
                scim_uint32tobytes (bytes, i->second);
                os.write ((char*)bytes, sizeof (unsigned char) * 4);
        }

        return os;
}
*/
std::istream&
PinyinEntry::input_text (const PinyinValidator &validator, std::istream &is)
{
        m_chars.clear();
        String value;
        uint32 n, len, freq;
        ucs4_t wc;

        m_key.input_text (validator, is);
        is >> n;
        m_chars.reserve (n+1);

        for (uint32 i=0; i<n; i++) {
                is >> value;
                if(strcmp(value.c_str(),"0")==0){
                        continue;
                }
                if ((len = utf8_mbtowc (&wc, (const unsigned char*)(value.c_str()), value.length())) > 0) {
                        if (value.length () > len)
                                freq = atoi (value.c_str() + len);
                        else
                                freq = 0;
                        m_chars.push_back (CharFrequencyPair (wc,freq));
                }
        }
        sort ();

        std::vector <CharFrequencyPair> (m_chars).swap (m_chars);

        return is;
}
/*
std::istream&
PinyinEntry::input_binary (const PinyinValidator &validator, std::istream &is)
{
        m_chars.clear();
        uint32 n, freq;
        ucs4_t wc;

        unsigned char bytes [8];

        m_key.input_binary (validator, is);

        is.read ((char*)bytes, sizeof (unsigned char) * 4);
        n = scim_bytestouint32 (bytes);
        m_chars.reserve (n+1);

        for (uint32 i=0; i<n; i++) {
                if ((wc = utf8_read_wchar (is)) > 0) {
                        is.read ((char*)bytes, sizeof (unsigned char) * 4);
                        freq = scim_bytestouint32 (bytes);
                        m_chars.push_back (CharFrequencyPair (wc, freq));
                }
        }
        sort ();

        std::vector <CharFrequencyPair> (m_chars).swap (m_chars);

        return is;
}
*/
//////////////////////////////////////////////////////////////////////////////
// implementation of PinyinTable
PinyinTable::PinyinTable (/*const PinyinCustomSettings &custom,*/
                                                  const PinyinValidator *validator,
                                                  std::istream &is)
        : /*m_revmap_ok (false),
          m_pinyin_key_less (custom),
          m_pinyin_key_equal (custom),*/
          m_validator (validator)
          //m_custom (custom)
{
        if (!m_validator) m_validator = &scim_default_pinyin_validator;

        input (is);
}

PinyinTable::PinyinTable (/*(const PinyinCustomSettings &custom,*/
                                                  const PinyinValidator *validator,
                                                  const char *tablefile)
        : /*m_revmap_ok (false),
          m_pinyin_key_less (custom),
          m_pinyin_key_equal (custom),*/
          m_validator (validator)
          // m_custom (custom)
{
        if (!m_validator) m_validator = &scim_default_pinyin_validator;

        if (tablefile) load_table (tablefile);
}

bool
PinyinTable::output (std::ostream &os, bool binary) const
{
        //unsigned char bytes [8];

        if (!binary) {
                os << scim_pinyin_table_text_header << "\n";
                os << scim_pinyin_table_version << "\n";
                os << m_table.size () << "\n";

                for (PinyinEntryVector::const_iterator i = m_table.begin(); i!=m_table.end(); i++)
                        i->output_text (os);

        }/* else {
                os << scim_pinyin_table_binary_header << "\n";
                os << scim_pinyin_table_version << "\n";

                scim_uint32tobytes (bytes, (uint32) m_table.size ());
                os.write ((char*)bytes, sizeof (unsigned char) * 4);

                for (PinyinEntryVector::const_iterator i = m_table.begin(); i!=m_table.end(); i++)
                        i->output_binary (os);
                        }*/
        return true;
}

bool
PinyinTable::input (std::istream &is)
{
        char header [40];
        bool binary;

        if (!is) return false;

        is.getline (header, 40);

        if (strncmp (header,
                scim_pinyin_table_text_header,
                strlen (scim_pinyin_table_text_header)) == 0) {
                binary = false;
        } else if (strncmp (header,
                scim_pinyin_table_binary_header,
                strlen (scim_pinyin_table_binary_header)) == 0) {
                binary = true;
        } else {
                return false;
        }

        is.getline (header, 40);
        if (strncmp (header, scim_pinyin_table_version, strlen (scim_pinyin_table_version)) != 0)
                return false;

        uint32 i;
        uint32 n;
        PinyinEntryVector::iterator ev;

        if (!binary) {
                is >> n;

                // load pinyin table
                for (i=0; i<n; i++) {
                        PinyinEntry entry (*m_validator, is/*, false*/);

                        //if (!m_custom.use_tone) {
                                entry.set_key (PinyinKey (entry.get_key ().get_initial (),
                                                                                          entry.get_key ().get_final (),
                                                                                          SCIM_PINYIN_ZeroTone));
                                //}

                        if (entry.get_key().get_final() == SCIM_PINYIN_ZeroFinal) {
                                std::cerr << "Invalid entry: " << entry << "\n";
                        } else {
                                if ((ev = find_exact_entry (entry)) == m_table.end())
                                        m_table.push_back (entry);
                                else {
                                        for (uint32 i=0; i<entry.size(); i++) {
                                                ev->insert (entry.get_char_with_frequency_by_index (i));
                                        }
                                }
                        }
                }
                /*
        } else {
                unsigned char bytes [8];
                is.read ((char*) bytes, sizeof (unsigned char) * 4);
                n = scim_bytestouint32 (bytes);

                // load pinyin table
                for (i=0; i<n; i++) {
                        PinyinEntry entry (*m_validator, is, true);

                        if (!m_custom.use_tone) {
                                entry.set_key (PinyinKey (entry.get_key ().get_initial (),
                                                                                          entry.get_key ().get_final (),
                                                                                          SCIM_PINYIN_ZeroTone));
                        }

                        if (entry.get_key().get_final() == SCIM_PINYIN_ZeroFinal) {
                                std::cerr << "Invalid entry: " << entry << "\n";
                        } else {
                                if ((ev = find_exact_entry (entry)) == m_table.end())
                                        m_table.push_back (entry);
                                else {
                                        for (uint32 i=0; i<entry.size(); i++) {
                                                ev->insert (entry.get_char_with_frequency_by_index (i));
                                        }
                                }
                        }
                        }*/
                }
        sort ();

        return true;
}

bool
PinyinTable::load_table (const char *tablefile)
{
        std::ifstream ifs(tablefile);
        if (!ifs) return false;
        if (input (ifs) && m_table.size () != 0) return true;
        return false;
}

bool
PinyinTable::save_table (const char *tablefile, bool binary) const
{
        std::ofstream ofs(tablefile);
        if (!ofs) return false;
        if (output (ofs, binary)) return true;
        return false;
}
/*
void
PinyinTable::update_custom_settings (const PinyinCustomSettings &custom,
                                                                         const PinyinValidator *validator)
{
        m_pinyin_key_less  = PinyinKeyLessThan (custom);
        m_pinyin_key_equal = PinyinKeyEqualTo (custom);
        m_validator = validator;

        if (!m_validator)
                m_validator = &scim_default_pinyin_validator;

        m_custom = custom;
        sort ();
}

int
PinyinTable::get_all_chars (std::vector<ucs4_t> &vec) const
{
        std::vector<CharFrequencyPair> all;

        vec.clear ();

        get_all_chars_with_frequencies (all);

        for (std::vector<CharFrequencyPair>::const_iterator i = all.begin ();
                        i != all.end (); ++i)
                vec.push_back (i->first);

        return vec.size ();
}

int
PinyinTable::get_all_chars_with_frequencies (std::vector<CharFrequencyPair> &vec) const
{
        vec.clear ();

        for (PinyinEntryVector::const_iterator i = m_table.begin (); i!= m_table.end (); i++)
                i->get_all_chars_with_frequencies (vec);

        if (!vec.size ()) return 0;

        std::sort (vec.begin (), vec.end (), CharFrequencyPairGreaterThanByCharAndFrequency ());
        vec.erase (std::unique (vec.begin (), vec.end (), CharFrequencyPairEqualToByChar ()), vec.end ());
        std::sort (vec.begin (), vec.end (), CharFrequencyPairGreaterThanByFrequency ());

        return vec.size ();
}
*/
int
PinyinTable::find_chars (std::vector <ucs4_t> &vec, PinyinKey key) const
{
        std::vector<CharFrequencyPair> all;

        vec.clear ();

        find_chars_with_frequencies (all, key);

        for (std::vector<CharFrequencyPair>::const_iterator i = all.begin ();
                        i != all.end (); ++i)
                vec.push_back (i->first);

        return vec.size ();
}

int
PinyinTable::find_chars_with_frequencies (std::vector <CharFrequencyPair> &vec, PinyinKey key) const
{
        vec.clear ();

        std::pair<PinyinEntryVector::const_iterator, PinyinEntryVector::const_iterator> range =
                std::equal_range(m_table.begin(), m_table.end(), key, m_pinyin_key_less);

        for (PinyinEntryVector::const_iterator i = range.first; i!= range.second; i++) {
                i->get_all_chars_with_frequencies (vec);
        }

        if (!vec.size ()) return 0;

        std::sort (vec.begin (), vec.end (), CharFrequencyPairGreaterThanByCharAndFrequency ());
        vec.erase (std::unique (vec.begin (), vec.end (), CharFrequencyPairEqualToByChar ()), vec.end ());
        std::sort (vec.begin (), vec.end (), CharFrequencyPairGreaterThanByFrequency ());

        return vec.size ();
}

void
PinyinTable::erase (ucs4_t hz, const char *key)
{
        erase (hz, PinyinKey (*m_validator, key));
}

void
PinyinTable::erase (ucs4_t hz, PinyinKey key)
{
        if (key.zero()) {
                for (PinyinEntryVector::iterator i = m_table.begin(); i != m_table.end(); i++)
                        i->erase (hz);
        } else {
                std::pair<PinyinEntryVector::iterator, PinyinEntryVector::iterator> range =
                        std::equal_range(m_table.begin(), m_table.end(), key, m_pinyin_key_less);
                for (PinyinEntryVector::iterator i = range.first; i!= range.second; i++)
                        i->erase (hz);
        }
        //erase_from_reverse_map (hz, key);
}

uint32
PinyinTable::get_char_frequency (ucs4_t ch, PinyinKey key)
{
        PinyinKeyVector keyvec;
        uint32 freq = 0;

        if (key.zero ())
                find_keys (keyvec, ch);
        else
                keyvec.push_back (key);

        for (PinyinKeyVector::iterator i = keyvec.begin (); i != keyvec.end (); ++i) {
                std::pair<PinyinEntryVector::iterator, PinyinEntryVector::iterator> range =
                        std::equal_range(m_table.begin(), m_table.end(), *i, m_pinyin_key_less);
                for (PinyinEntryVector::iterator vi = range.first; vi!= range.second; ++vi) {
                        freq += vi->get_char_frequency (ch);
                }
        }

        return freq;
}

void
PinyinTable::set_char_frequency (ucs4_t ch, uint32 freq, PinyinKey key)
{
        PinyinKeyVector keyvec;

        if (key.zero ())
                find_keys (keyvec, ch);
        else
                keyvec.push_back (key);

        for (PinyinKeyVector::iterator i = keyvec.begin (); i != keyvec.end (); ++i) {
                std::pair<PinyinEntryVector::iterator, PinyinEntryVector::iterator> range =
                        std::equal_range(m_table.begin(), m_table.end(), *i, m_pinyin_key_less);
                for (PinyinEntryVector::iterator vi = range.first; vi != range.second; ++vi) {
                        vi->set_char_frequency (ch, freq / (keyvec.size () * (range.second - range.first)));
                }
        }
}

void
PinyinTable::refresh (ucs4_t hz, uint32 shift, PinyinKey key)
{
        if (!hz) return;

        PinyinKeyVector keyvec;
        //uint32 freq, delta;

        if (key.zero ())
                find_keys (keyvec, hz);
        else
                keyvec.push_back (key);

        for (PinyinKeyVector::iterator i = keyvec.begin (); i != keyvec.end (); ++i) {
                std::pair<PinyinEntryVector::iterator, PinyinEntryVector::iterator> range =
                        std::equal_range(m_table.begin(), m_table.end(), *i, m_pinyin_key_less);
                for (PinyinEntryVector::iterator vi = range.first; vi!= range.second; ++vi) {
                        vi->refresh_char_frequency (hz, shift);
                }
        }
}

void
PinyinTable::insert (ucs4_t hz, const char *key)
{
        insert (hz, PinyinKey (*m_validator, key));
}

void
PinyinTable::insert (ucs4_t hz, PinyinKey key)
{
        PinyinEntryVector::iterator i =
                std::lower_bound (m_table.begin(), m_table.end(), key, m_pinyin_key_less);

        if (i != m_table.end() && m_pinyin_key_equal (*i, key)) {
                i->insert (CharFrequencyPair (hz,0));
        } else {
                PinyinEntry entry (key);
                entry.insert (CharFrequencyPair (hz,0));
                m_table.insert (i, entry);
        }
        //insert_to_reverse_map (hz, key);
}

size_t
PinyinTable::size () const
{
        size_t num = 0;
        for (PinyinEntryVector::const_iterator i = m_table.begin(); i!= m_table.end(); i++)
                num += i->size ();

        return num;
}

int
PinyinTable::find_keys (PinyinKeyVector &vec, ucs4_t code)
{
        //	if (!m_revmap_ok) create_reverse_map ();


        vec.clear ();
        /*
        std::pair<ReversePinyinMap::const_iterator, ReversePinyinMap::const_iterator> result =
                m_revmap.equal_range (code);

        for (ReversePinyinMap::const_iterator i = result.first; i != result.second; i++)
                vec.push_back (i->second);
        */
        return vec.size ();
}

int
PinyinTable::find_key_strings (std::vector<PinyinKeyVector> &vec, const WideString & str)
{
        vec.clear ();

        PinyinKeyVector *key_vectors = new PinyinKeyVector [str.size()];

        for (uint32 i=0; i<str.length (); i++)
                find_keys (key_vectors[i], str [i]);

        PinyinKeyVector key_buffer;

        create_pinyin_key_vector_vector (vec, key_buffer, key_vectors, 0, str.size());

        delete [] key_vectors;
        return vec.size ();
}

bool
PinyinTable::has_key (const char *key) const
{
        return has_key (PinyinKey (*m_validator, key));
}

bool
PinyinTable::has_key (PinyinKey key) const
{
        return std::binary_search (m_table.begin(), m_table.end(), key, m_pinyin_key_less);
}

void
PinyinTable::sort ()
{
        std::sort (m_table.begin(), m_table.end(), m_pinyin_key_less);
}
/*
void
PinyinTable::create_reverse_map ()
{
        m_revmap.clear();

        PinyinKey key;

        for (PinyinEntryVector::iterator i = m_table.begin(); i != m_table.end(); i++) {
                key = i->get_key();
                for (unsigned int j = 0; j < i->size (); j++) {
                        m_revmap.insert (ReversePinyinPair (i->get_char_by_index (j), key));
                }
        }

        m_revmap_ok = true;
}

void
PinyinTable::insert_to_reverse_map (ucs4_t code, PinyinKey key)
{
        if (key.zero())
                return;

        std::pair<ReversePinyinMap::iterator, ReversePinyinMap::iterator> result =
                m_revmap.equal_range (code);

        for (ReversePinyinMap::iterator i = result.first; i != result.second; i++)
                if (m_pinyin_key_equal (i->second, key)) return;

        m_revmap.insert (ReversePinyinPair (code, key));
}

void
PinyinTable::erase_from_reverse_map (ucs4_t code, PinyinKey key)
{
        if (key.zero()) {
                m_revmap.erase (code);
        } else {
                std::pair<ReversePinyinMap::iterator, ReversePinyinMap::iterator> result =
                        m_revmap.equal_range (code);

                for (ReversePinyinMap::iterator i = result.first; i != result.second; i++)
                        if (m_pinyin_key_equal (i->second, key)) {
                                m_revmap.erase (i);
                                break;
                        }
        }
}
*/
PinyinTable::PinyinEntryVector::iterator
PinyinTable::find_exact_entry (PinyinKey key)
{
        PinyinKeyExactEqualTo eq;
        for (PinyinEntryVector::iterator i=m_table.begin (); i!=m_table.end (); i++)
                if (eq (*i, key)) return i;
        return m_table.end ();
}

void
PinyinTable::create_pinyin_key_vector_vector (std::vector<PinyinKeyVector> &vv,
                                                                                          PinyinKeyVector &key_buffer,
                                                                                          PinyinKeyVector *key_vectors,
                                                                                          int index,
                                                                                          int len)
{
        for (unsigned int i=0; i< key_vectors[index].size(); i++) {
                key_buffer.push_back ((key_vectors[index])[i]);
                if (index == len-1) {
                        vv.push_back (key_buffer);
                } else {
                        create_pinyin_key_vector_vector (vv, key_buffer, key_vectors, index+1, len);
                }
                key_buffer.pop_back ();
        }
}


/*
vi:ts=4:nowrap:ai
*/
