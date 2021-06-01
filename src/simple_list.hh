#ifndef SIMPLE_LIST_HH_
#define SIMPLE_LIST_HH_

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace smp {

	/* void * Parameter == the list */
	typedef void (*sortfunc)(void *);

	template <typename T> class list {

		private:
			T * m_backlog;

			unsigned int m_absolute_size; // how many can fit before we need to expand?
			unsigned int m_relative_size; // how many elements in the list?

			void Expand() {
				const int FAC_RESIZE = 2;
				int new_absolute_size = m_absolute_size * FAC_RESIZE;

				T * new_backlog = (T *) malloc(sizeof(T)*new_absolute_size);
				memcpy(new_backlog, m_backlog, sizeof(T)*m_relative_size);

				m_absolute_size	= new_absolute_size;

				free(m_backlog);
				m_backlog		= new_backlog;
			}

		public:
			list() {
				const int start_size = 5;

				m_backlog 		= (T *) malloc(sizeof(T) * start_size);
				m_absolute_size	= start_size;
				m_relative_size	= 0;
			}

			list(const list& o) {
				m_backlog		= (T *) malloc(sizeof(T) * o.m_absolute_size);
				m_absolute_size = o.m_absolute_size;
				m_relative_size	= o.m_relative_size;

				memcpy(m_backlog, o.m_backlog, sizeof(T)*m_relative_size);
			}

			virtual ~list() {
				free(m_backlog);
			}

			void Add(T reference) {
				if(m_relative_size < m_absolute_size) {
					m_backlog[m_relative_size++] = reference;
				} else {
					Expand();
					Add(reference);
				}
			}

			void Rem(T reference) {
				for(unsigned int i = 0; i < m_relative_size; ++i) {
					if(m_backlog[i] == reference) {
						Rem(i);
					}
				}
			}

			void Rem(unsigned int index) {
				if(index >= 0 && index < m_relative_size) {
					T rtrn = m_backlog[index];

					if(index < m_relative_size - 1) {
						T * dest	= &(m_backlog[index]);
						T * src		= &(m_backlog[index+1]);
						int  amnt	= m_relative_size - (index+1);

						memmove(dest, src, amnt*sizeof(T));
					}

					m_relative_size--;
				}
			}

			T Get(unsigned int index) {
				if(index >= 0 && index < m_relative_size) {
					return m_backlog[index];
				}

				return 0;
			}
			
			void AsArray(T ** o, int * n) {
				if(o)
					*o = m_backlog;

				if(n)
					*n = m_relative_size;
			}

			void Set(unsigned int index, T reference) {
				if(index >= 0 && index < m_relative_size) {
					m_backlog[index] = reference;
				}
			}

			int IndexOf(T reference) {
				for(unsigned int i = 0; i < m_relative_size; ++i) {
					if(m_backlog[i] == reference) {
						return i;
					}
				}

				return -1;
			}
			
			void Clear() {
				free(m_backlog);

				const int start_size = 5;
				m_backlog = (T *) malloc(sizeof(T) * start_size);

				m_absolute_size = start_size;
				m_relative_size = 0;
			}

			int Size() {
				return m_relative_size;
			}

			void Sort(sortfunc sf) {
				sf(this);
			}

			T& operator[](int i) {
				return m_backlog[i];
			}

			smp::list<T>& operator+=(const T r) {
				Add(r);
				return *this;
			}

			smp::list<T>& operator-=(const T r) {
				Rem(r);
				return *this;
			}

			smp::list<T>& operator=(const smp::list<T>& o) {
				m_backlog		= (T *) malloc(sizeof(T) * o.m_absolute_size);
				m_absolute_size	= o.m_absolute_size;
				m_relative_size	= o.m_relative_size;

				memcpy(m_backlog, o.m_backlog, sizeof(T) * m_relative_size);
			}
	};
}

#endif /* SIMPLE_LIST_HH_ */
