/* Copyright 2012 William Woodall and John Harrison */

/*
  Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
  Copyright (c) 2022 Immo Wache <virtual.mkr@gmail.com>
*/

#include "serial.h"
#include "win.h"

using std::invalid_argument;
using std::min;
using std::numeric_limits;
using std::vector;
using std::size_t;
using std::string;

using serial::SerialClass;
using serial::SerialException;
using serial::IOException;
using serial::bytesize_t;
using serial::parity_t;
using serial::stopbits_t;
using serial::flowcontrol_t;

class SerialClass::ScopedReadLock {
public:
  ScopedReadLock(SerialImpl *pimpl) : pimpl_(pimpl) {
    this->pimpl_->readLock();
  }
  ~ScopedReadLock() {
    this->pimpl_->readUnlock();
  }
private:
  // Disable copy constructors
  ScopedReadLock(const ScopedReadLock&);
////  const ScopedReadLock& operator=(ScopedReadLock);

  SerialImpl *pimpl_;
};

class SerialClass::ScopedWriteLock {
public:
  ScopedWriteLock(SerialImpl *pimpl) : pimpl_(pimpl) {
    this->pimpl_->writeLock();
  }
  ~ScopedWriteLock() {
    this->pimpl_->writeUnlock();
  }
private:
  // Disable copy constructors
  ScopedWriteLock(const ScopedWriteLock&);
////  const ScopedWriteLock& operator=(ScopedWriteLock);
  SerialImpl *pimpl_;
};

SerialClass::SerialClass (const string &port, uint32_t baudrate, serial::Timeout timeout,
                bytesize_t bytesize, parity_t parity, stopbits_t stopbits,
                flowcontrol_t flowcontrol)
 : pimpl_(new SerialImpl (port, baudrate, bytesize, parity,
                                           stopbits, flowcontrol))
{
  pimpl_->setTimeout(timeout);
}

SerialClass::~SerialClass ()
{
  delete pimpl_;
}

void
SerialClass::open ()
{
  pimpl_->open ();
}

void
SerialClass::close ()
{
  pimpl_->close ();
}

bool
SerialClass::isOpen () const
{
  return pimpl_->isOpen ();
}

size_t
SerialClass::available ()
{
  return pimpl_->available ();
}

bool
SerialClass::waitReadable ()
{
  serial::Timeout timeout(pimpl_->getTimeout ());
  return pimpl_->waitReadable(timeout.read_timeout_constant);
}

void
SerialClass::waitByteTimes (size_t count)
{
  pimpl_->waitByteTimes(count);
}

size_t
SerialClass::read_ (uint8_t *buffer, size_t size)
{
  return this->pimpl_->read (buffer, size);
}

size_t
SerialClass::read (uint8_t *buffer, size_t size)
{
  ScopedReadLock lock(this->pimpl_);
  return this->pimpl_->read (buffer, size);
}

size_t
SerialClass::read (std::vector<uint8_t> &buffer, size_t size)
{
  ScopedReadLock lock(this->pimpl_);
  uint8_t *buffer_ = new uint8_t[size];
  size_t bytes_read = 0;

  try {
    bytes_read = this->pimpl_->read (buffer_, size);
  }
  catch (const std::exception &e) {
    (void)e;
    delete[] buffer_;
    throw;
  }

  buffer.insert (buffer.end (), buffer_, buffer_+bytes_read);
  delete[] buffer_;
  return bytes_read;
}

size_t
SerialClass::read (std::string &buffer, size_t size)
{
  ScopedReadLock lock(this->pimpl_);
  uint8_t *buffer_ = new uint8_t[size];
  size_t bytes_read = 0;
  try {
    bytes_read = this->pimpl_->read (buffer_, size);
  }
  catch (const std::exception &e) {
    (void)e;
    delete[] buffer_;
    throw;
  }
  buffer.append (reinterpret_cast<const char*>(buffer_), bytes_read);
  delete[] buffer_;
  return bytes_read;
}

string
SerialClass::read (size_t size)
{
  std::string buffer;
  this->read (buffer, size);
  return buffer;
}

size_t
SerialClass::write (const string &data)
{
  ScopedWriteLock lock(this->pimpl_);
  return this->write_ (reinterpret_cast<const uint8_t*>(data.c_str()),
                       data.length());
}

size_t
SerialClass::write (const std::vector<uint8_t> &data)
{
  ScopedWriteLock lock(this->pimpl_);
  return this->write_ (&data[0], data.size());
}

size_t
SerialClass::write (const uint8_t *data, size_t size)
{
  ScopedWriteLock lock(this->pimpl_);
  return this->write_(data, size);
}

size_t
SerialClass::write_ (const uint8_t *data, size_t length)
{
  return pimpl_->write (data, length);
}

void
SerialClass::setPort (const string &port)
{
  ScopedReadLock rlock(this->pimpl_);
  ScopedWriteLock wlock(this->pimpl_);
  bool was_open = pimpl_->isOpen ();
  if (was_open) close();
  pimpl_->setPort (port);
  if (was_open) open ();
}

string
SerialClass::getPort () const
{
  return pimpl_->getPort ();
}

void
SerialClass::setTimeout (serial::Timeout &timeout)
{
  pimpl_->setTimeout (timeout);
}

serial::Timeout
SerialClass::getTimeout () const {
  return pimpl_->getTimeout ();
}

void
SerialClass::setBaudrate (uint32_t baudrate)
{
  pimpl_->setBaudrate (baudrate);
}

uint32_t
SerialClass::getBaudrate () const
{
  return uint32_t(pimpl_->getBaudrate ());
}

void
SerialClass::setBytesize (bytesize_t bytesize)
{
  pimpl_->setBytesize (bytesize);
}

bytesize_t
SerialClass::getBytesize () const
{
  return pimpl_->getBytesize ();
}

void
SerialClass::setParity (parity_t parity)
{
  pimpl_->setParity (parity);
}

parity_t
SerialClass::getParity () const
{
  return pimpl_->getParity ();
}

void
SerialClass::setStopbits (stopbits_t stopbits)
{
  pimpl_->setStopbits (stopbits);
}

stopbits_t
SerialClass::getStopbits () const
{
  return pimpl_->getStopbits ();
}

void
SerialClass::setFlowcontrol (flowcontrol_t flowcontrol)
{
  pimpl_->setFlowcontrol (flowcontrol);
}

flowcontrol_t
SerialClass::getFlowcontrol () const
{
  return pimpl_->getFlowcontrol ();
}

void SerialClass::flush ()
{
  ScopedReadLock rlock(this->pimpl_);
  ScopedWriteLock wlock(this->pimpl_);
  pimpl_->flush ();
}

void SerialClass::flushInput ()
{
  ScopedReadLock lock(this->pimpl_);
  pimpl_->flushInput ();
}

void SerialClass::flushOutput ()
{
  ScopedWriteLock lock(this->pimpl_);
  pimpl_->flushOutput ();
}

void SerialClass::sendBreak (int duration)
{
  pimpl_->sendBreak (duration);
}

void SerialClass::setBreak (bool level)
{
  pimpl_->setBreak (level);
}

void SerialClass::setRTS (bool level)
{
  pimpl_->setRTS (level);
}

void SerialClass::setDTR (bool level)
{
  pimpl_->setDTR (level);
}

bool SerialClass::waitForChange()
{
  return pimpl_->waitForChange();
}

bool SerialClass::getCTS ()
{
  return pimpl_->getCTS ();
}

bool SerialClass::getDSR ()
{
  return pimpl_->getDSR ();
}

bool SerialClass::getRI ()
{
  return pimpl_->getRI ();
}

bool SerialClass::getCD ()
{
  return pimpl_->getCD ();
}
