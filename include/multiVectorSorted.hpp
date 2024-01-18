﻿//*********************************************************************************************************************************
//
// PROJECT:							Storage Class Library
// FILE:								multiVectorSorted.hpp
// SUBSYSTEM:						Sorted Vector implementation
// LANGUAGE:						C++20
// TARGET OS:						None.
// NAMESPACE:						SCL
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2023 Gavin Blakeman.
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
// OVERVIEW:						This is a templated sorted vector implementation.
//
// CLASSES INCLUDED:		vector_sorted - Implements an STL style sorted vector.
//
//
// HISTORY:             2023-11-21 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef SCL_MULTIVECTORSORTED_H
#define SCL_MULTIVECTORSORTED_H

  // Standard C++ library header files

#include <functional>
#include <iterator>
#include <stdexcept>
#include <vector>

  // SCL Library header files

#include "config.h"

  // Miscellaneous library header files

#include <GCL>

namespace SCL
{
  /// @brief    The multiVector_sorted class is and STL-like implementation of a sorted vector that allows multiple values.
  ///           This offers O(N) insertion and deletion times and O(log N) search time. The container is similar to a map.
  ///           The container does not support duplicates.
  /// @details  This is implmented as a std::vector that stores the objects. Sorting is performed on insertion.
  /// @note     1. This is a drop-in for std::vector.
  /// @note     2. The container is always kept in a sorted state. Some of the member functions behave differently to the
  ///              corresponding vector functions as a result. (Specifically push_back, emplace, insert). See the
  ///              relevant function specifications for more details.
  ///           3. Check the individual functions for complexity. Some of the functions can be quite expensive if called using
  ///              random data.

  template<class T,
           class Compare = std::less<T>,
           class Allocator = std::allocator<T>>
  class multiVector_sorted
  {
  public:
    using self_type = vector_sorted<T, Compare, Allocator>;
    using allocator_type = Allocator;
    using value_type = typename Allocator::value_type;
    using container_type = std::vector<value_type>;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type &;
    using size_type = typename container_type::size_type;
    using difference_type = typename Allocator::difference_type;
    using compare = Compare;
    using const_reference = const value_type&;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using reverse_iterator = typename container_type::reverse_iterator;
    using const_reverse_iterator = typename container_type::const_reverse_iterator;

  private:
    container_type data_;   // Data storage.
    Compare comp_;          // Comparison function

  protected:

  public:
    constexpr multiVector_sorted() noexcept(noexcept(Allocator())) : data_() {}

    explicit multiVector_sorted(Compare &comp, Allocator const &alloc = Allocator()) : data_(alloc) {}
    explicit multiVector_sorted(Allocator const &alloc) : data_(alloc) {}

    template<class InputIt>
    constexpr multiVector_sorted(InputIt first, InputIt last, Compare const &comp = Compare(), Allocator const & alloc = Allocator())
      : data_(first, last, alloc) {}

    template<class InputIt>
    multiVector_sorted(InputIt first, InputIt last, Allocator const & alloc()) : data_(alloc) {}

    multiVector_sorted(multiVector_sorted const &other) : data_(other.data_), comp_(other.comp_) {}
    multiVector_sorted(multiVector_sorted const &other, Allocator const &alloc);

    multiVector_sorted(multiVector_sorted &&other);
    multiVector_sorted(multiVector_sorted &&other, Allocator const &alloc);

    multiVector_sorted(std::initializer_list<value_type> init, Compare const &comp = Compare(), Allocator const &alloc = Allocator());
    multiVector_sorted(std::initializer_list<value_type> init, Allocator const &alloc);

    ~multiVector_sorted() {}

    constexpr multiVector_sorted &operator=(multiVector_sorted const &other);
    constexpr multiVector_sorted &operator=(multiVector_sorted &&other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value
                                                                       && std::is_nothrow_move_assignable<Compare>::value);
    constexpr multiVector_sorted &operator=(std::initializer_list<value_type> ilist);

    constexpr void assign(size_type count, T const &value);
    template<class InputIt>

    /// @brief    Replaces the contents with copies of those in the range [first, last). The behavior is undefined if either
    ///           argument is an iterator into *this.
    /// @param[in] first:
    /// @param[in] last:
    /// @version    2023-11-21/GGB - Function created.

    constexpr void assign(InputIt first, InputIt last)
    {
      clear();

      while (first != last)
      {
        push_back(*first);
        first++;
      }
    }

    constexpr void assign(std::initializer_list<T> iline);

    /// @brief      Returns the allocator associated with the container.
    /// @returns    The allocator associated with the container.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr allocator_type get_allocator() const noexcept
    {
      return data_.get_allocator();
    }

    /// @brief      Returns a reference to the element at the specified location pos, with bounds checking.
    /// @param[in]  pos: The index to the element.
    /// @returns    Reference to the element at pos.
    /// @throws     std::out_of_range
    /// @version    2023-11-21/GGB - Function created.
    ///
    constexpr reference at(size_type pos)
    {
      return data_.at(pos);
    }

    /// @brief      Returns a reference to the element at the specified location pos, with bounds checking.
    /// @param[in]  pos: The index to the element.
    /// @returns    Reference to the element at pos.
    /// @throws     std::out_of_range
    /// @version    2023-11-21/GGB - Function created.

    constexpr const_reference at(size_type pos) const
    {
      return data_.at(pos);
    }

    /// @brief      Returns a reference to the element at the specified location pos, without bounds checking.
    /// @param[in]  pos: The index to the element.
    /// @returns    Reference to the element at pos.
    /// @throws     None.
    /// @note       1. Accessing a nonexistent element through this operator is undefined behaviour.
    /// @note       2. Unlike std::map::operator[], this operator never inserts a new element into the container.
    /// @version    2023-11-21/GGB - Function created.

    constexpr reference operator[](size_type pos)
    {
      return data_[pos];
    }

    /// @brief      Returns a reference to the element at the specified location pos, without bounds checking.
    /// @param[in]  pos: The index to the element.
    /// @returns    Reference to the element at pos.
    /// @throws     None.
    /// @note       1. Accessing a nonexistent element through this operator is undefined behaviour.
    /// @note       2. Unlike std::map::operator[], this operator never inserts a new element into the container.
    /// @version    2023-11-21/GGB - Function created.

    constexpr const_reference operator[](size_type pos) const
    {
      return data_[pos];
    }

    /// @brief      Returns a reference to the first element in the container.
    /// @returns    Reference to the first element
    /// @throws
    /// @note       Calling front() on an empty container is undefined.
    /// @version    2023-11-21/GGB - Function created.

    constexpr reference front()
    {
      return data_.front();
    }

    /// @brief      Returns a reference to the first element in the container.
    /// @returns    Reference to the first element
    /// @throws
    /// @note       Calling front() on an empty container is undefined.
    /// @version    2023-11-21/GGB - Function created.

    constexpr const_reference front() const
    {
      return data_.front();
    }

    /// @brief      Returns a reference to the last element in the container.
    /// @returns    Reference to the last element
    /// @throws
    /// @note       Calling back() on an empty container is undefined behavior.
    /// @version    2023-11-21/GGB - Function created.

    constexpr reference back()
    {
      return data_.front();
    }

    /// @brief      Returns a reference to the last element in the container.
    /// @returns    Reference to the last element
    /// @throws
    /// @note       Calling back() on an empty container is undefined behavior.
    /// @version    2023-11-21/GGB - Function created.

    constexpr const_reference back() const
    {
      return data_.front();
    }

    /// @brief      Returns pointer to the underlying array serving as element storage. The pointer is such that range
    ///             [data(); data() + size()) is always a valid range, even if the container is empty (data() is not
    ///             dereferenceable in that case).
    /// @returns    Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to
    ///             the address of the first element.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr T *data() noexcept
    {
      return data_.data();
    }

    /// @brief      Returns pointer to the underlying array serving as element storage. The pointer is such that range
    ///             [data(); data() + size()) is always a valid range, even if the container is empty (data() is not
    ///             dereferenceable in that case).
    /// @returns    Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to
    ///             the address of the first element.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr T const *data() const noexcept
    {
      return data_.data();
    }

    /// @brief      Returns an iterator to the beginning.
    /// @returns    An iterator to the beginning.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr iterator begin() noexcept
    {
      return data_.begin();
    }

    /// @brief      Returns a const iterator to the beginning.
    /// @returns    A const iterator to the beginning.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr const_iterator begin() const noexcept
    {
      return data_.cbegin();
    }

    /// @brief      Returns a const iterator to the beginning.
    /// @returns    A const iterator to the beginning.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr const_iterator cbegin() const noexcept
    {
      return data_.cbegin();
    }

    /// @brief      Returns an iterator to the end.
    /// @returns    An iterator to the end.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created

    constexpr iterator end() noexcept
    {
      return data_.end();
    }

    /// @brief      Returns a const iterator to the end.
    /// @returns    A const iterator to the end.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr const_iterator end() const noexcept
    {
      return data_.cend();
    }

    /// @brief      Returns a const iterator to the end.
    /// @returns    An const iterator to the end.
    /// @throws     None.
    /// @version    2020-09-04/GGB - Function created.

    constexpr const_iterator cend() const noexcept
    {
      return data_.cend();
    }

    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;

    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    /// @brief      Checks if the container has no elements. IE whether begin() == end().
    /// @returns    true if the container is empty, false otherwise.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    [[nodiscard]] bool empty() const noexcept
    {
      return data_.empty();
    }

    /// @brief      Returns the number of elements in the container. ie std::distance(begin(), end()).
    /// @returns    The number of elements in the container.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr size_type size() const noexcept
    {
      return data_.size();
    }

    /// @brief      Returns the maximum number of elements the container is able to hold due to system or library implementation
    ///             limitations.
    /// @returns    The maximum number of elements the container can hold.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr size_type max_size() const noexcept
    {
      return data_.max_size();
    }

    /// @brief      Increase the capacity of the vector to a value that's greater or equal to new_cap. If new_cap is greater
    ///             than the current capacity(), new storage is allocated, otherwise the method does nothing.
    ///             reserve() does not change the size of the vector.
    ///             If new_cap is greater than capacity(), all iterators, including the past-the-end iterator, and all
    ///             references to the elements are invalidated. Otherwise, no iterators or references are invalidated.
    /// @param[in]  new_cap: The new capacity of the vector.
    /// @throws     std::length_error if new_cap > max_size().
    /// @throws     any exception thrown by Allocator::allocate() (typically std::bad_alloc)
    /// @note       If an exception is thrown, this function has no effect (strong exception guarantee).
    /// @version    2023-11-21/GGB - Function created.

    constexpr void reserve(size_type new_cap)
    {
      data_.reserve(new_cap);
    }

    /// @brief      Returns the number of elements that the container has currently allocated space for.
    /// @returns    Capacity of the currently allocated storage.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr size_type capacity() const noexcept
    {
      return data_.capacity();
    }

    /// @brief      Requests the removal of unused capacity.
    ///             It is a non-binding request to reduce capacity() to size(). It depends on the implementation whether the
    ///             request is fulfilled.
    ///             If reallocation occurs, all iterators, including the past the end iterator, and all references to the
    ///             elements are invalidated. If no reallocation takes place, no iterators or references are invalidated.
    /// @throws
    /// @version    2023-11-21/GGB - Function created.

    constexpr void shrink_to_fit()
    {
      data_.shrink_to_fit();
    }

    /// @brief      Erases all elements from the container. After this call, size() returns zero.
    ///             Invalidates and references, pointers or iterators referring to contained elements. Any past-the-end iterator
    ///             remains valid.
    /// @throws     None.
    /// @version    2023-11-21/GGB - Function created.

    constexpr void clear() noexcept
    {
      data_.clear();
    }

    /// @brief      Copies the value into the correct position in the container to ensure the container remains sorted.
    /// @param[in]  value: The value to insert.
    /// @returns    A std::pair with an iterator to the insertion position and a bool to indicate if insertion occurred.
    /// @throws
    /// @note       All the insertion style functions (insert, push_back) use this function.
    /// @version    2023-11-23/GGB - Bug#308 - Remove logic checking for duplicates.
    /// @version    2023-11-21/GGB - Function created.

    iterator insert_sorted(T const &value)
    {
      iterator i = lower_bound(begin(), end(), value, comp_);
      return data_.insert(i, value);
    }

    /// @brief      Moves the value into the correct position in the container to ensure the container remains sorted.
    /// @param[in]  value: The value to insert.
    /// @returns    An iterator to the insertion position.
    /// @throws
    /// @note       All the insertion style functions (insert, push_back) use this function.
    /// @version    2023-11-23/GGB - Bug#308 - Remove logic checking for duplicates.
    /// @version    2023-11-21/GGB - Function created.

    iterator insert_sorted(T &&value)
    {
      iterator i = lower_bound(begin(), end(), value, comp_);
      return data_.insert(i, value);
    }

    /// @brief      Inserts an element into the list. The sort order is always maintained using the comparison function.
    /// @param[in]  pos: This parameter is ignored.
    /// @param[in]  value: The value to insert.
    /// @returns    An iterator to the inserted item.
    /// @note       1. This function does not operate the same as std::vector::insert as the value is not inserted into the
    ///                specified position, BUT is inserted into the correct sorted position.
    /// @note       2. pos != the return value.
    /// @version    2023-11-21/GGB - Function created.

    constexpr iterator insert(T const &value)
    {
      return insert_sorted(value);
    }

    /// @brief      Inserts an element into the list. The sort order is always maintained using the comparison function.
    /// @param[in]  pos: This parameter is ignored.
    /// @param[in]  value: The value to insert.
    /// @returns    An iterator to the inserted item.
    /// @note       1. This function does not operate the same as std::vector::insert as the value is not inserted into the
    ///                specified position, BUT is inserted into the correct sorted position.
    /// @note       2. pos != the return value.
    /// @version    2023-11-21/GGB - Function created.

    constexpr iterator insert(T &&value)
    {
      return insert_sorted(value);
    }

    constexpr iterator insert( const_iterator pos, size_type count, T const &value );
    template< class InputIt >
    constexpr iterator insert( const_iterator pos, InputIt first, InputIt last);
    constexpr iterator insert( const_iterator pos, std::initializer_list<T> ilist);

    /// @brief      Emplace constructs the item in the correct position in the array.
    /// @param[in]  args: THe args to forward to the element constructor.
    /// @returns    An iterator to the inserted item
    /// @note       1. The item is initially move constucted to allow the insertion position to be determined and then move
    ///                inserted into the correct position in the vector.
    /// @throws
    /// @version    2023-11-21/GGB - Function created.

    template< class... Args >
    iterator emplace(Args&&... args)
    {
      T value(std::forward<Args>(args)...);
      return insert_sorted(std::move(value));
    }

    constexpr void resize(size_type count);
    constexpr void resize( size_type count, const value_type& value);

    constexpr void swap(multiVector_sorted& other ) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value
                                                  || std::allocator_traits<Allocator>::is_always_equal::value);


    /// @brief      Erases a single element as specified by the iterator from the container.
    /// @param[in]  pos: Iterator to the element to remove.
    /// @version    2020-09-04/GGB - Updated to STL like container.
    /// @version    2017-06-25/GGB - Updated to use new GCL error handling.
    /// @version    2013-01-01/GGB - Function created.

    constexpr iterator erase(const_iterator pos)
    {
      return data_.erase(pos);
    }

    /// @brief      Erases a range of elements as specified
    /// @param[in]  first: Iterator to the element to remove.
    /// @param[in]  last:
    /// @version    2020-09-04/GGB - Updated to STL like container.
    /// @version    2017-06-25/GGB - Updated to use new GCL error handling.
    /// @version    2013-04-21/GGB - When erasing to end of array elements are not null constructed. Just left as they where.
    /// @version    2013-01-01/GGB - Function created.

    constexpr iterator erase(const_iterator first, const_iterator last)
    {
      return data_.erase(first, last);
    }

    /// @brief      Removes the last element of the container.
    /// @note       1. Calling pop_back on an empty container results in undefined behavior.
    /// @note       2. Iterators and references to the last element, as well as the end() iterator, are invalidated.
    /// @note       3. As the container will remain sorted after this operation, there is no special treatment.
    /// @throws
    /// @version    2023-11-21/GGB - Function created.
    /// @todo       Define behavior. Check for empty container and throw an exception.

    constexpr void pop_back()
    {
      data_.pop_back();
    }

    /// @brief      Finds the element = val. If no element is found, then the end() is returned.
    /// @param[in]  value: The value to find.
    /// @throws     None.
    /// @version    2020-09-04/GGB - Converted to STL-like container.
    /// @version    2013-01-01/GGB - Function created.

    iterator find(T const &value)
    {
      const_iterator i = lower_bound(begin(), end(), value, comp_);
      return i == end() || comp(value, *i) ? end() : i;
    }
  };  // Class end.


}    // namespace SCL

#endif  // SCL_MULTIVECTORSORTED_H
