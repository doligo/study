#include "Block_Type.h"

void Block_Type::SetBlock(Block* _block)
{
	block = _block;
}

Block* Block_Type::GetBlock()
{
	return block;
}

bool Trigger_Block::EquipCheck()
{
	Entry_Potal* entry_potal = dynamic_cast<Entry_Potal*>(GetBlock());
	Exit_Potal* exit_potal = dynamic_cast<Exit_Potal*>(GetBlock());
	Game_Clear_Potal* game_clear_potal = dynamic_cast<Game_Clear_Potal*>(GetBlock());

	if (entry_potal != NULL && exit_potal == NULL && game_clear_potal == NULL)
	{
		cout << "입장 기능이 있는 블럭입니다(Trigger_Block) ";
		return entry_potal->Draw();
	}
	else if (entry_potal == NULL && exit_potal != NULL && game_clear_potal == NULL)
	{
		cout << "퇴장 기능이 있는 블럭입니다(Trigger_Block) ";
		return exit_potal->Draw();
	}
	else if (entry_potal == NULL && exit_potal == NULL && game_clear_potal != NULL)
	{
		cout << "게임클리어 기능이 있는 블럭입니다(Trigger_Block) ";
		return game_clear_potal->Draw();
	}
	else
		return false;

}

bool Move_Block::EquipCheck()
{
	Character* character = dynamic_cast<Character*>(GetBlock());


	if (character == NULL)
		return false;

	cout << "움직이는 기능이 있는 블럭입니다(Move_Block) ";

	return character->Draw();

}

bool Stopped_Block::EquipCheck()
{
	Wall* wall = dynamic_cast<Wall*>(GetBlock());

	if (wall == NULL)
		return false;


	cout << "아무런 기능이 없고 정적인 블럭입니다(Stopped_Block) ";

	return wall->Draw();

}