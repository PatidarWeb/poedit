/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Matei David 2014
// (C) Copyright Ion Gaztanaga 2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_BPTR_VALUE_HPP
#define BOOST_INTRUSIVE_BPTR_VALUE_HPP

#include <cassert>
#include <boost/intrusive/list.hpp>
#include "bounded_pointer.hpp"
#include "common_functors.hpp"


namespace boost {
namespace intrusive {

struct BPtr_Value
{
   static const bool constant_time_size = true;

   BPtr_Value(int value = 42)
      : value_(value)
   {}

   BPtr_Value(const BPtr_Value& rhs)
      : value_(rhs.value_)
   {}

   ~BPtr_Value()
   {
      if (is_linked())
      {
         std::cerr << "BPtr_Value dtor: destructing linked value: &=" << (void*)this << "\n";
         assert(false);
      }
   }

   // testvalue is used in boost::container::vector and thus prev and next
   // have to be handled appropriately when copied:
   BPtr_Value& operator = (const BPtr_Value& src)
   {
      if (is_linked())
      {
         std::cerr << "BPtr_Value asop: assigning to linked value: &=" << (void*)this << ", src=" << (void*)&src << "\n";
         assert(false);
      }
      value_ = src.value_;
      return *this;
   }

   // value
   int value_;

   // list node hooks
   bounded_pointer< BPtr_Value > m_previous;
   bounded_pointer< BPtr_Value > m_next;
   // tree node hooks
   bounded_pointer< BPtr_Value > m_parent;
   bounded_pointer< BPtr_Value > m_l_child;
   bounded_pointer< BPtr_Value > m_r_child;
   signed char m_extra;

   bool is_linked() const
   { return m_previous || m_next || m_parent || m_l_child || m_r_child; }

   friend bool operator< (const BPtr_Value &other1, const BPtr_Value &other2)
   {  return other1.value_ < other2.value_;  }

   friend bool operator< (int other1, const BPtr_Value &other2)
   {  return other1 < other2.value_;  }

   friend bool operator< (const BPtr_Value &other1, int other2)
   {  return other1.value_ < other2;  }

   friend bool operator== (const BPtr_Value &other1, const BPtr_Value &other2)
   {  return other1.value_ == other2.value_;  }

   friend bool operator== (int other1, const BPtr_Value &other2)
   {  return other1 == other2.value_;  }

   friend bool operator== (const BPtr_Value &other1, int other2)
   {  return other1.value_ == other2;  }

   friend bool operator!= (const BPtr_Value &other1, const BPtr_Value &other2)
   {  return !(other1 == other2);  }

   friend bool operator!= (int other1, const BPtr_Value &other2)
   {  return !(other1 == other2.value_);  }

   friend bool operator!= (const BPtr_Value &other1, int other2)
   {  return !(other1.value_ == other2);  }

   friend std::ostream& operator << (std::ostream& os, const BPtr_Value& v)
   {
      os << v.value_;
      return os;
   }
}; // class BPtr_Value

template < typename Node_Algorithms >
void swap_nodes(bounded_reference< BPtr_Value > lhs, bounded_reference< BPtr_Value > rhs)
{
   Node_Algorithms::swap_nodes(
      boost::intrusive::pointer_traits< bounded_pointer< BPtr_Value > >::pointer_to(lhs),
      boost::intrusive::pointer_traits< bounded_pointer< BPtr_Value > >::pointer_to(rhs));
}

struct List_BPtr_Node_Traits
{
   typedef BPtr_Value                     val_t;
   typedef val_t                          node;
   typedef bounded_pointer< val_t >       node_ptr;
   typedef bounded_pointer< const val_t > const_node_ptr;

   static node_ptr get_previous(const_node_ptr p)      { return p->m_previous; }
   static void set_previous(node_ptr p, node_ptr prev) { p->m_previous = prev; }
   static node_ptr get_next(const_node_ptr p)          { return p->m_next; }
   static void set_next(node_ptr p, node_ptr next)     { p->m_next = next; }
};

struct RBTree_BPtr_Node_Traits
{
   typedef BPtr_Value                     val_t;
   typedef val_t                          node;
   typedef bounded_pointer< val_t >       node_ptr;
   typedef bounded_pointer< const val_t > const_node_ptr;
   typedef signed char                    color;

   static node_ptr get_parent(const_node_ptr p)        { return p->m_parent; }
   static void set_parent(node_ptr p, node_ptr parent) { p->m_parent = parent; }
   static node_ptr get_left(const_node_ptr p)          { return p->m_l_child; }
   static void set_left(node_ptr p, node_ptr l_child)  { p->m_l_child = l_child; }
   static node_ptr get_right(const_node_ptr p)         { return p->m_r_child; }
   static void set_right(node_ptr p, node_ptr r_child) { p->m_r_child = r_child; }
   static color get_color(const_node_ptr p)            { return p->m_extra; }
   static void set_color(node_ptr p, color c)          { p->m_extra = c; }
   static color black()                                { return 0; }
   static color red()                                  { return 1; }
};

struct AVLTree_BPtr_Node_Traits
{
   typedef BPtr_Value                     val_t;
   typedef val_t                          node;
   typedef bounded_pointer< val_t >       node_ptr;
   typedef bounded_pointer< const val_t > const_node_ptr;
   typedef signed char                    balance;

   static node_ptr get_parent(const_node_ptr p)        { return p->m_parent; }
   static void set_parent(node_ptr p, node_ptr parent) { p->m_parent = parent; }
   static node_ptr get_left(const_node_ptr p)          { return p->m_l_child; }
   static void set_left(node_ptr p, node_ptr l_child)  { p->m_l_child = l_child; }
   static node_ptr get_right(const_node_ptr p)         { return p->m_r_child; }
   static void set_right(node_ptr p, node_ptr r_child) { p->m_r_child = r_child; }
   static balance get_balance(const_node_ptr p)        { return p->m_extra; }
   static void set_balance(node_ptr p, balance b)      { p->m_extra = b; }
   static balance negative()                           { return -1; }
   static balance zero()                               { return 0; }
   static balance positive()                           { return 1; }
};

struct Tree_BPtr_Node_Traits
{
   typedef BPtr_Value                     val_t;
   typedef val_t                          node;
   typedef bounded_pointer< val_t >       node_ptr;
   typedef bounded_pointer< const val_t > const_node_ptr;

   static node_ptr get_parent(const_node_ptr p)        { return p->m_parent; }
   static void set_parent(node_ptr p, node_ptr parent) { p->m_parent = parent; }
   static node_ptr get_left(const_node_ptr p)          { return p->m_l_child; }
   static void set_left(node_ptr p, node_ptr l_child)  { p->m_l_child = l_child; }
   static node_ptr get_right(const_node_ptr p)         { return p->m_r_child; }
   static void set_right(node_ptr p, node_ptr r_child) { p->m_r_child = r_child; }
};

template < typename NodeTraits >
struct BPtr_Value_Traits
{
   typedef NodeTraits                              node_traits;
   typedef typename node_traits::val_t             value_type;
   typedef typename node_traits::node_ptr          node_ptr;
   typedef typename node_traits::const_node_ptr    const_node_ptr;
   typedef node_ptr                                pointer;
   typedef const_node_ptr                          const_pointer;
   typedef bounded_reference< value_type >         reference;
   typedef bounded_reference< const value_type >   const_reference;
   typedef BPtr_Value_Traits<NodeTraits> *         value_traits_ptr;

   static const boost::intrusive::link_mode_type link_mode = boost::intrusive::safe_link;

   static const_node_ptr to_node_ptr(const_reference v) { return &v; }
   static node_ptr to_node_ptr(reference v)             { return &v; }
   static const_pointer to_value_ptr(const_node_ptr p)  { return p; }
   static pointer to_value_ptr(node_ptr p)              { return p; }
};

template < typename >
struct Value_Container;

template <>
struct Value_Container< BPtr_Value >
{
   typedef bounded_reference_cont< BPtr_Value > type;
};

namespace test{

template <>
class new_cloner< BPtr_Value >
{
   public:
   typedef BPtr_Value value_type;
   typedef bounded_pointer< value_type > pointer;
   typedef bounded_reference< const value_type > const_reference;
   typedef bounded_allocator< value_type > allocator_type;

   pointer operator () (const_reference r)
   {
      pointer p = allocator_type().allocate(1);
      new (p.raw()) value_type(r);
      return p;
   }
};

template <>
class delete_disposer< BPtr_Value >
{
   public:
   typedef BPtr_Value value_type;
   typedef bounded_pointer< value_type > pointer;
   typedef bounded_allocator< value_type > allocator_type;

   void operator () (pointer p)
   {
      p->~value_type();
      allocator_type().deallocate(p, 1);
   }
};

} // namespace test

} // namespace intrusive
} // namespace boost


#endif
