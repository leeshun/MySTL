/*
 * Created by Maou Lim on 2018/1/11.
 */

#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <cassert>

#include "iterator.h"
#include "memory.h"

namespace tools {

	enum rb_tree_color {
		rb_tree_red, rb_tree_black
	};

	struct rb_tree_node_base {
		typedef rb_tree_node_base* base_ptr;
		typedef rb_tree_color      color_type;

		color_type color;
		base_ptr   parent;
		base_ptr   left;
		base_ptr   right;

		explicit rb_tree_node_base(color_type color  = rb_tree_red,
		                           base_ptr   parent = nullptr,
		                           base_ptr   left   = nullptr,
		                           base_ptr   right  = nullptr) :
			color(color), parent(parent), left(left), right(right) { }

		static base_ptr maximum(base_ptr root) {
			assert(nullptr != root);
			while (nullptr != root->right) {
				root = root->right;
			}
			return root;
		}

		static base_ptr minimum(base_ptr root) {
			assert(nullptr != root);
			while (nullptr != root->left) {
				root = root->left;
			}
			return root;
		}

	};

	inline void rb_tree_left_rotate(rb_tree_node_base*  shaft,
	                                rb_tree_node_base*& root  ) {
		rb_tree_node_base* right = shaft->right;
		shaft->right = right->left;
		if (nullptr != right->left) {
			right->left->parent = shaft;
		}
		right->parent = shaft->parent;

		if (root == shaft) {
			root = right;
		}
		else if (shaft->parent->left == shaft) {
			shaft->parent->left = right;
		}
		else {
			shaft->parent->right = right;
		}

		right->left = shaft;
		shaft->parent = right;
	}

	inline void rb_tree_right_rotate(rb_tree_node_base*  shaft,
	                                 rb_tree_node_base*& root  ) {
		rb_tree_node_base* left = shaft->left;
		shaft->left = left->right;
		if (nullptr != left->right) {
			left->right->parent = shaft;
		}
		left->parent = shaft->parent;

		if (root == shaft) {
			root = left;
		}
		else if (shaft->parent->right == shaft) {
			shaft->parent->right = left;
		}
		else {
			shaft->parent->left = left;
		}

		left->right = shaft;
		shaft->parent = left;
	}

	inline void rb_tree_insert_rebalance(rb_tree_node_base* new_node,
	                                     rb_tree_node_base*& root    ) {
		new_node->color = rb_tree_red;
		while (root != new_node && rb_tree_red == new_node->parent->color) {
			if (new_node->parent == new_node->parent->parent->left) {
				rb_tree_node_base* uncle = new_node->parent->parent->right;
				if (nullptr != uncle && rb_tree_red == uncle->color) {
					new_node->parent->color = rb_tree_black;
					uncle->color = rb_tree_black;
					new_node->parent->parent->color = rb_tree_red;
					new_node = new_node->parent->parent;
				}
				else {
					if (new_node->parent->right == new_node) {
						new_node = new_node->parent;
						rb_tree_left_rotate(new_node, root);
					}
					new_node->parent->color = rb_tree_black;
					new_node->parent->parent->color = rb_tree_red;
					rb_tree_right_rotate(new_node->parent->parent, root);
				}
			}
			else {
				rb_tree_node_base* uncle = new_node->parent->parent->left;
				if (nullptr != uncle && rb_tree_red == uncle->color) {
					new_node->parent->color = rb_tree_black;
					uncle->color = rb_tree_black;
					new_node->parent->parent->color = rb_tree_red;
					new_node = new_node->parent->parent;
				}
				else {
					if (new_node->parent->left == new_node) {
						new_node = new_node->parent;
						rb_tree_right_rotate(new_node, root);
					}
					new_node->parent->color = rb_tree_black;
					new_node->parent->parent->color = rb_tree_red;
					rb_tree_left_rotate(new_node->parent->parent, root);
				}
			}
		}
		root->color = rb_tree_black;
	}

	inline void rb_tree_erase_rebalance() {
		// todo
	}

	template <typename _Val>
	struct rb_tree_node : rb_tree_node_base {

		typedef rb_tree_node<_Val>* link_type;
		typedef _Val                value_type;

		explicit rb_tree_node(const value_type& value) : val(value) { }
		~rb_tree_node() = default;

		value_type val;
	};

	struct rb_tree_base_iterator {
		typedef rb_tree_node_base::base_ptr     base_ptr;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t                       difference_type;

		base_ptr node;

		rb_tree_base_iterator() = default;

		explicit rb_tree_base_iterator(base_ptr p) : node(p) { }

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
				// todo
				if (node->right != p) {
					node = p;
				}
			}
		}

		void decrement() {
			if (rb_tree_red == node->color &&
				node        == node->parent->parent) {
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

	template <typename _Val>
	struct rb_tree_const_iterator : rb_tree_base_iterator {
	public:
		typedef _Val        value_type;
		typedef const _Val& reference;
		typedef const _Val* pointer;

	protected:
		typedef rb_tree_const_iterator<_Val>           self_type;
		typedef rb_tree_base_iterator                  base_type;
		typedef typename rb_tree_node<_Val>::link_type link_type;
	public:
		rb_tree_const_iterator() = default;
		rb_tree_const_iterator(const self_type&) = default;
		rb_tree_const_iterator(const base_type& other) : base_type(other.node) { }
		explicit rb_tree_const_iterator(link_type p) : base_type(p) { }

		reference operator*() const {
			return link_type(node)->val;
		}

		pointer operator->() const {
			return &(operator*());
		}

		self_type& operator++() {
			increment();
			return *this;
		}

		self_type& operator++(int) {
			self_type tmp = *this;
			increment();
			return tmp;
		}

		self_type& operator--() {
			decrement();
			return *this;
		}

		self_type& operator--(int) {
			self_type tmp = *this;
			decrement();
			return tmp;
		}
	};

	template <typename _Val>
	struct rb_tree_iterator : rb_tree_base_iterator {
	public:
		typedef _Val  value_type;
		typedef _Val& reference;
		typedef _Val* pointer;

		typedef rb_tree_iterator<_Val> iterator;

	protected:
		typedef rb_tree_iterator<_Val>                 self_type;
		typedef rb_tree_base_iterator                  base_type;
		typedef typename rb_tree_node<_Val>::link_type link_type;

	public:
		rb_tree_iterator() = default;
		rb_tree_iterator(const self_type&) = default;
		explicit rb_tree_iterator(link_type p) : base_type(p) { }

		reference operator*() const {
			return link_type(node)->val;
		}

		pointer operator->() const {
			return &(operator*());
		}

		self_type& operator++() {
			increment();
			return *this;
		}

		self_type& operator++(int) {
			self_type tmp = *this;
			increment();
			return tmp;
		}

		self_type& operator--() {
			decrement();
			return *this;
		}

		self_type& operator--(int) {
			self_type tmp = *this;
			decrement();
			return tmp;
		}
	};

	inline bool operator==(
		const rb_tree_base_iterator& left,
		const rb_tree_base_iterator& right
	) {
		return left.node == right.node;
	}

	inline bool operator!=(
		const rb_tree_base_iterator& left,
		const rb_tree_base_iterator& right
	) {
		return !operator==(left, right);
	}

	template <
		typename _Key,
		typename _Val,
		typename _KeyOf,
		typename _Comparator,
		typename _Allocator = std::allocator<rb_tree_node<_Val>>
	>
	class rb_tree {
	public:
		typedef _Key        key_type;
		typedef _Val        value_type;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;
		typedef _Val&       reference;
		typedef const _Val& const_reference;

		typedef rb_tree_node<_Val>* link_type;
		typedef size_t              size_type;
		typedef ptrdiff_t           difference_type;

	protected:
		typedef _Comparator                           comparator_type;
		typedef rb_tree_color                         color_type;
		typedef rb_tree_node_base*                    base_ptr;
		typedef rb_tree_node<_Val>                    node_type;
		typedef standard_alloc<node_type, _Allocator> allocator_type;

		typedef rb_tree<_Key, _Val, _KeyOf, _Comparator, _Allocator> self_type;

	protected:
		link_type get_node() { return allocator_type::allocate(); }
		void put_node(link_type p) { return allocator_type::deallocate(p); }

		link_type create_node(const value_type& val) {
			link_type node = get_node();
			construct(node, val);
			return node;
		}

		link_type clone_node(link_type node) {
			link_type clone = create_node(node->val);
			clone->color = node->color;
			return clone;
		}

		void destroy_node(link_type node) {
			destroy(node);
			put_node(node);
		}

	protected:
		size_type       m_count_node;
		link_type       m_header; /* virtual header */
		comparator_type m_comp;

		link_type& root()      const { return (link_type&) m_header->parent; }
		link_type& leftmost()  const { return (link_type&) m_header->left  ; }
		link_type& rightmost() const { return (link_type&) m_header->right ; }

	protected:
		typedef rb_tree_iterator<value_type>       inner_iterator;
		typedef rb_tree_const_iterator<value_type> inner_const_iterator;

	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<inner_const_iterator, self_type> const_iterator;

	private:
		void _initialize() {
			m_header = get_node();
			m_header->color = rb_tree_red;

			root()      = nullptr;
			leftmost()  = m_header;
			rightmost() = m_header;
		}

		void _clear() {
			// todo
		}

		iterator _insert(link_type current, link_type parent, const value_type& val) {
			link_type new_node = nullptr;
			auto key_of = _KeyOf();

			if (
				m_header == parent  ||
				nullptr  != current || // ???
				m_comp(key_of(val), key_of(parent->val))
			) {
				new_node = create_node(val);
				parent->left = new_node;
				if (m_header == parent) {
					root()     = new_node;
					leftmost() = new_node;
				}
				else if (leftmost() == parent) {
					leftmost() = new_node;
				}
			}
			else {
				new_node = create_node(val);
				parent->right = new_node;
				if (rightmost() == parent) {
					rightmost() = new_node;
				}
			}
			new_node->parent = parent;

			rb_tree_insert_rebalance(
				new_node, reinterpret_cast<rb_tree_node_base*&>(root())
			);
			++m_count_node;
			return inner_iterator(new_node);
		}

	public:
		explicit rb_tree(const comparator_type& comp = _Comparator()) :
			m_count_node(0), m_comp(comp) { _initialize(); }

		~rb_tree() {
			_clear();
			put_node(m_header);
		}

		self_type& operator=(const self_type& other) {
			// todo
			return *this;
		}

	public:
		const comparator_type& comparator() const { return m_comp; }

		iterator begin() { return iterator(inner_iterator(leftmost())); }
		const_iterator begin() const { return const_iterator(inner_const_iterator(leftmost())); }

		iterator end() { return iterator(inner_iterator(m_header)); }
		const_iterator end() const { return const_iterator(inner_const_iterator(m_header)); }

		bool empty() const { return 0 == m_count_node; }
		size_type size() const { return m_count_node; }
		size_type max_size() const { return size_type(-1); }

		const_iterator maximum() const {
			if (empty()) {
				return end();
			}

			return inner_const_iterator(
				(link_type) rb_tree_node_base::maximum(root())
			);
		}

		const_iterator minimum() const {
			if (empty()) {
				return end();
			}
			return inner_const_iterator(
				(link_type) rb_tree_node_base::minimum(root())
			);
		}

	public:
		std::pair<iterator, bool> insert_unique(const value_type& val) {
			link_type parent = m_header;
			link_type current = root();
			bool prior_to = true;
			auto key_of = _KeyOf();

			while (nullptr != current) {
				parent = current;
				prior_to = m_comp(key_of(val), key_of(current->val));
				current = static_cast<link_type>(
					prior_to ? current->left : current->right
				);
			}

			assert(nullptr == current);

			inner_iterator iter(parent);
			if (prior_to) {
				if (begin() == iter) {
					return std::pair<iterator, bool>(
						_insert(current, parent, val), true
					);
				}
				else {
					--iter;
				}
			}

			if (m_comp(key_of(*iter), key_of(val))) {
				return std::pair<iterator, bool>(
					_insert(current, parent, val), true
				);
			}

			return std::pair<iterator, bool>(iter, false);
		}

		iterator insert_equal(const value_type& val) {
			link_type parent = m_header;
			link_type current = root();
			auto key_of = _KeyOf();

			while (nullptr != current) {
				parent = current;
				current = static_cast<link_type>(
					m_comp(key_of(val), key_of(current->val)) ? current->left : current->right
				);
			}

			assert(nullptr == current);

			return _insert(current, parent, val);
		}

		iterator find(const key_type& key) {
			link_type parent  = m_header;
			link_type current = root();

			auto key_of = _KeyOf();

			while (nullptr != current) {
				if (!m_comp(key_of(current->val), key)) {
					parent = current;
					current = static_cast<link_type>(current->left);
				}
				else {
					current = static_cast<link_type>(current->right);
				}
			}

			inner_iterator iter(parent);
			return (end() == iter || m_comp(key, key_of(*iter))) ? end() : iter;
		}
	};
}

#endif //_RED_BLACK_TREE_H_
