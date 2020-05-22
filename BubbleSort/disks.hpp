///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the 
// alternating disks problem.
//
// As provided, this header has four functions marked with 
// TODO comments.
// You need to write in your own implementation of these 
// functions.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(dark_count() > 0);

      for (size_t i = 1; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t dark_count() const {
    return total_count() / 2;
  }

  size_t light_count() const {
    return dark_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
    // Loop through the entire list-1. If the disk at index i is one color,
    // check if the disk at index i+1 is the other color. If it's not we
    // return false. 
    for(size_t i=0; i<_colors.size()-1; i++)
    {
      if(_colors.at(i)==DISK_LIGHT)
      {
        if(_colors.at(i+1)!=DISK_DARK)
        {
          return false;
        }
      }
      else if(_colors.at(i)==DISK_DARK)
      {
        if(_colors.at(i+1)!=DISK_LIGHT)
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
    // Checking the first half for Light disks
    bool isS = false;
    for (size_t i = 0; i < (_colors.size() / 2); i++)
    {
      if (_colors.at(i) == DISK_LIGHT)
      {
        isS = true;
      }
      else
      {
        isS = false;
        return isS;
      }
    }
    // Just to make sure: Or Checking the second half for Dark disks
    for (size_t i = (_colors.size() / 2); i < _colors.size(); i++)
    {
      if (_colors.at(i) == DISK_DARK)
      {
        isS = true;
      }
      else
      {
        isS = false;
        return isS;
      }
    }
   return isS;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
  // check that the input is in alternating format  
  assert(before.is_alternating());

  // Create a new disk_state and copy it to 'before'.
  disk_state new_list=before;
  unsigned swap_count=0;

  for(size_t i=0; i<new_list.total_count()-1; i++)  // We loop only size-1 so we can index forward +1 without going
                                                    // out of bounds.
  {
    for(size_t j=0; j<new_list.total_count()-i-1; j++) // We check size-i-1 because the i disks will already be in place.
    {
      // We want to check if the disk is dark first
      // so we can bubble up the dark disks to the right side.
      if(new_list.get(j)==DISK_DARK && new_list.get(j+1)==DISK_LIGHT)
      {
        new_list.swap(j);
        swap_count++;
      }
    }
  }

  return sorted_disks(new_list, swap_count);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  // check that the input is in alternating format
  assert(before.is_alternating());

  disk_state new_list=before;
  // Counting all the swaps that are performed
  unsigned swapCount = 0;
 
  for(size_t k=0; k<(new_list.total_count()-1)/2; k++)
  {
    // Sorting from the left to the right direction
    // Since last dark disk is the last index, ignore it
    // Move/swap dark disk to the right direction if only the first disk is dark disk and the second disk is light
    for (size_t i = 0; i < (new_list.total_count() - 1); i++)
    {
      
      if (new_list.get(i) == DISK_DARK && new_list.get(i+1) == DISK_LIGHT)
      {
        new_list.swap(i);
        swapCount++;
      }
    }
    // Sorting from the right to the left direction
    // Going backwards, Starting at light disk, ignoring the final index
    // Move/swap light disk to the left direction if only the first disk is light disk and the second disk is dark
    for (size_t i = (new_list.total_count() - 1); i > 0; i--)
    {
      
      if (new_list.get(i) == DISK_LIGHT && new_list.get(i-1) == DISK_DARK)
      {
        new_list.swap(i-1);
        swapCount++;
      }
    }
  }
  return sorted_disks(new_list, swapCount);
}

