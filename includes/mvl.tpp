#include <iostream>

namespace mvl
{
	// Data Matrix
	template<typename T, size_t M, size_t N>
	DataMatrix<T,M,N>::DataMatrix()
	{
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				(*this)(i,j) = 0; } 
	template<typename T, size_t M, size_t N>
	DataMatrix<T,M,N>::DataMatrix(const DataMatrix<T,M,N>& d)
	{
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				(*this)(i,j) = d(i,j);
	}

	template<typename T, size_t M, size_t N>
	DataMatrix<T,M,N>& DataMatrix<T,M,N>::operator=(const DataMatrix<T,M,N>& d)
	{
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				(*this)(i,j) = d(i,j);
	}

	template<typename T, size_t M, size_t N>
	T& DataMatrix<T,M,N>::operator()(size_t i, size_t j)
	{
		return data[j][i];
	}

	template<typename T, size_t M, size_t N>
	const T& DataMatrix<T,M,N>::operator()(size_t i, size_t j) const
	{
		return data[j][i];
	}


	// Abstract Vector
	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>::AbstractVector()
	{
		mat = std::make_unique<DataMatrix<T,M,N>>();
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>::AbstractVector(const AbstractVector<T,M,N>& v)
	{
		mat = std::make_unique<DataMatrix<T,M,N>>();
		component_wise(v, [] (T& x, const T& y) { x = y; });
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>& AbstractVector<T,M,N>::operator=(const AbstractVector<T,M,N>& v)
	{
		mat = std::make_unique<DataMatrix<T,M,N>>();
		component_wise(v, [] (T& x, const T& y) { x = y; });
		return *this;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>::AbstractVector(AbstractVector<T,M,N>&& v)
		: mat(std::move(v.mat))
	{
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>& AbstractVector<T,M,N>::operator=(AbstractVector<T,M,N>&& v)
	{
		mat = std::move(v.mat);
		return *this;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>::~AbstractVector()
	{
	}

	template<typename T, size_t M, size_t N>	
	T& AbstractVector<T,M,N>::operator()(size_t i, size_t j)
	{
		return mat->operator()(i,j);
	}

	template<typename T, size_t M, size_t N>
	const T& AbstractVector<T,M,N>::operator()(size_t i, size_t j) const
	{
		return mat->operator()(i,j);
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,1,N> AbstractVector<T,M,N>::getRow(size_t i) const
	{
		AbstractVector<T,1,N> v;
		for (size_t j = 0; j < N; j++)
			v(0,j) = (*this)(i,j);
		return v;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,1> AbstractVector<T,M,N>::getCol(size_t i) const
	{
		AbstractVector<T,M,1> v;
		for (size_t j = 0; j < M; j++)
			v(j,0) = (*this)(j,i);
		return v;
	}

	template<typename T, size_t M, size_t N>
	void AbstractVector<T,M,N>::setRow(size_t i, const AbstractVector<T,1,N>& row)
	{
		for (size_t j = 0; j < N; j++)
			(*this)(i,j) = row(0,j);
	}

	template<typename T, size_t M, size_t N>
	void AbstractVector<T,M,N>::setCol(size_t i, const AbstractVector<T,M,1>& col)
	{
		for (size_t j = 0; j < M; j++)
			(*this)(j,i) = col(j,0);
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>& AbstractVector<T,M,N>::operator+=(const AbstractVector<T,M,N>& v)
	{
		component_wise(v, [] (T& x, const T& y) { x += y; });
		return *this;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>& AbstractVector<T,M,N>::operator-=(const AbstractVector<T,M,N>& v)
	{
		component_wise(v, [] (T& x, const T& y) { x -= y; });
		return *this;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>& AbstractVector<T,M,N>::operator*=(const T& t)
	{
		component_wise([t] (T& x) { x *= t; });
		return *this;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N>& AbstractVector<T,M,N>::operator/=(const T& t)
	{
		component_wise([t] (T& x) { x /= t; });
		return *this;
	}

	template<typename T, size_t M, size_t N>
	bool AbstractVector<T,M,N>::operator==(const AbstractVector<T,M,N>& v) const
	{
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				if ((*this)(i,j) != v(i,j))
					return false;
		return true;
	}

	template<typename T, size_t M, size_t N>
	bool AbstractVector<T,M,N>::operator!=(const AbstractVector<T,M,N>& v) const
	{
		return not ((*this) == v);
	}

	template<typename T, size_t M, size_t N>
	void AbstractVector<T,M,N>::print() const
	{
		for (size_t i = 0; i < M; i++)
		{
			std::cout << "[ " << (*this)(i,0);
			for (size_t j = 1; j < N; j++)
				std::cout << ", " << (*this)(i,j);
			std::cout << "]" << std::endl;
		}
	}

	template<typename T, size_t M, size_t N>
	void AbstractVector<T,M,N>::component_wise(std::function<void(T&)> action)
	{
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				action((*this)(i,j));
	}

	template<typename T, size_t M, size_t N>
	void AbstractVector<T,M,N>::component_wise(const AbstractVector<T,M,N>& v, std::function<void(T&, const T&)> action)
	{
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				action((*this)(i,j), v(i,j));
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N> operator+(AbstractVector<T,M,N> v, const AbstractVector<T,M,N>& u)
	{
		return v += u;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N> operator-(AbstractVector<T,M,N> v, const AbstractVector<T,M,N>& u)
	{
		return v -= u;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N> operator*(AbstractVector<T,M,N> v, const T& t)
	{
		return v *= t;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N> operator*(const T& t, AbstractVector<T,M,N> v)
	{
		return v *= t;
	}

	template<typename T, size_t M, size_t N>
	AbstractVector<T,M,N> operator/(AbstractVector<T,M,N> v, const T& t)
	{
		return v /= t;
	}

	template<typename T, size_t M, size_t N, size_t S>
	AbstractVector<T,M,S> operator*(const AbstractVector<T,M,N>& v, const AbstractVector<T,N,S>& u)
	{
		AbstractVector<T,M,S> w;
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < S; j++)
				for (size_t k = 0; k < N; k++)
					w(i,j) += v(i,k) * u(k,j);
		return w;
	}

	// Vector
	template<typename T, size_t N>
	T& Vector<T,N>::operator[](size_t i)
	{
		return (*this)(i,0);
	}

	template<typename T, size_t N>
	const T& Vector<T,N>::operator[](size_t i) const
	{
		return (*this)(i,0);
	}

	template<typename T, size_t N>
	void Vector<T,N>::print() const
	{
		std::cout << "[ " << (*this)[0];
		for (size_t i = 1; i < N; i++)
			std::cout << ", " << (*this)[i];
		std::cout << "]" << std::endl;
	}
};
