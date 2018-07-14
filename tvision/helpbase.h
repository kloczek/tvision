/*
 * helpbase.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

/** \file helpbase.h
 * helpbase.h
 */

/**
 * Undocumented.
 */
const long magicHeader = 0x46484246L; //"FBHF"

#define cHelpViewer "\x06\x07\x08"
#define cHelpWindow "\x80\x81\x82\x83\x84\x85\x86\x87"

// TParagraph

/**
 * Part of the help system.
 * @short Part of the help system
 */
class TParagraph
{
public:
    /**
     * Undocumented.
     */
    TParagraph() {}
    /**
     * Undocumented.
     */
    TParagraph *next;
    /**
     * Undocumented.
     */
    Boolean wrap;
    /**
     * Undocumented.
     */
    ushort size;
    /**
     * Undocumented.
     */
    char *text;
};

// TCrossRef

/**
 * Part of the help system.
 * @short Part of the help system
 */
class TCrossRef
{
public:
    /**
     * Undocumented.
     */
    TCrossRef() {}
    /**
     * Undocumented.
     */
    int ref;
    /**
     * Undocumented.
     */
    int offset;
    /**
     * Undocumented.
     */
    uchar length;
};

/**
 * Undocumented.
 */
typedef void (*TCrossRefHandler) ( opstream&, int );

/**
 * Part of the help system.
 * @short Part of the help system
 */
class THelpTopic: public TObject, public TStreamable
{
public:
    /**
     * Undocumented.
     */
    THelpTopic();
    /**
     * Undocumented.
     */
    THelpTopic( StreamableInit ) {}
    /**
     * Undocumented.
     */
    virtual ~THelpTopic();
    /**
     * Undocumented.
     */
    void addCrossRef( TCrossRef ref );
    /**
     * Undocumented.
     */
    void addParagraph( TParagraph *p );
    /**
     * Undocumented.
     */
    void getCrossRef( int i, TPoint& loc, uchar& length, int& ref );
    /**
     * Undocumented.
     */
    char *getLine( int line, char *buffer, int buflen );
    /**
     * Undocumented.
     */
    int getNumCrossRefs();
    /**
     * Undocumented.
     */
    int numLines();
    /**
     * Undocumented.
     */
    void setCrossRef( int i, TCrossRef& ref );
    /**
     * Undocumented.
     */
    void setNumCrossRefs( int i );
    /**
     * Undocumented.
     */
    void setWidth( int aWidth );
    /**
     * Undocumented.
     */
    TParagraph *paragraphs;
    /**
     * Undocumented.
     */
    int numRefs;
    /**
     * Undocumented.
     */
    TCrossRef *crossRefs;
private:
    char *wrapText( char *text, int size, int& offset, Boolean wrap, char *lineBuf, int lineBufLen );
    void readParagraphs( ipstream& s );
    void readCrossRefs( ipstream& s );
    void writeParagraphs( opstream& s );
    void writeCrossRefs( opstream& s );
    void disposeParagraphs();
    const char *streamableName() const
        { return name; }
    int width;
    int lastOffset;
    int lastLine;
    TParagraph *lastParagraph;
protected:
    virtual void write( opstream& os );
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Undocumented.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, THelpTopic& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, THelpTopic*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, THelpTopic& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, THelpTopic* cl )
    { return os << (TStreamable *)cl; }


// THelpIndex

/**
 * Part of the help system.
 * @short Part of the help system
 */
class THelpIndex : public TObject, public TStreamable
{
public:
    /**
     * Undocumented.
     */
    THelpIndex();
    /**
     * Undocumented.
     */
    THelpIndex( StreamableInit ) {}
    /**
     * Undocumented.
     */
    virtual ~THelpIndex();
    /**
     * Undocumented.
     */
    long position( int );
    /**
     * Undocumented.
     */
    void add( int, long );
    /**
     * Undocumented.
     */
    ushort size;
    /**
     * Undocumented.
     */
    long *index;
private:
    const char *streamableName() const
        { return name; }
protected:
    virtual void write( opstream& os );
    virtual void *read( ipstream& is );
public:
    /**
     * Undocumented.
     */
    static const char * const name;
    /**
     * Undocumented.
     */
    static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, THelpIndex& cl )
    { return is >> (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline ipstream& operator >> ( ipstream& is, THelpIndex*& cl )
    { return is >> (void *&)cl; }

/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, THelpIndex& cl )
    { return os << (TStreamable&)cl; }
/**
 * Undocumented.
 */
inline opstream& operator << ( opstream& os, THelpIndex* cl )
    { return os << (TStreamable *)cl; }


// THelpFile

/**
 * Part of the help system.
 * @short Part of the help system
 */
class THelpFile : public TObject
{
    static const char * invalidContext;
public:
    /**
     * Undocumented.
     */
    THelpFile( fpstream& s );
    /**
     * Undocumented.
     */
    virtual ~THelpFile();
    /**
     * Undocumented.
     */
    THelpTopic *getTopic( int );
    /**
     * Undocumented.
     */
    THelpTopic *invalidTopic();
    /**
     * Undocumented.
     */
    void recordPositionInIndex( int );
    /**
     * Undocumented.
     */
    void putTopic( THelpTopic* );
    /**
     * Undocumented.
     */
    fpstream *stream;
    /**
     * Undocumented.
     */
    Boolean modified;
    /**
     * Undocumented.
     */
    THelpIndex *index;
    /**
     * Undocumented.
     */
    long indexPos;
};

/**
 * Undocumented.
 */
extern void notAssigned( opstream& s, int value );

/**
 * Undocumented.
 */
extern TCrossRefHandler crossRefHandler;
