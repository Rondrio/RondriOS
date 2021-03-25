#ifndef SIMPLE_LIST_HH_
#define SIMPLE_LIST_HH_

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace smp {

	template <typename T> class list {

		private:
			const int c_resize_factor = 2;

			T * m_backlog;

			unsigned int m_absolute_size; // how many can fit before we need to expand?
			unsigned int m_relative_size; // how many elements in the list?

			void Expand() {

				int new_absolute_size = m_absolute_size * c_resize_factor;

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

			~list() {
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

			T Rem(T reference) {
				for(unsigned int i = 0; i < m_relative_size; ++i) {
					if(m_backlog[i] == reference) {
						return Rem(i);
					}
				}

				return 0;
			}

			T Rem(unsigned int index) {
				if(index >= 0 && index < m_relative_size) {
					T rtrn = m_backlog[index];

					if(index < m_relative_size - 1) {
						T * dest	= &(m_backlog[index]);
						T * src		= &(m_backlog[index+1]);
						int  amnt	= m_relative_size - (index+1);

						memmove(dest, src, amnt*sizeof(T));
					}

					m_relative_size--;
					return rtrn;
				}

				return 0;
			}

			T Get(unsigned int index) {
				if(index >= 0 && index < m_relative_size) {
					return m_backlog[index];
				}

				return 0;
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

			int Size() {
				return m_relative_size;
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
	};
}

#endif /* SIMPLE_LIST_HH_ */
