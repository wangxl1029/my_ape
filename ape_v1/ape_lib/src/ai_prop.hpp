#pragma once
#include "ai_comm.hpp"

namespace nsAI {
	template<class _T>
	class IProperty_RO : public CNoCopyable {
	public:
		~IProperty_RO() override = default;
		typedef _T data_type;
		virtual _T & get() = 0;
		virtual _T & operator=(IProperty_RO& r) { return r.get(); }
	protected:
		virtual void set(_T &) = 0;
	};

	template<class _T>
	class CProperty_RW : public IProperty_RO<_T> {
	public:
		~CProperty_RW() override = default;
		void set(_T &r) override { m_data = r; }
		_T & get() override { return m_data; }
	protected:
		_T m_data;
	};

	template<class _T>
	class CProperty_TSRW : public CProperty_RW<_T> {
	public:
		~CProperty_TSRW() override = default;
		void set(_T &r) override { std::lock_guard<std::mutex> _(m_mtx); CProperty_RW<_T>::set(r); }
		_T & get() override { std::lock_guard<std::mutex > _(m_mtx); return CProperty_RW<_T>::get(); }
	protected:
		std::mutex m_mtx;
	};
}

