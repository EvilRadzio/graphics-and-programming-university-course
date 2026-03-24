#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>

namespace px
{
	template <typename T>
	class Manager
	{
	public:

		class Handle
		{
			size_t id;

			Handle(size_t id) : id(id) {}

			friend Manager;
		};

		bool exists(const std::string& name) const { return m_mappings.count(name); }

		Handle set(T&& resource, const std::string& name)
		{
			if (exists(name))
			{
				Handle handle{ m_mappings[name] };
				m_resources[handle.id] = std::move(resource);
				return handle;
			}

			Handle handle(m_resources.size());
			m_mappings.emplace(name, handle.id);
			m_resources.push_back(std::move(resource));
			return handle;
		}

		const T& get(const Handle handle) const
		{
			assert(handle.id < m_resources.size());
			return m_resources[handle.id];
		}

		const T& get(const std::string& name) const
		{
			assert(exists(name));
			return m_resources[m_mappings.at(name)];
		}

		Handle handle(const std::string& name) const
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

		std::vector<T> m_resources;
		std::unordered_map<std::string, size_t> m_mappings;
	};
}