/*
 * tobjstrm.cc
 *
 * Turbo Vision - Version 2.0
 *
 * Copyright (c) 1994 by Borland International
 * All Rights Reserved.
 *
 * Modified by Sergio Sigala <sergio@sigala.it>
 */

#define Uses_TStreamable
#define Uses_TStreamableClass
#define Uses_TStreamableTypes
#define Uses_TPWrittenObjects
#define Uses_TPReadObjects
#define Uses_pstream
#define Uses_ipstream
#define Uses_opstream
#define Uses_iopstream
#define Uses_fpbase
#define Uses_ifpstream
#define Uses_ofpstream
#define Uses_fpstream
#include <tv.h>

#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <cstring>
#include <limits>
#include <fstream>

const uchar nullStringLen = UCHAR_MAX;

TStreamableClass::TStreamableClass(const char *n, BUILDER b, int d):name(n),
build(b), delta(d)
{
	pstream::initTypes();
	pstream::registerType(this);
}

TStreamableTypes::TStreamableTypes():TNSSortedCollection(5, 5)
{
}

void *TStreamableTypes::operator  new(size_t, void *arena)
{
	return arena;
}

TStreamableTypes::~TStreamableTypes()
{
}

void TStreamableTypes::registerType(const TStreamableClass *d)
{
	insert((void *)d);
}

const TStreamableClass *TStreamableTypes::lookup(const char *name)
{
	ccIndex loc;
	if (search((void *)name, loc))
		return (TStreamableClass *) at(loc);
	else
		return nullptr;
}

void *TStreamableTypes::keyOf(void *d)
{
	return (void *)((TStreamableClass *) d)->name;
}

int TStreamableTypes::compare(void *d1, void *d2)
{
	return std::strcmp((char *)d1, (char *)d2);
}

TPWrittenObjects::TPWrittenObjects():TNSSortedCollection(5, 5), curId(0)
{
}

TPWrittenObjects::~TPWrittenObjects()
{
}

void TPWrittenObjects::registerObject(const void *adr)
{
	TPWObj *o = new TPWObj(adr, curId++);
	insert(o);
}

P_id_type TPWrittenObjects::find(const void *d)
{
	ccIndex loc;
	if (search((void *)d, loc))
		return ((TPWObj *) at(loc))->ident;
	else
		return P_id_notFound;
}

void *TPWrittenObjects::keyOf(void *d)
{
	return (void *)((TPWObj *) d)->address;
}

int TPWrittenObjects::compare(void *o1, void *o2)
{
	if (o1 == o2)
		return 0;
	else if (((char *)o1) + 1 < ((char *)o2) + 1)	// force normalization
		return -1;
	else
		return 1;
}

TPWObj::TPWObj(const void *adr, P_id_type id):address(adr), ident(id)
{
}

TPReadObjects::TPReadObjects():TNSCollection(5, 5), curId(0)
{
}

TPReadObjects::~TPReadObjects()
{
}

void TPReadObjects::registerObject(const void *adr)
{
	ccIndex loc = insert((void *)adr);
#ifndef __UNPATCHED
	assert(loc == (ccIndex) curId);	// to be sure that TNSCollection
#else
	assert(loc == curId++);	// to be sure that TNSCollection
#endif
	// continues to work the way
	// it does now...
#ifndef __UNPATCHED
	curId++;		// Move the increment OUTSIDE the assertion.
#endif
}

const void *TPReadObjects::find(P_id_type id)
{
	return at(id);
}

pstream::pstream(std::streambuf *sb)
{
	init(sb);
}

pstream::~pstream()
{
}

void pstream::initTypes()
{
	if (types == nullptr)
		types = new TStreamableTypes;
}

int pstream::rdstate() const
{
	return state;
}

int pstream::eof() const
{
	return state & std::ios::eofbit;
}

int pstream::fail() const
{
	return state & (std::ios::failbit | std::ios::badbit);	//| ios::hardfail);
}

int pstream::bad() const
{
	return state & (std::ios::badbit);	//| ios::hardfail);
}

int pstream::good() const
{
	return state == 0;
}

void pstream::clear(int i)
{
	state = (i & 0xFF);	//| (state & ios::hardfail);
}

void pstream::registerType(TStreamableClass *ts)
{
	types->registerType(ts);
}

pstream::operator  void *()
const {
	return fail() ? nullptr : (void *)this;
} int pstream::operator!() const
{
	return fail();
}

std::streambuf * pstream::rdbuf()const
{
	return bp;
}

pstream::pstream()
{
}

void pstream::error(StreamableError)
{
	abort();
}

void pstream::error(StreamableError, const TStreamable &)
{
	abort();
}

void pstream::init(std::streambuf *sbp)
{
	state = 0;
	bp = sbp;
}

void pstream::setstate(int b)
{
	state |= (b & 0xFF);
}

ipstream::ipstream(std::streambuf *sb)
{
	pstream::init(sb);
}

ipstream::~ipstream()
{
	objs.shouldDelete = False;
	objs.shutDown();
}

std::streampos ipstream::tellg()
{
	return bp->pubseekoff(0, std::ios::cur, std::ios::in);
}

ipstream & ipstream::seekg(std::streampos pos)
{
	objs.removeAll();
	bp->pubseekoff(pos, std::ios::beg);
	return *this;
}

ipstream & ipstream::seekg(std::streamoff off, std::ios::seekdir dir)
{
	objs.removeAll();
	bp->pubseekoff(off, dir);
	return *this;
}

uchar ipstream::readByte()
{
	return bp->sbumpc();
}

ushort ipstream::readWord()
{
	/* SS: words are stored in little endian format (LSB first) */
	return readByte() | (readByte() << 8);
}

void ipstream::readBytes(void *data, size_t sz)
{
	bp->sgetn((char *)data, sz);
}

char *ipstream::readString()
{
	uchar len = readByte();
	if (len == nullStringLen)
		return nullptr;
	char *buf = new char[len + 1];
	if (buf == nullptr)
		return nullptr;
	readBytes(buf, len);
	buf[len] = EOS;
	return buf;
}

char *ipstream::readString(char *buf, unsigned maxLen)
{
	assert(buf != nullptr);

	uchar len = readByte();
	if (len > maxLen - 1)
		return nullptr;
	readBytes(buf, len);
	buf[len] = EOS;
	return buf;
}

ipstream & operator >>(ipstream & ps, char &ch)
{
	ch = ps.readByte();
	return ps;
}

ipstream & operator >>(ipstream & ps, signed char &ch)
{
	ch = ps.readByte();
	return ps;
}

ipstream & operator >>(ipstream & ps, unsigned char &ch)
{
	ch = ps.readByte();
	return ps;
}

ipstream & operator >>(ipstream & ps, signed short &sh)
{
	sh = ps.readWord();
	return ps;
}

ipstream & operator >>(ipstream & ps, unsigned short &sh)
{
	sh = ps.readWord();
	return ps;
}

ipstream & operator >>(ipstream & ps, signed int &i)
{
	/* SS: ints are stored in little endian format (LSB first) */
	i = ps.readByte() | (ps.readByte() << 8) | (ps.readByte() << 16) |
	    (ps.readByte() << 24);
	return ps;
}

ipstream & operator >>(ipstream & ps, unsigned int &i)
{
	/* SS: ints are stored in little endian format (LSB first) */
	i = ps.readByte() | (ps.readByte() << 8) | (ps.readByte() << 16) |
	    (ps.readByte() << 24);
	return ps;
}

ipstream & operator >>(ipstream & ps, signed long &l)
{
	/* SS: longs are stored in little endian format (LSB first) */
	l = ps.readByte() | (ps.readByte() << 8) | (ps.readByte() << 16) |
	    (ps.readByte() << 24);
	return ps;
}

ipstream & operator >>(ipstream & ps, unsigned long &l)
{
	/* SS: longs are stored in little endian format (LSB first) */
	l = ps.readByte() | (ps.readByte() << 8) | (ps.readByte() << 16) |
	    (ps.readByte() << 24);
	return ps;
}

ipstream & operator >>(ipstream & ps, float &f)
{
	ps.readBytes(&f, sizeof(f));
	return ps;
}

ipstream & operator >>(ipstream & ps, double &d)
{
	ps.readBytes(&d, sizeof(d));
	return ps;
}

ipstream & operator >>(ipstream & ps, long double &ld)
{
	ps.readBytes(&ld, sizeof(ld));
	return ps;
}

ipstream & operator >>(ipstream & ps, TStreamable & t)
{
	const TStreamableClass *pc = ps.readPrefix();
	ps.readData(pc, &t);
	ps.readSuffix();
	return ps;
}

ipstream & operator >>(ipstream & ps, void *&t)
{
	char ch = ps.readByte();
	switch (ch) {
	case pstream::ptNull:
		t = nullptr;
		break;
	case pstream::ptIndexed:
		{
			P_id_type index = ps.readWord();
			t = (void *)ps.find(index);
			assert(t != nullptr);
			break;
		}
	case pstream::ptObject:
		{
			const TStreamableClass *pc = ps.readPrefix();
			t = ps.readData(pc, nullptr);
			ps.readSuffix();
			break;
		}
	default:
		ps.error(pstream::peInvalidType);
		break;
	}
	return ps;
}

ipstream::ipstream()
{
}

const TStreamableClass *ipstream::readPrefix()
{
	char ch = readByte();
	assert(ch == '[');	// don't combine this with the previous line!
	// We must always do the read, even if we're
	// not checking assertions

	char name[128];
	readString(name, sizeof name);
	return types->lookup(name);
}

void *ipstream::readData(const TStreamableClass *c, TStreamable *mem)
{
	if (mem == nullptr)
		mem = c->build();

	registerObject((char *)mem - c->delta);	// register the actual address
	// of the object, not the address
	// of the TStreamable sub-object
	return mem->read(*this);
}

void ipstream::readSuffix()
{
	char ch = readByte();
	assert(ch == ']');	// don't combine this with the previous line!
	// We must always do the write, even if we're
	// not checking assertions

}

const void *ipstream::find(P_id_type id)
{
	return objs.find(id);
}

void ipstream::registerObject(const void *adr)
{
	objs.registerObject(adr);
}

opstream::opstream()
{
	objs = new TPWrittenObjects;
}

opstream::opstream(std::streambuf *buf)
{
	objs = new TPWrittenObjects;
	pstream::init(buf);
}

opstream::~opstream()
{
	objs->shutDown();
	delete objs;
}

opstream & opstream::seekp(std::streampos pos)
{
#ifndef __UNPATCHED
	objs->freeAll();	// CMF 07.11.92 --- delete the TPWObj's
#endif
	objs->removeAll();
	bp->pubseekoff(pos, std::ios::beg);
	return *this;
}

opstream & opstream::seekp(std::streamoff off, std::ios::seekdir dir)
{
#ifndef __UNPATCHED
	objs->freeAll();	// CMF 07.11.92 ... s.a.
#endif
	objs->removeAll();
	bp->pubseekoff(off, dir);
	return *this;
}

std::streampos opstream::tellp()
{
	return bp->pubseekoff(0, std::ios::cur, std::ios::out);
}

opstream & opstream::flush()
{
	bp->pubsync();
	return *this;
}

void opstream::writeByte(uchar ch)
{
	bp->sputc(ch);
}

void opstream::writeBytes(const void *data, size_t sz)
{
	bp->sputn((char *)data, sz);
}

void opstream::writeWord(ushort sh)
{
	/* SS: words are stored in little endian format (LSB first) */
	writeByte(sh & 0xff);
	writeByte((sh >> 8) & 0xff);
}

void opstream::writeString(const char *str)
{
	if (str == nullptr) {
		writeByte(nullStringLen);
		return;
	}
	int len = strlen(str);
	writeByte((uchar) len);
	writeBytes(str, len);
}

opstream & operator <<(opstream & ps, char ch)
{
	ps.writeByte(ch);
	return ps;
}

opstream & operator <<(opstream & ps, signed char ch)
{
	ps.writeByte(ch);
	return ps;
}

opstream & operator <<(opstream & ps, unsigned char ch)
{
	ps.writeByte(ch);
	return ps;
}

opstream & operator <<(opstream & ps, signed short sh)
{
	ps.writeWord(sh);
	return ps;
}

opstream & operator <<(opstream & ps, unsigned short sh)
{
	ps.writeWord(sh);
	return ps;
}

opstream & operator <<(opstream & ps, signed int i)
{
	/* SS: ints are stored in little endian format (LSB first) */
	ps.writeByte(i & 0xff);
	ps.writeByte((i >> 8) & 0xff);
	ps.writeByte((i >> 16) & 0xff);
	ps.writeByte((i >> 24) & 0xff);
	return ps;
}

opstream & operator <<(opstream & ps, unsigned int i)
{
	/* SS: ints are stored in little endian format (LSB first) */
	ps.writeByte(i & 0xff);
	ps.writeByte((i >> 8) & 0xff);
	ps.writeByte((i >> 16) & 0xff);
	ps.writeByte((i >> 24) & 0xff);
	return ps;
}

opstream & operator <<(opstream & ps, signed long l)
{
	/* SS: longs are stored in little endian format (LSB first) */
	ps.writeByte(l & 0xff);
	ps.writeByte((l >> 8) & 0xff);
	ps.writeByte((l >> 16) & 0xff);
	ps.writeByte((l >> 24) & 0xff);
	return ps;
}

opstream & operator <<(opstream & ps, unsigned long l)
{
	/* SS: longs are stored in little endian format (LSB first) */
	ps.writeByte(l & 0xff);
	ps.writeByte((l >> 8) & 0xff);
	ps.writeByte((l >> 16) & 0xff);
	ps.writeByte((l >> 24) & 0xff);
	return ps;
}

opstream & operator <<(opstream & ps, float f)
{
	ps.writeBytes(&f, sizeof(f));
	return ps;
}

opstream & operator <<(opstream & ps, double d)
{
	ps.writeBytes(&d, sizeof(d));
	return ps;
}

opstream & operator <<(opstream & ps, long double ld)
{
	ps.writeBytes(&ld, sizeof(ld));
	return ps;
}

opstream & operator <<(opstream & ps, TStreamable & t)
{
	ps.writePrefix(t);
	ps.writeData(t);
	ps.writeSuffix(t);
	return ps;
}

opstream & operator <<(opstream & ps, TStreamable *t)
{
	P_id_type index;
	if (t == nullptr)
		ps.writeByte(pstream::ptNull);
	else if ((index = ps.find(t)) != P_id_notFound) {
		ps.writeByte(pstream::ptIndexed);
		ps.writeWord(index);
	} else {
		ps.writeByte(pstream::ptObject);
		ps << *t;
	}
	return ps;
}

void opstream::writePrefix(const TStreamable & t)
{
	writeByte('[');
	writeString(t.streamableName());
}

void opstream::writeData(TStreamable & t)
{
	if (types->lookup(t.streamableName()) == nullptr)
		error(peNotRegistered, t);
	else {
		registerObject(&t);
		t.write(*this);
	}
}

void opstream::writeSuffix(const TStreamable &)
{
	writeByte(']');
}

P_id_type opstream::find(const void *adr)
{
	return objs->find(adr);
}

void opstream::registerObject(const void *adr)
{
	objs->registerObject(adr);
}

iopstream::iopstream(std::streambuf *sb)
{
	pstream::init(sb);
}

iopstream::~iopstream()
{
}

iopstream::iopstream()
{
}

fpbase::fpbase()
{
	pstream::init(&buf);
}

fpbase::fpbase(const char *name, std::ios::openmode omode)
{
	pstream::init(&buf);
	open(name, omode);
}

fpbase::~fpbase()
{
}

void fpbase::open(const char *b, std::ios::openmode m)
{
	if (buf.is_open())
		clear(std::ios::failbit);	// fail - already open
	else if (buf.open(b, m))
		clear(std::ios::goodbit);	// successful open
	else
		clear(std::ios::badbit);	// open failed
}

void fpbase::close()
{
	if (buf.close())
		clear(std::ios::goodbit);
	else
		setstate(std::ios::failbit);
}

void fpbase::setbuf(char *b, std::streamsize len)
{
	if (buf.pubsetbuf(b, len))
		clear(std::ios::goodbit);
	else
		setstate(std::ios::failbit);
}

std::filebuf * fpbase::rdbuf()
{
	return &buf;
}

ifpstream::ifpstream()
{
}

ifpstream::ifpstream(const char *name, std::ios::openmode omode):fpbase(name,
									omode |
									std::
									ios::
									in |
									std::
									ios::
									binary)
{
}

ifpstream::~ifpstream()
{
}

std::filebuf * ifpstream::rdbuf()
{
	return fpbase::rdbuf();
}

void ifpstream::open(const char *name, std::ios::openmode omode)
{
	fpbase::open(name, omode | std::ios::in | std::ios::binary);
}

ofpstream::ofpstream()
{
}

ofpstream::ofpstream(const char *name, std::ios::openmode omode):fpbase(name,
									omode |
									std::
									ios::
									out |
									std::
									ios::
									binary)
{
}

ofpstream::~ofpstream()
{
}

std::filebuf * ofpstream::rdbuf()
{
	return fpbase::rdbuf();
}

void ofpstream::open(const char *name, std::ios::openmode omode)
{
	fpbase::open(name, omode | std::ios::out | std::ios::binary);
}

fpstream::fpstream()
{
}

fpstream::fpstream(const char *name, std::ios::openmode omode):fpbase(name,
								      omode |
								      std::ios::
								      out |
								      std::ios::
								      binary)
{
}

fpstream::~fpstream()
{
}

std::filebuf * fpstream::rdbuf()
{
	return fpbase::rdbuf();
}

void fpstream::open(const char *name, std::ios::openmode omode)
{
	fpbase::open(name,
		     omode | std::ios::in | std::ios::out | std::ios::binary);
}
