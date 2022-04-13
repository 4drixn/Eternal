#pragma once
#include "Player.hpp"
#include "config.hpp"
#include <memory>

namespace teams {

	extern bool IsOnSameTeam(std::unique_ptr<c_player>* local, std::shared_ptr<c_player> other_player);
}