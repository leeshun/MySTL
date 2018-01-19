/*
 * Created by Maou Lim on 2018/1/19.
 */

#ifndef _TYPE_BASE_H_
#define _TYPE_BASE_H_

namespace tools {

	/* _true or _false */

	struct _true_type {
		typedef bool value_type;
		static const bool value = true;
	};

	struct _false_type {
		typedef bool value_type;
		static const bool value = false;
	};

	/* _identity */

	template <typename _Tp>
	struct _identity {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp*> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp*> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp* const> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp* const> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<_Tp&&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _identity<const _Tp&&> {
		typedef _Tp type;
	};

	template <typename _Tp>
	struct _is_identity : _true_type { };

}

#endif //_TYPE_BASE_H_
