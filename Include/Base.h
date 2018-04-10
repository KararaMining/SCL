//*************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Base.h
// SUBSYSTEM:           Base Class for Storage Class Library
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2010, 2013-2017 Gavin Blakeman.
//                      This file is part of the Storage Class Library (SCL)
//
//                      SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with SCL.  If not, see
//                      <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Provides a group of storage class implementations.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-05-21 GGB - Added functions to CPackage to support CAny.
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2010-04-28 GGB - Unscrabble Application Release 1.00
//
//*************************************************************************************************

#ifndef SCL_BASE_H
#define SCL_BASE_H

#include <ostream>
#include <typeinfo>

#include "common.h"

namespace SCL
{

  class CPackage
  {
  private:
  protected:
  public:
    CPackage(){}
    virtual  ~CPackage() {}
    virtual CPackage *createCopy() const { return nullptr; }

    inline virtual bool operator==(const CPackage &toTest) { return this == &toTest; }	// Check pointers equal.

    virtual std::string stringOutput() const { return std::string(); }

    virtual std::type_info const &type() const noexcept { return typeid(CPackage); }
  };
  typedef CPackage *PPackage;

  class CSortablePackage : public CPackage
  {
  private:
  protected:
  public:
    CSortablePackage() {}
    virtual ~CSortablePackage() {}

    virtual bool operator<(CSortablePackage const &) const { SCL_CODE_ERROR; }
    virtual bool operator==(CSortablePackage const &) const { SCL_CODE_ERROR; }
  };
  typedef CSortablePackage *PSortablePackage;

  class CContainer
  {
  public:
    typedef unsigned long size_type;
  private:
    size_type lPackageCount;						// Number of packages in the container
    bool bOwnsPackages;									// True if the container owns the contained packages

  protected:
    inline void incPackageCount() {lPackageCount++;}
    inline void decPackageCount() {lPackageCount--;}
    inline void resetPackageCount() {lPackageCount = 0;}
    inline void setPackageCount(size_type n) { lPackageCount = n; }

  public:
    CContainer() : lPackageCount(0), bOwnsPackages(true) {}
    virtual ~CContainer() {}

    virtual bool insert(CPackage *) { SCL_CODE_ERROR; }  // Insert an object into the container.
    virtual bool erase(CPackage *) { SCL_CODE_ERROR; }
    virtual void clear() { SCL_CODE_ERROR; }

    inline size_type size() const {return lPackageCount;}

    inline bool ownsPackages() const { return bOwnsPackages; }
    inline void ownsPackages(bool newOwner) { bOwnsPackages = newOwner; }

    inline bool empty() const { return lPackageCount == 0; }
  };

  class CUnsortedContainer : public CContainer
  {
  private:
  protected:
  public:
    CUnsortedContainer() {}
    virtual ~CUnsortedContainer() {}
  };

  class CSortedContainer : public CContainer
  {
  private:
  protected:
  public:
    CSortedContainer() : CContainer() {}
    virtual ~CSortedContainer() {}

  };

  class CIterator
  {
  private:
    bool bEOC;
    bool bBOC;

  protected:
    size_t lCurrent;

    CIterator(CIterator const &rhs) : bEOC(rhs.bEOC), bBOC(rhs.bBOC), lCurrent(rhs.lCurrent) {}

  public:
    CIterator() {}
    virtual ~CIterator() {}

    virtual  bool EOC()  { return bEOC; }
    virtual  bool BOC() const { return bBOC; }

    inline virtual void SetEOC() { bEOC = true; }
    inline virtual void SetBOC() { bBOC = true; }

    inline virtual void ResetEOC() { bEOC = false; }
    inline virtual void ResetBOC() { bBOC = false; }

    virtual void MoveFirst() = 0;
    virtual void MoveLast() = 0;
    virtual void MoveNext() = 0;
    virtual void MovePrevious() = 0;
  };
}

#endif // SCL_BASE_H
