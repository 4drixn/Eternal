#include "AutoArmor.hpp"
#include <mutex>
#include "SelfDestruct.hpp"

static int HELMET[]  {310, 306, 314, 302, 298};
static int CHESTPLATE[] {311, 307, 315, 303, 299};
static int LEGGINGS[] {312, 308, 316, 304, 300};
static int BOOTS[]  {313, 309, 317, 305, 301};

float getProt(std::shared_ptr<c_itemstack> item_stack)
{
	if (item_stack->get_object() == NULL)
		return 0;

	float prot = 0;

	auto item = std::make_shared<c_item>(item_stack->get_item());
	if (item->get_object() == NULL)
		return 0;
	if (item->is_armor())
	{
		//prot += item->get_damageReduceAmount() + (100 - item->get_damageReduceAmount());
		prot += item->get_damageReduceAmount();
	}

	return prot;
}

int isBestArmor(std::shared_ptr<c_player> player, std::shared_ptr<c_itemstack> item_stack)
{
	std::string itemName = item_stack->getUnlocalizedName();
	int type = -1;

	if (itemName.find("helmet") != std::string::npos)
		type = 5;
	else if (itemName.find("chestplate") != std::string::npos)
		type = 6;
	else if (itemName.find("leggings") != std::string::npos)
		type = 7;
	else if (itemName.find("boots") != std::string::npos)
		type = 8;

	if (type == -1)
		return true;

	float prot = getProt(item_stack);

	auto inv = player->getInventoryContainer();
	auto stack = inv->get_stack_in_slot(type);
	if (stack->get_object() != NULL)
	{
		if (getProt(stack) < prot)
		{
			// Upgrading armor


			inv->slotClick(player->get_object(), type);

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			player->drop_2(inv, type);
			return 2;
		}
	}
	else {
		// No armor equiped

		return 0;
	}
	

	return 1;
}

void autoarmor::autoarmor_()
{
	while (!selfdestruct::should_destruct)
	{
		if (autoarmor::m_enabled == false)
			return;

		auto env = getJNI();

		if (env != nullptr)
		{
			auto minecraft = std::make_unique<c_minecraft>(getMC(env), env);

			if (minecraft->get_object() != NULL && minecraft->getPlayer() != NULL && minecraft->getWorld() != NULL) // Ingame check
			{
				auto local_player = std::make_shared<c_player>(minecraft->getPlayer(), env);
				auto gui_screen = std::make_shared<c_guiscreen>(minecraft->getCurrentScreen());

				if (autoarmor::m_inventory)
					if (gui_screen->get_object() == nullptr || !gui_screen->inventory_open())
						continue;

				//if (!autoarmor::m_inventory)
				//	if (gui_screen->get_object() != NULL || !gui_screen->chat_open() || !gui_screen->chat_open())
				//		return;

				if (autoarmor::m_standing_still)
					if (local_player->get_moveforward() != 0.f || local_player->get_movestrafe() != 0.f)
						continue;

				auto inventory = local_player->getInventoryContainer();
				if (inventory->get_object() == nullptr)
					continue;

				for (int i = 9; i < 45; i++) {

					auto is = inventory->get_stack_in_slot(i);

					if (is == nullptr)
						continue;
					if (is->get_object() == nullptr)
						continue;

					if (isBestArmor(local_player, is) == 0)
					{
						local_player->shift_click(inventory, i);
					}


					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}



				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}
	TerminateThread(GetCurrentThread(), 0x01);
}