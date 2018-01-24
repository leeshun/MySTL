/*
 * Created by Maou Lim on 2018/1/23.
 */

#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include "functor.h"
#include "iterator.h"
#include "memory.h"
#include "tree_base.h"

namespace tools {

	struct _avl_tree_node_base : _bitree_node {
		typedef _avl_tree_node_base* base_ptr;
		typedef _bitree_node         base_type;
		typedef size_t               height_type;

		height_type height;

		explicit _avl_tree_node_base(base_ptr    p = nullptr,
		                             base_ptr    l = nullptr,
		                             base_ptr    r = nullptr,
		                             height_type h = 0) :
			base_type(p, l, r), height(h) { }
	};

	template <typename _Val>
	struct _avl_tree_node : _avl_tree_node_base {
	protected:
		typedef _avl_tree_node_base  base_type;
		typedef _avl_tree_node<_Val> self_type;

	public:
		typedef base_type::base_ptr    base_ptr;
		typedef base_type::height_type height;
		typedef _Val                   value_type;

	public:
		value_type val;

	public:
		explicit _avl_tree_node(const value_type& value) : val(value) { }
	};

	template <
		typename _Key,
		typename _Val,
		typename _KeyOf,
		typename _Comparator = less<_Key>,
		typename _Allocator  = std::allocator<_avl_tree_node<_Val>>
	>
	class _avl_tree {
	protected:
		typedef _avl_tree_node<_Val>            node_type;
		typedef node_type*                      link_type;
		typedef typename node_type::height_type height_type;

		typedef _avl_tree<_Key, _Val, _KeyOf, _Comparator, _Allocator> self_type;

	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef _Key        key_type;
		typedef _Comparator comparator_type;
		typedef size_t      size_type;
		typedef self_type   bst_type;

		typedef standard_alloc<node_type, _Allocator> allocator_type;

	protected:
		link_type get_node() { }

	protected:
		typedef _bitree_iterator<node_type>       inner_iterator;
		typedef _const_bitree_iterator<node_type> const_inner_iterator;
	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;


	private:
		link_type m_header;
		size_type m_count;
	};
}

#endif //_AVL_TREE_H_
