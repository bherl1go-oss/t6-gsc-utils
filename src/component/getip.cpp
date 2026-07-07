#include "stdinc.hpp"

#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "gsc.hpp"
#include "scripting.hpp"

namespace getip
{
    std::string get_client_ip(const int client_num)
    {
        const auto clients = game::svs_clients.get();

        if (!clients)
        {
            return "";
        }

        const auto& adr =
            clients[client_num]
            .header
            .netchan
            .remoteAddress;

        if (adr.type != game::netadrtype_t::NA_IP)
        {
            return "loopback";
        }

        return utils::string::va(
            "%u.%u.%u.%u",
            adr.ip[0],
            adr.ip[1],
            adr.ip[2],
            adr.ip[3]
        );
    }

    std::string get_client_ip_port(const int client_num)
    {
        const auto clients = game::svs_clients.get();

        if (!clients)
        {
            return "";
        }

        const auto& adr =
            clients[client_num]
            .header
            .netchan
            .remoteAddress;

        if (adr.type != game::netadrtype_t::NA_IP)
        {
            return "loopback";
        }

        return utils::string::va(
            "%u.%u.%u.%u:%u",
            adr.ip[0],
            adr.ip[1],
            adr.ip[2],
            adr.ip[3],
            ntohs(adr.port)
        );
    }

    class component final : public component_interface
    {
    public:
        void on_startup([[maybe_unused]] plugin::plugin* plugin) override
        {
            gsc::method::add("getip",
                [](const scripting::entity& player)
            {
                const auto entref = player.get_entity_reference();
                const auto client_num = entref.entnum;

                return get_client_ip(client_num);
            });

            gsc::method::add("getipport",
                [](const scripting::entity& player)
            {
                const auto entref = player.get_entity_reference();
                const auto client_num = entref.entnum;

                return get_client_ip_port(client_num);
            });
        }
    };
}

REGISTER_COMPONENT(getip::component)