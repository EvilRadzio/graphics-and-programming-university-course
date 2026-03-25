#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include <optional>

namespace px
{
	template <typename T>
	class Manager
	{
	public:

		Manager() = default;
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;

		class Handle
		{
			size_t id;
			size_t gen;

			Handle(size_t id, size_t gen) : id(id), gen(gen) {}

			friend Manager;
		};

		void setError(T&& resource)
		{
			m_errorResource.emplace(std::move(resource));
		}

		bool exists(const Handle handle) const { return handle.id < m_resources.size() && m_resources[handle.id].second == handle.gen; }
		bool exists(const std::string& name) const { return m_mappings.count(name); }

		Handle set(T&& resource, const std::string& name)
		{
			if (exists(name))
			{
				Handle handle = m_mappings.at(name);
				++handle.gen;
				m_resources[handle.id].first = std::move(resource);
				m_resources[handle.id].second = handle.gen;
				return handle;
			}

			Handle handle(m_resources.size(), 0);
			m_mappings.emplace(name, handle);
			m_resources.emplace_back(std::move(resource), 0);
			return handle;
		}

		const T* tryGet(const Handle handle) const
		{
			if (exists(handle))
			{
				return m_resources[handle.id].first;
			}

			return m_errorResource ? &m_errorResource.value() : nullptr;
		}

		const T* tryGet(const std::string& name) const
		{
			if (exists(name))
			{
				return m_resources[m_mappings.at(name).id].first;
			}

			return m_errorResource ? &m_errorResource.value() : nullptr;;
		}

		const T& get(const Handle handle) const
		{
			assert(exists(handle));
			return m_resources[handle.id].first;
		}

		const T& get(const std::string& name) const
		{
			assert(exists(name));
			return m_resources[m_mappings.at(name).id].first;
		}		

		Handle getHandle(const std::string& name) const
		{
			assert(exists(name));
			return m_mappings.at(name);
		}

		void unload()
		{
			m_resources.clear();
			m_mappings.clear();
		}

	private:

		std::vector<std::pair<T, size_t>> m_resources;
		std::unordered_map<std::string, Handle> m_mappings;
		std::optional<T> m_errorResource;
	};
}