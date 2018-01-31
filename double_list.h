//
// Created by leeshun on 2018/1/31.
//

#ifndef _DOUBLE_LIST_H_
#define _DOUBLE_LIST_H_

#include "iterator.h"
#include "memory.h"

namespace tools {
    struct dlist_node_base {
        typedef dlist_node_base* base_ptr;

        base_ptr next;
        base_ptr previous;

        explicit dlist_node_base(base_ptr pNext = nullptr,
                                 base_ptr pPrev = nullptr) :
                next(pNext),previous(pPrev){}
        ~dlist_node_base() {
            next = nullptr;
            previous = nullptr;
        }


    };


    template <typename _Val>
    struct dlist_node : dlist_node_base{
        typedef dlist_node<_Val>*   link_type;
        typedef _Val                value_type;

        explicit dlist_node(const value_type& value) :
                val(value) , dlist_node_base() {}

        value_type  val;
    };

    struct dlist_base_iterator {
        typedef dlist_node_base::base_ptr       base_ptr;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t                       difference_type;

        base_ptr node;

        dlist_base_iterator() = default;
        explicit dlist_base_iterator(base_ptr p) :node(p){}

        void increment() {
            node = node->next;
        }

        void decrement() {
            node = node->previous;
        }
    };

    template <typename _Val>
    struct dlist_const_iterator : dlist_base_iterator {
    public:
        typedef _Val        value_type;
        typedef const _Val& reference;
        typedef const _Val* pointer;

    protected:
        typedef dlist_const_iterator<_Val>              self_type;
        typedef dlist_base_iterator                     base_type;
        typedef typename dlist_node<_Val>::link_type    link_type;

    public:
        dlist_const_iterator() = default;
        dlist_const_iterator(const self_type&) = default;
        dlist_const_iterator(const base_type& other) : base_ptr (other.node){}
        explicit dlist_const_iterator(link_type p) : base_type(p){}

        reference operator*() const {return link_type(node);}
        pointer operator->() const {return &(operator*());}

        self_type& operator++() {
            increment();
            return *this;
        }

        const self_type operator++(int) {
            self_type old= *this;
            increment();
            return old;
        }

        self_type& operator--() {
            decrement();
            return *this;
        }

        const self_type operator--(int) {
            self_type old = *this;
            decrement();
            return old;
        }

    };

    template<typename _Val>
    struct dlist_iterator : dlist_base_iterator {
    public:
        typedef _Val        value_type;
        typedef const _Val& reference;
        typedef const _Val* pointer;

    protected:
        typedef dlist_const_iterator<_Val>              self_type;
        typedef dlist_base_iterator                     base_type;
        typedef typename dlist_node<_Val>::link_type    link_type;

    public:
        dlist_iterator() = default;
        dlist_iterator(const self_type&) = default;
        explicit dlist_iterator(link_type p) : base_type(p) {}

        reference operator*() const {
            return link_type (node);
        }

        pointer operator->() const {
            return &(operator*());
        }

        self_type& operator++() {
            increment();
            return *this;
        }

        const self_type operator++(int) {
            self_type old = *this;
            increment();
            return old;
        }

        self_type& operator--() {
            decrement();
            return *this;
        }

        const self_type operator--(int) {
            self_type old = *this;
            decrement();
            return old;
        }
    };

    inline bool operator==(const dlist_base_iterator& left,
                           const dlist_base_iterator& right) {
        return left.node == right.node;
    }

    template <typename _Val,
            typename _Allocator = std::allocator<dlist_node<_Val>>
    >
    class double_list {
    public:
        typedef _Val        value_type;
        typedef _Val*       pointer;
        typedef const _Val* const_pointer;
        typedef _Val&       reference;
        typedef const _Val& const_reference;

        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

    protected:
        typedef double_list<_Val,_Allocator>        self_type;
        typedef dlist_node<_Val>                    node_type;
        typedef node_type*                          link_type;

        typedef standard_alloc<node_type,_Allocator> allocator_type;

    private:
        link_type m_head;
        link_type m_tail;

    protected:
        link_type get_node() {
            return allocator_type::allocate();
        }

        void put_node(link_type p) {
            allocator_type::deallocate(p);
        }

        link_type create_node(const value_type& val) {
            link_type p = get_node();
            construct(p,val);
            return p;
        }

        link_type clone_node(link_type other) {
            return create_node(other->val);
        }

        void destory_node(link_type p) {
            destroy(p);
            put_node(p);
        }

        link_type& head() const {
            return (link_type&) (m_head ->next);
        }

        link_type& tail() const {
            return (link_type&)(m_tail);
        }

    private:

        void _initialize() {
            m_head = get_node();
            if(nullptr == m_head) {
                throw std::bad_alloc();
            }
            head() = m_head;
            tail() = m_head;
        }

        void _clear() {
            while (!empty()) {
                _erase_after(m_head);
            }
            tail() = m_head;
        }

        link_type _erase_after(link_type p) {
            link_type to_remove = (link_type) p->next;
            p->next = to_remove->next;
            to_remove->next->previous = p;
            destory_node(to_remove);
            return (link_type)p->next;

        }

        link_type _insert_after(link_type p,const value_type& val) {
            link_type new_node = create_node(val);
            new_node->next = p->next;
            new_node->previous = p;
            p->next->previous = new_node;
            p->next = new_node;
            return new_node;
        }

    public:
        double_list() {
            _initialize();
        }

        template <typename _InputIterator>
        double_list(_InputIterator first,_InputIterator last) {
            _initialize();
            while (first != last) {
                this->push_back(*first);
                ++first;
            }
        }

    protected:
        typedef dlist_iterator<value_type>          inner_iterator;
        typedef dlist_const_iterator<value_type >   const_inner_iterator;

    public:
        typedef _iterator_wrapper<inner_iterator, self_type>       iterator;
        typedef _iterator_wrapper<const_inner_iterator, self_type> const_iterator;

        typedef _reverse_iterator<iterator>       reverse_iterator;
        typedef _reverse_iterator<const_iterator> const_reverse_iterator;

    public:
        bool empty() const {
            return head() == m_head;
        }

        size_type  size() const {
            size_type count = 0;
            link_type cursor = head();
            while (m_head != cursor) {
                ++count;
                cursor = static_cast<link_type>(cursor->next);
            }
            return count;
        }

        size_type max_size() const {
            return size_type (-1);
        }

        const_reference  front() const {
            return head()->val;
        }

        reference front() {
            return const_cast<reference>(((const self_type*)this)->front());
        }

        const_reference back() const {
            return tail()->val;
        }

        reference back() {
            return const_cast<reference >((const self_type*)this)->head();
        }

        void clear() {
            _clear();
        }

        void pop_front() {
            if (empty()) {
                throw std::overflow_error("Empty list.");
            }
            _erase_after(m_head);

        }

        void pop_back() {
            link_type before_tail = (link_type)tail()->previous;
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

            if (new_node->next == m_head) {
                tail() = new_node;
            }

            return inner_iterator(new_node);
        }

        void push_front(const value_type& val) {
            link_type new_node = _insert_after(m_head, val);
            if (new_node->next == m_head) {
                tail() = new_node;
            }
        }

        void push_back(const value_type& val) {
            tail() = _insert_after(tail(),val);
        }

        iterator begin() {
            return inner_iterator(head());
        }

        const_iterator begin() const {
            return const_inner_iterator(head());
        }

        iterator end() {
            return inner_iterator(m_head);
        }

        const_iterator end() const {
            return const_inner_iterator(m_head);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }
    };
}

#endif //_DOUBLE_LIST_H_
