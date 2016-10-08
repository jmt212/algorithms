#ifndef __ITERATOR_H__
#define __ITERATOR_H__

template<class T<E>>
class MyIterator {
	private:

	public:
		MyIterator(const T<E> &container);
		E operator*();
		MyIterator &operator++();
		MyIterator &operator--();
		bool operator==();
		bool operator!=(); 
};

#endif