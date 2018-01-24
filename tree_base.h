/*
 * Created by Maou Lim on 2018/1/24.
 */

#ifndef _TREE_BASE_H_
#define _TREE_BASE_H_

#include <cassert>

#include "iterator.h"

namespace tools {

	struct _bitree_node {

		typedef _bitree_node* base_ptr;

		base_ptr parent;
		base_ptr left;
		base_ptr right;

		explicit _bitree_node(base_ptr p = nullptr,
		                      base_ptr l = nullptr,
		                      base_ptr r = nullptr) :
			parent(p), left(l), right(r) { }
	};

	struct _bitree_iterator_base {
		typedef _bitree_node::base_ptr          base_ptr;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t                       difference_type;

		base_ptr node;

		explicit _bitree_iterator_base(base_ptr p = nullptr) : node(p) { }

		void increment() {
			if (nullptr != node->right) {
				node = node->right;
				while (nullptr != node->left) {
					node = node->left;
				}
			}
			else {
				base_ptr p = node->parent;
				while (p->right == node) {
					node = p;
					p = p->parent;
				}
				if (node->right != p) {
					node = p;
				}
			}
		}

		void decrement() {
			if (node == node->parent->parent) {
				node = node->right;
			}

			else if (nullptr != node->left) {
				base_ptr l = node->left;
				while (nullptr != l->right) {
					l = l->right;
				}
				node = l;
			}

			else {
				base_ptr p = node->parent;
				while (p->left == node) {
					node = p;
					p = p->parent;
				}
				node = p;
			}
		}
	};

	inline bool operator==(const _bitree_iterator_base& left,
	                       const _bitree_iterator_base& right) {
		return left.node == right.node;
	}

	inline bool operator!=(const _bitree_iterator_base& left,
	                       const _bitree_iterator_base& right) {
		return !(left == right);
	}

	template <typename _Node>
	struct _const_bitree_iterator : _bitree_iterator_base {
	protected:
		typedef _const_bitree_iterator<_Node> self_type;
		typedef _bitree_iterator_base         base_type;
		typedef _Node                         node_type;
		typedef _Node*                        link_type;

	public:
		typedef typename node_type::value_type value_type;
		typedef const value_type&              reference;
		typedef const value_type*              pointer;

	public:
		_const_bitree_iterator() = default;
		_const_bitree_iterator(const base_type& other) : _bitree_iterator_base(other.node) { }
		explicit _const_bitree_iterator(link_type p) : base_type(p) { }

		reference operator*() const { return link_type(node)->value; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() { increment(); return *this; }
		self_type& operator++(int) { self_type tmp = *this; increment(); return tmp; }

		self_type& operator--() { decrement(); return *this; }
		self_type& operator--(int) { self_type tmp = *this; decrement(); return tmp; }
	};

	template <typename _Node>
	struct _bitree_iterator : _bitree_iterator_base {
	protected:
		typedef _bitree_iterator<_Node> self_type;
		typedef _bitree_iterator_base   base_type;
		typedef _Node                   node_type;
		typedef _Node*                  link_type;

	public:
		typedef typename node_type::value_type value_type;
		typedef value_type&                    reference;
		typedef value_type*                    pointer;

	public:
		_bitree_iterator() = default;
		_bitree_iterator(const self_type& other) : _bitree_iterator_base(other.node) { }
		explicit _bitree_iterator(link_type p) : base_type(p) { }

		reference operator*() const { return link_type(node)->value; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() { increment(); return *this; }
		self_type& operator++(int) { self_type tmp = *this; increment(); return tmp; }

		self_type& operator--() { decrement(); return *this; }
		self_type& operator--(int) { self_type tmp = *this; decrement(); return tmp; }
	};

	template <
		typename _Key,
		typename _Val,
		typename _KeyOf,
		typename _Comparator
	>
	struct _bstree_node : _bitree_node {
	protected:
		typedef _bitree_node                                  base_type;
		typedef _bstree_node<_Key, _Val, _KeyOf, _Comparator> self_type;

	public:
		typedef _Key        key_type;
		typedef _Val        value_type;
		typedef _Comparator comparator_type;
		typedef self_type   bst_type;

	public:
		value_type      value;

	public:
		explicit _bstree_node(const value_type& val) : value(val) { }
		virtual ~_bstree_node() = default;

	public:
		typedef _bitree_iterator<self_type>       iterator;
		typedef _const_bitree_iterator<self_type> const_iterator;
	};

	template <>
	struct _bstree_node<void, void, void, void> {

		typedef _bitree_node* base_ptr;

		static base_ptr minimum(base_ptr root) {
			assert(nullptr != root);
			while (nullptr != root->left) {
				root = root->left;
			}
			return root;
		}

		static base_ptr maximum(base_ptr root) {
			assert(nullptr != root);
			while (nullptr != root->right) {
				root = root->right;
			}
			return root;
		}
	};

	typedef _bstree_node<void, void, void, void> bstree_tool;

}

#endif //_TREE_BASE_H_
