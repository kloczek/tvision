/*-----------------------------------------------------------------------*/
/*                                                                       */
/*            Turbo Vision 1.0                                           */
/*            Turbo Vision Help Compiler Source File                     */
/*            Copyright (c) 1991 by Borland International                */
/*                                                                       */
/*-----------------------------------------------------------------------*/

/*===== TVHC ============================================================*/
/*  Turbo Vision help file compiler documentation.                       */
/*=======================================================================*/
/*                                                                       */
/*    Refer to DEMOHELP.TXT for an example of a help source file.        */
/*                                                                       */
/*    This program takes a help script and produces a help file (.HLP)   */
/*    and a help context file (.H).  The format for the help file is     */
/*    very simple.  Each context is given a symbolic name (i.e FileOpen) */
/*    which is then put in the context file (i.e. hcFileOpen).  The text */
/*    following the topic line is put into the help file.  Since the     */
/*    help file can be resized, some of the text will need to be wrapped */
/*    to fit into the window.  If a line of text is flush left with      */
/*    no preceeding white space, the line will be wrapped.  All adjacent */
/*    wrappable lines are wrapped as a paragraph.  If a line begins with */
/*    a space it will not be wrapped. For example, the following is a    */
/*    help topic for a File|Open menu item.                              */
/*                                                                       */
/*       |.topic FileOpen                                                */
/*       |  File|Open                                                    */
/*       |  ---------                                                    */
/*       |This menu item will bring up a dialog...                       */
/*                                                                       */
/*    The "File|Open" will not be wrapped with the "----" line since     */
/*    they both begin with a space, but the "This menu..." line will     */
/*    be wrapped.                                                        */
/*      The syntax for a ".topic" line is:                               */
/*                                                                       */
/*        .topic symbol[=number][, symbol[=number][...]]                 */
/*                                                                       */
/*    Note a topic can have multiple symbols that define it so that one  */
/*    topic can be used by multiple contexts.  The number is optional    */
/*    and will be the value of the hcXXX context in the context file     */
/*    Once a number is assigned all following topic symbols will be      */
/*    assigned numbers in sequence.  For example,                        */
/*                                                                       */
/*       .topic FileOpen=3, OpenFile, FFileOpen                          */
/*                                                                       */
/*    will produce the follwing help context number definitions,         */
/*                                                                       */
/*        hcFileOpen  = 3;                                               */
/*        hcOpenFile  = 4;                                               */
/*        hcFFileOpen = 5;                                               */
/*                                                                       */
/*    Cross references can be imbedded in the text of a help topic which */
/*    allows the user to quickly access related topics.  The format for  */
/*    a cross reference is as follows,                                   */
/*                                                                       */
/*        {text[:alias]}                                                 */
/*                                                                       */
/*    The text in the brackets is highlighted by the help viewer.  This  */
/*    text can be selected by the user and will take the user to the     */
/*    topic by the name of the text.  Sometimes the text will not be     */
/*    the same as a topic symbol.  In this case you can use the optional */
/*    alias syntax.  The symbol you wish to use is placed after the text */
/*    after a ':'. The following is a paragraph of text using cross      */
/*    references,                                                        */
/*                                                                       */
/*      |The {file open dialog:FileOpen} allows you specify which        */
/*       |file you wish to view.  If it also allow you to navigate       */
/*       |directories.  To change to a given directory use the           */
/*      |{change directory dialog:ChDir}.                                */
/*                                                                       */
/*    The user can tab or use the mouse to select more information about */
/*    the "file open dialog" or the "change directory dialog". The help  */
/*    compiler handles forward references so a topic need not be defined */
/*    before it is referenced.  If a topic is referenced but not         */
/*    defined, the compiler will give a warning but will still create a  */
/*    useable help file.  If the undefined reference is used, a message  */
/*    ("No help available...") will appear in the help window.           */
/*=======================================================================*/

#define Uses_fpstream
#define Uses_TSortedCollection
#include <tv.h>

#include "tvhc.h"
#include <cstring>
#include <limits.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>
#include <errno.h>

//======================= File Management ===============================//

TProtectedStream::TProtectedStream(char *aFileName,
				   std::ios::
				   openmode aMode):std::fstream(aFileName,
								aMode)
{
	strcpy(fileName, aFileName);
	mode = aMode;
}

void error(const char *text);

//----- replaceExt(fileName, nExt, force) -------------------------------//
//  Replace the extension of the given file with the given extension.    //
//  If an extension already exists Force indicates if it should be       //
//  replaced anyway.                                                     //
//-----------------------------------------------------------------------//

#if 0
char *replaceExt(char *fileName, char *nExt, Boolean force)
{
	char dir[MAXPATH];
	char name[MAXFILE];
	char ext[MAXEXT];
	char drive[MAXDRIVE];
	char buffer[MAXPATH];
	std::ostrstream os(buffer, MAXPATH);

	fnsplit(fileName, drive, dir, name, ext);
	if (force || (strlen(ext) == 0)) {
		os << dir << name << nExt << ends;
		return os.str();
	} else
		return fileName;
}
#endif

//----- fExist(fileName) ------------------------------------------------/
//  Returns true if the file exists false otherwise.                     /
//-----------------------------------------------------------------------/

Boolean fExists(char *fileName)
{
	if (access(fileName, R_OK) != 0)
		return (False);
	else
		return (True);
}

//======================== Line Management ==============================//
//----- getLine(s) ------------------------------------------------------//
//  Returns the next line out of the stream.                             //
//-----------------------------------------------------------------------//

char *getLine(std::fstream & s)
{
	if (s.eof()) {
		strcpy(line, "\x1A");
		return line;
	}
	if (!lineInBuffer) {
		s.getline(line, MAXSTRSIZE, '\n');

		//SS: remove carriage return, if present.  After this simple fix the
		//program will both handle unix and dos files.

		int l;
		if ((l = strlen(line)) >= 1 && line[l - 1] == '\r') {
			line[l - 1] = '\0';
		}
	}
	lineInBuffer = False;
	++lineCount;
	return line;
}

//----- unGetLine(s) ----------------------------------------------------//
//  Return given line into the stream.                                   //
//-----------------------------------------------------------------------//

void unGetLine(char *s)
{
	strcpy(line, s);
	lineInBuffer = True;
	--lineCount;
}

//========================= Error routines ==============================//

//----- prntMsg(text) ---------------------------------------------------//
//  Used by Error and Warning to print the message.                      //
//-----------------------------------------------------------------------//

void prntMsg(const char *pref, const char *text)
{
	if (lineCount > 0)
		std::cout << pref << ": " << helpName << "("
		    << lineCount << "): " << text << "\n";
	else
		std::cout << pref << ": " << helpName << " " << text << "\n";
}

//----- error(text) -----------------------------------------------------//
//  Used to indicate an error.  Terminates the program                   //
//-----------------------------------------------------------------------//

void error(const char *text)
{
	prntMsg("Error", text);
	exit(1);
}

//----- warning(text) ---------------------------------------------------//
//  Used to indicate an warning.                                         //
//-----------------------------------------------------------------------//

void warning(const char *text)
{
	prntMsg("Warning", text);
}

//====================== Topic Reference Management =====================//

void disposeFixUps(TFixUp *&p)
{
	TFixUp *q;

	while (p != nullptr) {
		q = p->next;
		delete p;
		p = q;
	}
}

//----- TRefTable -------------------------------------------------------//
//  TRefTable is a collection of TReference pointers used as a symbol    //
//  table. If the topic has not been seen, a forward reference is        //
//  inserted and a fix-up list is started.  When the topic is seen all   //
//  forward references are resolved.  If the topic has been seen already //
//  the value it has is used.                                            //
//-----------------------------------------------------------------------//

TRefTable::TRefTable(ccIndex aLimit, ccIndex aDelta):
TSortedCollection(aLimit, aDelta)
{
}

int TRefTable::compare(void *key1, void *key2)
{
	int compValue;

	compValue = strcmp((char *)key1, (char *)key2);
	if (compValue > 0)
		return 1;
	else if (compValue < 0)
		return (-1);
	else
		return (0);
}

void TRefTable::freeItem(void *item)
{
	TReference *ref;

	ref = (TReference *) item;
	if (ref->resolved == False)
		disposeFixUps(ref->val.fixUpList);
	delete ref->topic;
	delete ref;
}

TReference *TRefTable::getReference(char *topic)
{
	TReference *ref;
	int i;

	if (search(topic, i))
		ref = (TReference *) at(i);
	else {
		ref = new TReference;
		ref->topic = newStr(topic);
		ref->resolved = False;
		ref->val.fixUpList = nullptr;
		insert(ref);
	}
	return (ref);
}

void *TRefTable::keyOf(void *item)
{
	return (((TReference *) item)->topic);
}

//----- initRefTable ---------------------------------------------------//
//  Make sure the reference table is initialized.                       //
//----------------------------------------------------------------------//

void initRefTable()
{
	if (refTable == nullptr)
		refTable = new TRefTable(5, 5);
}

//---- RecordReference -------------------------------------------------//
//  Record a reference to a topic to the given stream.  This routine    //
//  handles forward references.                                         //
//----------------------------------------------------------------------//

void recordReference(char *topic, opstream & s)
{
	int i;
	TReference *ref;
	TFixUp *fixUp;

	initRefTable();
	ref = refTable->getReference(topic);
	if (ref->resolved == True)
		s << ref->val.value;
	else {
		fixUp = new TFixUp;
		fixUp->pos = s.tellp();
		i = -1;
		s << i;
		fixUp->next = ref->val.fixUpList;
		ref->val.fixUpList = fixUp;
	}
}

void doFixUps(TFixUp *p, ushort value, fpstream & s)
{
	long pos;

	for (pos = s.tellg(); (p != nullptr); p = p->next) {
		s.seekp(p->pos);
		s << value;
	}
	s.seekp(pos);
}

//----- resolveReference -----------------------------------------------//
//  Resolve a reference to a topic to the given stream.  This function  //
//  handles forward references.                                         //
//----------------------------------------------------------------------//

void resolveReference(char *topic, ushort value, fpstream & s)
{
	TReference *ref;
	char bufStr[MAXSIZE];

	initRefTable();
	ref = refTable->getReference(topic);
	if (ref->resolved) {
		strcpy(bufStr, "Redefinition of ");
		strcat(bufStr, ref->topic);
		error(bufStr);
	} else {
		doFixUps(ref->val.fixUpList, value, s);
		disposeFixUps(ref->val.fixUpList);
		ref->resolved = True;
		ref->val.value = value;
	}
}

//======================= Help file parser =============================//

void skipWhite(char *line, int &i)
{
	while (i <= (int)strlen(line) && ((line[i] == ' ') || (line[i] == 8)))
		++i;
}

int checkForValidChar(char ch)
{
	if (isalnum(ch) || (ch == '_'))
		return (0);
	return (-1);
}

void skipToNonWord(char *line, int &i)
{
	while (i <= (int)strlen(line) && (!checkForValidChar(line[i])))
		++i;
}

//----- getWord --------------------------------------------------------//
//   Extract the next word from the given line at offset i.             //
//----------------------------------------------------------------------//

char *getWord(char *line, int &i)
{
	int j;
	char *strptr;
	static char getword[MAXSIZE];	// SS

	skipWhite(line, i);
	j = i;
	if (j > (int)strlen(line))
		strcpy(getword, "");
	else {
		++i;
		if (!checkForValidChar(line[j]))
			skipToNonWord(line, i);
		strptr = line + j;
		strncpy(getword, strptr, i - j);
		getword[i - j] = '\0';
	}
	return getword;
}

//---- topicDefinition -------------------------------------------------//
//  Extracts the next topic definition from the given line at i.        //
//----------------------------------------------------------------------//

TTopicDefinition::TTopicDefinition(char *aTopic, ushort aValue)
{
	topic = newStr(aTopic);
	value = aValue;
	next = nullptr;
}

TTopicDefinition::~TTopicDefinition()
{
	delete topic;
	if (next != nullptr)
		delete next;
}

int is_numeric(char *str)
{
	int i;

	for (i = 0; i < (int)strlen(str); ++i)
		if (!isdigit(str[i]))
			return 0;
	return 1;
}

TTopicDefinition *topicDefinition(char *line, int &i)
{
	int j;
	char topic[MAXSTRSIZE], w[MAXSTRSIZE], *endptr;
	static int helpCounter = 2;	//1 is hcDragging

	strcpy(topic, getWord(line, i));
	if (strlen(topic) == 0) {
		error("Expected topic definition");
		return nullptr;
	} else {
		j = i;
		strcpy(w, getWord(line, j));
		if (strcmp(w, "=") == 0) {
			i = j;
			strcpy(w, getWord(line, i));
			if (!is_numeric(w))
				error("Expected numeric");
			else
				helpCounter = (int)strtol(w, &endptr, 10);
		} else
			++helpCounter;
		return (new TTopicDefinition(topic, helpCounter));
	}
}

//---- topicDefinitionList----------------------------------------------//
//  Extracts a list of topic definitions from the given line at i.      //
//----------------------------------------------------------------------//

TTopicDefinition *topicDefinitionList(char *line, int &i)
{
	int j;
	char w[MAXSTRSIZE];
	TTopicDefinition *topicList, *p;

	j = i;
	topicList = nullptr;
	do {
		i = j;
		p = topicDefinition(line, i);
		if (p == nullptr) {
			if (topicList != nullptr)
				delete topicList;
			return nullptr;
		}
		p->next = topicList;
		topicList = p;
		j = i;
		strcpy(w, getWord(line, j));
	} while (strcmp(w, ",") == 0);
	return (topicList);
}

//---- topicHeader -----------------------------------------------------//
//  Parse a Topic header                                                //
//----------------------------------------------------------------------//

TTopicDefinition *topicHeader(char *line)
{
	int i;
	char w[75];

	i = 0;
	strcpy(w, getWord(line, i));
	if (strcmp(w, commandChar) != 0)
		return nullptr;
	strcpy(w, getWord(line, i));
	if (strcasecmp(w, "TOPIC") == 0)
		return topicDefinitionList(line, i);
	else {
		error("TOPIC expected");
		return nullptr;
	}
}

void addToBuffer(char *line, Boolean wrapping)
{
	uchar *bufptr;

	bufptr = &buffer[ofs];
	ofs += strlen(line);
	if (ofs > bufferSize)
		error("Text too long");
	strcpy((char *)bufptr, line);
	bufptr += (strlen(line));
	if (wrapping == False)
		*bufptr = '\n';
	else
		*bufptr = ' ';
	ofs++;
}

void addXRef(char *xRef, int offset, uchar length, TCrossRefNode *&xRefs)
{
	TCrossRefNode *p, *pp, *prev;

	p = new TCrossRefNode;
	p->topic = newStr(xRef);
	p->offset = offset;
	p->length = length;
	p->next = nullptr;
	if (xRefs == nullptr)
		xRefs = p;
	else {
		pp = xRefs;
		prev = pp;
		while (pp != nullptr) {
			prev = pp;
			pp = pp->next;
		}
		prev->next = p;
	}
}

void replaceSpacesWithFF(char *line, int start, uchar length)
{
	int i;

	for (i = start; i <= (start + length); ++i)
		if (line[i] == ' ')
			line[i] = 0xFF;
}

void strdel(char *string, int pos, int len)
{
	char tempstr[MAXSTRSIZE];
	char *strptr;

	strncpy(tempstr, string, pos);
	tempstr[pos] = 0;
	strptr = string + pos + len;
	strcat(tempstr, strptr);
	strcpy(string, tempstr);
}

void scanForCrossRefs(char *line, int &offset, TCrossRefNode *&xRefs)
{
	int i;
	char begXRef = '{';
	char endXRef = '}';
	char aliasCh = ':';
	char *begPtr, *endPtr, *aliasPtr, *tempPtr;
	int begPos, endPos, aliasPos;
	char xRef[75];

	i = 0;
	do {
		if ((begPtr = strchr(line + i, begXRef)) == nullptr)
			i = 0;
		else {
			begPos = (int)(begPtr - (line + i));
			i += begPos + 1;
//                      if (line[i + 1] == begXRef)
			if (line[i] == begXRef)	// S.I. Clushin
			{
				strdel(line, i, 1);
				++i;
			} else {
				if ((endPtr =
				     strchr(line + i, endXRef)) == nullptr) {
					error("Unterminated topic reference.");
					++i;
				} else {
					endPos = (int)(endPtr - (line + i));
					aliasPtr = strchr(line + i, aliasCh);
					if ((aliasPtr == nullptr)
					    || (aliasPtr > endPtr)) {
						tempPtr = line + i;
						strncpy(xRef, tempPtr, endPos);
						xRef[endPos] = 0;
						addXRef(xRef,
							(offset + ofs + i),
							endPos, xRefs);
					} else {
						aliasPos =
						    (int)(aliasPtr -
							  (line + i));
						tempPtr = line;
						tempPtr += aliasPos + i + 1;
						strncpy(xRef, tempPtr,
							(endPos - aliasPos -
							 1));
						xRef[endPos - aliasPos - 1] = 0;
						addXRef(xRef,
							(offset + ofs + i),
							(aliasPos), xRefs);
						strdel(line, (i + aliasPos),
						       (endPos - aliasPos));
						endPtr = aliasPtr;
						endPos = aliasPos;
					}
					replaceSpacesWithFF(line, i,
							    endPos - 1);
					strdel(line, i + endPos, 1);
					strdel(line, i - 1, 1);
					i += (endPos - 2);
				}
			}
		}

	} while (i != 0);
}

Boolean isEndParagraph(State state)
{
	int flag;
	int wrapping = 1;
	int notWrapping = 2;

	flag =
	    ((line[0] == 0) ||
	     (line[0] == commandChar[0]) ||
	     (line[0] == 26) ||
	     ((line[0] == ' ') && (state == wrapping)) ||
	     ((line[0] != ' ') && (state == notWrapping)));
	if (flag)
		return (True);
	else
		return (False);
}

//---- readParagraph ----------------------------------------------------//
// Read a paragraph of the screen.  Returns the paragraph or 0 if the    //
// paragraph was not found in the given stream.  Searches for cross      //
// references and updates the xRefs variable.                            //
//-----------------------------------------------------------------------//

TParagraph *readParagraph(std::fstream & textFile, int &offset,
			  TCrossRefNode *&xRefs)
{
	State state;
	Boolean flag;
	char line[MAXSTRSIZE];
	TParagraph *p;

	ofs = 0;
	state = undefined;
	strcpy(line, getLine(textFile));
	while (strlen(line) == 0) {
		flag = (state == wrapping) ? True : False;
		addToBuffer(line, flag);
		strcpy(line, getLine(textFile));
	}

	if (isEndParagraph(state) == True) {
		unGetLine(line);
		return nullptr;
	}
	while (isEndParagraph(state) == False) {
		if (state == undefined) {
			if (line[0] == ' ')
				state = notWrapping;
			else
				state = wrapping;
		}
		scanForCrossRefs(line, offset, xRefs);
		flag = (state == wrapping) ? True : False;
		addToBuffer(line, flag);
		strcpy(line, getLine(textFile));
	}
	unGetLine(line);
	p = new TParagraph;
	p->size = ofs;
	p->wrap = (state == wrapping) ? True : False;
	p->text = new char[ofs];
	memmove(p->text, buffer, ofs);
	p->next = nullptr;
	offset += ofs;
	return (p);
}

void handleCrossRefs(opstream & s, int xRefValue)
{
	TCrossRefNode *p;

	for (p = xRefs; (xRefValue > 0); --xRefValue) {
		if (p != nullptr)
			p = p->next;
	}
	if (p != nullptr)
		recordReference((p->topic), s);
}

void skipBlankLines(std::fstream & s)
{
	char line[256];

	line[0] = 0;
	while (line[0] == 0)
		strcpy(line, getLine(s));
	unGetLine(line);
}

int xRefCount()
{
	int i;
	TCrossRefNode *p;

	i = 0;
	for (p = xRefs; (p != nullptr); p = p->next)
		++i;
	return (i);
}

void disposeXRefs(TCrossRefNode *p)
{
	TCrossRefNode *q;

	while (p != nullptr) {
		q = p;
		p = p->next;
		delete q->topic;
		delete q;
	}
}

void recordTopicDefinitions(TTopicDefinition *p, THelpFile & helpFile)
{
	while (p != nullptr) {
		resolveReference(p->topic, p->value, *(helpFile.stream));
		helpFile.recordPositionInIndex(p->value);
		p = p->next;
	}
}

//---- readTopic -------------------------------------------------------//
// Read a topic from the source file and write it to the help file      //
//----------------------------------------------------------------------//

void readTopic(std::fstream & textFile, THelpFile & helpFile)
{
	TParagraph *p;
	THelpTopic *topic;
	TTopicDefinition *topicDef;
	int i, j, offset;
	TCrossRef ref;
	TCrossRefNode *refNode;

	// Get screen command
	skipBlankLines(textFile);
	strcpy(line, getLine(textFile));

	topicDef = topicHeader(line);

	topic = new THelpTopic;

	// read paragraphs
	xRefs = nullptr;
	offset = 0;
	p = readParagraph(textFile, offset, xRefs);
	while (p != nullptr) {
		topic->addParagraph(p);
		p = readParagraph(textFile, offset, xRefs);
	}

	i = xRefCount();
	topic->setNumCrossRefs(i);
	refNode = xRefs;
	for (j = 0; j < i; ++j) {
		ref.offset = refNode->offset;
		ref.length = refNode->length;
		ref.ref = j;
		topic->setCrossRef(j, ref);
		refNode = refNode->next;
	}

	recordTopicDefinitions(topicDef, helpFile);

	crossRefHandler = handleCrossRefs;
	helpFile.putTopic(topic);

	if (topic != nullptr)
		delete topic;
	if (topicDef != nullptr)
		delete topicDef;
	disposeXRefs(xRefs);

	skipBlankLines(textFile);
}

void doWriteSymbol(void *p, void *p1)
{
	char str[80];

	TProtectedStream *symbFile = (TProtectedStream *) p1;
	if (((TReference *) p)->resolved) {
		sprintf(str, "  hc%-20s = %d,\n",
			(char *)((TReference *) p)->topic,
			((TReference *) p)->val.value);
		*symbFile << str;
	} else {
		sprintf(str, "Unresolved forward reference \"%s\"",
			((TReference *) p)->topic);
		warning(str);
	}
}

//---- writeSymbFile ---------------------------------------------------//
// Write the .H file containing all screen titles as constants.         //
//----------------------------------------------------------------------//

void writeSymbFile(TProtectedStream *symbFile)
{
	char header1[] = "const int\n";

	*symbFile << header1;
	refTable->forEach(doWriteSymbol, symbFile);
	symbFile->seekp(-2L, std::ios::end);
	*symbFile << ";\n";

}

//---- processtext -----------------------------------------------------//
// Compile the given stream, and output a help file.                    //
//----------------------------------------------------------------------//

void processText(TProtectedStream & textFile,
		 fpstream & helpFile, TProtectedStream & symbFile)
{
	THelpFile *helpRez;

	helpRez = new THelpFile(helpFile);

	while (!textFile.eof())
		readTopic(textFile, *helpRez);
	writeSymbFile(&symbFile);
	delete helpRez;
}

//========================== Program Block ==========================//

int main(int argc, char **argv)
{
	char *textName;
	char *symbName;
	fpstream *helpStrm;

	// Banner messages
	char initialText[] = "Help Compiler Version 1.0 Copyright (c) 1991"
	    " Borland International.\n";
	char helpText[] =
	    "\nUsage: tvhc <help_text> <help_file> <symbol_file.h>\n"
	    "\n"
	    "	help_text	Help file source\n"
	    "	help_file	Compiled help file\n"
	    "	symbol_file.h	An include file containing all the screen names as const's\n";

	char bufStr[MAXSTRSIZE];

	std::cout << initialText;
	if (argc != 4) {
		std::cout << helpText;
		exit(1);
	}
	//  Calculate file names
	textName = argv[1];
	if (!fExists(textName)) {
		strcpy(bufStr, "File ");
		strcat(bufStr, textName);
		strcat(bufStr, " not found.");
		error(bufStr);
	}

	helpName = argv[2];
	symbName = argv[3];

	TProtectedStream textStrm(textName, std::ios::in);
	TProtectedStream symbStrm(symbName, std::ios::out | std::ios::trunc);

	helpStrm = new fpstream(helpName, std::ios::out | std::ios::trunc);
	processText(textStrm, *helpStrm, symbStrm);
	return 0;
}
