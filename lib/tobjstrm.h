/*
 * tobjstrm.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

/** \file tobjstrm.h
 * tobjstrm.h
 */

/**
 * Undocumented.
 */
typedef unsigned P_id_type;

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TStreamable                                                     */
/*                                                                         */
/*   This is the base class for all storable objects.  It provides         */
/*   three member functions, streamableName(), read(), and write(), which  */
/*   must be overridden in every derived class.                            */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if !defined( __fLink_def )
#define __fLink_def
/**
 * Internal structure.
 * @short Internal structure
 */
struct fLink
{
    /**
     * Undocumented.
     */
    fLink *f;
    /**
     * Undocumented.
     */
    class TStreamableClass *t;
};
#endif

#define __link( s )             \
  extern TStreamableClass s;    \
  static fLink force ## s =     \
    { (fLink *)&force ## s, (TStreamableClass *)&s };

#if defined( Uses_TStreamable ) && !defined( __TStreamable )
#define __TStreamable

/**
 * Gives the streamable property to a class.
 *
 * @ref TView has two base classes, @ref TObject and the abstract class
 * TStreamable. All the viewable classes, derived ultimately from @ref TView,
 * therefore also inherit from TStreamable.
 *
 * Several non-view classes, such as @ref TCollection, @ref TStrListMaker and
 * @ref TStringList, also have TStreamable as a base class. Such classes are
 * known as streamable, meaning that their objects can be written to and read
 * from streams using the TVision stream manager.
 *
 * If you want to develop your own streamable classes, make sure that
 * TStreamable is somewhere in their ancestry. Using an existing streamable
 * class as a base class, of course, is an obvious way of achieving this.
 *
 * Since TStreamable is an abstract class, no objects of this class can be
 * instantiated. Before a streamable class can be used with streams, the class
 * must override the three pure virtual functions @ref streamableName(),
 * @ref read(), and @ref write().
 * @short Gives the streamable property to a class
 */
class TStreamable
{
    friend class opstream;
    friend class ipstream;
private:
    /**
     * Class TStreamable has no constructor. This function must be overridden
     * (or redeclared as pure virtual) by every derived class. Its purpose is
     * to return the name of the streamable class of the object that invokes
     * it.
     *
     * This name is used in the registering of streams by the stream manager.
     * The name returned must be a unique, 0-terminated string, so the safest
     * strategy is to use the name of the streamable class.
     */
    virtual const char *streamableName() const = 0;
protected:
    /**
     * This pure virtual function must be overridden (or redeclared as pure
     * virtual) in every derived class. The overriding read() function for
     * each streamable class must read the necessary data members from the
     * @ref ipstream object `is'. read() is usually implemented by calling
     * the base class's read() (if any), then extracting any additional data
     * members for the derived class.
     */
    virtual void *read( ipstream& is ) = 0;
    /**
     * This pure virtual function must be overridden (or redeclared as pure
     * virtual) in every derived class. The overriding write() function for
     * each streamable class must write the necessary data members to the
     * @ref opstream object `os'. write() is usually implemented by calling
     * the base class's write() (if any), then inserting any additional data
     * members for the derived class.
     */
    virtual void write( opstream& os ) = 0;
};

#endif  // Uses_TStreamable

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TStreamableClass                                                */
/*                                                                         */
/*   Used internally by TStreamableTypes and pstream.                      */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TStreamableClass ) && !defined( __TStreamableClass )
#define __TStreamableClass

#include <limits.h>

/**
 * Undocumented.
 */
const P_id_type P_id_notFound = UINT_MAX;

/** \enum BUILDER
 * Each streamable class has a builder function of type BUILDER. The builder
 * provides raw memory of the correct size and initializes the virtual table
 * pointers when objects are created by certain stream read operations.
 * The read() function of the streamable class reads data from the stream into
 * the raw object provided by the builder.
 * @see TStreamable::read
 */
typedef TStreamable *(*BUILDER)();

#define __DELTA( d ) ((int)(TStreamable*)(d*)1-1 )

/**
 * TStreamableClass is used by @ref TStreamableTypes and @ref pstream in the
 * registration of streamable classes.
 * @short TStreamableClass is used by TStreamableTypes and pstream in the
 * registration of streamable classes
 */
class TStreamableClass
{
    friend class TStreamableTypes;
    friend class opstream;
    friend class ipstream;
public:
    /**
     * Creates a TStreamable object with the given name and the given builder
     * function, then calls @ref pstream::registerTypes().
     *
     * Each streamable class TClassName has a build member function. There are
     * also the familiar non-member overloaded >> and << operators for stream
     * I/O associated with each streamable class.
     *
     * For type-safe object-stream I/O, the stream manager needs to access the
     * names and the type information for each class.
     *
     * To ensure that the appropriate functions are linked into any
     * application using the stream manager, you must provide an extern
     * reference such as this:
     *
     * <pre>
     * extern TStreamableClass registerTClassName;
     * </pre>
     *
     * where TClassName is the name of the class for which objects need to be
     * streamed. @ref BUILDER is typedefed as follows:
     *
     * <pre>
     * typedef TStreamable *(*BUILDER)();
     * </pre>
     */
    TStreamableClass( const char *aName, BUILDER aBuild, int aDelta );
private:
    const char *name;
    BUILDER build;
    int delta;
};

#endif  // Uses_TStreamableClass

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TStreamableTypes                                                */
/*                                                                         */
/*   Maintains a database of all registered types in the application.      */
/*   Used by opstream and ipstream to find the functions to read and       */
/*   write objects.                                                        */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TStreamableTypes ) && !defined( __TStreamableTypes )
#define __TStreamableTypes

/**
 * TStreamableTypes, derived privately from @ref TNSSortedCollection,
 * maintains a database of all registered streamable types used in an
 * application.
 *
 * Classes @ref opstream and @ref ipstream use this database to determine the
 * correct @ref read() and @ref write() functions for particular objects.
 * Because of the private derivation, all the inherited members are private
 * within TStreamableTypes.
 * @short Maintains a database of all registered streamable types used in an
 * application
 */
class TStreamableTypes : private TNSSortedCollection
{
public:
    /**
     * Calls the base @ref TNSSortedCollection constructor to create a
     * TStreamableTypes collection.
     * @see TNSSortedCollection::TNSSortedCollection
     */
    TStreamableTypes();
    /**
     * Sets the collection @ref limit to 0 without destroying the collection
     * (since the @ref shouldDelete data member is set to False).
     */
    ~TStreamableTypes();
    /**
     * Registers the argument class by inserting `d' in the collection.
     */
    void registerType( const TStreamableClass *d );
    /**
     * Returns a pointer to the class in the collection corresponding to the
     * argument `name', or returns 0 if no match.
     */
    const TStreamableClass *lookup( const char *name );
    /**
     * Undocumented.
     */
    void *operator new( size_t sz ) { return ::operator new( sz ); }
    /**
     * Undocumented.
     */
    void *operator new( size_t, void * );
private:
    /**
     * Undocumented.
     */
    virtual void *keyOf( void * );
    /**
     * Undocumented.
     */
    int compare( void *, void * );
};

#endif  // Uses_TStreamableTypes

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TPWrittenObjects                                                */
/*                                                                         */
/*   Maintains a database of all objects that have been written to the     */
/*   current object stream.                                                */
/*                                                                         */
/*   Used by opstream when it writes a pointer onto a stream to determine  */
/*   whether the object pointed to has already been written to the stream. */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TPWrittenObjects ) && !defined( __TPWrittenObjects )
#define __TPWrittenObjects

/**
 * TPWrittenObjects maintains a database of all objects that have been written
 * to the current object stream. This is used by @ref opstream when it writes a
 * pointer onto a stream.
 * @short Maintains a database of all objects that have been written to the
 * current object stream
 */
class TPWrittenObjects : public TNSSortedCollection
{
    friend class opstream;
public:
    /**
     * Undocumented.
     */
    void removeAll() { curId = 0; TNSSortedCollection::freeAll(); }
private:
    /**
     * This private constructor creates a non-streamable collection by calling
     * the base @ref TNSSortedCollection constructor. It is accessible only
     * by the member functions and friends.
     */
    TPWrittenObjects();
    /**
     * Sets the collection @ref limit to 0 without destroying the collection.
     */
    ~TPWrittenObjects();
    /**
     * Undocumented.
     */
    void registerObject( const void *adr );
    /**
     * Undocumented.
     */
    P_id_type find( const void *adr );
    /**
     * Undocumented.
     */
    void *keyOf( void * );
    /**
     * Undocumented.
     */
    int compare( void *, void * );
    /**
     * Undocumented.
     */
    P_id_type curId;
};

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TPWObj                                                          */
/*                                                                         */
/*   Used internally by TPWrittenObjects.                                  */
/*                                                                         */
/* ------------------------------------------------------------------------*/

/**
 * TPWObj is used internally by @ref TPWrittenObjects. The class
 * TPWrittenObjects is a friend of TPWobj, so all its member functions can
 * access the private members of TPWObj.
 * @short Used internally by TPWrittenObjects
 */
class TPWObj
{
    friend class TPWrittenObjects;
private:
    TPWObj( const void *adr, P_id_type id );
    const void *address;
    P_id_type ident;
};

#endif  // Uses_TPWrittenObjects

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class TPReadObjects                                                   */
/*                                                                         */
/*   Maintains a database of all objects that have been read from the      */
/*   current persistent stream.                                            */
/*                                                                         */
/*   Used by ipstream when it reads a pointer from a stream to determine   */
/*   the address of the object being referred to.                          */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TPReadObjects ) && !defined( __TPReadObjects )
#define __TPReadObjects

/**
 * TPReadObjects maintains a database of all objects that have been read from
 * the current object stream. This is used by @ref ipstream when it reads a
 * pointer from a stream to determine if other addresses for that objects
 * exist.
 * @short Maintains a database of all objects that have been read from the
 * current object stream
 */
class TPReadObjects : public TNSCollection
{
    friend class ipstream;
public:
    /**
     * Undocumented.
     */
    void removeAll() { curId = 0; TNSCollection::removeAll(); }
private:
    /**
     * This private constructor creates a non-streamable collection by calling
     * the base @ref TNSCollection constructor. It is accessible only by
     * member functions and friends.
     * @see TNSCollection::TNSCollection
     */
    TPReadObjects();
    /**
     * Sets the collection @ref limit to 0 without destroying the collection.
     */
    ~TPReadObjects();
    void registerObject( const void *adr );
    const void *find( P_id_type id );
    P_id_type curId;
};

#endif  // Uses_TPReadObjects

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class pstream                                                         */
/*                                                                         */
/*   Base class for handling streamable objects.                           */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_pstream ) && !defined( __pstream )
#define __pstream

#include <iostream>

class TStreamableTypes;

/**
 * pstream is the base class for handling streamable objects.
 * @short The base class for handling streamable objects
 */
class pstream
{
    friend class  TStreamableTypes;
public:
    /**
     * Undocumented.
     */
    enum StreamableError { peNotRegistered, peInvalidType };
    /**
     * Undocumented.
     */
    enum PointerTypes { ptNull, ptIndexed, ptObject };
    /**
     * This form creates a buffered pstream with the given buffer and sets the
     * @ref bp data member to `buf'. The @ref state data member is set to 0.
     */
    pstream( streambuf *buf );
    /**
     * Destroys the pstream object.
     */
    virtual ~pstream();
    /**
     * Returns the current @ref state value.
     */
    int rdstate() const;
    /**
     * Returns nonzero on end of stream.
     */
    int eof() const;
    /**
     * Returns nonzero if a stream operation fails.
     */
    int fail() const;
    /**
     * Returns nonzero if an error occurs.
     */
    int bad() const;
    /**
     * Returns nonzero if no state bits are set (that is, no errors occurred).
     */
    int good() const;
    /**
     * Set the stream @ref state data member to the given value (defaults
     * to 0).
     */
    void clear( int sState = 0 );
    /**
     * Overloads the pointer-to-void cast operator.
     *
     * Returns 0 if operation has failed (that is, @ref fail() returned
     * nonzero); otherwise returns nonzero.
     */
    operator void *() const;
    /**
     * Overloads the NOT operator. Returns the value returned by @ref fail().
     */
    int operator ! () const;
    /**
     * Returns the @ref bp pointer to this stream's assigned buffer.
     */
    streambuf * rdbuf() const;
    /**
     * Creates the associated @ref TStreamableTypes object types. Called by the
     * @ref TStreamableClass constructor.
     */
    static void initTypes();
    /**
     * Sets the given error condition, where StreamableError is defined as
     * follows:
     *
     * <pre>
     * enum StreamableError { peNotRegistered, peInvalidType };
     * </pre>
     */
    void error( StreamableError );
    /**
     * Sets the given error condition, where StreamableError is defined as
     * follows:
     *
     * <pre>
     * enum StreamableError { peNotRegistered, peInvalidType };
     * </pre>
     */
    void error( StreamableError, const TStreamable& );
    /**
     * Undocumented.
     */
    static void registerType( TStreamableClass *ts );
protected:
    /**
     * This form allocates a default buffer.
     */
    pstream();
    /**
     * Pointer to the stream buffer.
     */
    streambuf *bp;
    /**
     * The format state flags, as enumerated in ios. Use @ref rdstate() to
     * access the current state.
     */
    int state;
    /**
     * Initializes the stream: sets @ref state to 0 and @ref bp to `sbp'.
     */
    void init( streambuf *sbp );
    /**
     * Updates the @ref state data member with state |= (b & 0 xFF).
     */
    void setstate( int b );
    /**
     * Pointer to the @ref TStreamableTypes data base of all registered types
     * in this application.
     */
    static TStreamableTypes * types;
};

#endif  // Uses_pstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class ipstream                                                        */
/*                                                                         */
/*   Base class for reading streamable objects                             */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_ipstream ) && !defined( __ipstream )
#define __ipstream

#include <iostream>

class TStreamableClass;

/**
 * ipstream, a specialized input stream derivative of @ref pstream, is the
 * base class for reading (extracting) streamable objects. ipstream is
 * analogous to istream, defined in `iostream.h' for the standard C++ stream
 * library. ipstream is a friend class of @ref TPReadObjects.
 *
 * The overloaded operators >> extract (read) from the given ipstream object
 * to the given argument. A reference to the stream is returned, allowing you
 * to chain >> operations in the usual way. The data type of the argument
 * determines how the read is performed. For example, reading a signed char
 * is implemented using @ref readByte().
 * @see opstream
 * @short The base class for reading (extracting) streamable objects from
 * streams
 */
class ipstream : virtual public pstream
{
public:
    /**
     * This form creates a buffered ipstream with the given buffer and sets
     * the @ref bp data member to `buf'. The @ref state data member is set
     * to 0.
     */
    ipstream( streambuf *buf );
    /**
     * Destroys the ipstream object.
     */
    ~ipstream();
    /**
     * Returns the (absolute) current stream position.
     */
    std::streampos tellg();
    /**
     * This form moves the stream position to the absolute position given by
     * `pos'.
     */
    ipstream& seekg( std::streampos pos );
    /**
     * This form moves to a position relative to the current position by an
     * offset `off' (+ or -) starting at `dir'. Parameter `dir' can be set to:
     *
     * <pre>
     * beg (start of stream)
     *
     * cur (current stream position)
     *
     * end (end of stream)
     * </pre>
     */
    ipstream& seekg( std::streamoff off, std::ios::seek_dir dir );
    /**
     * Returns the character at the current stream position.
     */
    uchar readByte();
    /**
     * Reads `sz' bytes from current stream position, and writes them to
     * the address given in `data'.
     */
    void readBytes( void *data, size_t sz );
    /**
     * Returns the word at the current stream position.
     */
    ushort readWord();
    /**
     * Returns a string read from the current stream position.
     */
    char * readString();
    /**
     * Returns a string read from the current stream position.
     */
    char * readString( char *buf, unsigned maxLen );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, char& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, signed char& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, unsigned char& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, signed short& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, unsigned short& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, signed int& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, unsigned int& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, signed long& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, unsigned long& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, float& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, double& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, long double& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, TStreamable& );
    /**
     * Undocumented.
     */
    friend ipstream& operator >> ( ipstream&, void *& );
protected:
    /**
     * This form does nothing.
     */
    ipstream();
    /**
     * Returns the @ref TStreamableClass object corresponding to the class
     * name stored at the current position.
     */
    const TStreamableClass * readPrefix();
    /**
     * Invokes the appropriate read function to read from the stream to the
     * object `mem'. If `mem' is 0, the appropriate build function is called
     * first.
     */
    void * readData( const TStreamableClass *c, TStreamable *mem );
    /**
     * Reads and checks the final byte of an object's name field.
     */
    void readSuffix();
    /**
     * Returns a pointer to the object corresponding to `id'.
     */
    const void * find( P_id_type id );
    /**
     * Registers the class of the object pointed by `adr'.
     */
    void registerObject( const void *adr );
private:
    TPReadObjects objs;

};

#endif  // Uses_ipstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class opstream                                                        */
/*                                                                         */
/*   Base class for writing streamable objects                             */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_opstream ) && !defined( __opstream )
#define __opstream

#include <iostream>

class TStreamableClass;

/**
 * opstream, a specialized output stream derivative of @ref pstream, is the
 * base class for writing (inserting) streamable objects. opstream is
 * analogous to ostream, defined in `iostream.h' for the standard C++ stream
 * library. opstream is a friend class of @ref TPWrittenObjects.
 *
 * The overloaded operators << insert (write) the given argument to the given
 * opstream object. A reference to the stream is returned, allowing you
 * to chain << operations in the usual way. The data type of the argument
 * determines the form of write operation employed. For example, writing a
 * signed char is implemented using @ref writeByte().
 * @see ipstream
 * @short The base class for writing (inserting) streamable objects into
 * streams
 */
class opstream : virtual public pstream
{
public:
    /**
     * This form creates a buffered opstream with the given buffer and sets
     * the @ref bp data member to `buf'. The @ref state data member is set
     * to 0.
     */
    opstream( streambuf *buf );
    /**
     * Destroys the opstream object.
     */
    ~opstream();
    /**
     * Returns the (absolute) current stream position.
     */
    std::streampos tellp();
    /**
     * This form moves the stream's current position to the absolute position
     * given by `pos'.
     */
    opstream& seekp( std::streampos pos );
    /**
     * This form moves to a position relative to the current position by an
     * offset `off' (+ or -) starting at `dir'. Parameter `dir' can be set to:
     *
     * <pre>
     * beg (start of stream)
     *
     * cur (current stream position)
     *
     * end (end of stream)
     * </pre>
     */
    opstream& seekp( std::streamoff off, std::ios::seek_dir dir );
    /**
     * Flushes the stream.
     */
    opstream& flush();
    /**
     * Writes character `ch' to the stream.
     */
    void writeByte( uchar ch );
    /**
     * Writes `sz' bytes from `data' buffer to the stream.
     */
    void writeBytes( const void *data, size_t sz );
    /**
     * Writes the word `us' to the stream.
     */
    void writeWord( ushort us );
    /**
     * Writes `str' to the stream (together with a leading length byte).
     */
    void writeString( const char *str );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, char );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, signed char );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, unsigned char );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, signed short );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, unsigned short );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, signed int );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, unsigned int );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, signed long );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, unsigned long );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, float );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, double );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, long double );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, TStreamable& );
    /**
     * Undocumented.
     */
    friend opstream& operator << ( opstream&, TStreamable * );
protected:
    /**
     * This form allocates a default buffer.
     */
    opstream();
    /**
     * Writes the class name prefix to the stream.
     *
     * The << operator uses this function to write a prefix and suffix around
     * the data written with @ref writeData(). The prefix/suffix is used to
     * ensure type-safe stream I/O.
     */
    void writePrefix( const TStreamable& );
    /**
     * Writes data to the stream by calling the appropriate class's write
     * member function for the object being written.
     */
    void writeData( TStreamable& );
    /**
     * Writes the class name suffix to the stream.
     *
     * The << operator uses this function to write a prefix and suffix around
     * the data written with @ref writeData(). The prefix/suffix is used to
     * ensure type-safe stream I/O.
     */
    void writeSuffix( const TStreamable& );
    /**
     * Returns the type ID for the object ad address `adr'.
     */
    P_id_type find( const void *adr );
    /**
     * Registers the class of the object ad address `adr'.
     */
    void registerObject( const void *adr );
private:
    TPWrittenObjects *objs;
};

#endif  // Uses_opstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class iopstream                                                       */
/*                                                                         */
/*   Base class for reading and writing streamable objects                 */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_iopstream ) && !defined( __iopstream )
#define __iopstream

#include <iostream>

/**
 * Class iopstream is a simple "mix" of its bases, @ref opstream and
 * @ref ipstream. It provides the base class for simultaneous writing and
 * reading streamable objects.
 * @short The base class for simultaneous writing and reading streamable
 * objects to and from streams
 */
class iopstream : public ipstream, public opstream
{
public:
    /**
     * Creates a buffered iopstream with the given buffer and sets the @ref bp
     * data member to `buf'. The @ref state data member is set to 0.
     */
    iopstream( streambuf *buf );
    /**
     * Destroys the iopstream object.
     */
    ~iopstream();
protected:
    /**
     * Undocumented.
     */
    iopstream();
};

#endif  // Uses_iopstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class fpbase                                                          */
/*                                                                         */
/*   Base class for handling streamable objects on file streams            */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_fpbase ) && !defined( __fpbase )
#define __fpbase

#include <fstream>

/**
 * fpbase provides the basic operations common to all object file stream I/O.
 * @short Base class for handling streamable objects on file streams
 */
class fpbase : virtual public pstream
{
public:
    /**
     * Creates a buffered fpbase object.
     */
    fpbase();
    /**
     * Creates a buffered fpbase object. You can open a file and attach it to
     * the stream by specifying the `name', `omode', and `prot' (protection)
     * arguments.
     */
    fpbase( const char *name, int omode, int prot );
    /**
     * Creates a buffered fpbase object. You can open a file and attach it to
     * the stream by specifying the file descriptor, `f'.
     */
    fpbase( int f );
    /**
     * Creates a buffered fpbase object. You can set the size and initial
     * contents of the buffer with the `len' and `b' arguments. You can open
     * a file and attach it to the stream by specifying the file descriptor,
     * `f'.
     */
    fpbase( int f, char *b, int len);
    /**
     * Destroys the fpbase object.
     */
    ~fpbase();
    /**
     * Opens the named file in the given mode (app, ate, in, out, binary,
     * trunc, nocreate, noreplace) and protection. The opened file is
     * attached to this stream.
     */
    void open( const char *name, int omode, int prot );
    /**
     * Attaches the file with descriptor `f' to this stream if possible.
     */
    void attach( int f );
    /**
     * Closes the stream and associated file.
     */
    void close();
    /**
     * Allocates a buffer of size `len'.
     */
    void setbuf( char *buf, int len );
    /**
     * Returns a pointer to the current file buffer.
     */
    std::filebuf * rdbuf();
private:
    std::filebuf buf;
};

#endif  // Uses_fpbase

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class ifpstream                                                       */
/*                                                                         */
/*   Base class for reading streamable objects from file streams           */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_ifpstream ) && !defined( __ifpstream )
#define __ifpstream

#include <iostream>

/**
 * ifpstream is a simple "mix" of its bases, @ref fpbase and @ref ipstream.
 * It provides the base class for reading (extracting) streamable objects
 * from file streams.
 * @short Provides the base class for reading (extracting) streamable objects
 * from file streams.
 */
class ifpstream : public fpbase, public ipstream
{
public:
    /**
     * Creates a buffered ifpstream object.
     */
    ifpstream();
    /**
     * Creates a buffered ifpstream object. You can open a file and attach it
     * to the stream by specifying the `name', `omode', and `prot'
     * (protection) arguments.
     */
    ifpstream(const char *name, int omode = std::ios::in, int prot );
    /**
     * Creates a buffered ifpstream object. You can open a file and attach it
     * to the stream by specifying the file descriptor, `f'.
     */
    ifpstream( int f );
    /**
     * Creates a buffered ifpstream object. You can set the size and initial
     * contents of the buffer with the `len' and `b' arguments. You can open
     * a file and attach it to the stream by specifying the file descriptor,
     * `f'.
     */
    ifpstream( int f, char *b, int len );
    /**
     * Destroys the ifpstream object.
     */
    ~ifpstream();
    /**
     * Returns a pointer to the current file buffer.
     */
    std::filebuf * rdbuf();
    /**
     * Opens the the named file in the given mode (app, ate, in, out, binary,
     * trunc, nocreate, or noreplace) and protection. The default mode is in
     * (input) with openprot protection. The opened file is attached to this
     * stream.
     */
    void open( const char *name, int omode = ios::in, int prot );
};

#endif  // Uses_ifpstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class ofpstream                                                       */
/*                                                                         */
/*   Base class for writing streamable objects to file streams             */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_ofpstream ) && !defined( __ofpstream )
#define __ofpstream

#include <iostream>

/**
 * Class ofpstream is a simple "mix" of its bases, @ref fpbase and
 * @ref opstream. It provides the base class for writing (inserting)
 * streamable objects to file streams.
 * @short Provides the base class for writing (inserting) streamable objects
 * to file streams
 */
class ofpstream : public fpbase, public opstream
{
public:
    /**
     * Creates a buffered ofpstream object.
     */
    ofpstream();
    /**
     * Creates a buffered ofpstream object. You can open a file and attach it
     * to the stream by specifying the `name', `omode', and `prot'
     * (protection) arguments.
     */
    ofpstream( const char *name, int omode = std::ios::out, int prot );
    /**
     * Creates a buffered ofpstream object. You can open a file and attach it
     * to the stream by specifying the file descriptor, `f'.
     */
    ofpstream( int f );
    /**
     * Creates a buffered ofpstream object. You can set the size and initial
     * contents of the buffer using the `len' and `b' arguments. You can open
     * a file and attach it to the stream by specifying the file descriptor,
     * `f'.
     */
    ofpstream( int f, char *b, int len );
    /**
     * Destroys the ofpstream object.
     */
    ~ofpstream();
    /**
     * Returns the current file buffer.
     */
    std::filebuf * rdbuf();
    /**
     * Opens the the named file in the given mode (app, ate, in, out, binary,
     * trunc, nocreate, or noreplace) and protection. The default mode is out
     * (output) with openprot protection. The opened file is attached to this
     * stream.
     */
    void open( const char *name, int omode = std::ios::out, int prot );
};

#endif  // Uses_ofpstream

/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   class fpstream                                                        */
/*                                                                         */
/*   Base class for reading and writing streamable objects to              */
/*   bidirectional file streams                                            */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_fpstream ) && !defined( __fpstream )
#define __fpstream

#include <iostream>

/**
 * fpstream is a simple "mix" of its bases, @ref fpbase and @ref iopstream.
 * It provides the base class for simultaneous writing and reading streamable
 * objects to bidirectional file streams. It is analogous to class fstream,
 * defined in `fstream.h' for the standard C++ stream library.
 * @short Provides the base class for simultaneous writing and reading
 * streamable objects to bidirectional file streams
 */
class fpstream : public fpbase, public iopstream
{
public:
    /**
     * Creates a buffered fpstream object.
     */
    fpstream();
    /**
     * Creates a buffered fpstream object. You can open a file and attach it
     * to the stream by specifying the `name', `omode', and `prot'
     * (protection) arguments.
     */
    fpstream( const char *name, int omode, int prot );
    /**
     * Creates a buffered fpstream object. You can open a file and attach it
     * to the stream by specifying the file descriptor, `f'.
     */
    fpstream( int f );
    /**
     * Creates a buffered fpstream object. You can set the size and initial
     * contents of the buffer using the `len' and `b' arguments. You can open
     * a file and attach it to the stream by specifying the file descriptor,
     * `f'.
     */
    fpstream( int f, char *b, int len );
    /**
     * Destroys the fpstream object.
     */
    ~fpstream();
    /**
     * Returns the data member bp.
     */
    filebuf * rdbuf();
    /**
     * Opens the named file in the given mode (app, ate, in, out, binary,
     * trunc, nocreate, noreplace) and protection. The opened file is
     * attatched to this stream.
     */
    void open( const char *name, int omode, int prot);
};

#endif  // Uses_fpstream
