
typedef struct Text * Text_t;
typedef struct Sentence * Sentence_t;
typedef struct Word * Word_t;

Text_t Text_new();
void Text_delete(Text_t self);
void Text_addSentence(Text_t self, unsigned int index, Sentence_t sentence);
Sentence_t Text_getSentence(Text_t self, unsigned int index);
Sentence_t Text_removeSentence(Text_t self, unsigned int index);
void Text_parse(Text_t self, const char * text);
unsigned int Text_getSentNum(Text_t self);
void Text_leaveWordWithSpecLen(Text_t self, unsigned int leftB, unsigned int rightB);
void Text_sortWords(Text_t self);

Sentence_t Sentence_new();
void Sentence_delete(Sentence_t self);
void Sentence_addWord(Sentence_t self, unsigned int index, Word_t word);
Word_t Sentence_getWord(Sentence_t self, int index);
Word_t Sentence_removeWord(Sentence_t self, unsigned int index);
void Sentence_parse(Sentence_t self, const char * sent);
void Sentence_leaveWordWithSpecLen(Sentence_t self, unsigned int leftB, unsigned int rightB);
unsigned int Sentence_getWordsNum(Sentence_t self);

Word_t Word_new();
void Word_delete(Word_t self);
void Word_parse(Word_t self, const char * word);
const char * Word_toString(Word_t self);
