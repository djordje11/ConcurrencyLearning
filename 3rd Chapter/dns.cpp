#include <string>
#include <map>
#include <shared_mutex>

using ip_address = std::string;
using domain = std::string;

class dns
{
    std::map<domain, ip_address> m_entries;
    mutable std::shared_mutex m_mutex;
public:
    ip_address find_domain(domain d)
    {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        auto it = m_entries.find(d);
        return it  == m_entries.end() ? d : it->second;
    }
    void update_or_add_entry(domain d, ip_address ip)
    {
        std::lock_guard lock(m_mutex);
        m_entries[d] = ip;
    }

};

int main()
{

}