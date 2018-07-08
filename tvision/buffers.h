/*
 * buffers.h
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#if defined( Uses_TVMemMgr ) && !defined( TV_TVMemMgr )
#define TV_TVMemMgr

/** \file buffers.h
 * buffers.h
 */

/**
 * Gives the initial default safety pool size in bytes. You can change this
 * value by editing the declaration. If you call
 * @ref TVMemMgr::resizeSafetyPool() with no size argument, this default
 * value is assumed.
 * @short Initial safety pool size
 */
const int DEFAULT_SAFETY_POOL_SIZE = 4096;

/**
 * TBufListEntry, in conjunction with @ref TVMemMgr, is used internally by
 * TVision to create and manage the video cache buffers for group drawing
 * operations. All its members are private and will seldom if ever be
 * referenced explicitly in normal applications. @ref TVMemMgr is a friend
 * class and the global operator new is a friend function.
 * @short Part of the memory manager
 */
class TBufListEntry
{
private:
    TBufListEntry( void*& );
    ~TBufListEntry();
    void *operator new( size_t, size_t );
    /**
     * Tries to allocate `sz' bytes on the heap. A pointer to the allocation
     * is returned if new() succeeds.
     *
     * If the allocation fails, cache buffers (if any) are freed one by one
     * and the allocation attempt is retried. If this fails and the safety
     * pool is "exhausted", new() calls abort(). Otherwise, allocation in the
     * safety pool is attempted.
     *
     * This attempt, whether successful or not, sets the private
     * TVMemMgr::safetyPool pointer to 0 (indicating that the safety pool is
     * "exhausted").
     *
     * If new() does allocate successfully from the safety pool, a pointer to
     * the allocation is returned; otherwise, abort() is called. Operator
     * new() is a friend function of TBufListEntry.
     */
    void *operator new( size_t sz );
    /**
     * Frees the allocation block from the heap. If the safety pool is
     * exhausted, @ref TVMemMgr::resizeSafetyPool is called.
     *
     * This frees the old safety pool and allocates a new, default safety
     * pool (usually 4,096 bytes, as defined by constant
     * @ref DEFAULT_SAFETY_POOL_SIZE in file `buffers.h').
     */
    void operator delete( void * );
    TBufListEntry *next;
    TBufListEntry *prev;
    void*& owner;
    static TBufListEntry *bufList;
    static Boolean freeHead();
    friend class TVMemMgr;
    friend void *operator new( size_t );
    friend void * allocBlock( size_t );
};

/**
 * TVMemMgr, in conjunction with @ref TBufListEntry and the global operator
 * new, provides low-level memory management for TVision applications.
 *
 * In particular, TVMemMgr manages the safety pool. For most applications,
 * TVMemMgr and @ref TBufListEntry objects work behind the scenes without the
 * need for specific programmer action or intervention.
 * @short Part of the memory manager
 */
class TVMemMgr
{
public:
    /**
     * Constructor.
     *
     * Does nothing if the safety pool is already initialized (that is, if
     * private data member inited = 1).
     *
     * Otherwise, calls method @ref TVMemMgr::resizeSafetyPool to establish
     * a default safety pool with the size given by the constant
     * @ref DEFAULT_SAFETY_POOL_SIZE.
     *
     * The latter is currently set to 4,096 (bytes) in `buffers.h'. The
     * constructor also sets inited to 1.
     */
    TVMemMgr();
    /**
     * Resizes the safety pool to `sz' bytes.
     *
     * The previous safety pool, if one exists, is freed first. inited private
     * data member is set to 1, safetyPool is set to point to the new
     * safety pool, and safetyPoolSize is set to `sz'. If the `sz' argument
     * is omitted, the default value is assumed.
     *
     * If `sz' is 0, both safetyPool and safetyPoolSize private data members
     * are set to 0.
     */
    static void resizeSafetyPool( size_t sz = DEFAULT_SAFETY_POOL_SIZE );
    /**
     * Returns 1 (True) if the safety pool is initialized and its allocation
     * is exhausted. Otherwise, returns 0 (False).
     */
    static int safetyPoolExhausted();
#ifndef __UNPATCHED
    /**
     * Undocumented.
     */
    static void clearSafetyPool();
#endif
    /**
     * For internal use only. Tries to allocate a cache buffer
     * (@ref TBufListEntry object) of size `sz'.
     *
     * If successful, `adr' returns a pointer to the allocated buffer;
     * otherwise, `adr' is set to 0. @ref TGroup::getBuffer() calls
     * allocateDiscardable() with `adr' set to the @ref TGroup::buffer data
     * member.
     */
    static void allocateDiscardable( void *&adr, size_t sz );
    /**
     * For internal use only. Frees the buffer allocated at block by an earlier
     * @ref allocateDiscardable() call.
     *
     * @ref TGroup::freeBuffer() calls freeDiscardable() with block set to the
     * @ref TGroup::buffer data member.
     */
    static void freeDiscardable( void * );
#ifndef __UNPATCHED
    /**
     * Undocumented.
     */
    static void suspend(void);
#endif
private:
    /**
     * Variable safetyPool points to the safety pool memory allocation.
     *
     * This value is zero if no safety pool has been allocated or if the
     * safety pool is exhausted.
     */
    static void * safetyPool;
    /**
     * The size in bytes of the current safety pool.
     *
     * This private member is updated by @ref TVMemMgr::resizeSafetyPool().
     * The default safety pool size is determined by the constant
     * @ref DEFAULT_SAFETY_POOL_SIZE, which is declared in `buffers.h' and is
     * currently set at 4,096 bytes.
     */
    static size_t safetyPoolSize;
    /**
     * This data member indicates whether safety pool initialization has been
     * attempted; it is strictly for internal use.
     */
    static int inited;
    static int initMemMgr();
};

#endif  // Uses_TVMemMgr
