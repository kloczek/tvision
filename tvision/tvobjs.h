/*
 * tvobjs.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TObject ) && !defined( TV_TObject )
#define TV_TObject

/** \file tvobjs.h
 * tvobjs.h
 */

#include <stddef.h>

/**
 * TObject is the starting point for much of TVision's class hierarchy. It has
 * no parents but many descendants. Apart from @ref TPoint and @ref TRect,
 * most of TVision's standard classes are ultimately derived from TObject.
 *
 * Any object that uses TVision's stream facilities must trace its ancestry
 * back to TObject.
 * @see TView
 * @short The fundamental class
 */
class TObject
{
public:
    /**
     * Destructor.
     *
     * Performs the necessary cleanup and disposal for dynamic objects. Does
     * nothing.
     */
    virtual ~TObject();
    /**
     * Destroys the object pointed by `o'.
     *
     * destroy() deletes an object `o' of a type derived from TObject; that
     * is, any object created with operator new(). destroy() terminates the
     * object, correctly freeing the memory that it occupies.
     *
     * It calls `o->shutDown()' and after does `delete o'.
     * @see shutDown
     */
    static void destroy( TObject *o );
    /**
     * Releases TObject resources.
     *
     * Used internally by TObject::destroy() to ensure correct destruction of
     * derived and related objects. shutDown() is overridden in many classes
     * to ensure the proper setting of related data members when destroy is
     * called. Does nothing.
     * @see destroy
     */
    virtual void shutDown();
private:
};

inline void TObject::destroy( TObject *o )
{
    if( o != nullptr )
        o->shutDown();
    delete o;
}

#endif  // Uses_TObject

#if defined( Uses_TNSCollection ) && !defined( TV_TNSCollection )
#define TV_TNSCollection

/**
 * TNSCollection implements a nonstreamable collection of items. It provides
 * a base class for the streamable collection class, @ref TCollection.
 * TNSCollection provides @ref TCollection with the functions for adding,
 * accessing, and removing items from a collection.
 *
 * This class stores an array of pointers to generic objects. This array may
 * grow or shrink at run-time.
 *
 * Note: type ccIndex is defined in file `ttypes.h' as int.
 * @short Handles a non-streamable collection of objects
 */
class TNSCollection : public TObject
{
public:
    /**
     * Constructor.
     *
     * Creates a collection with @ref limit set to `aLimit' and @ref delta
     * set to `aDelta'. @ref count and @ref items data members are both set
     * to 0.
     * @ref shouldDelete is set True.
     *
     * The initial number of items will be limited to `aLimit', but the
     * collection is allowed to grow in increments of `aDelta' until memory
     * runs out or the number of items reaches @ref maxCollectionSize.
     *
     * @ref maxCollectionSize is defined in `tvconfig.h' as:
     *
     * <pre>
     * const int maxCollectionSize = INT_MAX / sizeof(void *);
     * </pre>
     * @see maxCollectionSize
     */
    TNSCollection( ccIndex aLimit, ccIndex aDelta );
    /**
     * Destructor.
     *
     * If @ref shouldDelete is True, the destructor removes and destroys all
     * items in the collection by calling @ref freeAll and setting @ref limit
     * to 0.
     *
     * If @ref shouldDelete is False, the destructor sets @ref limit to 0 but
     * does not destroy the collection.
     */
    ~TNSCollection();
    /**
     * Releases all the resources allocated by this class.
     *
     * If class flag @ref shouldDelete is True the function @ref freeAll() is
     * called. This will delete each object of the array.
     */
    virtual void shutDown();
    /**
     * Returns a pointer to the item indexed by `index' in the collection. If
     * `index' is less than 0 or greater than or equal to count, @ref error()
     * is called with an argument of coIndexError, and 0 is then returned.
     */
    void *at( ccIndex index );
    /**
     * Returns the index of the given item; that is, the converse operation
     * to @ref at(). If the item is not in the collection, @ref indexOf()
     * calls @ref error().
     *
     * The address of the item is passed in the `item' parameter.
     */
    virtual ccIndex indexOf( void *item );
    /**
     * Removes the object at position `index' from the array.
     *
     * Then calls delete on the object.
     */
    void atFree( ccIndex index );
    /**
     * Removes the object at position `index' from the array.
     *
     * Removes the item at the position `index' by moving the following items
     * up by one position. @ref count is decremented by 1, but the memory
     * allocated to the collection is not reduced. If `index' is greater than
     * or equal to @ref count, @ref error() is called.
     *
     * The item itself is not destroyed.
     */
    void atRemove( ccIndex index );
    /**
     * Removes the item given by `item' from the collection.
     *
     * Equivalent to `atRemove(indexOf(item))'. Does not call delete on the
     * object.
     * @see TNSCollection::atRemove
     * @see TNSCollection::indexOf
     */
    void remove( void *item );
    /**
     * Removes all items from the collection by just setting @ref count to 0.
     * @see TNSCollection::count
     */
    void removeAll();
    /**
     * Removes and destroys the given item.
     *
     * It just does `atRemove(indexOf(item))'. Then calls delete on the
     * object.
     * @see TNSCollection::atRemove
     * @see TNSCollection::indexOf
     */
    void free( void *item );
    /**
     * Removes and destroys all items in the collection and sets @ref count
     * to 0.
     *
     * The array is cleared out but not deleted.
     */
    void freeAll();
    /**
     * Inserts a new object at position `index'.
     *
     * Moves the following items down by one position, then inserts `item' at
     * the `index' position. If `index' is less than 0 or greater than
     * @ref count data member, @ref error() is called with an argument of
     * coIndexError and the new item is not inserted.
     *
     * If @ref count is equal to @ref limit data member before the call to
     * atInsert(), the allocated size of the collection is expanded by
     * @ref delta items using a call to @ref setLimit().
     *
     * If the @ref setLimit() call fails to expand the collection, the
     * @ref error() member function is called with an argument of coOverflow
     * and the new item is not inserted.
     */
    void atInsert( ccIndex index, void *item );
    /**
     * Replaces the object at position `index'.
     *
     * Replaces the item at position `index' with the given `item'. If `index'
     * is less than 0 or greater than or equal to @ref count,
     * @ref error() is called with an argument of coIndexError.
     *
     * Old object is lost.
     */
    void atPut( ccIndex index, void *item );
    /**
     * Inserts `item' into the collection, and adjusts other indexes if
     * necessary. By default, insertions are made at the end of the collection
     * by calling @ref atInsert().
     */
    virtual ccIndex insert( void *item );
#ifndef __UNPATCHED
    /**
     * This function is called on error conditions.
     *
     * By default calls function exit() to terminate the program.
     */
    virtual void error( ccIndex code, ccIndex info );
#else
    /**
     * Called whenever a collection error is encountered. By default, this
     * member function produces a run-time error of (212 - `code').
     */
    static void error( ccIndex code, ccIndex info );
#endif
    /**
     * firstThat() applies a @ref Boolean function `Test', along with an
     * argument list given by `arg' to each item in the collection until the
     * tested function returns True.
     * The result is the item pointer for which the call returns True, or 0 if
     * the call returned False for all items.
     *
     * `Test' is a pointer to a function whose type ccTestFunc is defined
     * as:
     *
     * <pre>
     * typedef Boolean (*ccTestFunc)(void *, void *)
     * </pre>
     *
     * This method returns when one object of the array passes the test or
     * when each object is tested without success. In the first case it
     * returns the address of the object. In the latter case it returns 0.
     * `arg' stores the argument of the function (if any).
     *
     * This method scans the array forward. This is an example:
     *
     * <pre>
     * #define Uses_TNSCollection
     *
     * #include "tv.h"
     *
     * class XObject {
     *     int value;
     * public:
     *     XObject(int aValue): value(aValue) {}
     *     int getValue() { return value; }
     * };
     *
     * Boolean matchTest(void *obj, void *value)
     * {
     *     if (((XObject *) obj)->getValue() == *((int *) value)) return True;
     *     return False;
     * }
     *
     * void main()
     * {
     *     TNSCollection array(10, 5);
     *     array.insert(new XObject(14));
     *     array.insert(new XObject(32));
     *     array.insert(new XObject(23));
     *     array.insert(new XObject(41));
     *     int find = 23;
     *     XObject *p = (XObject *) array.firstThat(&matchTest, &find);
     *     if (p != 0) array.free(p);
     * }
     * </pre>
     * @see TNSCollection::forEach
     * @see TNSCollection::lastThat
     */
    void *firstThat( ccTestFunc Test, void *arg );
    /**
     * lastThat() applies the @ref Boolean function `Test', together with
     * the `arg' argument list to each item in the collection, starting at
     * the last item, and scanning in reverse order until the tested function
     * returns True.
     * The result is the item pointer for which the call returns True, or 0 if
     * the call returned False for all items.
     *
     * This method scans the array backward.
     * @see TNSCollection::firstThat
     * @see TNSCollection::forEach
     */
    void *lastThat( ccTestFunc Test, void *arg );
    /**
     * The forEach() iterator applies an action, given by the function
     * `action', to each item in the collection. The `arg' pointer can be used
     * to pass additional arguments to the action.
     *
     * `action' is a pointer to a function whose type ccAppFunc is defined as:
     *
     * <pre>
     * typedef void (*ccAppFunc)(void *, void *);
     * </pre>
     *
     * This method scans the array forward.
     * @see TNSCollection::firstThat
     * @see TNSCollection::lastThat
     */
    void forEach( ccAppFunc action, void *arg );
    /**
     * Packs the array by removing null pointers from it.
     *
     * Deletes all null pointers in the collection and moves items up to fill
     * any gaps.
     */
    void pack();
    /**
     * Expands or shrinks the collection by changing the allocated size to
     * `aLimit'.
     *
     * -# If `aLimit' is less than @ref count, it is set to @ref count.
     * -# if `aLimit' is greater than @ref maxCollectionSize, it is set to
     *    @ref maxCollectionSize. Integer constant maxCollectionSize is
     *    defined in file `tvconfig.h' as:
     *
     * <pre>
     * const int maxCollectionSize = INT_MAX / sizeof(void *);
     * </pre>
     *
     * Then, if `aLimit' is different from the current @ref limit, a new items
     * array of `aLimit' elements is allocated, the old @ref items array is
     * copied into the new array, and the old array is deleted.
     */
    virtual void setLimit( ccIndex aLimit );
    /**
     * Returns the number of items stored in the collection, up to
     * @ref maxCollectionSize.
     */
    ccIndex getCount()
        { return count; }
protected:
    /**
     * Constructor.
     *
     * This constructor sets variable @ref shouldDelete to True and variables
     * @ref count, @ref limit and @ref delta to 0.
     */
    TNSCollection();
    /**
     * A pointer to an array of generic item pointers.
     * This variable stores the array starting address.
     */
    void **items;
    /**
     * This variable stores the number of objects in the array.
     * @see TNSCollection::items
     * @see TNSCollection::limit
     */
    ccIndex count;
    /**
     * The currently allocated size (in elements) of the @ref items list.
     * Current size of the array. Greater or equal than @ref count.
     */
    ccIndex limit;
    /**
     * This value is used every time the array must be enlarged. In this case
     * a number of delta pointers will be added to the array.
     *
     * delta is the number of items by which to increase the @ref items list
     * whenever it becomes full. If delta is zero, the collection cannot grow
     * beyond the size set by @ref limit.
     */
    ccIndex delta;
    /**
     * If set True (the default), the TNSCollection destructor will call
     * @ref freeAll() before setting @ref limit to 0. All objects will be
     * deleted when method @ref shutDown() is called.
     *
     * If set False, the destructor simply sets @ref limit to 0.
     */
    Boolean shouldDelete;
private:
    /**
     * Undocumented.
     */
    virtual void freeItem( void *item );
};

#endif  // Uses_TNSCollection

#if defined( Uses_TNSSortedCollection ) && !defined( TV_TNSSortedCollection )
#define TV_TNSSortedCollection

/**
 * The abstract class TNSSortedCollection is a specialized derivative of
 * @ref TNSCollection implementing non-streamable collections sorted by a
 * key (with or without duplicates). No instances of TNSSortedCollection are
 * allowed. It exists solely as a base for other standard or user-defined
 * derived classes.
 * @short Handles a non-streamable collection sorted by a key (with or without
 * duplicates)
 */
class TNSSortedCollection: public virtual TNSCollection
{
public:
    /**
     * Invokes the TNSCollection constructor to set @ref count, @ref items
     * and @ref limit to 0; calls setLimit(aLimit) to set the collection
     * @ref limit to `aLimit', then sets @ref delta to `aDelta'.
     * @see TNSCollection::setLimit
     *
     * Sets @ref duplicates data member to False. If you want to allow
     * duplicate keys, you must set @ref duplicates to True.
     */
    TNSSortedCollection( ccIndex aLimit, ccIndex aDelta) :
        TNSCollection( aLimit, aDelta ), duplicates(False)
            { delta = aDelta; setLimit( aLimit ); }
    /**
     * Returns True if the item identified by `key' is found in the sorted
     * collection. If the item is found, `index' is set to the found index;
     * otherwise `index' is set to the index where the item would be placed if
     * inserted and False is returned.
     */
    virtual Boolean search( void *key, ccIndex& index );
    virtual ccIndex indexOf( void *item );
    /**
     * If @ref duplicates data member is False, insert works as follows: if
     * the target item is not found in the sorted collection, it is inserted
     * at the correct index position. Calls @ref search() to determine if the
     * item exists, and if not, where to insert it.
     *
     * If @ref duplicates is True, the item is inserted ahead of any items
     * (if any) with the same key.
     */
    virtual ccIndex insert( void *item );
    /**
     * Set to True if duplicate indexes are allowed; otherwise set to False.
     * The default is False.
     *
     * If @ref duplicates data member is True methods @ref search(),
     * @ref insert() and @ref indexOf() work differently.
     */
    Boolean duplicates;
    /**
     * Undocumented.
     */
    virtual void *keyOf( void *item );
protected:
    /**
     * Undocumented.
     */
    TNSSortedCollection() : duplicates(False) {}
private:
    /**
     * compare() is a pure virtual function that must be overridden in all
     * derived classes. compare() should compare the two key values, and
     * return a result.
     */
    virtual int compare( void *key1, void *key2 ) = 0;
};

#endif  // Uses_TNSSortedCollection
