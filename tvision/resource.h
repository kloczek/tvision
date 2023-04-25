/*
 * resource.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TStringCollection ) && !defined( TV_TStringCollection )
#define TV_TStringCollection

/** \file resource.h
 * resource.h
 */

/**
 * TStringCollection is a simple derivative of @ref TSortedCollection
 * implementing a sorted list of ASCII strings.
 * @short Implements a sorted list of ASCII strings
 */
class TStringCollection:public TSortedCollection {
      public:
    /**
     * Creates a TStringCollection object with the given values.
     */
	TStringCollection(short aLimit, short aDelta);
      private:
    /**
     * Compares the "strings" `key1' and `key2' as follows:
     *
     * -# return < 0 if `key1' < `key2'
     * -# return 0 if `key1' == `key2'
     * -# return > 0 if `key1' > `key2'
     *
     * compare() is overridden to provide the conventional lexicographic ASCII
     * string ordering. You can override compare() to allow for other
     * orderings, such as those for non-English character sets.
     */
	 virtual int compare(void *key1, void *key2);
    /**
     * Removes the string `item' from the sorted collection and disposes of
     * the string.
     */
	virtual void freeItem(void *item);
    /**
     * Undocumented.
     */
	virtual const char *streamableName() const {
		return name;
	}
    /**
     * Called for each item in the collection. You'll need to override these
     * in everything derived from @ref TCollection or @ref TSortedCollection
     * in order to read the items correctly.
     */ virtual void *readItem(ipstream &);
    /**
     * Called for each item in the collection. You'll need to override these
     * in everything derived from @ref TCollection or @ref TSortedCollection
     * in order to write the items correctly.
     *
     * TSortedCollection already overrides this function.
     */
	virtual void writeItem(void *, opstream &);
      protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
	 TStringCollection(StreamableInit):TSortedCollection(streamableInit) {
      } public:
    /**
     * Undocumented.
     */
	static const char *const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
	static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TStringCollection & cl)
{
	return is >> static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TStringCollection *&cl)
{
	return is >> (void *&)cl;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TStringCollection & cl)
{
	return os << static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TStringCollection *cl)
{
	return os << static_cast < TStreamable * >(cl);
}

#endif // Uses_TStringCollection

#if defined( Uses_TResourceItem ) && !defined( TV_TResourceItem )
#define TV_TResourceItem

/**
 * Internal structure use by @ref TResourceCollection and @ref TResourceFile.
 * @short Internal structure use by TResourceCollection and TResourceFile
 */
struct TResourceItem {
    /**
     * Undocumented.
     */
	long pos;
    /**
     * Undocumented.
     */
	long size;
    /**
     * Undocumented.
     */
	char *key;
};

#endif // Uses_TResourceItem

#if defined( Uses_TResourceCollection ) && !defined( TV_TResourceCollection )
#define TV_TResourceCollection

/**
 * TResourceCollection is a derivative of @ref TStringCollection, which makes
 * it a sorted, streamable collection. It is used with @ref TResourceFile to
 * implement collections of resources. A resource file is a stream that is
 * indexed by key strings. Each resource item points to an object of type
 * @ref TResourceItem.
 * @short Implements a sorted, streamable collection of resources
 */
class TResourceCollection:public TStringCollection {
      public:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized vtable pointers. This is
     * achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
	TResourceCollection(StreamableInit):TStringCollection(streamableInit) {
	}
    /**
     * Creates a resource collection with initial size `aLimit' and the
     * ability to resize by `aDelta'.
     */ TResourceCollection(short aLimit, short aDelta);
    /**
     * Returns the key of the given item.
     */
	virtual void *keyOf(void *item);
      private:
   /**
    * Frees the given item from the collection by deleting both the key and
    * the item.
    */
	virtual void freeItem(void *item);
    /**
     * Undocumented.
     */
	virtual const char *streamableName() const {
		return name;
	}
    /**
     * Called for each item in the collection. In order to read items
     * correctly, you'll need to override these in everything derived from
     * @ref TCollection or @ref TSortedCollection. @ref TSortedCollection
     * already overrides this function.
     */ virtual void *readItem(ipstream &);
    /**
     * Called for each item in the collection. In order to write the items
     * correctly, you'll need to override these in everything derived from
     * @ref TCollection or @ref TSortedCollection. @ref TSortedCollection
     * already overrides this function.
     */
	virtual void writeItem(void *, opstream &);
      public:
    /**
     * Class name used by the stream manager.
     */
	static const char *const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
	static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TResourceCollection & cl)
{
	return is >> static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TResourceCollection *&cl)
{
	return is >> (void *&)cl;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TResourceCollection & cl)
{
	return os << static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TResourceCollection *cl)
{
	return os << static_cast < TStreamable * >(cl);
}

#endif // Uses_TResourceCollection

#if defined( Uses_TResourceFile ) && !defined( TV_TResourceFile )
#define TV_TResourceFile

class TResourceCollection;
class fpstream;

/**
 * TResourceFile implements a stream (of type @ref fpstream) that can be
 * indexed by string keys.
 *
 * To provide fast and efficient access to the objects stored in a resource
 * file, TResourceFile objects store the keys in a sorted string collection
 * (using @ref TResourceCollection) along with the position and size of the
 * resource data in the resource file.
 * @short Implements a stream that can be indexed by string keys
 */
class TResourceFile:public TObject {
      public:
    /**
     * Initializes a resource file using the stream given by `aStream' and
     * sets the @ref modified data member to False. The stream must have
     * already been initialized.
     *
     * During initialization, the TResourceFile constructor looks for a
     * resource file header at the current position of the stream. If a header
     * is not found, the constructor assumes that a new resource file is being
     * created together with a new resource collection.
     */
	TResourceFile(fpstream * aStream);
    /**
     * Flushes the resource file, using @ref flush() and then deletes
     * @ref index and @ref stream data members.
     */
	~TResourceFile();
    /**
     * Calls index->getCount() to return the number of resource items stored
     * in the associated @ref TResourceCollection.
     * @see index
     */
	short count();
    /**
     * If the resource indexed by `key' is not found, remove does nothing.
     * Otherwise it calls index->free() to remove the resource.
     */
	void remove(const char *key);
    /**
     * If the resource file has not been modified since the last flush (that
     * is, if @ref modified is False), flush() does nothing.
     *
     * Otherwise, flush() stores the updated index at the end of the stream
     * and updates the resource header at the beginning of the stream. It then
     * calls stream->flush() and resets @ref modified to False.
     */
	void flush();
    /**
     * Searches for the given key in the associated resource file collection
     * (given by the pointer @ref index). Returns 0 if the key is not found.
     */
	void *get(const char *key);
    /**
     * Uses index->at(i) to return the string key of the i'th resource in the
     * resource file.
     * @see TNSCollection::at
     *
     * The index of the first resource is zero and the index of the last
     * resource is @ref count() minus one. Using @ref count() and keyAt()
     * you can iterate over all resources in a resource file.
     */
	const char *keyAt(short i);
    /**
     * Adds the streamable object given by `item' to the resource file with
     * the key string given by `key' and sets @ref modified to True.
     *
     * If the index already contains the key, then the new object replaces the
     * old object; otherwise, the new object is appended in the correct
     * indexed position of the resource file.
     */
	void put(TStreamable * item, const char *key);
    /**
     * Undocumented.
     */
	fpstream *switchTo(fpstream * aStream, Boolean pack);
      protected:
    /**
     * Pointer to the file stream associated with this resource file.
     */
	 fpstream * stream;
    /**
     * Set True if the resource file has been modified since the last flush
     * call; otherwise False.
     */
	Boolean modified;
    /**
     * The base position of the stream (ignoring header information).
     */
	long basePos;
    /**
     * The current position of the stream relative to the base position.
     */
	long indexPos;
    /**
     * A pointer to the associated @ref TResourceCollection object.
     */
	TResourceCollection *index;
};

#endif // Uses_TResourceFile

#if defined( Uses_TStrIndexRec ) && !defined( TV_TStrIndexRec )
#define TV_TStrIndexRec

/**
 * Internal class used by @ref TStringList and @ref TStrListMaker.
 * @short Internal class used by TStringList and TStrListMaker
 */
class TStrIndexRec {
      public:
    /**
     * Undocumented.
     */
	TStrIndexRec();
    /**
     * The key.
     */
	ushort key;
    /**
     * Undocumented.
     */
	ushort count;
    /**
     * Undocumented.
     */
	ushort offset;
};

#endif // Uses_TStrIndexRec

#if defined( Uses_TStringList ) && !defined( TV_TStringList )
#define TV_TStringList

class TStrIndexRec;

/**
 * TStringList provides a mechanism for accessing strings stored on a stream.
 * Each string in a string list is identified by a unique number (ushort
 * @ref TStrIndexRec::key)
 * between 0 and 65,535.
 *
 * String lists take up less memory than normal string literals, since the
 * strings are stored on a stream instead of in memory. Also, string lists
 * permit easy internationalization, as the strings are not hard-coded in your
 * program.
 *
 * TStringList has member functions only for accessing strings; you must use
 * @ref TStrListMaker to create string lists.
 * @short A mechanism for accessing strings stored on a stream
 */
class TStringList:public TObject, public TStreamable {
      public:
    /**
     * Deallocates the memory allocated to the string list.
     */
	~TStringList();
    /**
     * Returns in `dest' the string given by `key', or an empty string if
     * there is no string with the given key.
     */
	void get(char *dest, ushort key);
      private:
	 ipstream * ip;
	long basePos;
	short indexSize;
	TStrIndexRec *index;
	virtual const char *streamableName() const {
		return name;
      } protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
	 TStringList(StreamableInit);
    /**
     * Writes to the output stream `os'.
     */
	virtual void write(opstream &) {
	}
    /**
     * Reads from the input stream `is'.
     */ virtual void *read(ipstream & is);
      public:
    /**
     * Undocumented.
     */
	static const char *const name;
    /**
     * Called to create an object in certain stream-reading situations.
     */
	static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TStringList & cl)
{
	return is >> static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TStringList *&cl)
{
	return is >> (void *&)cl;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TStringList & cl)
{
	return os << static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TStringList *cl)
{
	return os << static_cast < TStreamable * >(cl);
}

#endif // Uses_TStringList

#if defined( Uses_TStrListMaker ) && !defined( TV_TStrListMaker )
#define TV_TStrListMaker

/**
 * TStrListMaker is a simple object type used to create string lists for use
 * with @ref TStringList.
 * @short Used to create string lists for use with TStringList
 */
class TStrListMaker:public TObject, public TStreamable {
      public:
    /**
     * Creates an in-memory string list of size `aStrSize' with an index of
     * `aIndexSize' elements. A string buffer and an index buffer of the
     * specified size are allocated on the heap.
     *
     * `aStrSize' must be large enough to hold all strings to be added to the
     * string list; each string occupies its length plus a final 0.
     *
     * As strings are added to the string list (using @ref put()), a string
     * index is built.
     *
     * Strings with contiguous keys (such `as' and `sError' in the example
     * above) are recorded in one index record, up to 16 at a time.
     * `aIndexSize' must be large enough to allow for all index records
     * generated as strings are added. Each index entry occupies 6 bytes.
     */
	TStrListMaker(ushort aStrSize, ushort aIndexSize);
    /**
     * Frees the memory allocated to the string list maker.
     */
	~TStrListMaker();
    /**
     * Adds the given string `str' to the calling string list (with the given
     * numerical key).
     */
	void put(ushort key, char *str);
      private:
	 ushort strPos;
	ushort strSize;
	char *strings;
	ushort indexPos;
	ushort indexSize;
	TStrIndexRec *index;
	TStrIndexRec cur;
	void closeCurrent();
	virtual const char *streamableName() const {
		return TStringList::name;
      } protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized virtual table pointers.
     * This is achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
	 TStrListMaker(StreamableInit);
    /**
     * Writes to the output stream `os'.
     */
	virtual void write(opstream & os);
	virtual void *read(ipstream &) {
		return nullptr;
      } public:
    /**
     * Called to create an object in certain stream-reading situations.
     */
	static TStreamable *build();
};

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TStrListMaker & cl)
{
	return is >> static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TStrListMaker *&cl)
{
	return is >> (void *&)cl;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TStrListMaker & cl)
{
	return os << static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TStrListMaker *cl)
{
	return os << static_cast < TStreamable * >(cl);
}

#endif // Uses_TStrListMaker
