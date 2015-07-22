/*  
  Copyright (c) 2015 by Robert T. Adams 
  All Rights Reserved. 
  See LICENSE.txt for licensing information.
*/
#ifndef object_hpp_2015_0624_1622
#define object_hpp_2015_0624_1622

#include <list>
#include <string>
#include <set>
//#include <tuple>

class Object;

typedef std::string string;
//typedef std::unique_ptr<Object> object_ptr;
typedef std::list<Object> members_t;
typedef members_t::const_iterator iterator_t;
typedef std::set<string> tags_t;

namespace boost { namespace serialization { class access; }}

class Object {
public:
  Object();
  Object(string name, string type);

  auto name() const -> const string&;
  auto type() const -> const string&;

  auto tags() const -> const tags_t&;

  auto members() const -> const members_t&;

  void move(Object& other, const iterator_t& item, 
      const iterator_t insert_pos);

  auto insert(iterator_t& pos, Object&& obj) -> iterator_t;

  auto append(Object&& obj) -> iterator_t;

  auto operator==(const Object&) const -> bool;
  auto operator!=(const Object& rhs) const -> bool { return !(*this == rhs); }

  struct data_t {
    string name;
    string type;
    tags_t tags;
    members_t members;
  } data_;

private:
  members_t& members_;

};


inline
auto Object::operator==(const Object& rhs) const -> bool
{
  if(name() == rhs.name()) 
  {
    if(type() == rhs.type()) 
    {
      if(tags() == rhs.tags())
      {
        if(members() == rhs.members())
        {
          return true;
        }
      }
    }
  }
  return false;
}

inline
Object::Object() 
  : members_(data_.members) 
{
}

inline
Object::Object(string n, string t) 
  : data_({ n, t, tags_t(), members_t() }), members_(data_.members)
{
}

inline
auto Object::tags() const -> const tags_t&
{
  return data_.tags;
}

inline
auto Object::name() const -> const string&
{
  return data_.name;
}

inline
auto Object::type() const -> const string&
{
  return data_.type;
}

inline
void Object::move(Object& other, const iterator_t& item, 
    const iterator_t insert_pos)
{
  members_.splice(insert_pos, other.members_, item);
}

inline
auto Object::members() const -> const members_t&
{
  return data_.members;
}

inline
auto Object::insert(iterator_t& pos, Object&& o) -> iterator_t {
  return members_.insert(pos, std::move(o));
}

inline
auto Object::append(Object&& o) -> iterator_t {
  return members_.insert(members().end(), std::move(o));
}

#endif//object_hpp_2015_0624_1622
