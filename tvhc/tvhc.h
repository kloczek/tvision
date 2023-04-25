/*-----------------------------------------------------*/
/*                                                     */
/*   Turbo Vision 1.0                                  */
/*   Turbo Vision TVHC header file                     */
/*   Copyright (c) 1991 by Borland International       */
/*                                                     */
/*-----------------------------------------------------*/

#if !defined( TV_INC_TVHC_H )
#define TV_INC_TVHC_H

#define Uses_fstream
#define Uses_TSortedCollection
#define Uses_TObject
#define Uses_TPoint
#include <tv.h>

#include "helpbase.h"

const int MAXSIZE = 80;
const int MAXSTRSIZE = 256;
const char commandChar[] = ".";
const int bufferSize = 4096;

typedef enum State { undefined, wrapping, notWrapping } _State;

class TProtectedStream:public std::fstream {
      public:
	TProtectedStream(char *aFileName, std::ios::openmode aMode);

      private:
	char fileName[MAXSIZE];
	ushort mode;
};

// Topic Reference

struct TFixUp {
	long pos;
	TFixUp *next;
};

union Content {
//  ushort value;
	int value;		// SC: must be the same type as TCrossRef::ref!
	TFixUp *fixUpList;
};

struct TReference {
	char *topic;
	Boolean resolved;
	Content val;
};

class TRefTable:public TSortedCollection {
      public:
	TRefTable(ccIndex aLimit, ccIndex aDelta);
	virtual int compare(void *key1, void *key2);
	virtual void freeItem(void *item);

	TReference *getReference(char *topic);
	virtual void *keyOf(void *item);

      private:
	virtual void *readItem(ipstream &) {
		return nullptr;
	} virtual void writeItem(void *, opstream &) {
	}
};

struct TCrossRefNode {
	char *topic;
	int offset;
	uchar length;
	TCrossRefNode *next;
};

class TTopicDefinition:public TObject {
      public:
	TTopicDefinition(char *aTopic, ushort aValue);
	~TTopicDefinition(void);

	char *topic;
	ushort value;
	TTopicDefinition *next;
};

char *helpName;
uchar buffer[bufferSize];
int ofs;
TRefTable *refTable = nullptr;
TCrossRefNode *xRefs;
char line[MAXSTRSIZE] = "";
Boolean lineInBuffer = False;
int lineCount = 0;

Boolean fExists(char *fileName);
char *getLine(std::fstream & s);
void unGetLine(char *s);
void prntMsg(const char *pref, const char *text);
void warning(const char *text);
void disposeFixUps(TFixUp * &p);
void initRefTable();
void recordReference(char *topic, opstream & s);
void doFixUps(TFixUp * p, ushort value, fpstream & s);
void resolveReference(char *topic, ushort value, fpstream & s);
void skipWhite(char *line, int &i);
int checkForValidChar(char ch);
void skipToNonWord(char *line, int &i);
char *getWord(char *line, int &i);
int is_numeric(char *str);
TTopicDefinition *topicDefinition(char *line, int &i);
TTopicDefinition *topicDefinitionList(char *line, int &i);
TTopicDefinition *topicHeader(char *line);
void addToBuffer(char *line, Boolean wrapping);
void addXRef(char *xRef, int offset, uchar length, TCrossRefNode * &xRefs);
void replaceSpacesWithFF(char *line, int start, uchar length);
void strdel(char *string, int pos, int len);
void scanForCrossRefs(char *line, int &offset, TCrossRefNode * &xRefs);
Boolean isEndParagraph(State state);
TParagraph *readParagraph(std::fstream & textFile, int &offset,
			  TCrossRefNode * &xRefs);
void handleCrossRefs(opstream & s, int xRefValue);
void skipBlankLines(std::fstream & s);
int xRefCount();
void disposeXRefs(TCrossRefNode * p);
void recordTopicDefinitions(TTopicDefinition * p, THelpFile & helpFile);
void doWriteSymbol(void *p, void *p1);
void readTopic(std::fstream & textFile, THelpFile & helpFile);
void writeSymbFile(TProtectedStream * symbFile);
void processText(TProtectedStream & textFile,
		 fpstream & helpFile, TProtectedStream & symbFile);
void checkOverwrite(char *fName);

#endif // TV_INC_TVHC_H
