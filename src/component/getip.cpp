#include "stdinc.hpp"

#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "gsc.hpp"
#include "scripting.hpp"

namespace getip
{
    std::string get_client_ip(const int client_num)
    {
        const auto& adr =
            game::svs_clients.get()[client_num]
            .header
            .netchan
            .remoteAddress;

        return utils::string::va(
            "%u.%u.%u.%u",
            adr.ip[0],
            adr.ip[1],
            adr.ip[2],
            adr.ip[3]
        );
    }

    class component final : public component_interface
    {
    public:
        void on_startup(plugin::plugin*) override
        {
            gsc::method::add("getip",
                [](const scripting::entity& player)
            {
                const auto client_num =
                    player.get_entity_number();

                return get_client_ip(client_num);
            });
        }
    };
}

REGISTER_COMPONENT(getip::component)