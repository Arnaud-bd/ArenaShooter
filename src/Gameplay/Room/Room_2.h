#pragma once
#include "Room/Room.h"

class Room_2 : public Room
{
private:
	void OnUpdate() override;

	GameObject* test;
public:
	void OnInitRoom() override;
};