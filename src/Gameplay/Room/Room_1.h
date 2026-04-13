#pragma once
#include "Room/Room.h"

class Room_1 : public Room
{
private:
	void OnUpdate() override;

public:
	void OnInitRoom() override;
};