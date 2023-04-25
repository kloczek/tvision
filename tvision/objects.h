/*
 * objects.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TPoint ) && !defined( TV_TPoint )
#define TV_TPoint

/** \file objects.h
 * objects.h
 */

/**
 * A screen coordinate. TPoint implements points on the screen with several
 * overloaded operators for point manipulation.
 *
 * TPoint is a simple object that can be used to record a coordinate on the
 * screen. For this, two public variables are available: `x' and `y'.
 * @see TRect
 * @short Two-point screen coordinate
 */
class TPoint {
      public:
    /**
     * Adds the coordinate of another point to this point.
     * Returns *this.
     */
	TPoint & operator+=(const TPoint & adder);
    /**
     * Subtracts the coordinate of another point from this point.
     * Returns *this.
     */
	 TPoint & operator-=(const TPoint & subber);
    /**
     * Calculates the distance between two points.
     * Returns a point with the resulting difference.
     */
	friend TPoint operator -(const TPoint & one, const TPoint & two);
    /**
     * Calculates the sum of two points.
     * Returns a point with the resulting sum.
     */
	friend TPoint operator +(const TPoint & one, const TPoint & two);
    /**
     * Returns True if two points are equal (have the same coordinate),
     * returns False otherwise.
     */
	friend int operator ==(const TPoint & one, const TPoint & two);
    /**
     * Returns True if two points are not equal (have different coordinate),
     * returns False otherwise.
     */
	friend int operator !=(const TPoint & one, const TPoint & two);
    /**
     * Is the screen column of the point.
     */
	int x;
    /**
     * Is the screen row of the point.
     */
	int y;
};

inline TPoint & TPoint::operator +=(const TPoint & adder)
{
	x += adder.x;
	y += adder.y;
	return *this;
}

inline TPoint & TPoint::operator -=(const TPoint & subber)
{
	x -= subber.x;
	y -= subber.y;
	return *this;
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TPoint & p)
{
	return is >> p.x >> p.y;
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TPoint *&p)
{
	return is >> p->x >> p->y;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TPoint & p)
{
	return os << p.x << p.y;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TPoint *p)
{
	return os << p->x << p->y;
}

#endif // Uses_TPoint

#if defined( Uses_TRect ) && !defined( TV_TRect )
#define TV_TRect

/**
 * A screen rectangular area.
 *
 * TRect is used to hold two coordinates on the screen, which usually specify
 * the upper left corner and the lower right corner of views. Sometimes the
 * second coordinate speficy the size (extension) of the view. The two
 * coordinates are named @ref a and @ref b.
 *
 * TRect has several inline member functions for manipulating rectangles.
 * The operators == and != are overloaded to provide the comparison of two
 * rectangles in a natural way.
 * @see TPoint
 * @see TRect::operator==
 * @see TRect::operator!=
 * @short Screen rectangular area
 */
class TRect {
      public:
    /**
     * Constructor.
     *
     * Initializes the rectangle coordinates using the four integer
     * parameters.
     */
	TRect(int ax, int ay, int bx, int by);
    /**
     * Constructor.
     *
     * Initializes the rectangle coordinates using two points.
     * @see TPoint
     */
	TRect(TPoint p1, TPoint p2);
    /**
     * Constructor.
     *
     * Does nothing. The two coordinates are not initialized, so they contain
     * garbage.
     */
	TRect();
    /**
     * Moves the rectangle to a new position.
     *
     * The two parameters are added to the two old coordinates as delta
     * values. Both parameters can be negative or positive.
     */
	void move(int aDX, int aDY);
    /**
     * Enlarges the rectangle by a specified value.
     *
     * Changes the size of the calling rectangle by subtracting `aDX' from
     * a.x, adding `aDX' to b.x, subtracting `aDY' from a.y, and adding `aDY'
     * to b.y.
     *
     * The left side is left-moved by `aDX' units and the right side is
     * right-moved by `aDX' units. In a similar way the upper side is
     * upper-moved by `aDY' units and the bottom side is bottom-moved by `aDY'
     * units.
     */
	void grow(int aDX, int aDY);
    /**
     * Calculates the intersection between this rectangle and the parameter
     * rectangle.
     *
     * The resulting rectangle is the largest rectangle which contains both
     * part of this rectangle and part of the parameter rectangle.
     */
	void intersect(const TRect & r);
    /**
     * Calculates the union between this rectangle and the `r' parameter
     * rectangle.
     *
     * The resulting rectangle is the smallest rectangle which contains both
     * this rectangle and the `r' rectangle.
     */
	void Union(const TRect & r);
    /**
     * Returns True if the calling rectangle (including its boundary) contains
     * the point `p', returns False otherwise.
     * @see TPoint
     */
	Boolean contains(const TPoint & p) const;
    /**
     * Returns True if `r' is the same as the calling rectangle; otherwise,
     * returns False.
     */
	Boolean operator ==(const TRect & r) const;
    /**
     * Returns True if `r' is not the same as the calling rectangle;
     * otherwise, returns False.
     */
	Boolean operator !=(const TRect & r) const;
    /**
     * Checks if the rectangle is empty, i.e. if the first coordinate is
     * greater than the second one.
     *
     * Empty means that (a.x >=  b.x || a.y >= b.y).
     */
	Boolean isEmpty();
    /**
     * Is the point defining the top-left corner of a rectangle on the screen.
     */
	TPoint a;
    /**
     * Is the point defining the bottom-right corner of a rectangle on the
     * screen.
     */
	TPoint b;
};

inline TRect::TRect(int ax, int ay, int bx, int by)
{
	a.x = ax;
	a.y = ay;
	b.x = bx;
	b.y = by;
}

inline TRect::TRect(TPoint p1, TPoint p2)
{
	a = p1;
	b = p2;
}

inline TRect::TRect()
{
}

inline void TRect::move(int aDX, int aDY)
{
	a.x += aDX;
	a.y += aDY;
	b.x += aDX;
	b.y += aDY;
}

inline void TRect::grow(int aDX, int aDY)
{
	a.x -= aDX;
	a.y -= aDY;
	b.x += aDX;
	b.y += aDY;
}

inline void TRect::intersect(const TRect & r)
{
	a.x = max(a.x, r.a.x);
	a.y = max(a.y, r.a.y);
	b.x = min(b.x, r.b.x);
	b.y = min(b.y, r.b.y);
}

inline void TRect::Union(const TRect & r)
{
	a.x = min(a.x, r.a.x);
	a.y = min(a.y, r.a.y);
	b.x = max(b.x, r.b.x);
	b.y = max(b.y, r.b.y);
}

inline Boolean TRect::contains(const TPoint & p) const
{
	return Boolean(p.x >= a.x && p.x < b.x && p.y >= a.y && p.y < b.y);
}

inline Boolean TRect::operator ==(const TRect & r) const
{
	return Boolean(a == r.a && b == r.b);
}

inline Boolean TRect::operator !=(const TRect & r) const
{
	return Boolean(!(*this == r));
}

inline Boolean TRect::isEmpty()
{
	return Boolean(a.x >= b.x || a.y >= b.y);
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TRect & r)
{
	return is >> r.a >> r.b;
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TRect *&r)
{
	return is >> r->a >> r->b;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TRect & r)
{
	return os << r.a << r.b;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TRect *r)
{
	return os << r->a << r->b;
}

#endif // Uses_TRect

#if defined( Uses_TCollection ) && !defined( TV_TCollection )
#define TV_TCollection

/**
 * TCollection implements a streamable collection of arbitrary items,
 * including other objects.
 *
 * Its main purpose is to provide a base class for more useful streamable
 * collection classes. @ref TNSCollection (the nonstreamable collection
 * class) is a virtual base class for TCollection, providing the functions
 * for adding, accessing, and removing items from a collection.
 * @ref TStreamable provides TCollection with the ability to create and name
 * streams.
 *
 * Several friend functions and the overloaded operators, >> and <<, provide
 * the ability to write and read collections to and from streams.
 *
 * A collection is a more general concept than the traditional array, set, or
 * list.
 *
 * TCollection objects size themselves dynamically at run time and offer a
 * base for more specialized derived classes such as @ref TSortedCollection,
 * @ref TStringCollection, and @ref TResourceCollection.
 *
 * TCollection inherits from @ref TNSCollection the member functions for
 * adding and deleting items, as well as several iterator routines that call
 * a function for each item in the collection.
 *
 * Note: type ccIndex is defined in file `ttypes.h' as int.
 * @short Streamable collection of items
 */
class TCollection:public virtual TNSCollection, public TStreamable {
      public:
    /**
     * Constructor.
     *
     * Creates a collection with limit set to `aLimit' and delta set to
     * `aDelta'. The initial number of items will be limited to `aLimit', but
     * the collection is allowed to grow in increments of `aDelta' until
     * memory runs out or the number of items reaches @ref maxCollectionSize,
     * an integer constant defined in `tvconfig.h' as follows:
     *
     * <pre>
     * const int @ref maxCollectionSize = INT_MAX / sizeof(void *);
     * </pre>
     */
	TCollection(ccIndex aLimit, ccIndex aDelta) {
		delta = aDelta;
		setLimit(aLimit);
      } private:
	virtual const char *streamableName() const {
		return name;
	}
    /**
     * Define this pure virtual function in your derived class to read and
     * return an item from the ipstream in a type-safe manner.
     *
     * This is usually done with a sequence of >> operations for each data
     * member in your derived class.
     */ virtual void *readItem(ipstream &) = 0;
    /**
     * Define this pure virtual function in your derived class to write an
     * item to the opstream.
     *
     * This is usually done with a sequence of << operations for each data
     * member in your derived class.
     */
	virtual void writeItem(void *, opstream &) = 0;
      protected:
    /**
     * Constructor.
     *
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized vtable pointers. This is
     * achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
	TCollection(StreamableInit);
    /**
     * Reads a collection from the input stream `is' to the associated
     * TCollection object.
     */
	virtual void *read(ipstream & is);
    /**
     * Writes the associated collection to the output stream `os'.
     */
	virtual void write(opstream & os);
      public:
    /**
     * The name of the collection class, "TCollection". Used internally by the
     * stream manager.
     */
	static const char *const name;
};

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TCollection & cl)
{
	return is >> static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TCollection *&cl)
{
	return is >> (void *&)cl;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TCollection & cl)
{
	return os << static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TCollection *cl)
{
	return os << static_cast < TStreamable * >(cl);
}

#endif // Uses_TCollection

#if defined( Uses_TSortedCollection ) && !defined( TV_TSortedCollection )
#define TV_TSortedCollection

/**
 * The abstract class TSortedCollection is a specialized derivative of both
 * @ref TCollection and @ref TNSSortedCollection. It implements streamable
 * collections sorted by a key (with or without duplicates).
 *
 * No instances of TSortedCollection are allowed. It exists solely as a base
 * for other standard or user-defined derived classes.
 * @short Sorted, streamable collection of objects
 */
class TSortedCollection:public TNSSortedCollection, public TCollection {
      public:
    /**
     * Invokes the @ref TCollection constructor to set @ref count and
     * @ref items data members to 0; calls setLimit(aLimit) to set the
     * collection @ref limit to `aLimit', then sets @ref delta to `aDelta'.
     * Note that ccIndex is a typedef of int.
     *
     * @ref duplicates is set to False. If you want to allow duplicate keys,
     * you must set @ref duplicates True.
     */
	TSortedCollection(ccIndex aLimit, ccIndex aDelta):TCollection(aLimit,
								      aDelta) {
      } private:
    /**
     * compare() is a pure virtual function that must be overridden in all
     * derived classes (or redefined as pure virtual).
     */
	virtual int compare(void *key1, void *key2) = 0;
	virtual const char *streamableName() const {
		return name;
	}
    /**
     * Called for each item in the collection. You'll need to override these
     * in everything derived from @ref TCollection or TSortedCollection in
     * order to read the items correctly. TSortedCollection overrides this
     * function with a pure virtual function.
     */ virtual void *readItem(ipstream & is) = 0;
    /**
     * Called for each item in the collection. You'll need to override these
     * in everything derived from @ref TCollection or TSortedCollection in
     * order to write the items correctly. TSortedCollection overrides this
     * function with a pure virtual function.
     */
	virtual void writeItem(void *, opstream & os) = 0;
      protected:
    /**
     * Each streamable class needs a "builder" to allocate the correct memory
     * for its objects together with the initialized vtable pointers. This is
     * achieved by calling this constructor with an argument of type
     * @ref StreamableInit.
     */
	TSortedCollection(StreamableInit);
    /**
     * Reads a sorted collection from the input stream `is'.
     */
	virtual void *read(ipstream & is);
    /**
     * Writes the associated TSortedCollection object to the output stream
     * `os'.
     */
	virtual void write(opstream & os);
      public:
    /**
     * Undocumented.
     */
	static const char *const name;
};

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TSortedCollection & cl)
{
	return is >> static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline ipstream & operator >>(ipstream & is, TSortedCollection *&cl)
{
	return is >> (void *&)cl;
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TSortedCollection & cl)
{
	return os << static_cast < TStreamable & >(cl);
}

/**
 * Undocumented.
 */
inline opstream & operator <<(opstream & os, TSortedCollection *cl)
{
	return os << static_cast < TStreamable * >(cl);
}

#endif // Uses_TSortedCollection
