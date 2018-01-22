/*
 * Created by Maou Lim on 2018/1/22.
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include "iterator.h"

namespace tools {

	template <typename _RandomAccessIterator, typename _Comparator>
	inline void _push_heap(_RandomAccessIterator first,
	                       _RandomAccessIterator last ) {
		// todo
	};

	template <typename _RandomAccessIterator, typename _Comparator>
	inline void _pop_heap(_RandomAccessIterator first,
	                      _RandomAccessIterator last ,
	                      _Comparator           comp ) {
		_pop_heap(first, last - 1, last - 1, *(last - 1), comp);
	}

	template <
		typename _RandomAccessIterator,
		typename _ValueType,
		typename _Comparator,
	>
	inline void _pop_heap(_RandomAccessIterator first ,
	                      _RandomAccessIterator last  ,
	                      _RandomAccessIterator result,
	                      _ValueType            value ,
	                      _Comparator           comp  ) {
		typedef typename
			_iterator_traits<_RandomAccessIterator>::difference_type
		difference_type;

		*result = *first;
		_adjust_heap(first, difference_type(0), last - first, value, comp);
	};

	template <
		typename _RandomAccessIterator,
		typename _ValueType,
		typename _Difference,
		typename _Comparator
	>
	inline void _adjust_heap(_RandomAccessIterator base  ,
	                         _Difference           root  ,
	                         _Difference           length,
	                         _ValueType            value ,
							 _Comparator           comp  ) {
		_Difference child = root * 2 + 2;

		while (child < length) {
			if (comp(*(base + child), *(base + child - 1))) {
				--child;
			}
			*(base + root) = *(base + child);
			root = child;
			child = child * 2 + 2;
		}

		if (child == length) {
			*(base + root) = *(base + child - 1);
			root = child - 1;
		}

		*(base + root) = value;
	};

	template <typename _RandomAccessIterator, typename _Comparator>
	void _heap_sort(_RandomAccessIterator first,
	                _RandomAccessIterator last ,
	                _Comparator           comp ) {
		while (1 < last - first) {
			_pop_heap(first, last--, comp);
		}
	};

	template <typename _RandomAccessIterator, typename _Comparator>
	inline void _make_heap(_RandomAccessIterator first,
	                       _RandomAccessIterator last ,
						   _Comparator           comp ) {
		typedef typename
			_iterator_traits<_RandomAccessIterator>::value_type
		value_type;

		typedef typename
			_iterator_traits<_RandomAccessIterator>::difference_type
		difference_type;

		if (last - first < 2) {
			return;
		}

		difference_type length = last - first;
		difference_type root = (length - 2) / 2;

		while (0 <= root) {
			_adjust_heap(first, root, length, value_type(*(first + root)), comp);
			if (0 == root) { return; }
			--root;
		}
	}

}

#endif //_HEAP_H_
