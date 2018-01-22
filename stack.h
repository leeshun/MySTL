/*
 * Created by Maou Lim on 2018/1/22.
 */

#ifndef _STACK_H_
#define _STACK_H_

#include <utility>

namespace tools {

	template <typename _Val, typename _Container>
	class stack {
	public:
		typedef _Val        value_type;
		typedef _Val*       pointer;
		typedef const _Val* const_pointer;
		typedef _Val&       reference;
		typedef const _Val& const_reference;

		typedef typename _Container::size_type size_type;

	protected:
		typedef stack<_Val, _Container> self_type;
		typedef _Container              container_type;

	private:
		container_type m_container;

	public:
		bool empty() const { return m_container.empty(); }
		size_type size() const { return m_container.size(); }
		size_type max_size() const { return m_container.max_size(); }

		const_reference top() const { return m_container.back(); }
		reference top() { return const_cast<reference>(((const self_type*) this)->top()); }

		void pop() { m_container.pop_back(); }
		void push(const value_type& val) { return m_container.push_back(val); }

		template <typename... _Args>
		void emplace(_Args&&... args) { m_container.emplace_back(std::forward<_Args>(args)...); }
	};
}

#endif //_STACK_H_
