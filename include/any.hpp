﻿//*********************************************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                any.hpp
// SUBSYSTEM:           SCL::any
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2015-2020 Gavin Blakeman.
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
// OVERVIEW:            Implements SCL::any that is similar to std::any with some additional support for outputting the stored
//                      values as strings.
//
// HISTORY:             2020-05-05 GGB - Renamed file to any.hpp and class to any.
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2015-09-07 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_ANY_H
#define SCL_ANY_H

  // Standard C++ library header files.

#include <any>      // For bad_any_cast
#include <memory>
#include <ostream>
#include <sstream>
#include <typeinfo>
#include <type_traits>

  // SCL header files

#include "Base.h"
#include "error.h"

namespace SCL
{
  /// @brief  The SCL::any Class allows any type of data to be stored. The class is a templatised class created on top of the
  ///         CPackage class.
  /// @details  CPackage is used as the base class for the templatised data class. (TData) This allows pointers to CPackage to
  ///           be stored and the virtual function mechanism to work with the correct class.
  ///           The class does not implement small buffer optimisation, instead allocating all objects as unique_ptr. SBO could be
  ///           implemented, but as objects that were SBO'd would not be able to use virtual dispatch mechanisms to implement the
  ///           stringgify functionality these would have to be specialised in the class implementation.
  ///           The class tries to mimic std::any as far as possible and provides an equivilent interface. The additional
  ///           functionality is provided on top of the std::any functionality, but this is not provided using std::any as a
  ///           ancestor.
  /// @note 1.  The SCL::any class is necessary for GCL::sqlWriter functionality.
  /// @todo 1.  Change stringify functions to potentially use boost::format.
  /// @todo 2.  Add functionality (not sure how) to implement format specifiers per instance, or per object type.
  /// @todo 3.  Implement SBO if possible. (very low priority)

  class any
  {
    template<typename T>
    class TData : public CPackage
    {
    public:
      std::unique_ptr<T> data;

      constexpr TData() noexcept : data() {}

      TData(T &&value) : data(std::make_unique<T>(value)) {}

      //TData(T const &toCopy) : data(new T(toCopy)) {}
      virtual ~TData() {}
//      virtual CPackage *createCopy() const
//      {
//        TData *returnValue = new TData(*data);
//        return dynamic_cast<CPackage *>(returnValue);
//      }

      /// @brief Converts the value type to a string output. This is done by calling the insertion operator for the type.
      /// @returns The value as a std::string
      /// @throws
      /// @note 1. For 'char' types, the char is converted to a short before output. This ensures the value, not the char is
      ///          output.
      /// @version 2019-12-08/GGB - Updated to output char types as values.

      virtual std::string stringOutput() const
      {
        std::ostringstream valueStream;
        std::string returnValue;

        if constexpr (std::is_same_v<std::uint8_t, T>)
        {
          returnValue = std::to_string(static_cast<std::uint16_t>(*data));
        }
        else if constexpr (std::is_same_v<std::int8_t, T>)
        {
          returnValue = std::to_string(static_cast<std::int16_t>(*data));
        }
        else
        {
          valueStream << (*data);
          returnValue = valueStream.str();
        };

        return returnValue;
      }

      virtual std::type_info const &type() const noexcept { return typeid(T); }
    };


  private:
    std::unique_ptr<CPackage> packageData;

  protected:
  public:
    /// @brief Default constructor. Matches
    constexpr any() noexcept {}

    any(any const &toCopy) : packageData(toCopy.packageData->createCopy()) {}



    any(any &&other) noexcept : packageData()
    {
      if (other.has_value())
      {
        packageData = std::move(other.packageData);
      }
    }

    /// @brief rvalue (move) constructor.
    /// @tparam valueType: The type fo the value to initialise into the any().
    /// @param[in] value: The value to initialise the any() with.

    template<class valueType>
    any(valueType &&value) : packageData()
    {
      if constexpr (std::is_same_v<valueType, SCL::any>)
      {
          // Creating another SCL::any class.

        packageData = std::move(value.packageData);

      }
      else
      {
          /* I spent a lot of time on the following line. std::forward<valueType>(value) did not work for forwarding the
           * arguments as std::unique_ptr complained about converting rvalue&& to lvalue. The cv qualifiers were also
           * causing the same issue. When looking at the std::forward vs std::decay, std::decay removes all references
           * as well as cv types.
           * For instance (and this is where I found the issue)
           *
           * f(SCL::any const &)
           * g(std::string const &str)
           * {
           *    f(str); // need to create an SCL any with std::string const &
           * }
           *
           * In the example above, the underlying SCL::any needs to get a copy of the string, (not move) and the string in the
           * any does not need to be cv qualified.
           */

        packageData = std::make_unique<TData<std::decay_t<valueType>>>(std::decay_t<valueType>(value));
      };
    }

    /// @brief Constructs and object with initial content an object of type std::decay_t<valueType>, direct-non-list-initialised
    ///        from std::forward<Args>(args)...
    /// @tparam valueType: The type of the object to initialise.
    /// @tparam Args...: The arguments type.

    template<class valueType, class... Args>
    explicit any(std::in_place_type_t<valueType>, Args&&... args)
      : packageData(std::make_unique<std::decay_t<valueType>>(std::forward<Args>(args)...))
    {
    }


    template<class valueType, class U, class... Args>
    explicit any(std::in_place_type_t<valueType>, std::initializer_list<U> il, Args&&... args) {}


    virtual ~any() { packageData.reset(); }

    void reset() noexcept { packageData.reset(); }

    void swap(any &other) noexcept { packageData.swap(other.packageData); }

    bool has_value() const noexcept { return static_cast<bool>(packageData); }

    std::type_info const &type() const noexcept
    {
      if (packageData)
      {
        return typeid(*packageData);
      }
      else
      {
        return typeid(void);
      }
    }

    virtual std::string stringOutput() const
    {
      if (packageData)
      {
        return packageData->stringOutput();
      }
      else
      {
        throw std::bad_any_cast();
      };
    }

    friend std::ostream & operator<<(std::ostream &strm, any const &op) { strm << (op.packageData->stringOutput()); return strm; }

  };  // class any
} // namespace SCL

#endif // SCL_ANY_H
