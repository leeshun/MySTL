/*
 * Created by Maou Lim on 2018/1/17.
 */

#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <memory>
#include <cassert>

#include "iterator.h"
#include "memory.h"

namespace tools {

	template <
		typename _Val,
		typename _Allocator = std::allocator<_Val>
	>
	class sequence {
	public:
		typedef _Val        value_type;
		typedef _Val&       reference;
		typedef const _Val& const_reference;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;

		typedef ptrdiff_t difference_type;
		typedef size_t    size_type;

	protected:
		typedef sequence<_Val, _Allocator>       self_type;
		typedef standard_alloc<_Val, _Allocator> allocator_type;

	private:
		pointer m_base;
		pointer m_end;
		pointer m_finish;

	protected:
		pointer get_space(size_type n) { return allocator_type::allocate(n); }
		void put_space(pointer p, size_type n) { return allocator_type::deallocate(p, n); }

	private:
		void _initialize_with_n(size_type n) throw (std::bad_alloc) {
			m_base = get_space(n);
			if (nullptr == m_base) {
				throw std::bad_alloc();
			}
			m_finish = m_base + n;
			m_end = m_base;
		}

		template <typename _InputIterator>
		void _fill(_InputIterator first, _InputIterator last) {
			while (first != last) {
				construct(m_end++, *first);
				++first;
			}
		}

		void _fill(pointer first, pointer last) {
			size_type offset = last - first;
			memcpy(m_base, first, sizeof (value_type) * offset);
			m_end = m_base + offset;
		}

		void _extend(size_type new_size) {
			pointer old_base = m_base;
			pointer old_end  = m_end;
			pointer old_finish = m_finish;

			try {
				_initialize_with_n(new_size);
			}
			catch (...) {
				m_base = old_base;
				m_end  = old_end;
				m_finish = old_finish;
				throw;
			}

			_fill(old_base, old_end);
			put_space(old_base, old_finish - old_base);
		}

		void _extend_when_full() {
			_extend(0 == capacity() ? 1 : capacity() * 2);
		}

		void _resize(size_type new_size) {
			if (capacity() == new_size) {
				return;
			}

			if (capacity() < new_size) {
				_extend(new_size);
			}
			else if (size() < new_size) {
				construct(m_end, m_base + new_size, value_type());
				m_end = m_base + new_size;
			}
			else {
				destroy(m_base + new_size, m_end);
				m_end = m_base + new_size;
			}
		}

		void _swap() { /* todo */ }

		void _destroy() {
			pointer cursor = m_base;
			while (m_end != cursor) {
				destroy(cursor);
				++cursor;
			}
			put_space(m_base, capacity());
		}

		bool _full() const { return m_end == m_finish; }

		/* [start, end) */
		void _copy_backward(pointer start, pointer end, pointer new_start) {
			difference_type offset = new_start - start;
			pointer p = end + offset - 1;

			while (new_start <= p) {
				memcpy(p, p - offset, sizeof (value_type));
				--p;
			}
		}

		void _copy_forward(pointer start, pointer end, pointer new_start) {
			while (start != end) {
				memcpy(new_start++, start++, sizeof (value_type));
			}
		}

	public:
		sequence() : m_base(nullptr), m_end(nullptr), m_finish(nullptr) { }

		explicit sequence(size_type capacity)
			throw (std::bad_alloc) { _initialize_with_n(capacity); }

		sequence(const self_type& other)
			throw (std::bad_alloc) {
			_initialize_with_n(other.size());
			_fill(other.begin(), other.end());
		}

		template <typename _InputIterator>
		sequence(_InputIterator first, _InputIterator last) {
			while (first != last) {
				this->push_back(*first);
				++first;
			}
		}

		~sequence() { _destroy(); }

	public:
		self_type& operator=(const self_type& other) {
			if (this == &other) {
				return *this;
			}

			_destroy();
			_initialize_with_n(other.size());
			_fill(other.m_base, other.m_end);

			return *this;
		}

		const_reference operator[](size_type index) const {
			assert(index < size());
			return *(m_base + index);
		}

		reference operator[](size_type index) {
			return const_cast<reference>(
				((const self_type*) this)->operator[](index)
			);
		}

	public:
		typedef _iterator_wrapper<pointer, self_type>       iterator;
		typedef _iterator_wrapper<const_pointer, self_type> const_iterator;

	public:
		bool empty() const { return m_base == m_end; }
		size_type size() const { return m_end - m_base; }
		size_type max_size() const { return size_type(-1); }
		size_type capacity() const { return m_finish - m_base; }

		reference at(size_type index) { return operator[](index); }
		const_reference at(size_type index) const { return operator[](index); }

		pointer data() { return const_cast<pointer>(((const self_type*) this)->data()); }
		const_pointer data() const { return m_base; }

		reference front() { return const_cast<reference>(((const self_type*) this)->front()); }
		const_reference front() const { assert(!empty()); return *m_base; }

		reference back() { return const_cast<reference>(((const self_type*) this)->back()); }
		const_reference back() const { assert(!empty()); return *(m_end - 1); }

		void clear() { m_end = m_base; }

		void resize(size_type new_size) { _resize(new_size); }

		void push_back(const value_type& val) { insert(end(), val); }

		void pop_back() { erase(--end()); }

		void insert(const_iterator pos, const value_type& val) {
			difference_type offset = pos - begin();
			if (_full()) {
				_extend_when_full();
			}

			pointer p = m_base + offset;
			_copy_backward(p, m_end, p + 1);
			construct(p, val);
			++m_end;
		}

		iterator erase(const_iterator pos) {
			if (empty() || end() == pos) {
				return std::overflow_error("Invalid iterator or empty sequence.");
			}

			difference_type offset = pos - begin();
			pointer to_erase = m_base + offset;

			destroy(to_erase);
			_copy_forward(to_erase + 1, m_end, to_erase);

			--m_end;
			return iterator(to_erase);
		}

		iterator begin() { return iterator(m_base); }
		const_iterator begin() const { return const_iterator(m_base); }

		iterator end() { return iterator(m_end); }
		const_iterator end() const { return const_iterator(m_end); }

	};
}

#endif //_SEQUENCE_H_
