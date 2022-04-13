#include "AutoTool.hpp"
#include "timer.hpp"
#include "MovingObjectPosition.hpp"
#include "Block.hpp"
#include "player_utils.hpp"

float strongestStrength;

int get_strongest_item(std::unique_ptr<c_player>* player, std::shared_ptr<c_block>* block)
{
	float strength = -0.00000000001;
	int strongest = -1;

	auto inventory = player->get()->getInventory()->getMainInventory();
	for (int i = 0; i < 9; i++)
	{
		if (inventory[i] == NULL)
			continue;

		float itemStrength;
		auto itemstack = std::make_shared<c_itemstack>(inventory[i]);

		auto item = std::make_shared<c_item>(itemstack->get_item());

		if (item->get_object() != NULL && block->get()->get_object() != NULL)
		{
			itemStrength = itemstack->getItemDamageVsBlock(block);
			if (itemStrength > strength)
			{
				strongest = i;
				strength = itemStrength;
				strongestStrength = itemStrength;
			}
		}		

	}

	return strongest;
}

void autotool::autotool(c_context* ctx) 
{
	static std::shared_ptr<c_timer> timer = std::make_shared<c_timer>();
	static std::shared_ptr<c_timer> timer_ = std::make_shared<c_timer>();

	if (!autotool::m_enabled) return;

	if (!timer->has_passed(40))
		return;

	auto minecraft = ctx->minecraft->get();
	auto local_player = ctx->local_player->get();
	auto world = ctx->world->get();

	if (GetAsyncKeyState(VK_LBUTTON))
	{

		if (local_player->is_breaking_block())
		{
			if (local_player->get_currentBlock() != NULL)
			{
				auto inventory = local_player->getInventory();
				auto IBlockState = world->get_blockstate(local_player->get_currentBlock());
				auto block = std::make_shared<c_block>(world->get_block(IBlockState));

				strongestStrength = 0;

				int item = get_strongest_item(ctx->local_player, &block);
				if (item < 0)
					return;

				if (block->get_object() == NULL)
					return;

				if (local_player->get_current_equiped_item() != NULL)
				{
					auto item_stack = std::make_shared<c_itemstack>(local_player->get_current_equiped_item());
					if (item_stack->get_object() == NULL)
						return;

					float strength = item_stack->getItemDamageVsBlock(&block);
					if (strength >= strongestStrength) {
						return;
					}

				}
				else {
					if (strongestStrength == 1)
						return;
				}

				if (inventory->get_current_item() == item)
					return;

				if (!timer_->has_passed(autotool::m_delay))
					return;

				inventory->set_current_item(item);

				timer_->reset();
			}
		}
	}

	timer->reset();
}