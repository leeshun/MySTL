/*
 * Created by Maou Lim on 2018/1/18.
 */

#ifndef _SINGLE_LIST_H_
#define _SINGLE_LIST_H_

#include "iterator.h"
#include "memory.h"

namespace tools {

	struct slist_node_base {
		typedef slist_node_base* base_ptr;

		explicit slist_node_base(base_ptr p = nullptr) : next(p) { }
		~slist_node_base() { next = nullptr; }

		base_ptr next;

		static base_ptr last(base_ptr p, base_ptr terminator = nullptr) {
			if (nullptr == p) {
				return nullptr;
			}

			while (terminator != p->next) {
				p = p->next;
			}
			return p;
		}
	};

	template <typename _Val>
	struct slist_node : slist_node_base {
		typedef slist_node<_Val>* link_type;
		typedef _Val              value_type;

		explicit slist_node(const value_type& value) : val(value) { }

		value_type val;
	};

	struct slist_base_iterator {
		typedef slist_node_base::base_ptr base_ptr;
		typedef std::forward_iterator_tag iterator_category;
		typedef ptrdiff_t                 difference_type;

		base_ptr node;

		slist_base_iterator() = default;
		explicit slist_base_iterator(base_ptr p) : node(p) { }

		void increment() {
			node = node->next;
		}
	};

	template <typename _Val>
	struct slist_const_iterator : slist_base_iterator {
	public:
		typedef _Val        value_type;
		typedef const _Val& reference;
		typedef const _Val* pointer;

	protected:
		typedef slist_const_iterator<_Val>           self_type;
		typedef slist_base_iterator                  base_type;
		typedef typename slist_node<_Val>::link_type link_type;

	public:
		slist_const_iterator() = default;
		slist_const_iterator(const self_type&) = default;
		slist_const_iterator(const base_type& other) : base_type(other.node) { }
		explicit slist_const_iterator(link_type p) : base_type(p) { }

		reference operator*() const { return link_type(node)->val; }
		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			increment();
			return *this;
		}

		const self_type operator++(int) {
			self_type tmp = *this;
			increment();
			return tmp;
		}
	};

	template <typename _Val>
	struct slist_iterator : slist_base_iterator {
	public:
		typedef _Val  value_type;
		typedef _Val& reference;
		typedef _Val* pointer;

	protected:
		typedef slist_iterator<_Val>                 self_type;
		typedef slist_base_iterator                  base_type;
		typedef typename slist_node<_Val>::link_type link_type;

	public:
		slist_iterator() = default;
		slist_iterator(const self_type&) = default;
		explicit slist_iterator(link_type p) : base_type(p) { }

		reference operator*() const {return link_type(node)->val; }

		pointer operator->() const { return &(operator*()); }

		self_type& operator++() {
			increment();
			return *this;
		}

		const self_type operator++(int) {
			self_type tmp = *this;
			increment();
			return tmp;
		}
	};

	inline bool operator==(const slist_base_iterator& left,
	                       const slist_base_iterator& right) {
		return left.node == right.node;
	}

	template <
		typename _Val,
		typename _Allocator = std::allocator<slist_node<_Val>>
	>
	class single_list {
	public:
		typedef _Val        value_type;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;
		typedef _Val&       reference;
		typedef const _Val& const_reference;

		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;

	protected:
		typedef single_list<_Val, _Allocator> self_type;
		typedef slist_node<_Val>              node_type;
		typedef node_type*                    link_type;

		typedef standard_alloc<node_type, _Allocator> allocator_type;

	private:
		link_type m_before_head;
		link_type m_tail;

	protected:
		link_type get_node() { return allocator_type::allocate(); }
		void put_node(link_type p) { allocator_type::deallocate(p); }

		link_type create_node(const value_type& val) {
			link_type p = get_node();
			construct(p, val);
			return p;
		}

		link_type clone_node(link_type other) {
			return create_node(other->val);
		}

		void destroy_node(link_type p) {
			destroy(p);
			put_node(p);
		}

		link_type& head() const { return (link_type&) (m_before_head->next); }
		link_type& tail() const { return (link_type&) m_tail; }

	private:
		void _initialize() {
			m_before_head = get_node();
			if (nullptr == m_before_head) {
				throw std::bad_alloc();
			}
			head() = m_before_head;
			tail() = m_before_head;
		}

		void _clear() {
			while (!empty()) {
				_erase_after(m_before_head);
			}
			tail() = m_before_head;
		}

		link_type _erase_after(link_type p) {
			link_type to_remove = (link_type) p->next;
			p->next = to_remove->next;
			destroy_node(to_remove);
			return (link_type) p->next;
		}

		link_type _insert_after(link_type p, const value_type& val) {
			link_type new_node = create_node(val);
			new_node->next = p->next;
			p->next = new_node;
			return new_node;
		}

	public:
		single_list() { _initialize(); }

		template <typename _InputIterator>
		single_list(_InputIterator first, _InputIterator last) {
			_initialize();
			while (first != last) {
				this->push_back(*first);
				++first;
			}
		}

		~single_list() {
			_clear();
			put_node(m_before_head);
		}

	protected:
		typedef slist_iterator<value_type>       inner_iterator;
		typedef slist_const_iterator<value_type> const_inner_iterator;
	public:
		typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
		typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

		typedef _reverse_iterator<iterator>       reverse_iterator;
		typedef _reverse_iterator<const_iterator> const_reverse_iterator;

	public:
		bool empty() const { return head() == m_before_head; }

		size_type size() const {
			size_type count = 0;
			link_type cursor = head();
			while (m_before_head != cursor) {
				++count;
				cursor = (link_type) cursor->next;
			}
			return count;
		}

		size_type max_size() const { return size_type(-1); }

		reference front() { return const_cast<reference>(((const self_type*) this)->front()); }
		const_reference front() const { return head()->val; }

		reference back() { return const_cast<reference>(((const self_type*) this)->back()); }
		const_reference back() const { return tail()->val; }

		void clear() { _clear(); }

		void pop_front() {
			if (empty()) {
				throw std::overflow_error("Empty list.");
			}
			_erase_after(m_before_head);
		}

		void pop_back() {
			if (empty()) {
				throw std::overflow_error("Empty list.");
			}

			link_type before_tail =
				(link_type) slist_node_base::last(m_before_head, tail());
			_erase_after(before_tail);
			tail() = before_tail;
		}

		iterator erase_after(const_iterator pos) {
			if (empty() || end() == pos) {
				throw std::overflow_error("Invalid iterator or empty list.");
			}

			return inner_iterator(_erase_after((link_type) pos.base().node));
		}

		iterator insert_after(const_iterator pos, const value_type& val) {
			link_type new_node = _insert_after((link_type) pos.base().node, val);

			if (new_node->next == m_before_head) {
				tail() = new_node;
			}

			return inner_iterator(new_node);
		}

		void push_front(const value_type& val) {
			link_type new_node = _insert_after(m_before_head, val);
			if (new_node->next == m_before_head) {
				tail() = new_node;
			}
		}

		void push_back(const value_type& val) { tail() = _insert_after(tail(), val); }

		iterator begin() { return inner_iterator(head()); }
		const_iterator begin() const { return const_inner_iterator(head()); }

		iterator end() { return inner_iterator(m_before_head); }
		const_iterator end() const { return const_inner_iterator(m_before_head); }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
	};

}

#endif //_SINGLE_LIST_H_
