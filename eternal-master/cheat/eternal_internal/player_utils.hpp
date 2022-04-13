#pragma once

#include "Player.hpp"
#include "utils.hpp"
#include "Block.hpp"
#include <iostream>

__forceinline bool is_npc(const std::shared_ptr<c_player>* player)
{
	std::string name = player->get()->get_name();
	if (name.length() == 0) return true;
	return utils::has_special_char(name);
}

__forceinline bool is_hypixel_npc(const std::shared_ptr<c_player>* player)
{
	if (player->get()->getDisplayName()->getUnformattedText().find("NPC") != std::string::npos)
		return true;
	return false;
}

__forceinline float getItemDamageVsBlock(std::shared_ptr<c_block> block, std::shared_ptr<c_item> item, std::shared_ptr<c_itemstack> item_stack)
{
	auto this_class = global::jenv->GetObjectClass(item->get_object());
	jmethodID mid = global::jenv->GetMethodID(this_class, global::is_badlion ? "a" : "func_150893_a", global::is_badlion ? "(Lzx;Lafh;)F" : "(Lnet/minecraft/item/ItemStack;Lnet/minecraft/block/Block;)F");

	float strength = global::jenv->CallFloatMethod(item->get_object(), mid, item_stack->get_object(), block->get_object());
	global::jenv->DeleteLocalRef(this_class);

	return strength;
}

namespace blockutils {
	__forceinline bool isBlockUnder(std::unique_ptr<c_player>* _player_, std::unique_ptr<c_world>* _world_)
	{
		auto player = _player_->get();
		vec3 pos = player->get_position();
		for (int offset = 0; offset < pos.y + player->get_eye_height(); offset += 2)
		{
			auto bb = player->get_bounding_box()->offset(0, -offset, 0);

			if (_world_->get()->getCollidingBoundingBoxes(player->get_object(), bb->get_object()) > 0)
				return true;
		}
		return false;
	}


}