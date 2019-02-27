#ifndef __CXX_INTRUSIVE_LIST_H__
#define __CXX_INTRUSIVE_LIST_H__

#include <bsd/list/slink.h>

template <typename T, snode T:: *member>
struct __slist: slist {
	__slist() {};
	__slist(slist &head): slist(head) {};
};

template <typename T, snode T:: *member> constexpr
T *slist_head(__slist<T, member> *const head)
{
	return __slist_entry(__slist_head(head), T, member);
}

template <typename T, snode T:: *member> constexpr
T *slist_first(__slist<T, member> *const head)
{
	return __slist_entry(__slist_first(head), T, member);
}

template <typename T, snode T:: *member>
T *slist_next(const __slist<T, member> *const, T *const x)
{
	return __slist_entry(__slist_next(&(x->*member)), T, member);
}

template <typename T, snode T:: *member> constexpr
T *slist_begin(__slist<T, member> *const head)
{
	return __slist_entry(__slist_begin(head), T, member);
}

template <typename T, snode T:: *member> constexpr
T *slist_end(__slist<T, member> *const head)
{
	return __slist_entry(__slist_end(head), T, member);
}

/*
template <typename T, snode T:: *member>
void slist_add_head(slist<T, member> *const head, T *const x)
{
	__slist_insert_head(head, &(x->*member));
}

template <typename T, snode T:: *member>
void slist_add_after(slist<T, member> *const, T *const x, T *const y)
{
	__slist_add_after(&(x->*member), &(y->*member));
}

template <typename T, snode T:: *member>
void slist_del_after(slist<T, member> *const, T *const x)
		
{
	__slist_del_after(&(x->*member));
}
*/

#ifdef CONFIG_URCU

template <typename T, snode T:: *member>
void slist_remove_after_rcu(slist<T, member> *const, T *const x)
		
{
	__slist_remove_after_rcu(&(x->*member));
}

#endif/*CONFIG_URCU*/

/*
std::for_each(first, last, [](const auto& it) {
	cout << it->name;
});
*/
#endif
