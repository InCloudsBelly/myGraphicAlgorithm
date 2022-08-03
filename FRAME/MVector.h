//--------------------------------------------------------------------------------------
// Copyright (c) xh Zhao. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <map>
#include <vector>
#include <iostream>
namespace MyGraphics
{
	template <typename _DataType>
	class _Vector
	{
	public:
		void	 push_back(const std::string &vDataName, const _DataType &vData);
		void	 push_back(const std::string &vDataName, const _DataType &vData, int vPointerFlag);
		template <typename Pred>
		void	 push_back(const std::string &vDataName, const _DataType &vData, Pred vPred);
		void	 reserve(size_t vCapacity);
		void	 clear();
		bool	 empty();
		bool	 empty() const;
		bool	 find(const std::string &vDataName);
		bool	 find(const std::string &vDataName) const;
		size_t	 size();
		size_t	 size() const;
		auto&	 operator[](int i);
		auto&	 operator[](const std::string &vDataName);
		auto&	 operator[](int i) const;

		auto	 begin();
		auto	 begin() const;
		auto	 end();
		auto	 end() const;

	private:
		std::vector<_DataType> m_DataSet;
		std::map<std::string, _DataType> m_DataMap;
	};
}

template <typename _DataType>
void MyGraphics::_Vector<_DataType>::push_back(const std::string &vDataName, const _DataType &vData)
{
	m_DataSet.insert(0,vData);
	sort(m_DataSet->begin(),m_DataSet->end(), [](const _DataType &vPass1, const _DataType &vPass2) {return vPass1 < vPass2; })
	m_DataMap[vDataName] = vData;
}

template <typename _DataType>
void MyGraphics::_Vector<_DataType>::push_back(const std::string &vDataName, const _DataType &vData, int vPointerFlag)
{
	m_DataSet.insert(std::lower_bound(m_DataSet.begin(), m_DataSet.end(), vData, [](const _DataType &vPass1, const _DataType &vPass2) {return *vPass1 < *vPass2; }), vData); 
	m_DataMap[vDataName] = vData;
}



template <typename _DataType>
template <typename Pred>
void MyGraphics::_Vector<_DataType>::push_back(const std::string &vDataName, const _DataType &vData, Pred vPred)
{
	m_DataSet.insert(std::lower_bound(m_DataSet.begin(), m_DataSet.end(), vData, vPred), vData);   
}

template <typename _DataType>
auto& MyGraphics::_Vector<_DataType>::operator [](int i)
{
	return const_cast<_DataType&>(static_cast<const _Vector&>(*this)[i]);
}

template <typename _DataType>
auto& MyGraphics::_Vector<_DataType>::operator [](const std::string &vDataName)
{
	_ASSERT(!vDataName.empty() && m_DataMap.find(vDataName) != m_DataMap.end());
	return m_DataMap[vDataName];
}

template <typename _DataType>
auto& MyGraphics::_Vector<_DataType>::operator [](int i) const 
{
	_ASSERT(i < m_DataSet.size() && i >= 0);
	return m_DataSet[i];
}

template <typename _DataType>
auto MyGraphics::_Vector<_DataType>::begin()
{
	return m_DataSet.begin();
}

template <typename _DataType>
auto MyGraphics::_Vector<_DataType>::begin() const
{
	return m_DataSet.begin();
}

template <typename _DataType>
auto MyGraphics::_Vector<_DataType>::end()
{
	return m_DataSet.end();
}

template <typename _DataType>
auto MyGraphics::_Vector<_DataType>::end() const
{
	return m_DataSet.end();
}

template <typename _DataType>
bool MyGraphics::_Vector<_DataType>::empty()
{
	return m_DataSet.empty() || m_DataMap.empty();
}

template <typename _DataType>
bool MyGraphics::_Vector<_DataType>::empty() const
{
	return m_DataSet.empty() || m_DataMap.empty();
}

template <typename _DataType>
void MyGraphics::_Vector<_DataType>::clear()
{
	m_DataSet.clear();
	m_DataMap.clear();
}

template <typename _DataType>
size_t MyGraphics::_Vector<_DataType>::size()
{
	return static_cast<const _Vector&>(*this).size();
}

template <typename _DataType>
size_t MyGraphics::_Vector<_DataType>::size() const
{
	return m_DataSet.size();
}

template <typename _DataType>
void MyGraphics::_Vector<_DataType>::reserve(size_t vCapacity)
{
	_ASSERT(vCapacity >= 0);
	m_DataSet.reserve(vCapacity);
}

template <typename _DataType>
bool MyGraphics::_Vector<_DataType>::find(const std::string &vDataName)
{
	return m_DataMap.find(vDataName) != m_DataMap.end();
}

template <typename _DataType>
bool MyGraphics::_Vector<_DataType>::find(const std::string &vDataName) const
{
	return m_DataMap.find(vDataName) != m_DataMap.end();
}