/*
 * Created by Maou Lim on 2018/1/23.
 */

#ifndef _FUNCTOR_H_
#define _FUNCTOR_H_

namespace tools {

	template <typename _TpL, typename _TpR = _TpL>
	struct less {
		bool operator()(const _TpL& left, const _TpR& right) const {
			return left < right;
		}
	};

	template <typename _TpL, typename _TpR = _TpL>
	struct larger {
		bool operator()(const _TpL& left, const _TpR& right) const {
			return left > right;
		}
	};

	/* mappings */

	template <typename _Tp>
	struct self {
		const _Tp& operator()(const _Tp& val) const { return val; }
	};

}

#endif //_FUNCTOR_H_
